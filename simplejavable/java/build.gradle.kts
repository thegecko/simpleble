import org.gradle.internal.os.OperatingSystem

plugins {
    id("java-library")
}

group = "org.simplejavable"
version = "v${file("../../VERSION").readText().trim()}"

repositories {
    mavenCentral()
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(17))
    }
}

// Native library acquisition options
val nativeLibPath: String? by project // -PnativeLibPath=...
val buildFromCMake: String? by project // -PbuildFromCMake (presence is what matters)

// Build native libraries using CMake
tasks.register<Exec>("buildNativeCMake") {
    val cmakePath = "../cpp" // Default CMake location
    val cmakeBuildPath = layout.buildDirectory.dir("build_cpp").get().asFile
    workingDir(cmakePath)
    commandLine("cmake",
        "-B", cmakeBuildPath.absolutePath,
        "-DCMAKE_BUILD_TYPE=Release")

    doLast {
        exec {
            workingDir(cmakePath)
            commandLine("cmake", "--build", cmakeBuildPath.absolutePath, "--config", "Release")
        }
    }
}

// Add native libraries to jar based on the selected mode
tasks.jar {
    // TODO: Remove this once main class is not needed.
    manifest {
        attributes["Main-Class"] = "org.simplejavable.Main"
    }

    buildFromCMake?.let {
        // Build from CMake when explicitly requested
        dependsOn("buildNativeCMake")
        val cmakeBuildOutputPath = layout.buildDirectory.dir("build_cpp/lib").get().asFile
        val currentArch = System.getProperty("os.arch").let { arch ->
            when {
                arch.contains("amd64") || arch.contains("x86_64") -> "x86_64"
                arch.contains("aarch64") -> "aarch64"
                else -> error("Unsupported architecture: $arch")
            }
        }
        from(cmakeBuildOutputPath) {
            include("**/*.so", "**/*.dll", "**/*.dylib")
            into("native/$currentArch")
        }
    } ?: nativeLibPath?.let { path ->
        // Use local path approach when CMake build not requested
        from(file(path)) {
            include("**/*.so", "**/*.dll", "**/*.dylib") // Ensure only shared libraries are included
            into("native/x86_64") // Target directory inside the JAR
        }
    } ?: error("Please provide -PnativeLibPath or use -PbuildFromCMake to build from CMake")
}