# - Try to find libcbor include dirs and libraries
#
# Usage of this module as follows:
#
#   find_package(CBOR)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  CBOR_ROOT_DIR             Set this variable to the root installation of
#                            libcbor if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  CBOR_FOUND                System has libcbor, include and library dirs found
#  CBOR_INCLUDE_DIR          The libcbor include directories.
#  CBOR_LIBRARY              The libcbor library

find_path(CBOR_ROOT_DIR NAMES include/cbor.h)
find_path(CBOR_INCLUDE_DIR NAMES cbor.h HINTS ${CBOR_ROOT_DIR}/include)
find_library(CBOR_LIBRARY NAMES cbor HINTS ${CBOR_ROOT_DIR}/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CBOR DEFAULT_MSG CBOR_ROOT_DIR)

mark_as_advanced(
    CBOR_ROOT_DIR
    CBOR_INCLUDE_DIR
    CBOR_LIBRARY
)

if(CBOR_FOUND)
    add_library(CBOR::CBOR INTERFACE IMPORTED)
    set_property(TARGET CBOR::CBOR PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${CBOR_INCLUDE_DIR})
    set_property(TARGET CBOR::CBOR PROPERTY INTERFACE_LINK_LIBRARIES ${CBOR_LIBRARY})
endif()