add_executable(c03_TLS1 main.cpp)

target_compile_features(c03_TLS1 PRIVATE cxx_std_23)

set_target_properties(c03_TLS1 PROPERTIES
    OUTPUT_NAME "c03_TLS1"
)
