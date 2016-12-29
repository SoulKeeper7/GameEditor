#include <stdlib.h>
#include<stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <GL/glut.h>
#include "SOIL.h"
#include<Windows.h>
#include<ShellAPI.h>
#include <fstream>
#include<Windows.h>
#include<sstream>
#include<iostream>
using namespace std;

GLuint	texture[3];	
int maxx = 100;int maxz=100,life=89;
std::vector < std::vector <int> > wallpos;
std::vector <std::vector <int> > es;//empty space array
std::vector < std::vector <float> > randpos;
int showcrosshair=0;
int LoadGLTextures()                                    // Load Bitmaps And Convert To Textures
{
    /* load an image file directly as a new OpenGL texture */
    texture[0] = SOIL_load_OGL_texture("H:/Data/wall512x128.bmp",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
   texture[1] = SOIL_load_OGL_texture("H:/Data/grass512x512.bmp",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    texture[2] = SOIL_load_OGL_texture("H:/Data/c1.bmp",SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_INVERT_Y);
    if(texture[0] == 0) return false;
 
 
    // Typical Texture Generation Using Data From The Bitmap
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_REPEAT);
 
    return true;                                        // Return Success
}

	// angle of rotation for the camera direction

// actual vector representing the camera's direction
float lx=0.0f,lz=1.0f;

		int m=0;
// XZ wallposition of the camera
float x=10.0f, z=15.0f;
float stableheight=3.5f;
float angle = 0.0f;
float ypos = stableheight;
float forwardspeed=1.0;
int jump=0;
float jheight =12.0f;
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

	
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine ( NEW )
	{
		return 0;									// If Texture Didn't Load Return FALSE
	}

								// Enable Texture Mapping ( NEW )
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	return 1;										// Initialization Went OK
}

void lowhealth();

void randompos(int index)
{
	
	//printf("%d , %d\n",randpos.at(0).at(0), randpos.at(0).at(1));
	glPushMatrix();
	glTranslatef(randpos.at(index).at(0), 0.0f, randpos.at(index).at(1));
	glColor3f(1.0,1.0,0.0);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f,  0.0f);
		glVertex3f( 1.0f, 5.0f,  0.0f);
		glVertex3f( 0.0f, 5.0f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f,  1.0f);
		glVertex3f( 0.0f, 5.0f,  1.0f);
		glVertex3f( 0.0f, 5.0f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f,  1.0f);
		glVertex3f( 0.0f, 5.0f, 1.0f);
	glEnd();
	
	glPopMatrix();	

	
}





void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio =  w * 1.0 / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45.0f, ratio, 0.01f, 1000.0f);
	
	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}


void health(GLfloat x,GLfloat y,void *font,char *string)
{

	char *c;
	glScalef(1,-1,1);
	
	
	
	glPushMatrix();
	glTranslatef(0,y,15 );
	glRotatef(-angle*180/3.141,0,1,0);	
	glColor4f(0.0,0.0,1.0,0.20);
	
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);glVertex2f(350.0f, 150.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex2f(-150.0f, 150.0f);
		glTexCoord2f(1.0f, 1.0f);glVertex2f( -150.0f, -650.0f);
		glTexCoord2f(1.0f, 0.0f);glVertex2f( 350.0f, -650.0f);
	glEnd();
		if(showcrosshair)
	{
		glColor4f(1.0,1.0,1.0,0.5);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f);glVertex2f(-250.0f, -50.0f);
		glTexCoord2f(0.0f, 1.0f);glVertex2f(-250.0, 2000.0f);
		glTexCoord2f(1.0f, 1.0f);glVertex2f(1850.0f, 2000.0f);
		glTexCoord2f(1.0f, 0.0f);glVertex2f( 1850.0f, -50.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
		else
	{
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_LINES);
		glVertex2f(750,900);
		glVertex2f(750,1000);
		glEnd();
		glBegin(GL_LINES);
		glVertex2f(700,950);
		glVertex2f(800,950);
		glEnd();
	glColor3f(0.0,1.0,1.0);
		}
	
	for (c=string; *c != '\0'; c++) 
	{
	
		glutStrokeCharacter(font, *c);
		
		
	}
		
	glPopMatrix();
	

	
	
}

void loaddata(int len)
{string line;

  ifstream myfile ("H:/Data/passed.txt");
  if (myfile.is_open())
  {
	   vector<int>  temp;
   for(int i=0;i<2*len;i++)
    {
		
      getline (myfile,line);
      temp.push_back(stoi(line));
	  
	  if(i%2==1){wallpos.push_back(temp);
	  temp.clear();}
    }
    myfile.close();
  }

  else cout << "Unable to open file"; 

                         
        
	
  
}
void draw() {

	glColor3f(0.7f, 0.3f, 0.3f);

// Draw Body
	glTranslatef(0.0f ,0.0f, 0.0f);
	
		glEnable(GL_TEXTURE_2D);
		 glBindTexture(GL_TEXTURE_2D, texture[0]);
		glBegin(GL_QUADS);

		glTexCoord2f(0.0, 0.0);glVertex3f(0.0f, 0.0f, 0.0f);	
		glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, 0.0f,  0.0f);	
		glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 5.0f,  0.0f);	
		glTexCoord2f(1.0, 0.0);glVertex3f( 0.0f, 5.0f, 0.0f);	
	glEnd();
	
	
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);glVertex3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0, 1.0);glVertex3f(1.0f, 0.0f,  1.0f);
		glTexCoord2f(1.0, 1.0);glVertex3f( 1.0f, 5.0f,  1.0f);
		glTexCoord2f(1.0, 0.0);glVertex3f( 1.0f, 5.0f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex3f(0.0f, 0.0f, 0.0f);
	glTexCoord2f(0.0, 1.0);glVertex3f(0.0f, 0.0f,  1.0f);
	glTexCoord2f(1.0, 1.0);		glVertex3f( 0.0f, 5.0f,  1.0f);
	glTexCoord2f(1.0, 0.0);	glVertex3f( 0.0f, 5.0f, 0.0f);
	glEnd();
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0);	glVertex3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0, 1.0);	glVertex3f(1.0f, 0.0f,  1.0f);
	glTexCoord2f(1.0, 1.0);	glVertex3f( 1.0f, 5.0f,  1.0f);
	glTexCoord2f(1.0, 0.0);	glVertex3f( 0.0f, 5.0f, 1.0f);
	glEnd();
	
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 5.0f, 1.0f);
		glVertex3f(1.0f, 5.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f,  0.0f);
		glVertex3f( 0.0f, 5.0f, 0.0f);
	glEnd();
		glDisable(GL_TEXTURE_2D);


}

void computewallpos(float deltaMove) {
	if(es.at(x+deltaMove*lx*0.1f).at(z + deltaMove * lz * 0.1f)==0)
	{
		x += deltaMove * lx * 0.1f;
		z += deltaMove * lz * 0.1f;
	}
	else if(es.at(x).at(z + deltaMove * lz * 0.1f)==0 )
		{
		
		z += deltaMove * lz * 0.1f;
	}
	else if(es.at(x+deltaMove*lx*0.1f).at(z)==0 )
	{
		x += deltaMove * lx * 0.1f;
		
	}
}
void sop()
{


	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	
	glClearColor(0.0,0.0,0.0,0.0);
	glLoadIdentity();
	glOrtho(0,2000,0,2000,-9999,9999);
	glScalef(1,-1,1);
	glTranslatef(250,-320,0);
	glColor3f(0.05,0.05,0.05);
	glMatrixMode(GL_MODELVIEW);
	
	

		char *string;
	string = new char[256];
	sprintf( string,"%d", life );
	
	health(-200,-300,GLUT_STROKE_ROMAN,string);
	if(life<90)
		{
		glScalef(1,-1,1);
	glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_ONE_MINUS_SRC_COLOR);
	
	
	glPushMatrix();
	
	glRotatef(-angle*180/3.141,0,1,0);	
	glColor4f(1.0,0.0,0.0,0.0);
	GLfloat left=-300,bottom=500,right=1800,top=-1800,centerx=0,centery=0;
	centerx=(right+left)/2;
	centery=(top+bottom)/2;
	glBegin(GL_TRIANGLES);
		glColor4f(0.0,0.0,0.0,0.0);
		glVertex2f(centerx,centery);
		glColor4f((100-life)/100.0,0.0,0.0,(life)/100.0f);
		glVertex2f(right,bottom);
		glColor4f((100-life)/100.0,0.0,0.0,(life)/100.0f);
		glVertex2f( right,top);
	glEnd();
	glBegin(GL_TRIANGLES);
		glColor4f(0.0,0.0,0.0,0.0);
		glVertex2f(centerx,centery);
		glColor4f((100-life)/100.0,0.0,0.0,(life)/100.0);
		glVertex2f(right,bottom);
		glVertex2f( left,bottom);
	glEnd();
	glBegin(GL_TRIANGLES);
		glColor4f(0.0,0.0,0.0,0.0);
		glVertex2f(centerx,centery);
		glColor4f((100.00-life)/100.0,0.0,0.0,(life)/100.0);
		glVertex2f(left,bottom);
		glVertex2f( left,top);
	glEnd();
	glBegin(GL_TRIANGLES);
		glColor4f(0.0,0.0,0.0,0.0);
		glVertex2f(centerx,centery);
		glColor4f((100-life)/100.0,0.0,0.0,(life)/100.0);
		glVertex2f(left,top);
		glVertex2f( right,top);
	glEnd();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		}
	
}
void rp()
{
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
}








void renderScene(void) {
	glClearColor(0.6,0.6,1.0,0.2);
	if(deltaAngle)
	{
			angle += deltaAngle;
			lx = sin(angle + deltaAngle);
							
			lz = -cos(angle + deltaAngle);
			
	}
	
	if (deltaMove)
		computewallpos(deltaMove);
	if(jump==1)
		{
			if(ypos>=jheight)
				jump=2;
			else
			
				ypos=ypos+(jheight+.2-ypos)/10;
		}
	else if(jump==2)
	{
			if(ypos<=stableheight)
				jump=0;
			else
			
				ypos=ypos-(jheight+.2-ypos)/10;
		}
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	for(int i=0;i<maxx;i=i++)
	{
	glBegin(GL_LINES);
	glColor3f(0.5f-(float)x/maxx/2+0.11-(x>i?x-i:i-x)/(10*(i+x)), 0.5f-((float)x/maxx+(float)z/maxz)/4+0.11-(x>i?x-i:i-x)/(10*(i+x)), 1.0-(float)z/maxz);
	glVertex3f((float)i,0,0);
	glVertex3f((float)i,0,maxz);
	glEnd();
	}
	for(int i=0;i<maxz;i=i++)
	{
	glBegin(GL_LINES);
	
	glColor3f(0.5f-(float)x/maxx/2+0.11-(z>i?z-i:i-z)/(10*(i+z)), 0.5f-((float)x/maxx+(float)z/maxz)/4+0.11-(z>i?z-i:i-z)/(10*(i+z)), 1.0-(float)z/maxz);
	
	glVertex3f(0,0,(float)i);
	glVertex3f(maxz,0,(float)i);
	glEnd();
	}
	gluLookAt(	x, ypos, z,
			x+lx, ypos,  z+lz,
			0.0f, ypos,  0.0f);
	
// Draw ground
	 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glEnable(GL_TEXTURE_2D);
	//glColor3f(0.5f-(float)x/maxx/2, 0.5f-((float)x/maxx+(float)z/maxz)/4, 1.0-(float)z/maxz);
	glColor3f(1.0,1.0,1.0);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);glVertex3f( 0.0f, 0.0f, 0.0f);	
		glTexCoord2f(1.0, 0.0);glVertex3f(0.0f, 0.0f,  maxz);
		glTexCoord2f(1.0, 1.0);glVertex3f( maxx, 0.0f,  maxz);
		glTexCoord2f(0.0, 1.0);glVertex3f( maxx, 0.0f, 0.0f);	
	glEnd();
	//CRISS CROSS
	glDisable(GL_TEXTURE_2D);
	
	glColor3f(1.0f-(float)x/maxx/2, 0.5f-((float)x/maxx+(float)z/maxz)/4, 1.0-(float)z/maxz);
	//cross on ground
	glBegin(GL_LINES);
	glVertex3f(1,0,1);
	glVertex3f(11,0,11);
	glEnd();
	
	glBegin(GL_LINES);
	glVertex3f(1,0,11);
	glVertex3f(11,0,1);
	glEnd();
	// Draw 36 objects

		for(unsigned int i=0;i<wallpos.size();i++)
		{
                     glPushMatrix();
                     glTranslatef(wallpos.at(i).at(0),0,wallpos.at(i).at(1));
					 				//	printf("%d , %d",(int) wallpos.at(i).at(0),(int) wallpos.at(i).at(1));
						 es.at((int) wallpos.at(i).at(0)).at((int) wallpos.at(i).at(1))=1;


                     draw();
                     glPopMatrix();
        }
		if(randpos.at(0).at(0)==randpos.at(0).at(2) || randpos.at(0).at(1)==randpos.at(0).at(3) )
	{
		srand(time(NULL));
		randpos.at(0).at(2)=rand()%maxx;
		randpos.at(0).at(3)=rand()%maxz;
	}
	else
	{
		randpos.at(0).at(0)+=(randpos.at(0).at(0)<randpos.at(0).at(2)?0.0:(-0.0));
		randpos.at(0).at(1)+=(randpos.at(0).at(1)<randpos.at(0).at(3)?0.0:(-0.0));
	}
		randompos(0);
		sop();
		
		rp();

	//	renderStrokeFontString(x,z,GLUT_STROKE_ROMAN,"15951");
        glutSwapBuffers();
		glutPostRedisplay();
	
		if(life<0)
			{
				printf("____________________________________-GAME OVER-__________________________________________!!!");
				
				exit(0);
		}
		
	// Reset Matrix


	//glLoadIdentity();
//glutSwapBuffers();
	
		glPopMatrix();
		
		glFlush();
} 

void processNormalKeys(unsigned char key, int xx, int yy) { 	

        if (key == 27)
              exit(0);
		
		if (key == 32)
              if(!jump)jump=1;
		if(key=='a')
			stableheight++;
		if(key=='b')
			stableheight--;
		if (key=='p')
		{
			

		}
		if(key=='q'||key=='Q')
			life++;
		if(key=='w'||key=='W')
			life--;
} 

void pressKey(int key, int xx, int yy) {

       switch (key) {
             case GLUT_KEY_UP : deltaMove = forwardspeed; break;
             case GLUT_KEY_DOWN : deltaMove = -1*forwardspeed; break;
			 case GLUT_KEY_LEFT : 
									deltaAngle = -5 * 0.003f;
								
									break;				 
			case GLUT_KEY_RIGHT : 
									deltaAngle = 5 * 0.003f;
								
									break;		
       }
} 

void releaseKey(int key, int x, int y) { 	

        switch (key	) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : deltaMove = 0;break;
				 case GLUT_KEY_LEFT : if(deltaAngle<0)deltaAngle=0; break;
					 case GLUT_KEY_RIGHT : if(deltaAngle>0)deltaAngle=0; break;
        }
} 


void mouseMove(int x, int y) { 	
	
         // this will only be true when the top button is down
       //  if (xOrigin >= 0)
//	{

		// update deltaAngle
	deltaAngle = (x-xOrigin>0?x-xOrigin:xOrigin-x)* 0.005f;		
	if(x-xOrigin>0)angle += deltaAngle;
	else if (x-xOrigin<0) angle-=deltaAngle;	

		// update camera's direction
		lx = sin(angle + deltaAngle) ;
		lz = -cos(angle + deltaAngle);
		xOrigin=x;
		
		deltaAngle=0.0;

//	}
}

void mouseButton(int button, int state, int x, int y) 
{
	printf("%d , %d\n",x,y);
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_UP) {
	if((float)(randpos.at(0).at(0)-::x)/(float)(randpos.at(0).at(1)-z)<= (float)lx/(float)lz+0.1 && (randpos.at(0).at(0)-::x)/(float)(randpos.at(0).at(1)-z)> (float)lx/(float)lz-0.1)
		life--;
		printf("%f  \n  %f\n   lx=%f\n\nlz=%f\n\n",(float)(randpos.at(0).at(0)-::x)/(float)(randpos.at(0).at(1)-z),(float)lx/(float)lz,lx,lz );
	}
	}
	if (button == GLUT_RIGHT_BUTTON) {
		if (state == GLUT_UP) {
			showcrosshair=showcrosshair==0?1:0;
		}
	}
	/*
	// only start motion if the top button is pressed
	if (button == GLUT_LEFT_BUTTON) {

		// when the button is released
		if (state == GLUT_UP) {
			
		//	xOrigin = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
			printf("%f , %f\n",::x,z);
		}
	}
	*/
	}

int main(int argc, char **argv)
{
	wallpos.clear();

	
	//egister a random object
	vector<float> te;
	te.push_back(20.0);
	te.push_back(30.0);
	te.push_back(20.0);
	te.push_back(30.0);
	randpos.push_back(te);
	for(int i=0;i<maxx;i++)
	{
		std::vector<int> temp;
		for(int j=0;j<maxz;j++)
		{
			if(i==0 || i==maxx || j==0 || j==maxz)
			temp.push_back(1);
			else
				temp.push_back(0);
		}
		es.push_back(temp);
		temp.clear();
	}
	for(int i=0;i<maxx;i++)
	{
		es.at(0).at(i)=1;
		es.at(i).at(0)=1;
		es.at(maxx-1).at(i)=1;
		es.at(i).at(maxx-1)=1;
	}
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Project");
	InitGL();
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glScalef(1,-1,1);


	glutIgnoreKeyRepeat(0);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);
	glutPassiveMotionFunc(mouseMove);
	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		for(int i = 0; i < 8; i++)
		for(int j=0; j < 8; j++) 
		{
			std::vector <int> temp;
			temp.push_back(i*10.0);
			temp.push_back(j);
			wallpos.push_back(temp);
			temp.clear();
		}
		if(argc>1)loaddata(stoi(argv[1]));
	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
void lowhealth()
{

		//sop();
	//rp();


}
