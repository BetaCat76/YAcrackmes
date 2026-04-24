add_executable(c02_vtable main.cpp)

target_compile_features(c02_vtable PRIVATE cxx_std_23)

set_target_properties(c02_vtable PROPERTIES
    OUTPUT_NAME "c02_vtable"
)
