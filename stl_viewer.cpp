#include "ascii_stl_reader.cpp"
#include "opengl_viewer.h"


unsigned dispay_obj_index = 0;

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(camera.getX(), camera.getY(), camera.getZ(),
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0);
  // Draw a white grid "floor" for the tetrahedron to sit on.
  glColor3f(1.0, 1.0, 1.0);
  glBegin(GL_LINES);
  for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
    glVertex3f(i, 0, 2.5); glVertex3f(i, 0, -2.5);
    glVertex3f(2.5, 0, i); glVertex3f(-2.5, 0, i);
  }
  glEnd();

  // Draw the tetrahedron.  It is a four sided figure, so when defining it
  // with a triangle strip we have to repeat the last two vertices.
  glBegin(GL_TRIANGLES);

  std::vector<FACE> faces = objects[dispay_obj_index].object_faces;

  for(size_t i = 0; i < faces.size(); i++){
		// std::cout << "face " << i << " normal x coord = "<< faces[i].n.x << std::endl;
		// if (faces[i].n.y > 0){
		// 	glColor3f(0, 0, 1);	
		// } else {
		// 	glColor3f(1, 0, 1);
		// }
		if (faces[i].n.y <= 0){
			glColor3f(faces[i].n.x, faces[i].n.y, faces[i].n.z);

	  	// std::cout << faces[i].n.z << std::endl;

			for(size_t j = 0; j < faces[i].vertices.size(); j++){
				// std::cout << "vertex " << y << " x coord = " << faces[i].vertices[y].x << std::endl;
				// std::cout << faces[i].vertices[y].x << ";" << faces[i].vertices[y].y << ";" << faces[i].vertices[y].z << std::endl;
				GLfloat x = faces[i].vertices[j].x;
				GLfloat y = faces[i].vertices[j].y;
				// GLfloat y = 0;
				GLfloat z = faces[i].vertices[j].z;
				glVertex3f(x/10, y/10, z/10);
			}
		}
		// std::cout << std::endl;
	}

  glEnd();

  glFlush();
  glutSwapBuffers();

}

int view_stl(int argc, char *argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow(objects[dispay_obj_index].filename.c_str());
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutTimerFunc(100, timer, 0);
	init();
	glutMainLoop();

	return 0;
}


int main(int argc, char *argv[])
{	

	process_stl_file(argv[1]);
	view_stl(argc, argv);

	return 0;
}