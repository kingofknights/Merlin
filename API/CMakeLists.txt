set(FILES
        Adaptor.cpp
        Strategy.cpp
)

add_library(API ${FILES})
target_link_libraries(API PUBLIC fmt::fmt)
target_link_libraries(API PRIVATE Gawain)
target_link_libraries(API INTERFACE ${PROJECT_NAME}_OPTIONS ${PROJECT_NAME}_WARNINGS)
