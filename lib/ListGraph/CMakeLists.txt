add_library(ListGraph ListGraph.cpp)
target_include_directories(ListGraph PUBLIC ./)
target_link_libraries(ListGraph IGraph)
