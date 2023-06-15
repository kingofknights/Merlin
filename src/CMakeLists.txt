set (SOURCES
	    Merlin.cpp
		SocketServer.cpp
		Connection.cpp
		Global.cpp
		Compression.cpp
		Strategy.cpp
		External.cpp
	)

add_library(source ${SOURCES})
target_link_libraries(source PUBLIC xcd)
