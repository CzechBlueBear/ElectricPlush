cmake_minimum_required(VERSION 2.8)

project(electricplush)

add_executable(electricplush camera.cpp app.cpp gl.cpp gl_error.cpp vao.cpp navigation_event_handler.cpp vbo.cpp shader_program.cpp shader.cpp event_handler.cpp main.cpp)
target_link_libraries(electricplush GL)
target_link_libraries(electricplush SDL2)
target_link_libraries(electricplush GLEW)
set_target_properties(electricplush PROPERTIES COMPILE_FLAGS "-std=c++11 -O2 -ggdb")

install(TARGETS electricplush RUNTIME DESTINATION bin)
