#
# Copyright © 2019 CZ.NIC, z. s. p. o.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at https://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.9)
project(cdns VERSION 0.2.0 DESCRIPTION "C-DNS format exporter")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ZLIB REQUIRED)

find_package(LibLZMA REQUIRED)

find_package(CBOR REQUIRED)

find_package(Doxygen)

option(BUILD_TESTS "Set to ON to build tests that use Google Test framework" OFF)
option(BUILD_DOC "Generate Doxygen documentation" ON)

file(GLOB sources "src/*.cpp")
file(GLOB headers "src/*.h")
add_library(cdns SHARED
    ${sources}
)

target_link_libraries(cdns CBOR::CBOR ZLIB::ZLIB ${LIBLZMA_LIBRARIES})
target_include_directories(cdns PUBLIC ${LIBLZMA_INCLUDE_DIRS})

include(CheckCCompilerFlag)
check_c_compiler_flag(-msse4 SSE4_FLAG)
if(SSE4_FLAG)
    target_compile_options(cdns PUBLIC -msse4)
else()
    message(FATAL_ERROR "SSE4 is required for compilation!")
endif()

set_target_properties(cdns PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cdns PROPERTIES SOVERSION 0)
set_target_properties(cdns PROPERTIES PUBLIC_HEADER "${headers}")

configure_file(src/cdns.pc.in src/cdns.pc @ONLY)

target_include_directories(cdns PRIVATE .)

install(TARGETS cdns
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/cdns
)

install(FILES ${CMAKE_BINARY_DIR}/src/cdns.pc 
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)

if (BUILD_DOC)
    if(DOXYGEN_FOUND)
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

        add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API Doxygen documentation"
            VERBATIM)

    else(DOXYGEN_FOUND)
        message("Install Doxygen to generate documentation")
    endif(DOXYGEN_FOUND)
endif(BUILD_DOC)
