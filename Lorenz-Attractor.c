/*
 * Mitchell Allen
 * CSCI5229: Computer Graphics
 * Homework 1: Lorenz Attractor
 * 2022-06-05
 */

// Includes (from ex6)
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef USEGLEW
#include <GL/glew.h>
#endif

//  OpenGL with prototypes for glext (from ex6)
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>

// Tell Xcode IDE to not gripe about OpenGL deprecation (from ex6)
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif

//  Default resolution (from ex6)
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif

//  Globals (changed some values from ex6)
int th=-63;       // Azimuth of view angle
int ph=-47;       // Elevation of view angle
int mode=2;     // Dimension (1-4)
double z=0;     // Z variable
double w=1;     // W variable
double dim=2.5;   // Dimension of orthogonal box
const char* text[] = {"","2D","3D constant Z","3D","4D"};  // Dimension display text

// Lorenz Parameters  (modified the r value from lorenz.c to become a torus knot)
double s  = 10;
double b  = (8/3);
double r  = 99.96;

/*
 *  Convenience routine to output raster text (from ex7)
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  // Maximum length of text string
void Print(const char* format , ...)
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
 *  Print message to stderr and exit (from ex7)
 */
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Check for OpenGL errors (from ex7)
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

// (modified from lorenz.c with suggestions from Paul Hoffman)
void lorenz()
{
   int i;

   /*  Coordinates  */
   double x = 1;
   double y = 1;
   double z = 1;

   /*  Time step  */
   double dt = 0.001;

   // printf("%5d %8.3f %8.3f %8.3f\n",0,x,y,z);
   /*
    *  Integrate 50,000 steps (50 time units with dt = 0.001)
    *  Explicit Euler integration
    */
   for (i=0;i<50000;i++)
   {
      double dx = s*(y-x);
      double dy = x*(r-z)-y;
      double dz = x*y - b*z;
      x += dt*dx;
      y += dt*dy;
      z += dt*dz;
      glColor3f(x/7.0,y/18.0,z/99.0);
      glVertex3f(x/49.0,y/98.0,z/97.0);
   }
}

/*
 *  Display the scene (from ex6)
 */
void display()
{
   //  Clear the image
   glClear(GL_COLOR_BUFFER_BIT);
   //  Set background color to gray instead of black
   glClearColor(0.3,0.3,0.3,0);
   //  Reset previous transforms
   glLoadIdentity();
   //  Set view angle
   glRotated(ph,1,0,0);
   glRotated(th,0,1,0);

   //  Draw the Lorenz
   glBegin(GL_LINE_STRIP); //Draws a connected group of line segments from the first vertex to the last
   lorenz();
   glEnd();

   //  Draw axes (modified colors from ex7)
   int len = 2.8;
   glColor3f(2.0,0.5,1.0);
   glBegin(GL_LINES);
   glVertex3d(0,0,0);
   glVertex3d(len,0,0);
   glVertex3d(0,0,0);
   glVertex3d(0,len,0);
   glVertex3d(0,0,0);
   glVertex3d(0,0,len);
   glEnd();
   //  Label axes (from ex7)
   glColor3f(1.0,0,1.0);
   glRasterPos3d(len,0,0);
   Print("X");
   glRasterPos3d(0,len,0);
   Print("Y");
   glRasterPos3d(0,0,len);
   Print("Z");
   //  Display parameters (changed mode and values displayed)
   glColor3f(1,0,1);
   glWindowPos2i(5,30);
   Print("View Angle=%d,%d",th,ph);
   Print("  s=%.1f",s);
   Print("  b=%.1f",b);
   Print("  r=%.1f",r);
   // if (mode==2)
   //  Print("  z=%.1f",z);
   // else if (mode==4)
   //  Print("  w=%.1f",w);
   //  Flush and swap
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed (from ex7)
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase azimuth by 1 degrees
   if (key == GLUT_KEY_RIGHT)
      th += 1;
   //  Left arrow key - decrease azimuth by 1 degrees
   else if (key == GLUT_KEY_LEFT)
      th -= 1;
   //  Up arrow key - increase elevation by 1 degrees
   else if (key == GLUT_KEY_UP)
      ph += 1;
   //  Down arrow key - decrease elevation by 1 degrees
   else if (key == GLUT_KEY_DOWN)
      ph -= 1;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed (modified keys from ex7)
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == '0')
      th = ph = 0;
   // Reset sigma, beta, and rho values
   else if (ch == 'x' || ch == 'X')
      {
         s  = 10;
         b  = (8/3);
         r  = 99.96;
      }
   //  Change s (wW for increase sS for decrease)
   else if (ch == 'w' || ch == 'W')
      {
         s += 1;
      }
   else if (ch == 's' || ch == 'S')
      {
         s += -1;
      }
   //  Change b (aA for increase dD for decrease)
   else if (ch == 'a' || ch == 'A')
      {
         b += 1;
      }
   else if (ch == 'd' || ch == 'D')
      {
         b += -1;
      }   
    //  Change r (qQ for increase eE for decrease)
   else if (ch == 'q' || ch == 'Q')
      {
         r += 1;
      }
   else if (ch == 'e' || ch == 'E')
      {
         r += -1;
      }   
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized (from ex7)
 */
void reshape(int width,int height)
{
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Orthogonal projection box adjusted for the
   //  aspect ratio of the window
   double asp = (height>0) ? (double)width/height : 1;
   glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Start up GLUT and tell it what to do (from ex7)
 */
int main(int argc,char* argv[])
{
  //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window 
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
   //  Request 500 x 500 pixel window
   glutInitWindowSize(500,500);
   //  Create the window
   glutCreateWindow("HW1_Allen_Mitchell");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Pass control to GLUT so it can interact with the user
   glutMainLoop();
   //  Return code
   return 0;
}
