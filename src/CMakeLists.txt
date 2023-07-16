set(SOURCES
        Merlin.cpp
        SocketServer.cpp
        Connection.cpp
        Global.cpp
        External.cpp
        )

add_library(source ${SOURCES})
target_link_libraries(source PUBLIC Lancelot Greeks)
