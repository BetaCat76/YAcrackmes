add_executable(c01_helloworld main.cpp)

target_compile_features(c01_helloworld PRIVATE cxx_std_23)

set_target_properties(c01_helloworld PROPERTIES
    OUTPUT_NAME "c01_helloworld"
)
