set (SOURCES
	    Merlin.cpp
		SocketServer.cpp
		Connection.cpp
		Global.cpp
		Compression.cpp
	)

add_library(source ${SOURCES})
target_link_libraries(source PUBLIC XCD)
