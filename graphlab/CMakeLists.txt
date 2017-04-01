project(GraphLab)


add_graphlab_executable(s_pagerank s_pagerank.cpp)

add_graphlab_executable(warp_parfor_pagerank warp_parfor_pagerank.cpp)

add_graphlab_executable(warp_engine_pagerank warp_engine_pagerank.cpp)
