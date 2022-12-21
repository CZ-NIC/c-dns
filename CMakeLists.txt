#
# Copyright © 2019 CZ.NIC, z. s. p. o.
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at https://mozilla.org/MPL/2.0/.
#

cmake_minimum_required(VERSION 3.5)
project(cdns VERSION 1.1.0)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(Boost REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(Doxygen)

option(BUILD_TESTS "Set to ON to build tests that use Google Test framework" OFF)
option(BUILD_DOC "Generate Doxygen documentation" ON)
option(BUILD_PYTHON_BINDINGS "Generate Python bindings" OFF)

file(GLOB sources "src/*.cpp")
file(GLOB headers "src/*.h")
add_library(cdns SHARED
    ${sources}
)

target_link_libraries(cdns ${Boost_LIBRARIES} ZLIB::ZLIB ${LIBLZMA_LIBRARIES})
target_include_directories(cdns PUBLIC ${Boost_INCLUDE_DIRS} ${LIBLZMA_INCLUDE_DIRS})

include(CheckCCompilerFlag)
check_c_compiler_flag(-msse4 SSE4_FLAG)
if(SSE4_FLAG)
    target_compile_options(cdns PUBLIC -msse4)
else()
    message(FATAL_ERROR "SSE4 is required for compilation!")
endif()

set_target_properties(cdns PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cdns PROPERTIES SOVERSION 1)
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

if (BUILD_PYTHON_BINDINGS)
    find_package(Python3 REQUIRED)
    find_package(pybind11 REQUIRED)

    set(SETUP_PY_IN "${CMAKE_CURRENT_SOURCE_DIR}/python/setup.py.in")
    set(SETUP_PY    "${CMAKE_CURRENT_BINARY_DIR}/setup.py")
    set(PYPROJECT   "${CMAKE_CURRENT_SOURCE_DIR}/python/pyproject.toml")

    file(COPY ${PYPROJECT} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    configure_file(${SETUP_PY_IN} ${SETUP_PY})

    add_custom_target(pycdns ALL
        COMMAND pip3 install --prefix=${CMAKE_CURRENT_BINARY_DIR}/build_python/ ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_dependencies(pycdns cdns)

    install(
        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/build_python/
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        PATTERN "*"
    )
endif(BUILD_PYTHON_BINDINGS)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
    add_test(NAME UnitTests COMMAND tests)
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
