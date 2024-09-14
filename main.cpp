#include <GL/glut.h>
#include <iostream>
#include <sstream>
#include <vector>

#include "gen.hpp"
#include "graph.hpp"

using namespace std;
using vi = vector<int>;
using sstream = stringstream;

// using mi = vector<int>;


// Callback function to handle mouse input
void mouseClick(int button, int state, int x, int y)
{
  // Check if the left mouse button is clicked
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    cout << "Mouse clicked at: (" << x << ", " << y << ")" << endl;
  }
}


const int node_size = 5;
// Size of matrix and each square (cell)
const int window_height = 800;
const int window_width = 1000;

vector<point> points;
// Function to draw the matrix
void displayMatrix()
{
  glClear(GL_COLOR_BUFFER_BIT);
  for (auto [x, y] : points) {
    glColor3f(1.0f, 1.0f, 1.0f);// Black for 1

    glBegin(GL_QUADS);
    glVertex2i(x - 2, y - 2);// Bottom left
    glVertex2i(x + 2, y + 2);// Bottom right
    glVertex2i(x + 2, y + 2);// Top right
    glVertex2i(x - 2, y + 2);// Top left
    glEnd();
  }

  glFlush();// Flush the OpenGL buffer to the window
}

void displayCallback() { displayMatrix(); }
// GLUT display callback

// GLUT initialization
void initializeGLUT(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(window_height, window_width);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Matrix Visualization");

  // Set background color to white
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Set up 2D orthographic projection with the size of the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, window_height, window_width, 0);// Set (0,0) as top-left corner

  glutDisplayFunc(displayCallback);// Register display callback

  // Register the mouse click callback
  glutMouseFunc(mouseClick);
}

int main(int argc, char **argv)
{

  sstream input_stream{};
  const size_t data_sample_size = 1000;

  points = vector<point>(data_sample_size);
  out_random_data({ .min = 100, .max = 1000 }, data_sample_size, input_stream);

  int i = 0;
  while (input_stream >> points[i].x and input_stream >> points[i].y) { i++; }

  // Book
  // planning with markov decision processes: An AI Perspective.


  initializeGLUT(argc, argv);// Initialize GLUT
  glutMainLoop();// Enter the GLUT main loop
  return 0;
}

/*

cd "/home/marcus/+projects/IA_opengl/" && [ ! -f main.out ] || [ main.cpp -nt ./main.out ] && g++-14 -lncurses
-std=c++23 -O0 -g3 -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -Wall -Wextra -Wshadow
-Wnon-virtual-dtor -pedantic -Wold-style-cast -Wformat=2 main.cpp -o main.out -lpthread && ./main.out || ./main.out

 */