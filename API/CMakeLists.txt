set(FILES
	    Adaptor.cpp
		Adaptor.hpp OrderUtility.cpp OrderUtility.hpp)
add_library(API ${FILES})
target_link_libraries(API PUBLIC fmt::fmt xcd)
