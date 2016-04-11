#include <GL/freeglut.h>

void RenderSceneRC()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 02");

	glClearColor(0.8f, 0.3f, 0.1f, 1.0f);

	glutDisplayFunc(RenderSceneRC);

	glutMainLoop();

	return 0;
}