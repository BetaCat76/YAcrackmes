cmake_minimum_required(VERSION 3.28)

project(YAcrackmes LANGUAGES CXX)

# Require Windows
if(NOT WIN32)
    message(FATAL_ERROR "This project is Windows-only.")
endif()

# Global C++23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MSVC-specific flags: UTF-8 source and execution charset, warnings
if(MSVC)
    add_compile_options(
        /utf-8          # Source and execution charset: UTF-8
        /W4             # High warning level
        /WX-            # Warnings are not errors (relax for crackme dev)
        /EHsc           # Standard C++ exception handling
    )

    # Release build: strip all symbols and debug information
    # Remove any debug-info compile flags that CMake may inject into Release
    foreach(_flag /Zi /ZI /Z7)
        string(REPLACE "${_flag}" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
        string(REPLACE "${_flag}" "" CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}")
    endforeach()

    # Linker: no PDB, dead-code elimination, identical-COMDAT folding
    add_link_options(
        $<$<CONFIG:Release>:/DEBUG:NONE>
        $<$<CONFIG:Release>:/OPT:REF>
        $<$<CONFIG:Release>:/OPT:ICF>
    )
endif()

# ── Crackme targets ──────────────────────────────────────────────────────────
# To add a new crackme, create a subdirectory under crackmes/ and add an
# add_subdirectory() call here pointing to it.  See README.md for details.

add_subdirectory(crackmes/c01_helloworld)
add_subdirectory(crackmes/c02_vtable)
