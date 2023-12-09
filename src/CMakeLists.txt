set(SOURCES
        Merlin.cpp
        SocketServer.cpp
        Connection.cpp
        Global.cpp
        External.cpp
        ManualOrder.cpp)

add_library(source ${SOURCES})
target_link_libraries(source PUBLIC Lancelot Greeks Gawain)
target_link_libraries(source INTERFACE ${PROJECT_NAME}_OPTIONS ${PROJECT_NAME}_WARNINGS)
