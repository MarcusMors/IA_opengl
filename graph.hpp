#ifndef __GRAPH_H__
#define __GRAPH_H__

// Copyright (C) 2022 José Enrique Vilca Campana
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
#include <algorithm>
#include <cassert>
#include <cmath>
#include <ctime>//to include size_t
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <utility>
#include <vector>


#include "./enums/enums.hpp"


#define all(x) (x).begin(), (x).end()

using namespace std;


struct point
{
  int x{};
  int y{};
};


template<class graph_type> struct node
{
  using edge_type = typename graph_type::edge_type;
  using value_type = typename graph_type::node_value_type;

  node(value_type t_v) : value{ t_v } {}

  std::list<edge_type *> edges{};
  value_type value{};
};

template<class graph_type> struct edge
{
  using node_type = typename graph_type::node_type;
  using value_type = typename graph_type::edge_value_type;

  // edge(node_type *t_a, node_type *t_b, value_type t_e, bool t_d) : value{ t_e }, dir{ t_d }
  edge(node_type *t_a, node_type *t_b, value_type t_e, bool t_d) : value{ t_e }, dir{ t_d }
  {
    nodes[0] = t_a;
    nodes[1] = t_b;

    nodes[0]->edges.push_back(this);
    nodes[1]->edges.push_back(this);
  }

  node_type *nodes[2]{ nullptr, nullptr };// NOLINT C-style array
  // std::vector<node_type *> nodes{ nullptr, nullptr };// NOLINT C-style array
  // node_type *&first_end = &nodes[0];
  // node_type *&second_end = &nodes[1];
  value_type value{};
  bool dir;// 0=bidireccional, 1 = nodes[0]=>nodes[1]
};

template<class t_node_value_type, class t_edge_value_type> class graph
{
public:
  using node_value_type = t_node_value_type;
  using edge_value_type = t_edge_value_type;
  using graph_type = graph<node_value_type, edge_value_type>;
  using node_type = node<graph_type>;
  using edge_type = edge<graph_type>;

  node_type *insert_node(node_value_type t_n)
  {
    nodes.push_back(node_value_type{ t_n });
    node_type *ptr = &(*nodes.rbegin());
    return ptr;
  }

  bool insert_edge(node_type *t_a, node_type *t_b, edge_value_type t_e, bool t_direction = false)
  {
    new edge_type(t_a, t_b, t_e, t_direction);
    return true;
  }

  enum direction : bool {
    first_to_second,
    second_to_first,
  };

  ~graph()
  {
    // enter all nodes
    while (not nodes.empty()) {
      node_type &node = nodes.front();
      while (not node.edges.empty()) {
        edge_type *e = node.edges.front();
        node_type *opposite_node = e->nodes[0] == &node ? e->nodes[1] : e->nodes[0];

        auto it = std::find_if(all(opposite_node->edges), [&e](edge_type *t_e) { return t_e == e; });

        assert(it != opposite_node->edges.end());// use g++ -NDEBUG flag to disable assertions
        opposite_node->edges.erase(it);
        delete e;
        node.edges.pop_front();
      }
      nodes.pop_front();
    }
  }

  // bool RemoveEdge(node_type *a, node_type *b, edge_value_type e) {}

  // bool RemoveNode(node_type *n)// node_value_type _n;
  // {}

  // private:
  std::list<node_type> nodes;
};


float distance(auto n1, auto n2)
{
  auto [x1, y1] = (n1.value);
  auto [x2, y2] = (n2.value);
  return sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

template<class node_value_type, class edge_value_type> struct Relative_dist
{
  using graph_type = graph<node_value_type, edge_value_type>;
  using node_type = node<graph_type>;

  node_type *destiny{};
  float operator()(node_type *t_node) { return distance(*destiny, *t_node); }
};

// f(n)=g(n)+h(n),
using t_graph_type = graph<point, float>;
using t_node_type = t_graph_type::node_type;
using t_edge_type = t_graph_type::edge_type;


// auto min_func = [](t_node_type *a, t_node_type *b) { return *a > *b; };// Min-heap: "greater" means lower priority
class a_star
{
  t_node_type *destiny{};
  map<t_node_type *, bool> visited{};
  Relative_dist<point, float> relative_dist{};

  // <key: node, value: cost>
  map<t_node_type *, float> node_costs;
  size_t graph_size{};
  map<t_node_type *, pair<t_node_type *, float>> paths_and_weights;

public:
  void operator()(t_node_type *t_origin, t_node_type *t_destiny, vector<t_node_type *> &best_path, size_t t_graph_size)
  {
    destiny = t_destiny;
    relative_dist = Relative_dist<point, float>{ .destiny = t_destiny };
    visited[t_origin] = true;
    graph_size = t_graph_size;
    paths_and_weights[t_origin] = { nullptr, 0 };
    main_function(t_origin, best_path);
  }

private:
  void main_function(t_node_type *t_origin, vector<t_node_type *> &best_path)
  {

    map<float, set<t_node_type *>> pri_queue;
    for (t_edge_type *e : t_origin->edges) {
      const bool origin_node = e->nodes[1] == t_origin;
      t_node_type *the_other_end = e->nodes[not origin_node];
      pri_queue[e->value + relative_dist(the_other_end)].insert(the_other_end);
      paths_and_weights[the_other_end] = { t_origin, e->value };
    }

    t_node_type *front_node = (*(pri_queue.begin()->second.begin()));
    while (front_node != destiny) {
      t_node_type *curr_node = *(pri_queue.begin()->second.begin());
      float curr_weight = paths_and_weights[curr_node].second;

      for (t_edge_type *e : curr_node->edges) {
        const bool origin_node = e->nodes[1] == curr_node;
        t_node_type *the_other_end = e->nodes[not origin_node];
        if (the_other_end == t_origin) { continue; }

        auto it = paths_and_weights.find(the_other_end);
        if (it != paths_and_weights.end()) {// it was already visited
          // check if this path is better
          if (paths_and_weights[the_other_end].second > curr_weight + e->value) {
            auto [prev_node, prev_weight] = paths_and_weights[the_other_end];
            paths_and_weights[the_other_end] = { curr_node, curr_weight + e->value };

            // update priority_queue
            auto it_pr_queue = pri_queue[prev_weight].find(prev_node);
            pri_queue[prev_weight].erase(it_pr_queue);

            float new_key = curr_weight + e->value + relative_dist(the_other_end);

            pri_queue[new_key].insert(the_other_end);
          }
        } else {// it wasn't visited;
          // add path and weight
          float key = curr_weight + e->value + relative_dist(the_other_end);
          pri_queue[key].insert(the_other_end);
          paths_and_weights[the_other_end] = { curr_node, curr_weight + e->value };
        }
      }

      pri_queue.begin()->second.erase(pri_queue.begin()->second.begin());
      if (pri_queue.begin()->second.empty()) { pri_queue.erase(pri_queue.begin()); }

      front_node = *(pri_queue.begin()->second.begin());
    }
  }
};

ostream &operator<<(ostream &os, point p) { return os << "(" << p.x << "," << p.y << ")"; }


#endif// __GRAPH_H__