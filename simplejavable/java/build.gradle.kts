import org.gradle.internal.os.OperatingSystem

plugins {
    kotlin("jvm") version "2.0.20"
}

group = "org.simplejavable"
version = "v${file("../../VERSION").readText().trim()}"

repositories {
    mavenCentral()
}

kotlin {
    jvmToolchain(17)
}

// Native library acquisition options
val nativeLibUrl: String? by project // -PnativeLibUrl=...
val buildFromCMake: String? by project // -PbuildFromCMake (presence is what matters)

// OS and architecture detection for CMake builds
val currentOs = when {
    OperatingSystem.current().isWindows -> "windows"
    OperatingSystem.current().isMacOsX -> "macos"
    OperatingSystem.current().isLinux -> "linux"
    else -> error("Unsupported OS")
}

val currentArch = System.getProperty("os.arch").let { arch ->
    when {
        arch.contains("amd64") || arch.contains("x86_64") -> "x86_64"
        arch.contains("aarch64") -> "aarch64"
        else -> error("Unsupported architecture: $arch")
    }
}

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
        from(cmakeBuildOutputPath) {
            include("**/*.so", "**/*.dll", "**/*.dylib")
            into("native/$currentOs/$currentArch")
        }
    } ?: nativeLibUrl?.let { url ->
        // Use URL approach when CMake build not requested
        from(uri(url)) {
            into("native")
        }
    } ?: error("Please provide -PnativeLibUrl or use -PbuildFromCMake to build from CMake")
}