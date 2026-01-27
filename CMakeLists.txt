cmake_minimum_required(VERSION 3.15)

project(Jobs LANGUAGES C)

# ------------------------------------------------------------------------------
# Includes
# ------------------------------------------------------------------------------

include(${CMAKE_CURRENT_LIST_DIR}/jobsFilePaths.cmake)

# ------------------------------------------------------------------------------
# Library targets
# ------------------------------------------------------------------------------

add_library(jobs_for_aws INTERFACE)

target_sources(jobs_for_aws INTERFACE ${JOBS_SOURCES})

target_include_directories(jobs_for_aws INTERFACE ${JOBS_INCLUDE_PUBLIC_DIRS})

message(STATUS "jobs_for_aws library requires coreJSON as a dependency.")
