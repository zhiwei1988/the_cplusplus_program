# 获取当前工程根目录
#EXECUTE_PROCESS(COMMAND git rev-parse --show-toplevel
#        COMMAND tr -d '\n'
#        OUTPUT_VARIABLE CUR_PROJ_ROOT)

# 添加全局编译器选项
macro(ADD_GLOBAL_COMPILE_OPTION object)
# 自定义选项增加
# 1、调试选项
# 2、打开所有warning
# 3、增加不带路径的宏定义
target_compile_options(${object} PRIVATE -g -Wall -Werror)

# 公司要求的安全编译选项
get_target_property(TARGET_TYPE ${object} TYPE)
if(TARGET_TYPE STREQUAL "EXECUTABLE")
    # 可执行程序选项
    target_compile_options(${object} PRIVATE -fPIE -pie -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now)
else()
    # 库选项
    target_compile_options(${object} PRIVATE -fPIC -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now)
endif()

if(CMAKE_C_COMPILER_VERSION GREATER_EQUAL 4.9)
    target_compile_options(${object} PRIVATE -fstack-protector-strong)
else()
    target_compile_options(${object} PRIVATE -fstack-protector-all)
endif()
endmacro()