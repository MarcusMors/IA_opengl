#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>
#include <utils>

#include <ctime>//to include size_t

#include "./enums/enums.hpp"
#include "./graph.hpp"

using namespace std;

struct point
{
  int x{};
  int y{};
};
// struct color_point
// {
//   int x{};
//   int y{};
//   Color color{ none };
// };

//

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


auto min_func = [](t_node_type *a, t_node_type *b) { return *a > *b; };// Min-heap: "greater" means lower priority
class a_star
{
  t_node_type *destiny{};
  map<t_node_type *, bool> visited{};
  Relative_dist<point, float> relative_dist{};
  //
  // <key: node, value: [cost, cost+abs_dist]>
  // <key: node, value: cost>
  map<t_node_type *, float> node_costs;
  size_t graph_size{};

public:
  void operator()(t_node_type *t_origin, t_node_type *t_destiny, vector<t_node_type *> &best_path, size_t t_graph_size)
  {
    destiny = t_destiny;
    relative_dist = Relative_dist<point, float>{ .destiny = destiny };
    visited[t_origin] = true;
    graph_size = t_graph_size;
    main_function(t_origin, best_path);
  }

private:
  struct edge_item
  {
    using node_value_type = t_graph_type::node_value_type;
    using edge_value_type = t_graph_type::edge_value_type;
    bool origin_node{};
    t_edge_type *edge{};
    edge_value_type distance{};

    t_node_type *origin() { return edge->nodes[origin_node]; };
    t_node_type *destiny() { return edge->nodes[not origin_node]; };
    edge_value_type value() { return edge->value; };
  };

  void main_function(t_node_type *t_origin, vector<t_node_type *> &relative_dist)
  {
    // <key: weight, value: nodes to expand>
    priority_queue<t_node_type *, vector<t_node_type *>, decltype(min_func)> heap(min_func);
    // priority_queue<heap_pack, vector<t_node_type *>, decltype(min_func)> heap(min_func);

    // relative_dist()


    while (visited.size() < graph_size) {
      for (t_edge_type *e : t_origin->edges) {
        const bool origin_node = e->nodes[1] == t_origin;
        t_node_type *the_other_end = e->nodes[not origin_node];

        auto visited_it = visited.find(the_other_end);
        if (const bool was_visited = visited_it != visited.end(); was_visited) {
          //
          continue;
        }
        heap.push({ .origin_node = origin_node, .edge = e });

        // add cost to node_costs;
        // node_costs[];
        // heap.push(the_other_end)
      }
      heap.pop();
    }

    map<t_node_type *, pair<t_node_type *, float>> paths_and_weights;
    paths_and_weights[origin] = { nullptr, 0 };

    map<float, set<t_node_type *>> pri_queue;
    for (t_edge_type *e : t_origin->edges) {
      const bool origin_node = e->nodes[1] == t_origin;
      t_node_type *the_other_end = e->nodes[not origin_node];
      pri_queue[e->value + relative_distance(the_other_end)].insert(the_other_end);
      paths_and_weights[the_other_end] = { origin, e->value };
    }

    while (pri_queue.begin().second.begin() != destiny) {
      t_node_type *curr_node = pri_queue.begin().second.begin();
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
            auto it = pri_queue[prev_weight].find(prev_node);
            pri_queue[prev_weight].erase(it);

            float new_key = curr_weight + e->value + relative_dist(the_other_end);

            pri_queue[new_key].push(the_other_end);
          }
        } else {// it wasn't visited;
          // add path and weight
          float key = curr_weight + e->value + relative_distance(the_other_end);
          pri_queue[key].insert(the_other_end);
          paths_and_weights[the_other_end] = { curr_node, curr_weight + e->value };
        }
      }
    }
  }
};


int main()
{
  graph<point, float> g;

  auto *n1 = g.insert_node({ .x = 0, .y = 100 });
  auto *n2 = g.insert_node({ .x = 0, .y = 100 });
  auto *n3 = g.insert_node({ .x = 0, .y = 100 });
  auto *n4 = g.insert_node({ .x = 0, .y = 100 });


  g.insert_edge(n1, n2, distance(*n1, *n2));
  // g.insert_edge(n1, n2, distance(*n1, *n2));// second

  g.insert_edge(n2, n3, distance(*n2, *n3));
  // g.insert_edge(n2, n3, distance(*n2, *n3));// second

  g.insert_edge(n3, n4, distance(*n3, *n4));
  // g.insert_edge(n3, n4, distance(*n3, *n4));// second

  g.insert_edge(n4, n1, distance(*n4, *n1));
  // g.insert_edge(n4, n1, distance(*n4, *n1));// second


  for (auto &n : g.nodes) { cout << n.value << endl; }

  return 0;
}

/*
node<graph<color_point, int> >*
node<graph<color_point, int> >
 */