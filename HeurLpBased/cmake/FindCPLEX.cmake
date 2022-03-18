# Thanks to https://github.com/ampl/mp/blob/master/support/cmake/FindCPLEX.cmake
# Try to find the CPLEX, Concert, IloCplex and CP Optimizer libraries.
#
# Once done this will add the following imported targets:
#
#  cplex-library - the CPLEX library
#  cplex-concert - the Concert library
#  ilocplex - the IloCplex library
#  cplex-cp - the CP Optimizer library

include(FindPackageHandleStandardArgs)

# Find the path to CPLEX Studio.
# CPLEX Studio 12.4 can be installed in the following default locations:
#   /opt/ibm/ILOG/CPLEX_Studio<edition>124 - Linux
#   /opt/IBM/ILOG/CPLEX_Studio<edition>124 - UNIX
#   ~/Applications/IBM/ILOG/CPLEX_Studio<edition>124 - Mac OS X
#   C:\Program Files\IBM\ILOG\CPLEX_Studio<edition>124 - Windows
set(CPLEX_ILOG_DIRS /opt/ibm/ILOG /opt/IBM/ILOG /opt /volper/users/wachengh/dependances /Applications/CPLEX_Studio1210)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CPLEX_ARCH x86-64)
else ()
    set(CPLEX_ARCH x86)
endif ()

set(CPLEX_LIB_PATH_SUFFIXES lib/${CPLEX_ARCH}_sles10_4.1/static_pic lib/${CPLEX_ARCH}_linux/static_pic lib/x86-64_osx/static_pic)

if (NOT CPLEX_STUDIO_DIR)
    foreach (dir ${CPLEX_ILOG_DIRS})
        file(GLOB CPLEX_STUDIO_DIRS "${dir}/CPLEX_Studio*" "${dir}/cplex*")
        list(SORT CPLEX_STUDIO_DIRS)
        list(REVERSE CPLEX_STUDIO_DIRS)
        if (CPLEX_STUDIO_DIRS)
            list(GET CPLEX_STUDIO_DIRS 0 CPLEX_STUDIO_DIR_)
            message(STATUS "Found CPLEX Studio: ${CPLEX_STUDIO_DIR_}")
            break ()
        endif ()
    endforeach ()
    if (NOT CPLEX_STUDIO_DIR_)
        set(CPLEX_STUDIO_DIR_ CPLEX_STUDIO_DIR-NOTFOUND)
        message(STATUS "Not Found CPLEX Studio!")
    endif ()
    set(CPLEX_STUDIO_DIR ${CPLEX_STUDIO_DIR_} CACHE PATH
        "Path to the CPLEX Studio directory")
endif ()


find_package(Threads)

# ----------------------------------------------------------------------------
# CPLEX

set(CPLEX_CONCERT_DIR ${CPLEX_STUDIO_DIR}/concert)
if (APPLE)
    set(CPLEX_DIR ${CPLEX_STUDIO_DIR})
endif()
# Find the CPLEX include directory.
find_path(CPLEX_INCLUDE_DIR ilcplex/cplex.h PATHS ${CPLEX_DIR}/include)

macro(find_win_cplex_library var path_suffixes)
    foreach (s ${path_suffixes})
        file(GLOB CPLEX_LIBRARY_CANDIDATES "${CPLEX_DIR}/${s}/cplex*.lib")
        if (CPLEX_LIBRARY_CANDIDATES)
            list(GET CPLEX_LIBRARY_CANDIDATES 0 ${var})
            break ()
        endif ()
    endforeach ()
    if (NOT ${var})
        set(${var} NOTFOUND)
    endif ()
endmacro()

# Find the CPLEX library.
if (UNIX)
    find_library(CPLEX_LIBRARY NAMES cplex
        PATHS ${CPLEX_DIR} PATH_SUFFIXES ${CPLEX_LIB_PATH_SUFFIXES})
    set(CPLEX_LIBRARY_DEBUG ${CPLEX_LIBRARY})
elseif (NOT CPLEX_LIBRARY)
    # On Windows the version is appended to the library name which cannot be
    # handled by find_library, so search manually.
    find_win_cplex_library(CPLEX_LIB "${CPLEX_LIB_PATH_SUFFIXES}")
    set(CPLEX_LIBRARY ${CPLEX_LIB} CACHE FILEPATH "Path to the CPLEX library")
    find_win_cplex_library(CPLEX_LIB "${CPLEX_LIB_PATH_SUFFIXES_DEBUG}")
    set(CPLEX_LIBRARY_DEBUG ${CPLEX_LIB} CACHE
        FILEPATH "Path to the debug CPLEX library")
    if (CPLEX_LIBRARY MATCHES ".*/(cplex.*)\\.lib")
        file(GLOB CPLEX_DLL_ "${CPLEX_DIR}/bin/*/${CMAKE_MATCH_1}.dll")
        set(CPLEX_DLL ${CPLEX_DLL_} CACHE PATH "Path to the CPLEX DLL.")
    endif ()
endif ()

# Handle the QUIETLY and REQUIRED arguments and set CPLEX_FOUND to TRUE
# if all listed variables are TRUE.
find_package_handle_standard_args(
    CPLEX DEFAULT_MSG CPLEX_LIBRARY CPLEX_LIBRARY_DEBUG CPLEX_INCLUDE_DIR)

mark_as_advanced(CPLEX_LIBRARY CPLEX_LIBRARY_DEBUG CPLEX_INCLUDE_DIR)

if (CPLEX_FOUND AND NOT TARGET cplex-library)
    set(CPLEX_LINK_LIBRARIES ${CMAKE_THREAD_LIBS_INIT})
    check_library_exists(m floor "" HAVE_LIBM)
    if (HAVE_LIBM)
        set(CPLEX_LINK_LIBRARIES ${CPLEX_LINK_LIBRARIES} m dl)
    endif ()
    add_library(cplex-library STATIC IMPORTED GLOBAL)
    set_target_properties(cplex-library PROPERTIES
        IMPORTED_LOCATION "${CPLEX_LIBRARY}"
        IMPORTED_LOCATION_DEBUG "${CPLEX_LIBRARY_DEBUG}"
        INTERFACE_INCLUDE_DIRECTORIES "${CPLEX_INCLUDE_DIR}"
        INTERFACE_LINK_LIBRARIES "${CPLEX_LINK_LIBRARIES}")
endif ()

