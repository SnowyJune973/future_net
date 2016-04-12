#include "route.h"
#include "lib/lib_record.h"
#include "Graph.h"

//你要完成的功能总入口


void search_route(char *topo[5000], int edge_num, char *demand) {
    Graph* graph = new Graph();
    graph->init();
    for (int i = 0; i < edge_num; ++i) {
        graph->add_edge(topo[i]);
    }
    graph->add_route(demand);
    graph->search();

    std::vector<int> result_vec;

    if (graph->_best_sulotion.empty()){
        return;
    }

    for (auto i : graph->_best_sulotion) {
        result_vec.push_back(i);
    }
    int cost = 0;
    for (int i = 0; i < result_vec.size() - 1; ++i) {
        int start = result_vec[i];
        int end = result_vec[i + 1];
        cost += graph->_weights[start][end];
        record_result(graph->_edge_indices[start][end]);
    }
    printf("cost:%d\n", graph->_min_solution_cost);
    delete graph;

    /*
    while (startItr != graph._solution_start_side.end()) {
        auto next = startItr;
        next++;
        if (next == graph._solution_start_side.end()){
            break;
        }
        int edge_index = graph._edge_indices[*startItr][*next];
        record_result(edge_index);
    }

    while (endItr != graph._solution_end_side.begin()) {
        endItr--;
        auto next = endItr;
        next--;
        int edge_index = graph._edge_indices[*startItr][*next];
        record_result(edge_index);
    }
     */

}
