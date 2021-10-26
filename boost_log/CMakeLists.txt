cmake_minimum_required(VERSION 3.10)

project(boost_log CXX)

# Compile as C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# we will use static libs
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost REQUIRED COMPONENTS
    filesystem
    log
    log_setup
    regex
    system
)

find_package(Threads)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

set(SRC_FILES main.cpp)

message(STATUS "==========")
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
message(STATUS "==========")

add_executable(boost_log_exec ${SRC_FILES}
)
target_link_libraries(boost_log_exec
    # Unfortunately, in ${Boost_LIBRARIES}, `boost_log_setup` follows
    # `boost_log` rather than preceeds it, hence the link errors.
    # ${Boost_LIBRARIES}
    #
    # In order to link successfully, we need to put `log_setup` before `log`.
    # The CMake target `Boost::log` contains all the needed library files so
    # we don't have to list them explicitly. Use `cmake --build . --verbose`
    # to see the underlying command lines.
    Boost::log_setup
    Boost::log
    Threads::Threads
)
