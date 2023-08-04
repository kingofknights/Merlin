add_library(DemoAdaptor SHARED
        DemoAdaptor.cpp
        DemoAdaptor.hpp)

target_include_directories(DemoAdaptor PUBLIC ../API)
set_property(TARGET DemoAdaptor PROPERTY ENABLE_EXPORTS ON)


add_library(ButterFly SHARED
        DemoStrategy.cpp
        DemoStrategy.hpp)

target_include_directories(ButterFly PUBLIC ../API)
set_property(TARGET ButterFly PROPERTY ENABLE_EXPORTS ON)

