cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 11)

project(Homework1)

add_executable(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_sources(${PROJECT_NAME} PRIVATE src/Skeleton.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Werror
#        -Wextra
)

add_subdirectory(vendor/framework)
target_link_libraries(${PROJECT_NAME} PRIVATE framework)
