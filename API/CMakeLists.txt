set(FILES
        Adaptor.cpp
        Strategy.cpp
        )

add_library(API ${FILES})
target_link_libraries(API PUBLIC fmt::fmt)
