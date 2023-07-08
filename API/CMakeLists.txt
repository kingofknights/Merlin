set(FILES
        Adaptor.cpp
        Adaptor.hpp
        Strategy.cpp
        Strategy.hpp
        OrderUtility.cpp
        OrderUtility.hpp)

add_library(API ${FILES})
target_link_libraries(API PUBLIC fmt::fmt)
