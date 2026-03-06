cmake_minimum_required(VERSION 3.15)

project(AwsIoTJobs LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/jobsFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library( aws_iot_jobs )


target_sources( aws_iot_jobs PUBLIC ${JOBS_SOURCES} ${OTA_HANDLER_SOURCES} )

target_include_directories( aws_iot_jobs PUBLIC ${JOBS_INCLUDE_PUBLIC_DIRS} ${OTA_HANDLER_INCLUDES} )


include(FetchContent)
FetchContent_Declare(
CoreJSON
GIT_REPOSITORY https://github.com/FreeRTOS/coreJSON.git
GIT_TAG cffa492da18c890181d64462f8af63992a69d3b0)

FetchContent_MakeAvailable(CoreJSON)

# Add coreJSON library
include("${corejson_SOURCE_DIR}/jsonFilePaths.cmake")
add_library(coreJSON ${JSON_SOURCES})
target_include_directories(coreJSON PUBLIC ${JSON_INCLUDE_PUBLIC_DIRS})

target_link_libraries(aws_iot_jobs PUBLIC coreJSON)
