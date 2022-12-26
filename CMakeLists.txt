cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(zmq_example VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(main main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

# This will allow the using conan-generated FindXXX.cmake files
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

if (APPLE)
    conan_basic_setup(TARGETS         # Allow to target_link packets as CONAN_PKG::packet
                    NO_OUTPUT_DIRS  # Do not adjust the build output directories.
                    KEEP_RPATHS
                    )
else ()
    conan_basic_setup(TARGETS         # Allow to target_link packets as CONAN_PKG::packet
                    NO_OUTPUT_DIRS  # Do not adjust the build output directories.
                    )
endif()

find_package(cppzmq)
find_package(stduuid)
find_package(concurrentqueue REQUIRED)
target_link_libraries(main cppzmq stduuid::stduuid CONAN_PKG::concurrentqueue)
# Set targets pre-processor defines
target_compile_definitions(main PUBLIC -DZMQ_BUILD_DRAFT_API=1)