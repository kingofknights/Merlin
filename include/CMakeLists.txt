set(HEADER
        Merlin.hpp
        SocketServer.hpp
        Connection.hpp
        Structure.hpp
        Global.hpp
        ManualOrder.hpp)

add_library(include INTERFACE ${HEADER})
set_target_properties(include PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(include INTERFACE Boost::headers fmt::fmt)
target_link_libraries(include INTERFACE ${PROJECT_NAME}_OPTIONS ${PROJECT_NAME}_WARNINGS)