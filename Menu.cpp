/*
	TRON GAME!
Please note, for the soil library to work we need to install the libraries in the
linux system and this can be achieved by typing the following comman in the termainal,
"sudo apt-get install libsoil-dev". Thank you. :D

*/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <string.h>

#define TEXTURE_PATH "menu.jpg"
#define ESCAPE 27

GLuint texture;
void init();
void display();
void mykeyboard(unsigned char key, int x, int y);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
       	glutInitWindowSize(1000,1000);
        glutInitWindowPosition(0,0);	
	glutCreateWindow("TRON");
	glutFullScreen();
	glutDisplayFunc(display);
	glutKeyboardFunc(mykeyboard);
	init();
	glutMainLoop();
	return 0;
}

void init()
{
	glClearColor(0, 0, 0, 1);
	gluOrtho2D(0, 1, 0, 1);
	// Enable 2D textures
	glEnable(GL_TEXTURE_2D);
	// Use soil library to load the image
	texture = SOIL_load_OGL_texture
	(
	TEXTURE_PATH,
	SOIL_LOAD_AUTO,
	SOIL_CREATE_NEW_ID,
	SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	// check for an error during the load process
	if(texture == 0)
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glBegin( GL_QUADS );
	glTexCoord2f(0.0,0.0);
	glVertex2d(0, 0);
	glTexCoord2d(1.0,0.0);
	glVertex2d(1, 0);
	glTexCoord2d(1.0,1.0);
	glVertex2d(1, 1);
	glTexCoord2d(0.0,1.0);
	glVertex2d(0, 1);
	glEnd();
	glFlush();
	glutSwapBuffers();
        
       
}
void mykeyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'p':
			system("./TRON SeaofSimulation.mp3");
			
		case 27:
			exit(0);
		default:
			break;
	}
}
