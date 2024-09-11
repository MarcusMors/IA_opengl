#include <GL/glut.h>
#include <iostream>
#include <vector>

using namespace std;
using vi = vector<int>;
// using mi = vector<int>;

// Callback function to handle mouse input
void mouseClick(int button, int state, int x, int y)
{
  // Check if the left mouse button is clicked
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    cout << "Mouse clicked at: (" << x << ", " << y << ")" << endl;
  }
}


// Size of matrix and each square (cell)
const int MATRIX_SIZE = 250;
const int CELL_SIZE = 4;
const int WINDOW_SIZE = MATRIX_SIZE * CELL_SIZE;

// Matrix of integers (1000x1000)
vector<vi> matrix(MATRIX_SIZE, vi(MATRIX_SIZE));

// Function to initialize the matrix with some values (e.g., 0 or random integers)
void initializeMatrix()
{
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    for (int j = 0; j < MATRIX_SIZE; ++j) { matrix[i][j] = rand() % 2; }
  }
}

// Function to draw the matrix
void displayMatrix()
{
  glClear(GL_COLOR_BUFFER_BIT);

  for (int i = 0; i < MATRIX_SIZE; ++i) {
    for (int j = 0; j < MATRIX_SIZE; ++j) {
      // Set color based on matrix value (0 = white, 1 = black)
      if (matrix[i][j] == 0) {
        glColor3f(1.0f, 1.0f, 1.0f);// White for 0
      } else {
        glColor3f(0.0f, 0.0f, 0.0f);// Black for 1
      }

      // Draw each cell as a 4x4 square
      glBegin(GL_QUADS);
      glVertex2i(j * CELL_SIZE, i * CELL_SIZE);// Bottom left
      glVertex2i((j + 1) * CELL_SIZE, i * CELL_SIZE);// Bottom right
      glVertex2i((j + 1) * CELL_SIZE, (i + 1) * CELL_SIZE);// Top right
      glVertex2i(j * CELL_SIZE, (i + 1) * CELL_SIZE);// Top left
      glEnd();
    }
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
  glutInitWindowSize(WINDOW_SIZE, WINDOW_SIZE);
  glutInitWindowPosition(50, 50);
  glutCreateWindow("Matrix Visualization");

  // Set background color to white
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Set up 2D orthographic projection with the size of the window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_SIZE, WINDOW_SIZE, 0);// Set (0,0) as top-left corner

  glutDisplayFunc(displayCallback);// Register display callback

  // Register the mouse click callback
  glutMouseFunc(mouseClick);
}

int main(int argc, char **argv)
{
  initializeMatrix();// Initialize matrix with data
  initializeGLUT(argc, argv);// Initialize GLUT

  glutMainLoop();// Enter the GLUT main loop
  return 0;
}

/*

cd "/home/marcus/+projects/IA_opengl/" && [ ! -f main.out ] || [ main.cpp -nt ./main.out ] && g++-14 -lncurses
-std=c++23 -O0 -g3 -fno-omit-frame-pointer -fsanitize=address -fsanitize=undefined -Wall -Wextra -Wshadow
-Wnon-virtual-dtor -pedantic -Wold-style-cast -Wformat=2 main.cpp -o main.out -lpthread && ./main.out || ./main.out

 */