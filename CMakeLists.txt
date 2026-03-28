cmake_minimum_required(VERSION 3.10)
project(arch_bootstrap)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(arch_bootstrap
    main.cpp
    base_package-instal/base_package-instal.cpp
    check_internet_connected/check_internet_connected.cpp
    create_script/create_script.cpp
    disk_partitioning/disk_partitioning.cpp
)

target_include_directories(arch_bootstrap PUBLIC
    base_package-instal
    check_internet_connected
    create_script
    disk_partitioning
)
