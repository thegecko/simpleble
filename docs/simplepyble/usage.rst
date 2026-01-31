=====
Usage
=====

SimpleBLE works on Windows, Linux and MacOS. Please follow the instructions below
to build and run SimplePyBLE in your specific environment.


Installing from PyPI
====================

The fastest way to get going with SimplePyBLE is to install it from PyPI: ::

   pip install simplepyble

Please take into consideration that when using this library on Linux, you will
need to have the following dependencies installed: ::

   sudo apt-get install libdbus-1-dev


Installing locally
====================

If you wish to install SimplePyBLE locally, either for development purposes or
to access the latest features, you can do so by cloning the repository and
installing it with pip: ::

   cd <path-to-simpleble>/simplepyble
   pip install .


Built-in REST Server
====================

SimplePyBLE includes a built-in REST server that allows you to control BLE devices
using a REST API. To use the server, you need to install the optional dependencies: ::

   pip install simplepyble[server]

Once the dependencies are installed, you can run the server using the following command: ::

   python3 -m simplepyble.server --host 127.0.0.1 --port 8000

API Endpoints
-------------

The server provides the following endpoints:

* **GET /**: Returns a status message.
* **GET /adapters**: Returns a list of available Bluetooth adapters.
* **POST /scan?timeout_ms=5000**: Scans for nearby BLE devices.
* **POST /connect/{address}**: Connects to a device by its address.
* **POST /disconnect/{address}**: Disconnects from a device.
* **GET /device/{address}**: Returns information about a connected device, including its services and characteristics.
* **POST /device/{address}/read/{service_uuid}/{char_uuid}**: Reads the value of a characteristic.
* **POST /device/{address}/write/{service_uuid}/{char_uuid}**: Writes a value to a characteristic (Request).
  * Body: ``{"data": "<hex_string>"}``
* **POST /device/{address}/write_command/{service_uuid}/{char_uuid}**: Writes a value to a characteristic (Command).
  * Body: ``{"data": "<hex_string>"}``
* **POST /device/{address}/notify/{service_uuid}/{char_uuid}**: Subscribes to notifications.
* **POST /device/{address}/indicate/{service_uuid}/{char_uuid}**: Subscribes to indications.
* **POST /device/{address}/unsubscribe/{service_uuid}/{char_uuid}**: Unsubscribes from notifications/indications.
* **GET /device/{address}/notifications**: Retrieves and clears the buffer of received notifications for a device.

Interactive Documentation
-------------------------

When the server is running, you can access the interactive Swagger UI at ``http://127.0.0.1:8000/docs`` and the ReDoc documentation at ``http://127.0.0.1:8000/redoc``.

