cmake_minimum_required(VERSION 3.15)

project(Fleet-Provisioning LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/fleetprovisioningFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library(fleet_provisioning_for_aws INTERFACE)

target_sources(fleet_provisioning_for_aws INTERFACE ${FLEET_PROVISIONING_SOURCES})

target_include_directories(fleet_provisioning_for_aws INTERFACE ${FLEET_PROVISIONING_INCLUDE_PUBLIC_DIRS})

message(STATUS "fleet_provisioning_for_aws library requires FLEET_PROVISIONING_DO_NOT_USE_CUSTOM_CONFIG definition or a fleet_provisioning_config.h file.")
