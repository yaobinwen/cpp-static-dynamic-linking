cmake_minimum_required(VERSION 3.10)

project(cpp-static-dynamic-linking CXX)

# Compile as C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(simplecalc_static STATIC
    simplecalc.cpp
)

add_library(simplecalc_dynamic SHARED
    simplecalc.cpp
)

add_library(simplestr_static STATIC
    simplestr.cpp
)

add_library(simplestr_dynamic SHARED
    simplestr.cpp
)

# Use static linking
add_executable(main_cmake_static_exe
    main.cpp
)
target_link_libraries(main_cmake_static_exe
    simplecalc_static
    simplestr_static
)

# Use dynamic linking
add_executable(main_cmake_dynamic_exe
    main.cpp
)
target_link_libraries(main_cmake_dynamic_exe
    simplecalc_dynamic
    simplestr_dynamic
)

# Use mixed linking
add_executable(main_cmake_mix_exe
    main.cpp
)
target_link_libraries(main_cmake_mix_exe
    simplecalc_static
    simplestr_dynamic
)
