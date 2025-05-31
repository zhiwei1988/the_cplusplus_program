set(SRC main.cpp auto_clean.cpp)

add_executable(app ${SRC})

ADD_GLOBAL_COMPILE_OPTION(app)

target_link_libraries(app pthread)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# 设置target输出目录
set_target_properties(app PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_BIN})