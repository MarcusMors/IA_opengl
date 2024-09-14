#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <queue>
#include <stack>
#include <thread>
#include <vector>

// prob(cost_inm + cost_fut)
// V(S2)  = 0.7(4 + 0) + 0.3(3+ V(S0))
// V(S0)  = 0.6(5 + 1) + 0.4(2+ V(S2))
//

// x  = 2.8 + 0.9 + 0.3*y
// x = y/0.4 - 3.6/0.4 - 0.8/0.4

// y  = 3.6 + 0.8 + 0.4*x

using namespace std;

struct Node
{
  float x, y;
  vector<int> neighbors;
};

vector<Node> graph;
int n = 500;// Número de nodos
int p = 5;// Cantidad de nodos más cercanos

bool selected = false;
int firstNode = -1, secondNode = -1;

float distance(const Node &a, const Node &b) { return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y)); }

void drawNode(float x, float y, float r, float g, float b)
{
  glColor3f(r, g, b);
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  glEnd();
}

void drawEdge(float x1, float y1, float x2, float y2, float r, float g, float b)
{
  glColor3f(r, g, b);
  glBegin(GL_LINES);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

void createGraph()
{
  graph.clear();
  for (int i = 0; i < n; ++i) {
    Node node = { 2.0f * (rand() % 500) / 500.0f - 1.0f, 2.0f * (rand() % 500) / 500.0f - 1.0f };
    graph.push_back(node);
  }

  for (int i = 0; i < n; ++i) {
    vector<pair<float, int>> distances;
    for (int j = 0; j < n; ++j) {
      if (i != j) { distances.push_back({ distance(graph[i], graph[j]), j }); }
    }
    std::sort(distances.begin(), distances.end());
    for (int k = 0; k < p; ++k) { graph[i].neighbors.push_back(distances[k].second); }
  }
}

void drawEdgeWithDelay(float x1, float y1, float x2, float y2, float r, float g, float b, float delay)
{
  drawEdge(x1, y1, x2, y2, r, g, b);
  glfwSwapBuffers(glfwGetCurrentContext());
  glfwPollEvents();
  this_thread::sleep_for(chrono::milliseconds(static_cast<int>(delay * 1000)));
}

void waitForClick(GLFWwindow *window)
{
  bool clicked = false;
  while (!clicked) {
    glfwPollEvents();
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) { clicked = true; }
    this_thread::sleep_for(chrono::milliseconds(10));
  }
}

void aStarSearch(int start, int goal)
{
  vector<float> gScore(n, numeric_limits<float>::max());
  vector<float> fScore(n, numeric_limits<float>::max());
  vector<int> parent(n, -1);
  vector<bool> visited(n, false);

  auto heuristic = [](const Node &a, const Node &b) { return distance(a, b); };

  gScore[start] = 0.0f;
  fScore[start] = heuristic(graph[start], graph[goal]);

  priority_queue<pair<float, int>, vector<pair<float, int>>, greater<pair<float, int>>> openSet;
  openSet.push({ fScore[start], start });

  while (!openSet.empty()) {
    int current = openSet.top().second;
    openSet.pop();

    if (current == goal) {
      int current = goal;
      while (current != start) {
        int prev = parent[current];
        drawEdgeWithDelay(graph[prev].x, graph[prev].y, graph[current].x, graph[current].y, 1.0f, 0.0f, 0.0f, 0.1f);
        current = prev;
      }
      drawNode(graph[goal].x, graph[goal].y, 1.0f, 0.0f, 0.0f);
      waitForClick(glfwGetCurrentContext());
      return;
    }

    visited[current] = true;

    for (int neighbor : graph[current].neighbors) {
      if (visited[neighbor]) continue;

      float tentative_gScore = gScore[current] + distance(graph[current], graph[neighbor]);

      if (tentative_gScore < gScore[neighbor]) {
        parent[neighbor] = current;
        gScore[neighbor] = tentative_gScore;
        fScore[neighbor] = gScore[neighbor] + heuristic(graph[neighbor], graph[goal]);
        openSet.push({ fScore[neighbor], neighbor });

        drawEdgeWithDelay(
          graph[current].x, graph[current].y, graph[neighbor].x, graph[neighbor].y, 0.0f, 0.0f, 1.0f, 0.1f);
      }
    }
  }
}

void hillClimbingSearch(int start, int goal)
{
  vector<bool> visited(n, false);
  vector<int> parent(n, -1);

  auto heuristic = [](const Node &a, const Node &b) { return distance(a, b); };

  int current = start;
  visited[current] = true;

  while (current != goal) {
    int next = -1;
    float minHeuristic = numeric_limits<float>::max();

    for (int neighbor : graph[current].neighbors) {
      if (!visited[neighbor] && heuristic(graph[neighbor], graph[goal]) < minHeuristic) {
        minHeuristic = heuristic(graph[neighbor], graph[goal]);
        next = neighbor;
      }
    }

    if (next == -1) {
      cout << "No se encontró un camino." << endl;
      return;
    }

    parent[next] = current;
    visited[next] = true;
    drawEdgeWithDelay(graph[current].x, graph[current].y, graph[next].x, graph[next].y, 0.0f, 0.0f, 1.0f, 0.1f);
    current = next;
  }

  while (current != start) {
    int prev = parent[current];
    drawEdgeWithDelay(graph[prev].x, graph[prev].y, graph[current].x, graph[current].y, 1.0f, 0.0f, 0.0f, 0.1f);
    current = prev;
  }
  drawNode(graph[goal].x, graph[goal].y, 1.0f, 0.0f, 0.0f);
  waitForClick(glfwGetCurrentContext());
}

void renderGraph()
{
  for (const auto &node : graph) {
    drawNode(node.x, node.y, 1.0f, 1.0f, 1.0f);
    for (const auto &neighbor : node.neighbors) {
      drawEdge(node.x, node.y, graph[neighbor].x, graph[neighbor].y, 0.7f, 0.7f, 0.7f);
    }
  }

  if (firstNode != -1) { drawNode(graph[firstNode].x, graph[firstNode].y, 1.0f, 0.0f, 0.0f); }

  if (secondNode != -1) { drawNode(graph[secondNode].x, graph[secondNode].y, 1.0f, 0.0f, 0.0f); }
}

int findNearestNode(float x, float y)
{
  int nearest = -1;
  float minDistance = numeric_limits<float>::max();
  for (int i = 0; i < n; ++i) {
    float dist = sqrt((x - graph[i].x) * (x - graph[i].x) + (y - graph[i].y) * (y - graph[i].y));
    if (dist < minDistance) {
      minDistance = dist;
      nearest = i;
    }
  }
  return nearest;
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    int width, height;
    glfwGetWindowSize(window, &width, &height);

    float x = (2.0f * xpos / width) - 1.0f;
    float y = 1.0f - (2.0f * ypos / height);

    int nearestNode = findNearestNode(x, y);

    if (firstNode == -1) {
      firstNode = nearestNode;
      cout << "Primer nodo seleccionado: " << firstNode << endl;
    } else if (secondNode == -1) {
      secondNode = nearestNode;
      cout << "Segundo nodo seleccionado: " << secondNode << endl;
      selected = true;
    }
  }
}

int main()
{
  if (!glfwInit()) {
    cerr << "Failed to initialize GLFW" << endl;
    return -1;
  }

  GLFWwindow *window = glfwCreateWindow(1000, 1000, "Grafo con A* y Hill Climbing", nullptr, nullptr);
  if (!window) {
    cerr << "Failed to create GLFW window" << endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glewInit();

  createGraph();
  glfwSetMouseButtonCallback(window, mouseButtonCallback);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    renderGraph();

    if (selected) {
      cout << "Iniciando A* y Hill Climbing..." << std::endl;

      aStarSearch(firstNode, secondNode);
      glClear(GL_COLOR_BUFFER_BIT);

      renderGraph();

      hillClimbingSearch(firstNode, secondNode);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      this_thread::sleep_for(chrono::seconds(3));

      selected = false;
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}