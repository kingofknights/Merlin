
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Merlin
        VERSION 1.0
        LANGUAGES C CXX
        )


set(CMAKE_CXX_CPPCHECK "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_ENABLE_EXPORTS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(Boost_NO_WARN_NEW_VERSIONS ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads)
find_package(nlohmann_json REQUIRED)
find_package(fmt REQUIRED)
find_package(SQLiteCpp REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem)
include_directories(${Boost_INCLUDE_DIRS})

include(FetchContent)
FetchContent_Declare(
        xcd
        GIT_REPOSITORY https://github.com/kingofknights/xcd.git
        OVERRIDE_FIND_PACKAGE
)

FetchContent_GetProperties(xcd)
if (NOT xcd_POPULATED)
    FetchContent_Populate(xcd)
    add_subdirectory(${xcd_SOURCE_DIR} ${xcd_BINARY_DIR})
    include_directories(${xcd_SOURCE_DIR})
    message(STATUS "xcd_SOURCE_DIR" ${xcd_SOURCE_DIR})
    message(STATUS "xcd_BINARY_DIR" ${xcd_BINARY_DIR})
endif ()

FetchContent_Declare(
        lancelot
        GIT_REPOSITORY https://github.com/kingofknights/Lancelot.git
        OVERRIDE_FIND_PACKAGE
)

FetchContent_GetProperties(lancelot)
if (NOT lancelot_POPULATED)
    FetchContent_Populate(lancelot)
    add_subdirectory(${lancelot_SOURCE_DIR} ${lancelot_BINARY_DIR})
    include_directories(${lancelot_SOURCE_DIR}/include)
    message(STATUS "lancelot_SOURCE_DIR" ${lancelot_SOURCE_DIR})
    message(STATUS "lancelot_BINARY_DIR" ${lancelot_BINARY_DIR})
endif ()

add_subdirectory(API)
add_subdirectory(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} PRIVATE source API)
target_link_libraries(${PROJECT_NAME} PUBLIC fmt::fmt Boost::filesystem xcd)
target_link_libraries(${PROJECT_NAME} PRIVATE Lancelot SQLiteCpp)


add_subdirectory(Demo)
