
cmake_minimum_required(VERSION 3.14)

project(Merlin
	    VERSION 1.0
		LANGUAGES C CXX
)


set(CMAKE_CXX_CPPCHECK "")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
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
message(STATUS ${Boost_INCLUDE_DIRS})

if (WIN32)
	    add_compile_definitions(WIN32_LEAN_AND_MEAN)
		add_compile_definitions(WINVER=0x0A00)
		add_compile_definitions(_WIN32_WINNT=0x0A00)
		add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
		add_compile_options(/bigobj)
		include_directories($ENV{MSVC_INCLUDE_PATH})

endif ()

add_subdirectory(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} PRIVATE source)
