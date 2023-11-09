
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


add_subdirectory(repo/Lancelot)
add_subdirectory(repo/Greeks)
add_subdirectory(repo/Gawain)

add_subdirectory(API)
add_subdirectory(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} PRIVATE source API)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Lancelot Gawain)

add_subdirectory(Demo)

foreach (target "API" "source" ${PROJECT_NAME} "DemoAdaptor" "ButterFly")
    target_include_directories(${target} PUBLIC ${Boost_INCLUDE_DIRS})
    target_include_directories(${target} PRIVATE repo/Lancelot/include/)
    target_include_directories(${target} PRIVATE repo/Greeks/)
    target_include_directories(${target} PRIVATE repo/Gawain/include)
endforeach ()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/merlin.json ${PROJECT_BINARY_DIR}/
)