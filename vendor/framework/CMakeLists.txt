add_library(framework)

target_include_directories(framework PUBLIC .)

target_sources(framework PUBLIC
        framework.cpp)

find_package(GLEW REQUIRED)
target_link_libraries(framework PUBLIC GLEW::GLEW)

find_package(FreeGLUT CONFIG REQUIRED)
target_link_libraries(framework PUBLIC
        $<IF:$<TARGET_EXISTS:FreeGLUT::freeglut>,FreeGLUT::freeglut,FreeGLUT::freeglut_static>)
