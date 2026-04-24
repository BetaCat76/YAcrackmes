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
endif()

# ── Crackme targets ──────────────────────────────────────────────────────────
# To add a new crackme, create a subdirectory under crackmes/ and add an
# add_subdirectory() call here pointing to it.  See README.md for details.

add_subdirectory(crackmes/c01_helloworld)
