set(HEADER
        Merlin.hpp
        SocketServer.hpp
        Connection.hpp
        Logger.hpp
        Structure.hpp
        Enums.hpp
        Global.hpp
        Compression.h
        )

add_library(include INTERFACE ${HEADER})
set_target_properties(include PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(include INTERFACE Boost::headers fmt::fmt)
