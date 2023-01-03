#include <windows.h>
#pragma comment(lib, "user32.lib") 

#include "Environment.h"
#include <fstream>
#include <vector>

#include <GL/glut.h>  // (or others, depending on the system in use)

///////////////////////////////////////////////////////////

//extern bool toggleAttract;
extern bool toggleAvoid;
extern bool toggleAttract;

Cell::Cell() {  
  blocked = false;
}
void Cell::Init(int _i, int _j, Point _center, double _dx, double _dy, bool _blocked) {
  xi = _i;
  xj = _j;
  dx = _dx;
  dy = _dy;
  center = _center;
  minPt.x = center.x-dx/2.0;
  minPt.y = center.y-dy/2.0;
  maxPt.x = center.x+dx/2.0;
  maxPt.y = center.y+dy/2.0;
  blocked = _blocked;
}

void Cell::Draw() {

  if( blocked ) {
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
  }
  else {
    glColor3f(0.6, 0.6, 0.6);
    glBegin(GL_LINE_LOOP);
  }
  glVertex2f(minPt.x, minPt.y);
  glVertex2f(maxPt.x, minPt.y);
  glVertex2f(maxPt.x, maxPt.y);
  glVertex2f(minPt.x, maxPt.y);
  glEnd();
}

///////////////////////////////////////////////////////////
Environment::Environment() {
}
Environment::Environment(string _inputFile, double _xmin, double _xmax, double _ymin, double _ymax ) {
  inputFile = _inputFile;

  xmin = _xmin;
  xmax = _xmax;
  ymin = _ymin;
  ymax = _ymax;
  cout << " Environment boundaries: [" << xmin << ", " << xmax << " | " << ymin << ", " << ymax << "]"<< endl;

  cout << "Attempting to load environment file: " << inputFile << endl;
  ifstream infile;
  infile.open(inputFile.c_str());
  if( !infile ) {
    cout << "Could NOT open infile: " << inputFile << endl;
    //exit(-1);
  }
  if(infile >> rows) {
    cout << "There will be " << rows << " rows ";
  }
  if(infile >> columns) {
    cout << "There will be " << columns << " rows ";
  }
  cells = new Cell*[rows];
  for(int i=0; i<rows; i++) {
    cells[i] = new Cell[columns];
  }
  double dx = (_xmax-_xmin)/columns;
  double dy = (_ymax-_ymin)/rows;
  string colData;
  string tmpStr;
  getline(infile, tmpStr); //just to get rid of carriage return
  int rowIndex=0;
  while( getline(infile, colData) ) {
    //cout << rowIndex << " going to process '"<<colData<<"'"<<endl;
    for(int i=0; i<colData.length(); i++) {
      Cell& cell = cells[rowIndex][i];
      bool isBlocked = (colData.at(i)=='x'||colData.at(i)=='X')? true: false;
      Point center(xmin+ dx/2+dx*i,ymin+ dy/2+dy*rowIndex);
      //cout << "Initializing cell: " << rowIndex << ", " << i << " center: " << center.GetX() << " " << center.GetY() << " dx " << dx << " dy " << dy << " isBlocked " << isBlocked << endl;
      cell.Init(rowIndex,i,center,dx,dy,isBlocked);
    }
    rowIndex++;
  }
  infile.close();
}

void drawAttractionPoint(double radius, int divisions) {
	double deltaAng = TWOPI / divisions;
	double curAng = 0;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i<divisions; i++) {
		double x = radius * cos(curAng);
		double y = radius * sin(curAng);
//		if (agDebug) cout << "Circle " << i << " x= " << x << " y= " << y << endl;
		glVertex2f(x, y);
		curAng += deltaAng;
//		if (agDebug) cout << "curAng= " << curAng << " deltaAng= " << deltaAng << endl;

	}
	glEnd();
}

void drawAvoidPoint(double radius, int divisions) {
	double deltaAng = TWOPI / divisions;
	double curAng = 0;
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_POLYGON);
	for (int i = 0; i<divisions; i++) {
		double x = radius * cos(curAng);
		double y = radius * sin(curAng);
		//		if (agDebug) cout << "Circle " << i << " x= " << x << " y= " << y << endl;
		glVertex2f(x, y);
		curAng += deltaAng;
		//		if (agDebug) cout << "curAng= " << curAng << " deltaAng= " << deltaAng << endl;

	}
	glEnd();
}

void Environment::AddAttractionPoint(double x, double y) {

	Vector3d newV(x, y, 0);

	if (toggleAttract) {
		attractionPoints.push_back(newV);
	}
	else {
		attractionPoints.clear();
	}
}

void Environment::AddAvoidPoint(double x, double y) {

	Vector3d newV(x, y, 0);

	if (toggleAvoid) {
		avoidPoints.push_back(newV);
	}
	else {
		avoidPoints.clear();
	}
}

void Environment::Draw() {
  for(int i=0; i<rows; i++) 
    for(int j=0; j<columns; j++) {
      cells[i][j].Draw();
    }

  for (int i = 0; i < attractionPoints.size(); i++) {
	  Vector3d tv = attractionPoints[i];
	  glPushMatrix();
	  glTranslatef(tv.GetX(), tv.GetY(), 0);
	  drawAttractionPoint(10, 20);
	  glPopMatrix();
  }

  for (int i = 0; i < avoidPoints.size(); i++) {
	  Vector3d tv = avoidPoints[i];
	  glPushMatrix();
	  glTranslatef(tv.GetX(), tv.GetY(), 0);
	  drawAvoidPoint(10, 20);
	  glPopMatrix();
  }
}

Vector3d Environment::GetWrappedPosition(Vector3d p, bool& updated) {
  Vector3d pNew(p);
  if( pNew.GetX()>xmax ) {
    pNew.SetX( pNew.GetX() - (xmax-xmin) );
    updated=true;
  }
  else if( pNew.GetX()<xmin ) {
    pNew.SetX(pNew.GetX() + (xmax - xmin));
    updated=true;
  }

  if( pNew.GetY()>ymax ) {
   
    pNew.SetY(pNew.GetY() - (ymax - ymin));
    updated=true;
  }
  else if( pNew.GetY()<ymin ) {
    pNew.SetY(pNew.GetY() + (ymax - ymin));
    updated=true;
  }
  return pNew;
}
