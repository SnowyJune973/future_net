/*
 * AuthorList: Danielhu
 *             email: yiminghu@hustunique.com
 *             Copyright 2016 yiminghu
 */
#include "Graph.h"
#include <string>
#include <cstdlib>
#include <set>

#include "Graph.h"
#include <string>
#include <cstdlib>
#include <stack>
#include <algorithm>
#include <cassert>
#include <sys/timeb.h>

void Graph::init() {
    for (int i = 0; i < 600; ++i) {
        for (int j = 0; j < 600; ++j) {
            _min_cost[i][j] = 200000;
            _weights[i][j] = 200000;
            _close_path[i][j] = -1;
        }
        _weights[i][i] = 0;
        _min_cost[i][i] = 0;
        _solution_visited[i] = false;
        _route_points[i] = false;
    }
    _min_solution_cost = 2000;
    //_min_sub_cost = 600;
    _current_cost = 0;
}

void Graph::add_edge(char *edge) {
    std::vector<std::string> infos = split(edge, ',');
    int from = atoi(infos[1].c_str());
    int to = atoi(infos[2].c_str());
    int weight = atoi(infos[3].c_str());
    if (weight < _weights[from][to]) {
        _points[to]._from.push_back(from);
        _points[from]._to.push_back(to);
        _size = to + 1 > _size ? to + 1 : _size;
        _size = from + 1 > _size ? from + 1 : _size;
        _edge_indices[from][to] = atoi(infos[0].c_str());
        _weights[from][to] = weight;
        _edges[_edge_indices[from][to]] = std::make_pair(from, to);
    }
}


void Graph::add_route(char *route) {
    std::vector<std::string> route_info = split(route, ',');

    _start = atoi(route_info[0].c_str());
    _end = atoi(route_info[1].c_str());

    std::vector<std::string> route_points = split(route_info[2].c_str(), '|');

    _route_unvisited_num = route_points.size();
    for (auto point : route_points) {
        _route_points[atoi(point.c_str())] = 1;
        _routes.insert(atoi(point.c_str()));
    }

    for (int i = 0; i < _size; ++i) {
        _min_cost[i][i] = 0;
    }
}


bool Graph::is_route_token() {
    return false;
}
void Graph::fill_route(int index) {
    _route_points[index] = 1;
    _route_unvisited_num--;
}
void Graph::reset_route(int index) {
    _route_points[index] = 0;
    _route_unvisited_num++;
}

std::vector<std::string> Graph::split(const char *origin, char division) {
    int index(0);
    std::vector<std::string> result;
    std::string item;
    while (origin[index] != '\n' && origin[index] != EOF && origin[index] != '\0') {
        if (origin[index] == division) {
            result.push_back(item);
            item = "";
        } else {
            item += origin[index];
        }
        index++;
    }
    result.push_back(item);
    return result;
}

void Graph::run_dijkstra_forward(const int start) {
    printf("run dijkstra %d \n", start);
    Less_Forward less(this, start);
    std::priority_queue<int,std::vector<int>, Less_Forward> visited(less);
    //std::set<int, Less_Forward> visited(less);
    _min_cost[start][start] = 0;
    visited.push(start);
    while (!visited.empty()) {
        int least = visited.top();
        visited.pop();
        for (auto next : _points[least]._to) {
            if (release(start, least, next)) {
                //if (!_solution_visited[next]) {
                    visited.push(next);
                //}
            }

        }
    }
}

void Graph::run_dijkstra_backward(int end) {
    Less_Backward less(this, end);
    std::priority_queue<int,std::vector<int>, Less_Backward> unvisited(less);
    _min_cost[end][end] = 0;
    for (int i = 0; i < _size; ++i) {
        if (!_solution_visited[i]) {
            unvisited.push(i);
        }
    }
    while (!unvisited.empty()) {
        int least = unvisited.top();
        for (auto next : _points[least]._from){
            if (_min_cost[least][end] + _weights[least][end] < _min_cost[next][end]) {
                _min_cost[next][end] = _min_cost[least][end] + _weights[next][least];
                _close_path[next][end] = least;
            }
        }
        unvisited.pop();
    }
}
bool Graph::search_forward(int start) {
    Less_Forward less(this, start);
    std::priority_queue<int, std::vector<int>, Less_Forward> next_closed(less);
    if (_route_unvisited_num == 0) {
        if (_min_cost[start][_end] < 200000) {
            //_changing_times = 0;
            if(push_solution_forward(_end)){
                splice_solution();
                if (_current_cost < _min_solution_cost){
                    _best_sulotion = _solution_start_side;
                    _min_solution_cost = _current_cost ;
                }
                printf("solution found!, time:%d, cost:%d\n", get_doration(), _current_cost);
                if (_end_flag){
                    return true;
                }
                /*
                if (get_doration() > 9800){
                    return true;
                }*/
                pop_solution_forward(start);
            }
        }
        return false;
    }

    for (int i = 0; i < _size; ++i) {
        if (_route_points[i] && !_solution_visited[i]) {
            if (_min_cost[start][i] < 200000) {
                next_closed.push(i);
            }
        }
    }
    while (!next_closed.empty()) {
        int next = next_closed.top();
        next_closed.pop();
        if (_end_flag){
            return true;
        }
        if (push_solution_forward(next)) {
            _route_unvisited_num--;
            splice_solution();
            if(search_forward(next)){
                return true;
            }
            _route_unvisited_num++;
            pop_solution_forward(start);
        }
    }
    return false;
}
bool Graph::push_solution_forward(int target) {

    /*
    if (get_doration() > 9.5f) {
        return false;
    }
    */

    if (_solution_visited[target]) {
        //_changing_times++;
        return false;
    }

    if (!_sub_solution.empty()) {
        _current_sub_cost += _weights[target][_sub_solution.front()];
    }

    if (_current_sub_cost + _current_cost > _min_solution_cost) {
        if (!_sub_solution.empty()) {
            _current_sub_cost -= _weights[target][_sub_solution.front()];
        }
        return false;
    }

    _solution_visited[target] = 1;
    _sub_solution.push_front(target);
    Route_Greater route_greater(this, _solution_start_side.back(), target);
    int test_size = std::min((int)_points[target]._from.size(), 2);
    std::sort(_points[target]._from.begin(), _points[target]._from.end(), route_greater);
    std::vector<int> froms(_points[target]._from.begin(), _points[target]._from.begin()+test_size);
    for (auto from : froms){
        /*
        if (get_doration() > 9.5f){
            return false;
        }
         */
        if (_solution_start_side.back() == from) {
            _current_sub_cost += _weights[from][target];
            if (_current_sub_cost + _current_cost > _min_solution_cost){
                _current_sub_cost -= _weights[from][target];
                continue;
            }
            return true;
        }
        if(_route_points[from]){
            continue;
        }
        if (push_solution_forward(from)) {
            return true;
        }
    }
    _solution_visited[target] = 0;
    _sub_solution.pop_front();
    if (!_sub_solution.empty()) {
        _current_sub_cost -= _weights[target][_sub_solution.front()];
    }
    return false;
}
void Graph::pop_solution_forward(int origin) {
    auto curr = _solution_start_side.end();
    curr--;
    auto next = curr;
    next--;
    while(*curr != origin) {
        _current_cost -= _weights[*next][*curr];
        curr--;
        next--;
    }
    while (_solution_start_side.back() != origin) {
        _solution_visited[_solution_start_side.back()] = 0;
        _solution_start_side.pop_back();
    }
}

bool Graph::go_to_forward(int start, int end) {
    _min_cost[start][end] = 200000;
    _close_path[start][end] = -1;
    run_dijkstra_forward(start);
    if (_min_cost[start][end] == 200000) {
        return false;
    }
    push_solution_forward(end);
    return true;
}

bool Graph::release(int source, int start, int end) {
    if (_min_cost[source][start] + _weights[start][end] <= _min_cost[source][end]) {
        _min_cost[source][end] = _min_cost[source][start] + _weights[start][end];
        _close_path[source][end] = start;
        return true;
    }
    return false;
}
void Graph::search() {
    run_dijkstra_forward(_start);
    for(auto route : _routes) {
        run_dijkstra_forward(route);
    }
    printf("dij time:%d\n", get_doration());
    _solution_start_side.push_back(_start);
    //_solution_visited[_start] = 1;
    search_forward(_start);
}
void Graph::splice_solution() {
    _current_cost += _current_sub_cost;
    _current_sub_cost = 0;
    //_min_sub_cost = 600;
    _solution_start_side.splice(_solution_start_side.end(), _sub_solution);
    _sub_solution.clear();
    //_best_sub_solution.clear();
}
int Graph::get_doration() {
    struct timeb current_time;
    ftime(&current_time);
    return  (int) (1000.0 * (current_time.time - _begin_time.time)
        + (current_time.millitm - _begin_time.millitm));
}
