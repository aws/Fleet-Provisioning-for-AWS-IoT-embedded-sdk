
# AWS IoT Fleet Provisioning Library

Note: This library is under development.

The Fleet Provisioning library enables you to provision IoT devices without device certificates using the [AWS IoT Fleet Provisioning Service](https://docs.aws.amazon.com/iot/latest/developerguide/provision-wo-cert.html). For an overview of provisioning options availible, see [Device provisioning](https://docs.aws.amazon.com/iot/latest/developerguide/iot-provision.html).

This library has no dependencies on any additional libraries other than the
standard C library, and therefore, can be used with any MQTT client library.
This library is distributed under the [MIT Open Source License](LICENSE).

## AWS IoT Fleet Provisioning Client Config File

The AWS IoT Fleet Provisioning Client Library exposes build configuration macros
that are required for building the library. A list of all the configurations and
their default values are defined in
[fleet_provisioning_config_defaults.h](source/include/fleet_provisioning_config_defaults.h).
To provide custom values for the configuration macros, a config file named
`fleet_provisioning_config.h` can be provided by the application to the library.

By default, a `fleet_provisioning_config.h` config file is required to build the library.
To disable this requirement and build the library with default configuration
values, provide `FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG` as a compile time
preprocessor macro.

**Thus, the Fleet Provisioning client library can be built by either**:
* Defining a `fleet_provisioning_config.h` file in the application, and adding it to the
include directories list of the library.

**OR**

* Defining the `FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG` preprocessor macro for the
library build.

## Building the Library

The [fleetprovisioningFilePaths.cmake](fleetprovisioningFilePaths.cmake) file contains the
information of all source files and the header include paths required to build
the Device Defender client library.

As mentioned in the previous section, either a custom config file
(i.e. `fleet_provisioning_config.h`) or `FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG` macro needs to
be provided to build the Fleet Provisioning client library.

For a CMake example of building the Fleet Provisioning client library with the
`fleetprovisioningFilePaths.cmake` file, refer to the `coverity_analysis` library target
in [test/CMakeLists.txt](test/CMakeLists.txt) file.

## Building Unit Tests

### Platform Prerequisites

- For running unit tests:
    - **C90 compiler** like gcc.
    - **CMake 3.13.0 or later**.
- For running the coverage target, **gcov** and **lcov** are additionally required.

### Steps to build **Unit Tests**

1. Go to the root directory of this repository.

1. Run the *cmake* command: `cmake -S test -B build -DBUILD_CLONE_SUBMODULES=ON`.

1. Run this command to build the library and unit tests: `make -C build all`.

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `cd build && ctest` to execute all tests and view the test run summary.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```

## Contributing

See [CONTRIBUTING.md](./.github/CONTRIBUTING.md) for information on contributing.
