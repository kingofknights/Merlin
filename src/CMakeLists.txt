set(SOURCES
        Merlin.cpp
        SocketServer.cpp
        Connection.cpp
        Global.cpp
        External.cpp
        ManualOrder.cpp)

add_library(source ${SOURCES})
target_link_libraries(source PUBLIC Lancelot Greeks)
target_link_libraries(source INTERFACE project_options project_warnings)
