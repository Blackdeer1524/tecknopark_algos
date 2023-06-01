add_library(SetGraph SetGraph.cpp)
target_include_directories(SetGraph PUBLIC ./)
target_link_libraries(SetGraph IGraph)
