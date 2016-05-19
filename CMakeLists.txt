# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (ThreadPool)

# set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)

add_compile_options(-std=c++11)
add_compile_options(-DBOOST_LOG_DYN_LINK)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(${PROJECT_SOURCE_DIR}/src DIR_SRCS)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/lib)

include_directories(${PROJECT_SOURCE_DIR}/src/include)
include_directories(${PROJECT_SOURCE_DIR}/src/lib)

# 指定生成目标
add_executable(main ${PROJECT_SOURCE_DIR}/src/main.cpp)
add_executable(test ${PROJECT_SOURCE_DIR}/src/test.cpp)

target_link_libraries(main tp pthread boost_log)
target_link_libraries(test tp pthread boost_log)