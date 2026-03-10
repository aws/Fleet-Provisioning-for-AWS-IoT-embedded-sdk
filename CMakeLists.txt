cmake_minimum_required(VERSION 3.15)

project(FleetProvisioning LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/fleetprovisioningFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library( aws_fleet_provisioning )

target_sources( aws_fleet_provisioning PUBLIC ${FLEET_PROVISIONING_SOURCES} )

target_include_directories( aws_fleet_provisioning PUBLIC ${FLEET_PROVISIONING_INCLUDE_PUBLIC_DIRS} )

if(STANDALONE_TEST_BUILD_UNIX)
    target_compile_definitions( aws_fleet_provisioning PUBLIC
                                FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG=1 )

    message( WARNING "This is a standalone build using default values for config. Please"
                     " verify that the default configuration is appropriate for your application.")
endif()