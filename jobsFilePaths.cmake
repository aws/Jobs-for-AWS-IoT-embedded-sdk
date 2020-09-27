# This file is to add source files and include directories
# into variables so that it can be reused from different repositories
# in their Cmake based build system by including this file.
#
# Files specific to the repository such as test runner, platform tests
# are not added to the variables.

# AWS IoT Jobs library source files.
set( JOBS_SOURCES
     ${CMAKE_CURRENT_LIST_DIR}/source/jobs.c )

# AWS IoT Jobs library Public Include directories.
set( JOBS_INCLUDE_PUBLIC_DIRS
     ${CMAKE_CURRENT_LIST_DIR}/source/include )
