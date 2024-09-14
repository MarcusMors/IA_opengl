


int main()
{
  graph<point, float> g;

  auto *n1 = g.insert_node({ .x = 100, .y = 100 });
  auto *n2 = g.insert_node({ .x = 100, .y = 100 });
  auto *n3 = g.insert_node({ .x = 100, .y = 100 });
  auto *n4 = g.insert_node({ .x = 1000, .y = 1000 });
  auto *n5 = g.insert_node({ .x = 500, .y = 500 });
  auto *n6 = g.insert_node({ .x = 250, .y = 250 });
  auto *n7 = g.insert_node({ .x = 750, .y = 750 });


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