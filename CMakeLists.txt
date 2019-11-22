cmake_minimum_required(VERSION 3.9)
project(cdns VERSION 0.0.1 DESCRIPTION "C-DNS format exporter")

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 17)

find_package(ZLIB REQUIRED)

find_package(LibLZMA REQUIRED)

file(GLOB sources "src/*.cpp")
add_library(cdns SHARED
    ${sources}
)

target_link_libraries(cdns -lcbor ZLIB::ZLIB LibLZMA::LibLZMA)

include(CheckCCompilerFlag)
check_c_compiler_flag(-msse4 SSE4_FLAG)
if(SSE4_FLAG)
    target_compile_options(cdns PUBLIC -msse4)
else()
    message(FATAL_ERROR "SSE4 is required for compilation!")
endif()

set_target_properties(cdns PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cdns PROPERTIES SOVERSION 0)
set_target_properties(cdns PROPERTIES PUBLIC_HEADER src/cdns.h)

configure_file(src/cdns.pc.in src/cdns.pc @ONLY)

target_include_directories(cdns PRIVATE .)

install(TARGETS cdns
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/src/cdns.pc 
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)
