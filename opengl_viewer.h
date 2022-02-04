// This is a simple introductory program; its main window contains a static
// picture of a tetrahedron, whose top vertex is white and whose bottom
// vertices are red, green and blue.  The program illustrates viewing by
// defining an object at a convenient location, then transforming it so that
// it lies within the view volume.  This is a lousy way to do things (it's
// easier to use gluLookAt()), but it's nice to see how viewing is done at
// a very low level.

#ifdef __APPLE_CC__
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#include <cmath>

// A camera.  It moves horizontally in a circle centered at the origin of
// radius 10.  It moves vertically straight up and down.
class Camera {
  double theta;      // determines the x and z positions
  double y;          // the current y position
  double dTheta;     // increment in theta for swinging the camera around
  double dy;         // increment in y for moving the camera up/down
public:
  Camera(): theta(0), y(3), dTheta(0.04), dy(0.2) {}
  double getX() {return 10 * cos(theta);}
  double getY() {return y;}
  double getZ() {return 10 * sin(theta);}
  void moveRight() {theta += dTheta;}
  void moveLeft() {theta -= dTheta;}
  void moveUp() {y += dy;}
  void moveDown() {if (y > dy) y -= dy;}
};


Camera camera;

// Clears the window and draws the tetrahedron.  The tetrahedron is  easily
// specified with a triangle strip, though the specification really isn't very
// easy to read.
// void display() {
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   glLoadIdentity();
//   gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
//             0.0, 0.0, 0.0,
//             0.0, 1.0, 0.0);
//   // Draw a white grid "floor" for the tetrahedron to sit on.
//   glColor3f(1.0, 1.0, 1.0);
//   glBegin(GL_LINES);
//   for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
//     glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
//     glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
//   }
//   glEnd();

//   // Draw the tetrahedron.  It is a four sided figure, so when defining it
//   // with a triangle strip we have to repeat the last two vertices.
//   glBegin(GL_TRIANGLES);
//     // (0, 2, 0) (-1, 0, 1) (1, 0, 1) (0, 0, -1.4)
//     // glColor3f(1, 1, 1); //white
//     // glVertex3f(0, 2, 0);
//     // glVertex3f(-1, 0, 1);
//     // glVertex3f(1, 0, 1);
    

//     // glColor3f(1, 0, 0); //red
//     // glVertex3f(-1, 0, 1);
//     // glVertex3f(1, 0, 1);
//     // glVertex3f(0, 0, -1.4);
    

//     // glColor3f(0, 1, 0); // green
//     // glVertex3f(1, 0, 1);
//     // glVertex3f(0, 0, -1.4);
//     // glVertex3f(0, 2, 0);


//     glColor3f(1, 0, 1); // magenta
//     glVertex3f(0, 0, -1.4);
//     glVertex3f(-1, 0, 1);
//     glVertex3f(0, 2, 0);

//     glColor3f(1, 0, 1); // magenta
//     glVertex3f(0.0, 0, 1);
//     glVertex3f(-3, 0, 2);
//     glVertex3f(-4, 0, 4);

//   glEnd();

//   glFlush();
//   glutSwapBuffers();

// }

// Moves the camera according to the key pressed, then ask to refresh the
// display.
void special(int key, int, int) {
  switch (key) {
    case GLUT_KEY_LEFT: camera.moveLeft(); break;
    case GLUT_KEY_RIGHT: camera.moveRight(); break;
    case GLUT_KEY_UP: camera.moveUp(); break;
    case GLUT_KEY_DOWN: camera.moveDown(); break;
  }
  glutPostRedisplay();
}

// Sets up global attributes like clear color and drawing color, enables and
// initializes any needed modes (in this case we want backfaces culled), and
// sets up the desired projection and modelview matrices. It is cleaner to
// define these operations in a function separate from main().
void init() {
  glEnable(GL_DEPTH_TEST);

  // Set the current clear color to sky blue and the current drawing color to
  // white.
  glClearColor(0.1, 0.39, 0.88, 1.0);
  glColor3f(1.0, 1.0, 1.0);

  // Tell the rendering engine not to draw backfaces.  Without this code,
  // all four faces of the tetrahedron would be drawn and it is possible
  // that faces farther away could be drawn after nearer to the viewer.
  // Since there is only one closed polyhedron in the whole scene,
  // eliminating the drawing of backfaces gives us the realism we need.
  // THIS DOES NOT WORK IN GENERAL.
  // glEnable(GL_CULL_FACE);
  // glCullFace(GL_BACK);

  // Set the camera lens so that we have a perspective viewing volume whose
  // horizontal bounds at the near clipping plane are -2..2 and vertical
  // bounds are -1.5..1.5.  The near clipping plane is 1 unit from the camera
  // and the far clipping plane is 40 units away.
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-2, 2, -1.5, 1.5, 1, 40);

  // Set up transforms so that the tetrahedron which is defined right at
  // the origin will be rotated and moved into the view volume.  First we
  // rotate 70 degrees around y so we can see a lot of the left side.
  // Then we rotate 50 degrees around x to "drop" the top of the pyramid
  // down a bit.  Then we move the object back 3 units "into the screen".
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -3);
  glRotatef(50, 1, 0, 0);
  glRotatef(70, 0, 1, 0);
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, GLfloat(w) / GLfloat(h), 1.0, 150.0);
  glMatrixMode(GL_MODELVIEW);
}

// Requests to draw the next frame.
void timer(int v) {
  glutPostRedisplay();
  glutTimerFunc(1000/60, timer, v);
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
// int main(int argc, char** argv) {
//   glutInit(&argc, argv);
//   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
//   glutInitWindowPosition(80, 80);
//   glutInitWindowSize(800, 600);
//   glutCreateWindow("A Simple Tetrahedron");
//   glutDisplayFunc(display);
//   glutReshapeFunc(reshape);
//   glutSpecialFunc(special);
//   glutTimerFunc(100, timer, 0);
//   init();
//   glutMainLoop();
// }
