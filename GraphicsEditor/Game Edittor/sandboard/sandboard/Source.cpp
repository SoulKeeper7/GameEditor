#include <stdlib.h>
#include<stdio.h>
#include <math.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <GL/glut.h>
#include <fstream>
#include<Windows.h>
#include<sstream>
using namespace std;


 unsigned char * bmpdata;
int maxx = 100;int maxz=100,life=89;
std::vector < std::vector <int> > wallpos;
std::vector <std::vector <int> > es;//empty space array
std::vector < std::vector <int> > randpos;
float pointerx=0, pointerz=0;
	// angle of rotation for the camera direction
/*  Create checkerboard texture  */

GLuint texture;
	
// actual vector representing the camera's direction
float lx=0.0f,lz=1.0f;

		int m=0;
// XZ wallposition of the camera
float x=10.0f, z=15.0f;
float stableheight=2.0f;
float angle = 0.0f;
float ypos = stableheight;
float forwardspeed=1.0;
int jump=0;
float jheight =12.0f;

void save()
{
	ofstream outputFile;            
    outputFile.open("H:/data/passed.txt");  
   
   for (int i = 0 ; i < wallpos.size() ; i++)
   { 
       outputFile << wallpos.at(i).at(0) << endl;  
       outputFile <<wallpos.at(i).at(1) << endl;
       
   }
  int a = wallpos.size();
stringstream ss;
ss << a;
string str = ss.str();
    outputFile.close();
	ShellExecute(
    NULL,
    "open",
    "C:\\Users\\Abhishek\\Documents\\Visual Studio 2010\\Projects\\t1\\Debug\\t1.exe ", str.c_str()
    ,
    NULL,
    SW_SHOW
);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

GLuint LoadTexture( const char * filename )
{

  GLuint texture;

  int width, height;
  
  unsigned char * data;

  FILE * file;

  file = fopen( filename, "rb" );

  if ( file == NULL ) return 0;
  width = 1024;
  height = 512;
  data = (unsigned char *)malloc( width * height * 3 );
  //int size = fseek(file,);
  fread( data, width * height * 3, 1, file );
  fclose( file );

 for(int i = 0; i < width * height ; ++i)
{
   int index = i*3;
   unsigned char B,R;
   B = data[index];
   R = data[index+2];

   data[index] = R;
   data[index+2] = B;

}


glGenTextures( 1, &texture );
glBindTexture( GL_TEXTURE_2D, texture );
glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST );


glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,GL_REPEAT );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,GL_REPEAT );
gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width, height,GL_RGB, GL_UNSIGNED_BYTE, data );
free( data );

return texture;
}
void drawmarker()
{
	glColor3f(1.0,1.0,0.0);
	glPushMatrix();
	glTranslatef(pointerx, 0.0f,pointerz);
	glBegin(GL_QUADS);
		glVertex3f( 0.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f,  1.0f);
		glVertex3f( 0.0f, 0.0f,  1.0f);
	
	glEnd();
glPopMatrix();

}


int xOrigin = -1;

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
	gluPerspective(45.0f, ratio, 0.01f, 10000.0f);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}




void draw() {
	
 
	glColor3f(0.0f, 1.0f, 1.0f);
	glPushMatrix();
// Draw Body
	glTranslatef(0.0f ,0.0f, 0.0f);
	//glutSolidSphere(0.75f,20,20);
		glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f); 
		glVertex3f(1.0f, 0.0f,  0.0f);	 
		glVertex3f( 1.0f, 5.0f,  0.0f);	 
		glVertex3f( 0.0f, 5.0f, 0.0f);	
	glEnd();
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glBegin(GL_QUADS);
		glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f,  1.0f);
		glVertex3f( 0.0f, 5.0f,  1.0f);
		glVertex3f( 0.0f, 5.0f, 0.0f);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(1.0f, 0.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f,  1.0f);
		glVertex3f( 0.0f, 5.0f, 1.0f);
	glEnd();
	glColor3f(0.5,0.5,0.5);
	glBegin(GL_QUADS);
		glVertex3f(0.0f, 5.0f, 1.0f);
		glVertex3f(1.0f, 5.0f,  1.0f);
		glVertex3f( 1.0f, 5.0f,  0.0f);
		glVertex3f( 0.0f, 5.0f, 0.0f);
	glEnd();
	
	glPopMatrix();
}

void computewallpos(float deltaMove) //this is  for collision between user and wall
{
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






void renderScene(void) {
	
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();
	// Set the camera
	

// Draw ground
	glColor3f(0.5f-(float)x/maxx/2, 0.5f-((float)x/maxx+(float)z/maxz)/4, 1.0-(float)z/maxz);
	
	
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);glVertex3f( 0.0f, 0.0f, 0.0f);	
	glTexCoord2f(0.0f, 1.0f);glVertex3f(000.0f, 0.0f,  100.0f);
	glTexCoord2f(1.0f, 1.0f);glVertex3f( 100.0f, 0.0f,  100.0f);
	glTexCoord2f(1.0f, 0.0f);glVertex3f( 100.0f, 0.0f, 000.0f);	
	glEnd();
		gluLookAt(	50.0, 120.0, 50.0,
				50.0, 0.0, 49.0,
			0.0f, 0.5,  0.0f);
	
	
	//CRISS CROSS

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
	//wallpos.clear();
	
		for(unsigned int i=0;i<wallpos.size();i++)
		{
                     glPushMatrix();
                     glTranslatef(wallpos.at(i).at(0),0,wallpos.at(i).at(1));
					 				//	printf("%d , %d",(int) wallpos.at(i).at(0),(int) wallpos.at(i).at(1));
						 es.at((int) wallpos.at(i).at(0)).at((int) wallpos.at(i).at(1))=1;


                     draw();
                     glPopMatrix();
        }
	
	
		drawmarker();

        glutSwapBuffers();
		glutPostRedisplay();

	
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
			save();
			
		}
		if(key=='q'||key=='Q')
			life++;
		if(key=='w'||key=='W')
		{
			std::vector <int> temp;
			temp.push_back(pointerx);
			temp.push_back(pointerz);
			wallpos.push_back(temp);
			temp.clear();
			printf("w");
		}
} 

void pressKey(int key, int xx, int yy) {

       switch (key) {
             case GLUT_KEY_UP :  pointerz--;break;
             case GLUT_KEY_DOWN :  pointerz++;break;
			 case GLUT_KEY_LEFT : pointerx--;
									break;				 
			case GLUT_KEY_RIGHT : pointerx++;
									break;		
       }
} 

void releaseKey(int key, int x, int y) { 	

        switch (key	) {
             case GLUT_KEY_UP :
             case GLUT_KEY_DOWN : 
				 case GLUT_KEY_LEFT : break;
					 case GLUT_KEY_RIGHT :  break;
        }
} 


void mouseMove(int x, int y)
{ 	

    
}

void mouseButton(int button, int state, int x, int y) 
{

	
}

int main(int argc, char **argv)
{
	//egister a random object
	vector<int> te;
	te.push_back(20);
	te.push_back(30);
	te.push_back(20);
	te.push_back(30);
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
	   
   glClearColor (0.0, 0.0, 0.0, 0.0);
   printf("%ud ",texture);
  texture= LoadTexture( "vtr.bmp" );
  
	glBindTexture (GL_TEXTURE_2D, texture);


  
   
	// init GLUT and create window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("Project");

	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glScalef(1,-1,1);


	glutIgnoreKeyRepeat(0);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(pressKey);
	glutSpecialUpFunc(releaseKey);

	// here are the two new functions
	glutMouseFunc(mouseButton);
	glutMotionFunc(mouseMove);

	// OpenGL init
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 1;
}
void lowhealth()
{

		//sop();
	//rp();


}
