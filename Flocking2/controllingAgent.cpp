#include <windows.h>

#pragma comment(lib, "user32.lib") 

#include <GL/glut.h>  // (or others, depending on the system in use)
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
using namespace std;

#include "Color.h"
#include "Vector.h"
using namespace mathtool;
#include "Simulator.h"
#include "Environment.h"
#include "Agent.h"
#include "Basic.h"
#include "controllingAgent.h"

double theta = 0;
double speedTheta = 0.1;
int drawMode=1;

bool toggleAvoid = true;
bool toggleAttract = true;

bool toggleAlign = true;
bool toggleSeparation = true;
bool toggleCohesion = true;

int agName = 0;

int numDraws=0;
int windowW = 800;
int windowH = 800;
static int window_id;
static int menu_id;
MyColor bColor(1,1,1);
bool isConvex=true;

int singleAgent;
int groupAgent;

Simulator gSim;
Agent drawAgent;
Environment* gEnv;
ControllingAgent tog;
bool isSimulating=false;

double x, y, z;
Vector3d newCoordinate(x, y, z);


void init() {
	glClearColor(bColor.r, bColor.g, bColor.b, 0.0); //Set display-window color to white.

	glMatrixMode(GL_PROJECTION);      //Set projection parameters.
	gluOrtho2D(-windowW/2,windowW/2, -windowH/2, windowH/2);
        gEnv = new Environment("env.txt",-windowW/2,windowW/2, -windowH/2, windowH/2);
}

void menu(int num) {
	if(num==0) {
		glutDestroyWindow(window_id);
		exit(0);
	}
	else if (num == 1) {
		tog.singleAgent = true;
		tog.groupAgent = false;
		tog.attractPoint = false;
		tog.avoidPoint = false;
	}
	else if (num == 2) {
		tog.singleAgent = false;
		tog.groupAgent = true;
		tog.attractPoint = false;
		tog.avoidPoint = false;
	}
	else if (num == 3) {
		tog.singleAgent = false;
		tog.groupAgent = false;
		tog.attractPoint = true;
		tog.avoidPoint = false;
		toggleAttract = true;
	}
	else if (num == 4) {
		tog.groupAgent = false;
		tog.singleAgent = false;
		tog.attractPoint = false;
		tog.avoidPoint = true;
		toggleAvoid = true;
	}
}

void createMenu() {
	menu_id = glutCreateMenu(menu);
	glutAddMenuEntry("Quit",0);
	glutAddMenuEntry("Add Agent", 1);
	glutAddMenuEntry("Add Agents", 2);
	glutAddMenuEntry("Add Attraction Point", 3);
	glutAddMenuEntry("Add Avoid Point", 4);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int homeX, homeY;
bool mousePressed=false;
int timeSincePress=0;
int changeY=0;
int changeX=0;
double alpha = 0.5;
bool camT = false;

void customDraw();
void update() {
  if( isSimulating ) 
    gSim.Update();

  //cout << "update mousePressed=" << mousePressed << endl;
  if( mousePressed ) {
    speedTheta = changeY*0.2;
    timeSincePress += 1;
  }
  customDraw();
}

void camTrack() {
	newCoordinate = gSim.agents[agName].GetPos();

	glMatrixMode(GL_PROJECTION);      //Set projection parameters.
	glLoadIdentity();
	gluOrtho2D(newCoordinate.GetX() - windowW / 5, newCoordinate.GetX() + windowW / 5, newCoordinate.GetY() - windowH / 5, newCoordinate.GetY() + windowH / 5);

}
void customDraw() {
  if( ++numDraws % 100 == 0 ) {
    cout << " customDraw numDraws = " << numDraws << " drawMode: " << drawMode << endl;
  }

  glClearColor(bColor.r, bColor.g, bColor.b, 0.0); //Set display-window color to white.
  glClear(GL_COLOR_BUFFER_BIT);     //Clear display window.

  if (camT == true) {
	  camTrack();
  }

  gEnv->Draw();
  gSim.Draw();

  glFlush();                        //Process all OpenGL routines as quickly as possible.
}

void mousebutton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if( !mousePressed ) {
			homeX = x;
			homeY = y;
			timeSincePress = 0;
		}
		mousePressed = true;

		double newX = (x - (windowW/2));
		double newY = ((windowH/2) - y);

		if (tog.singleAgent == true) {
			gSim.AddAgent(newX, newY);
			cout << "agent dispersing" << endl;
		}
		else if (tog.groupAgent == true) {
			for (int i = 0; i < 5; i++) {
				gSim.AddAgent(newX, newY);
			}
			cout << "group of agents" << endl;
		}
		else if (tog.attractPoint == true) {
			gEnv->AddAttractionPoint(newX, newY);
		}
		else if (tog.avoidPoint == true) {
			gEnv->AddAvoidPoint(newX, newY);
		}
		glutPostRedisplay();
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		mousePressed = false;
		timeSincePress = 0;
 	}
	if( button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN ) {
	  cout << "glut_middle_button == down" << endl;
	}
}

void keyboard(unsigned char key, int x, int y) {
	double newX = (x - (windowW / 2));
	double newY = ((windowH / 2) - y);
	int counter = 0;

	switch (key)
	{
	case '1':
		gSim.AddAgent(newX, newY);
		break;
	case '5':
		for (int i = 0; i < 5; i++) gSim.AddAgent(newX, newY);
		break;
	case '*':
		camT = true;
		break;
	case '+':
		agName++;
		break;
	case 'a':
		gSim.ToggleAlignment();
		break;
	
	case 's':
		gSim.ToggleSeparation();
		break;
	case 'c':
		gSim.ToggleCohesion();
		break;
	case 'f':
		break;
	case '-':
		if (agName > 0) {
			agName--;
			break;
		}
		else
		break;
	case 'q':
	case 27:       // ESCAPE key
		exit (0);
		break;
	case ' ':
		isSimulating = !isSimulating; break;
	case 'd':
		toggleAvoid = false;
		gEnv->AddAvoidPoint(newX, newY);
		toggleAvoid = true;
		break;

	case 'D':
		toggleAttract = false;
		gEnv->AddAttractionPoint(newX, newY);
		toggleAttract = true;
		break;
	}
}

void mouseMove(int x, int y) {




}

void otherKeyInput(int key, int x, int y) {

	switch (key) {
		char arrow;
	case GLUT_KEY_RIGHT:
		arrow = 'r';
		gSim.changeAgent(arrow);
		break;
	case GLUT_KEY_LEFT:
		arrow = 'l';
		gSim.changeAgent(arrow);
		break;
	case GLUT_KEY_UP:
		arrow = 'u';
		gSim.changeAgent(arrow);
		break;
	case GLUT_KEY_DOWN:
		arrow = 'd';
		gSim.changeAgent(arrow);
		break;

	

	}
  glutPostRedisplay();
}


int main(int argc, char** argv) {
	glutInit(&argc, argv);                         //Initialize GLUT.
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);   //Set display mode.
	//glutInitWindowPosition(50, 100);               //Set top-left display-window position.
	glutInitWindowSize(windowW, windowH);                  //Set display-window width and height.
	
	window_id = glutCreateWindow("2D Flock");  //Create display window.
	init();                                        //Execute initialization procedure.
	createMenu();
	
	glutIdleFunc(update);
	glutDisplayFunc(customDraw);                   //Send graphics to display window.
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mousebutton);                    //How to handle mouse press events. 
	//glutMotionFunc(mouseMove);
	glutSpecialFunc(otherKeyInput);
	glutMainLoop();                                //Display everything and wait.
}

