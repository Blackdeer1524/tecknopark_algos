add_executable(MatrixGraph MatrixGraph.cpp)
target_include_directories(MatrixGraph PUBLIC ./)
target_link_libraries(MatrixGraph IGraph)
