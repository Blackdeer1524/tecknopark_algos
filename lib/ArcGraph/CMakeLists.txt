add_library(ArcGraph ArcGraph.cpp)
target_include_directories(ArcGraph PUBLIC ./)
target_link_libraries(ArcGraph IGraph)
