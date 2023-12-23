
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(Merlin
        VERSION 1.0
        LANGUAGES C CXX
)

set(WARNINGS_AS_ERRORS FALSE)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(${PROJECT_NAME}_OPTIONS INTERFACE)
target_compile_features(${PROJECT_NAME}_OPTIONS INTERFACE cxx_std_20)

if (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if (ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif ()
endif ()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(${PROJECT_NAME}_WARNINGS INTERFACE)

# enable cache system
set(ENABLE_CACHE OFF)
include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(${PROJECT_NAME}_WARNINGS)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(${PROJECT_NAME}_OPTIONS)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which is a good idea if any #define's change
    #
    # consider breaking this out per project as necessary
    target_precompile_headers(
            ${PROJECT_NAME}_OPTIONS
            INTERFACE
            <vector>
            <string>
            <map>
            <utility>)
endif ()


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


add_subdirectory(third_party/Lancelot)
add_subdirectory(third_party/Greeks)
add_subdirectory(third_party/Gawain)

add_subdirectory(API)
add_subdirectory(include)
add_subdirectory(src)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} INTERFACE include)
target_link_libraries(${PROJECT_NAME} PRIVATE source API)
target_link_libraries(${PROJECT_NAME} PUBLIC Boost::filesystem)
target_link_libraries(${PROJECT_NAME} PRIVATE Lancelot Gawain)
target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}_OPTIONS ${PROJECT_NAME}_WARNINGS)

add_subdirectory(Demo)

foreach (target "API" "source" ${PROJECT_NAME} "DemoAdaptor" "ButterFly")
    target_include_directories(${target} PUBLIC ${Boost_INCLUDE_DIRS})
    target_include_directories(${target} PRIVATE third_party/Lancelot/include/)
    target_include_directories(${target} PRIVATE third_party/Greeks/)
    target_include_directories(${target} PRIVATE third_party/Gawain/include)
    target_link_libraries(${target} INTERFACE ${PROJECT_NAME}_OPTIONS ${PROJECT_NAME}_WARNINGS)
endforeach ()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/merlin.json ${PROJECT_BINARY_DIR}/
)
