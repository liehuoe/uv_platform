cmake_minimum_required(VERSION 3.5.0)
project(uv_platform VERSION 1.0.0 LANGUAGES C CXX)
# C++ 20 标准
set(CMAKE_CXX_STANDARD 20)
if(MSVC)
    # 解决 warning C4819
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()
# 静态连接 C/C++ 的标准库, 减少对系统环境的依赖
if(MSVC)
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()
# liuv-static: 核心依赖
find_package(libuv CONFIG REQUIRED)
link_libraries(libuv::uv_a)
include_directories(utils)
# projects/CMakeLists.txt: 配置每个项目共同依赖的库或定义
if(EXISTS projects/CMakeLists.txt)
    add_subdirectory(projects)
endif()
# projects/ 下的一个目录视为独立的一个项目
file(GLOB PROJECTS "projects/*")
foreach(PROJECT ${PROJECTS})
    message(STATUS "Include project : ${PROJECT}")
    if(IS_DIRECTORY ${PROJECT})
        add_subdirectory(${PROJECT})
    endif()
endforeach()
