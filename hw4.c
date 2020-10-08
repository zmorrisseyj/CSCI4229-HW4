
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=60;   //  Size of world
double aX = 0;
double aY = 0;
double dX = 0;
double dY = 0;
double dZ = -1;
double pX = 0;
double pY = 0;
double pZ = 100;



//  Macro for sin & cos in degrees - BORROWED FROM EX9
#define Cos(th) cos(3.1415926/180*(th))
#define Sin(th) sin(3.1415926/180*(th))



/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...) // - BORROWED FROM EX9
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Set projection - BORROWED FROM EX9
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode==1||mode==2)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
 void cube(double x,double y,double z,
              double dx,double dy,double dz,
              double cx, double cy, double cz,
              double th, double ph, double zh,
              int type)
 {
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(zh,0,0,1);
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   glBegin(type);

   //  Front
   glColor3f(cx,0,0);
   glVertex3f(-1,-1, 1);
   glVertex3f(+1,-1, 1);
   glColor3f(0,0,cz);
   glVertex3f(+1,+1, 1);
   glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(0,0,cz);
   glVertex3f(+1,-1,-1);
   glVertex3f(-1,-1,-1);
   glColor3f(cx,0,0);
   glVertex3f(-1,+1,-1);
   glVertex3f(+1,+1,-1);
   //  Right
   glColor3f(cx,cy,0);
   glVertex3f(+1,-1,+1);
   glVertex3f(+1,-1,-1);
   glColor3f(cx,0,0);
   glVertex3f(+1,+1,-1);
   glVertex3f(+1,+1,+1);
   //  Left
   glColor3f(0,cy,0);
   glVertex3f(-1,-1,-1);
   glVertex3f(-1,-1,+1);
   glColor3f(0,cy,cz);
   glVertex3f(-1,+1,+1);
   glVertex3f(-1,+1,-1);
   //  Top
   glColor3f(0,cy,cz);
   glVertex3f(-1,+1,+1);
   glVertex3f(+1,+1,+1);
   glColor3f(cx,0,0);
   glVertex3f(+1,+1,-1);
   glVertex3f(-1,+1,-1);
   //  Bottom
   glColor3f(cx,0,cz);
   glVertex3f(-1,-1,-1);
   glVertex3f(+1,-1,-1);
   glColor3f(0,cy,cz);
   glVertex3f(+1,-1,+1);
   glVertex3f(-1,-1,+1);
   //  End
   glEnd();
   glPopMatrix();
 }

static void sphere(double x,double y,double z,
                   double cx,double cy,double cz,
                   double r) //FROM ex8
{
   const int d=15;
   int th,ph;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r,r);

   //  Latitude bands
   for (ph=-90;ph<90;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=360;th+=d)
      {
         //glColor3f(th/360*cz,th/360*cz,th/360*cz);
         glColor3f(Sin(th)*Cos(ph)*cx,Sin(ph)*cy,cz);
         glVertex3d(Sin(th)*Cos(ph) , Sin(ph) , Cos(th)*Cos(ph));
         glVertex3d(Sin(th)*Cos(ph+d) , Sin(ph+d) , Cos(th)*Cos(ph+d));
      }
      glEnd();
   }

   //  Undo transformations
   glPopMatrix();
}




void frame(double x, double y, double z,
           double dx, double dy, double dz,
           double cx, double cy, double cz,
           double th)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);
  cube(0,2.5,0,  1,2.5,2,  cx,cy,cz,  0,0,0, GL_QUADS); //torso
  cube(0,-2.5,1.2,  1,2.5,0.8,  cx,cy,cz,  0,0,0, GL_QUADS); //legs
  cube(0,-2.5,-1.2,  1,2.5,0.8,  cx,cy,cz,  0,0,0, GL_QUADS);

  sphere(0,4.5,2,  cx,cy,cz, 1.2);
  sphere(0,4.5,-2,  cx,cy,cz, 1.2);
  cube(0,4.5,-3  ,0.7,0.7,2,  cx,cy,cz,  0,0,0, GL_QUADS);//arms
  cube(0,4.5,3  ,0.7,0.7,2,  cx,cy,cz,  0,0,0, GL_QUADS);
  sphere(0,5.5,0, cx,cy,cz, 1.5); //head
  glPopMatrix();
}

void stage(double x,double y, double z,
           double dx, double dy, double dz,
           double th)
{
  glPushMatrix();
  glTranslated(x,y,z);
  glRotated(th,0,1,0);
  glScaled(dx,dy,dz);

  cube(0,-5,0,50,5,10,0.1,0.1,0.1,0,0,0, GL_QUADS);
  cube(0,-5,0,50,5,10,1,0,1,0,0,0, GL_LINE_STRIP); //highlight
  cube(0,-5,25,30,5,15,0.2,0.2,0.2,0,0,0, GL_QUADS);
  cube(0,-5,25,30,5,15,1,0,1,0,0,0, GL_LINE_STRIP);
  cube(0,-5,-25,30,5,15,0.2,0.2,0.2,0,0,0, GL_QUADS);
  cube(0,-5,-25,30,5,15,1,0,1,0,0,0, GL_LINE_STRIP);

  glBegin(GL_TRIANGLES);
  glColor3f(1,0,1);

  glVertex3f(50,0,10);
  glVertex3f(30,0,40);
  glVertex3f(30,0,10);

  glVertex3f(50,0,-10);
  glVertex3f(30,0,-40);
  glVertex3f(30,0,-10);

  glVertex3f(-50,0,10);
  glVertex3f(-30,0,40);
  glVertex3f(-30,0,10);

  glVertex3f(-50,0,-10);
  glVertex3f(-30,0,-40);
  glVertex3f(-30,0,-10);

  glVertex3f(50,-10,10);
  glVertex3f(30,-10,40);
  glVertex3f(30,-10,10);

  glVertex3f(50,-10,-10);
  glVertex3f(30,-10,-40);
  glVertex3f(30,-10,-10);

  glVertex3f(-50,-10,10);
  glVertex3f(-30,-10,40);
  glVertex3f(-30,-10,10);

  glVertex3f(-50,-10,-10);
  glVertex3f(-30,-10,-40);
  glVertex3f(-30,-10,-10);

  glEnd();

  glBegin(GL_QUADS);

  glVertex3f(50,0,10);
  glVertex3f(50,-10,10);
  glVertex3f(30,-10,40);
  glVertex3f(30,0,40);

  glVertex3f(-50,0,10);
  glVertex3f(-50,-10,10);
  glVertex3f(-30,-10,40);
  glVertex3f(-30,0,40);

  glVertex3f(50,0,-10);
  glVertex3f(50,-10,-10);
  glVertex3f(30,-10,-40);
  glVertex3f(30,0,-40);

  glVertex3f(-50,0,-10);
  glVertex3f(-50,-10,-10);
  glVertex3f(-30,-10,-40);
  glVertex3f(-30,0,-40);

  glEnd();

  sphere(0,-25,0,0.2,0.1,0.2,24);

  double iy = 0;
  double iz = 0;
  double ix = 0;
  for(int i = 1; i<=400; i+=1)
  {
    ix+=0.1;
    iy+=0.1;
    iz+=0.1;
    cube(ix,-50+(iy*iy)/50,(3*iz)/5,2,0.3,2,1,1,1,60,0,iz+10,GL_QUADS);
    cube(-ix,-50+(iy*iy)/50,(3*iz)/5,2,0.3,2,1,1,1,60,0,-iz-10,GL_QUADS);
    cube(ix,-50+(iy*iy)/50,-(3*iz)/5,2,0.3,2,1,1,1,60,0,iz+10,GL_QUADS);
    cube(-ix,-50+(iy*iy)/50,-(3*iz)/5,2,0.3,2,1,1,1,60,0,-iz-10,GL_QUADS);
  }


  glPopMatrix();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   double Ex = -2*dim*Sin(th)*Cos(ph);
   double Ey = +2*dim        *Sin(ph);
   double Ez = +2*dim*Cos(th)*Cos(ph);
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glLoadIdentity();
   switch (mode){
     case 0: //Orthoganal
       glRotatef(ph,1,0,0);
       glRotatef(th,0,1,0);
       break;
     case 1: //Perspetive
       gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
       break;
     case 2: //first person
       gluLookAt(pX,pY,pZ, pX+dX,pY+dY,pZ+dZ, 0,Cos(ph),0);
       break;
   }
   frame(-10,5,0,  1,1,1,  1,1,1, 0);
   frame(20,10,0,  1,2,1,  1,0,1, 80);
   frame(-40,10,0,  1,2,1,  1,1,0, 40);
   stage(1,1,1,1,1,1,0);


   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed - BORROWED FROM EX9
 */
void special(int key,int x,int y)
{
  float fraction = 0.3;
  if (mode==2) //added this section, modified from the stack overflow code I borrowed
  switch(key)
  {
      case GLUT_KEY_LEFT:
          aY -= 0.5f;
          dX = Sin(aY);
          dZ = -Cos(aY);
          break;
      case GLUT_KEY_RIGHT:
          aY += 0.5f;
          dX = Sin(aY);
          dZ = -Cos(aY);
          break;
      case GLUT_KEY_UP:
          aX += 0.5f;
          dY = Sin(aX);
          dZ = -Cos(aX);
          break;
      case GLUT_KEY_DOWN:
          aX -= 0.5f;
          dY = Sin(aX);
          dZ = -Cos(aX);
          break;
      case GLUT_KEY_PAGE_UP:
          pX += dX * fraction;
          pZ += dZ * fraction;
          pY += dY * fraction;
          break;
      case GLUT_KEY_PAGE_DOWN:
          pX -= dX * fraction;
          pZ -= dZ * fraction;
          pY -= dY * fraction;
          break;
     }
   else{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 5;
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 5;
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP)
      ph += 5;
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 5;
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
 }
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed - BORROWED FROM EX9
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   //  Toggle axes
   else if (ch == 'a' || ch == 'A')
      axes = 1-axes;
   //  Switch display mode
   else if (ch == 'm' || ch == 'M')
      mode = (mode + 1)%3;
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov--;
   else if (ch == '+' && ch<179)
      fov++;
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized - BORROWED FROM EX9
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   Project();
}


 int main(int argc, char* argv[])
 {
   glutInit(&argc,argv); //Initialize glut with user input
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); //Initialize RGB, double buffering, and depth testing
   glutInitWindowSize(500,500); //Initialize window size
   glutCreateWindow("Homework 4 - Zach Morrissey - Projections"); //Create window with input string as title
   glutDisplayFunc(display); //calls the display function which draws the program
   glutReshapeFunc(reshape);
   glutSpecialFunc(special); //calls a function for arrow key interaction
   glutKeyboardFunc(key); //calls a function for keyboard interaction
   glutMainLoop();

   return 0;
 }
