/*
 * AuthorList: Danielhu
 *             email: yiminghu@hustunique.com
 *             Copyright 2016 yiminghu
 */
#ifndef FUTURE_NET_GRAPH_H
#define FUTURE_NET_GRAPH_H

#include <stddef.h>
#include <vector>
#include <map>
#include <list>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <chrono>
#include <sys/timeb.h>

struct Point {
    std::vector<int> _to;
    std::vector<int> _from;
    int lower_cost = -1;
};



class Graph {
private:

    struct Less_Forward{
        explicit Less_Forward(Graph* graph, int start)
            :_graph(graph), _start(start){}
        const Graph * _graph;
        int _start;
        bool operator()(int l, int r){
            return  _graph->_min_cost[_start][l] > _graph->_min_cost[_start][r];
        }
    };

  struct Route_Greater{
    explicit Route_Greater(Graph* graph, int source, int target)
        :_graph(graph), _source(source), _target(target){}
    const Graph * _graph;
    int _source;
    int _target;
    bool operator()(int l, int r){
      return  _graph->_min_cost[_source][l]+_graph->_weights[l][_target]
          < _graph->_min_cost[_source][r]+_graph->_weights[r][_target];
    }
  };

  struct Less_Backward{
    explicit Less_Backward(Graph* graph, int end)
        :_graph(graph), _end(end){}
    const Graph * _graph;
    int _end;
    bool operator()(int l, int r){
      return  _graph->_min_cost[l][_end] > _graph->_min_cost[r][_end];
    }
  };

    Point _points[600];
    std::set<int> _routes;
    int _route_points[600];
    int _solution_visited[600];
    int _size;
    int _route_unvisited_num;
public:
    Graph():_size(0), _end_flag(false){
        ftime(&_begin_time);
    }
    ~Graph(){};

    struct timeb _begin_time;

    int get_doration();



    std::list<int> _solution_start_side;
    std::list<int> _solution_end_side;

    std::list<int> _best_sulotion;

    std::list<int> _sub_solution;
    //std::list<int> _best_sub_solution;

    //int _changing_times;

    int _current_cost;
    int _min_solution_cost;

    int _current_sub_cost;
    //int _min_sub_cost;

    int _edge_indices[600][600];
    int _close_path[600][600];
    int _min_cost[600][600];
    int _weights[600][600];

    bool _end_flag;

    int _start, _end;

    std::map<int, std::pair<int, int> > _edges;

    void run_dijkstra_forward(int start);

    void run_dijkstra_backward(int end);

    bool push_solution_forward(int target);

    void pop_solution_forward(int origin);

    bool search_forward(int start);

    void search();

    bool go_to_forward(int start, int end);

    inline void splice_solution();

    bool release(int source, int start, int end);

    void init();

    std::vector<std::string> split(const char *origin, char division);
    void add_edge(char* edge);
    void add_route(char* route);
    bool is_route_token();
    void fill_route(int index);
    void reset_route(int index);
};


#endif //FUTURE_NET_GRAPH_H
