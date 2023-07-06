add_library(DemoAdaptor SHARED
	DemoAdaptor.cpp
	DemoAdaptor.hpp)

target_include_directories(DemoAdaptor PUBLIC ../API)
set_property(TARGET DemoAdaptor PROPERTY ENABLE_EXPORTS ON)

#[[
add_library(DemoStrategy SHARED
	DemoStrategy.cpp
	DemoStrategy.hpp
)

target_include_directories(DemoStrategy PUBLIC ../API)
set_property(TARGET DemoStrategy PROPERTY ENABLE_EXPORTS ON)
]]
