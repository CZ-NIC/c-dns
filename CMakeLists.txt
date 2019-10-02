cmake_minimum_required(VERSION 3.9)
project(cdns VERSION 0.0.1 DESCRIPTION "C-DNS format exporter")

include(GNUInstallDirs)

add_library(cdns SHARED
    cdns.c
)

target_link_libraries(cdns -lcbor)

set_target_properties(cdns PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(cdns PROPERTIES SOVERSION 0)
set_target_properties(cdns PROPERTIES PUBLIC_HEADER cdns.h)

configure_file(cdns.pc.in cdns.pc @ONLY)

target_include_directories(cdns PRIVATE .)

install(TARGETS cdns
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES ${CMAKE_BINARY_DIR}/cdns.pc 
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)