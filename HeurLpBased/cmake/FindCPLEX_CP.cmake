# ----------------------------------------------------------------------------
# CP Optimizer - depends on Concert

set(CPLEX_CP_DIR ${CPLEX_STUDIO_DIR}/../cpoptimizer)

# Find the CP Optimizer include directory.
find_path(CPLEX_CP_INCLUDE_DIR ilcp/cp.h PATHS ${CPLEX_CP_DIR}/include)

# Find the CP Optimizer library.
find_cplex_library(CPLEX_CP_LIBRARY cp ${CPLEX_CP_DIR})

if (WIN32)
    set(CPLEX_CP_EXTRA_LIBRARIES Ws2_32.lib)
endif ()

# Handle the QUIETLY and REQUIRED arguments and set CPLEX_CP_FOUND to TRUE
# if all listed variables are TRUE.
find_package_handle_standard_args(
        CPLEX_CP DEFAULT_MSG CPLEX_CP_LIBRARY CPLEX_CP_LIBRARY_DEBUG
        CPLEX_CP_INCLUDE_DIR CPLEX_CONCERT_FOUND)

mark_as_advanced(CPLEX_CP_LIBRARY CPLEX_CP_LIBRARY_DEBUG CPLEX_CP_INCLUDE_DIR)

if (CPLEX_CP_FOUND AND NOT TARGET cplex-cp)
    add_library(cplex-cp STATIC IMPORTED GLOBAL)
    set_target_properties(cplex-cp PROPERTIES
            IMPORTED_LOCATION "${CPLEX_CP_LIBRARY}"
            IMPORTED_LOCATION_DEBUG "${CPLEX_CP_LIBRARY_DEBUG}"
            INTERFACE_INCLUDE_DIRECTORIES "${CPLEX_CP_INCLUDE_DIR}"
            INTERFACE_LINK_LIBRARIES "cplex-concert;${CPLEX_CP_EXTRA_LIBRARIES}")
endif ()
