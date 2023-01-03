#include <windows.h>
#pragma comment(lib, "user32.lib") 

#include "controllingAgent.h"
#include "Agent.h"
#include "Environment.h"

#include <GL/glut.h>  // (or others, depending on the system in use)
bool agDebug = false;// true;

extern Environment* gEnv;
extern bool toggle;

char ag;
extern int agName;
bool agColor;

Agent::Agent() { 
  initialized = false;
  drawForce = true;
  drawVelocity = true;
  drawVR = false;
} //empty constructor  

extern bool toggleAlign;
extern bool toggleSeparation;
extern bool toggleCohesion;

ControllingAgent c;

void Agent::Init(int _id, Vector3d _pos, Vector3d _vel, double _mass, 
                 double _maxVel, double _maxAccel, double _viewRadius){
  initialized = true;
  id = _id;
  pos = _pos;
  vel = _vel;
  mass = _mass;
  maxVel = _maxVel;
  maxAccel = _maxAccel;
  viewRadius = _viewRadius;
  radius = 10;
  
  drawForce = true;
  drawVelocity = true;
  drawVR = false;

  alignmentComponent = 40;
  separationComponent = 40;
  cohesionComponent = 70;

}

Agent::Agent(const Agent& other) {
  initialized = other.initialized;
  id = other.id;
  pos = other.pos;
  vel = other.vel;
  separationComponent = other.separationComponent;
  cohesionComponent = other.cohesionComponent;
  alignmentComponent = other.alignmentComponent;
  radius = other.radius;
  mass = other.mass;
  maxVel = other.maxVel;
  maxAccel = other.maxAccel;
  viewRadius = other.viewRadius;
  gridCell = other.gridCell;
  drawForce = other.drawForce;
  drawVelocity = other.drawVelocity;
  drawVR = other.drawVR;
}

void Agent::Update(vector<Agent>& agents, double dt) {
  if( !initialized ) {
    cout << "Agent::Update id: " << id << " HAS NOT BEEN INITIALIZED> " << endl;
    exit(-1);
  }
  double newVel = atan2(vel.GetY(), vel.GetX());
  //double separationComponent = 240;
  Vector3d separation(0,0,0);
  Vector3d cohesion(0,0,0);
  Vector3d alignment(0,0,0);
  Vector3d center(0,0,0);
  Vector3d force(0,0,0);
  Vector3d attractionPointForce(0, 0, 0);
  Vector3d avoidPointForce(0, 0, 0);
  Vector3d controlForceX(1000, 0, 0);
  Vector3d controlForceY(0, 1000, 0);

  separationForce.reset();
  cohesionForce.reset();
  alignmentForce.reset();
  int numSeen = 0;
  if( agDebug ) cout << "Update for agent: " << id << endl;
  //generate the forces
  for(int i=0; i<(int)agents.size(); i++) {
    if( id == i ) continue; //skip self
    //see if within view radius
    double dist = (agents[i].GetPos()-pos).norm();
    if( dist < viewRadius ) {
      if( agDebug ) cout << "Agent: " << id << " is within range of agent: " << i << " dist= " << dist << " viewRadius= " << viewRadius << endl;
      if( agDebug ) cout << "SEPDBG agent[" << i << "].GetPos() " << agents[i].GetPos() << " pos["<<id<< "]" << pos << endl;
      if( agDebug ) cout << "SEPDBG agent["<<i<<"]: separation component: " << agents[i].GetPos()-pos << endl;
      
      separation += (pos-agents[i].GetPos()).normalize()*(1.0-dist/viewRadius);
      center += agents[i].GetPos();
      alignment += agents[i].GetVel();
	  numSeen++;
    }
  }//endfor i

  if (toggleAlign) alignmentComponent = 40;
  else alignmentComponent = 0;

//  if (toggleSeparation) separationComponent = 20;
//  else separationComponent = 0;

//  if (toggleCohesion) cohesionComponent = 70;
//  else cohesionComponent = 0;

  if (id == agName) {
	  if (ag == 'r') force += controlForceX;
	  if (ag == 'l') force -= controlForceX;
	  if (ag == 'u') force += controlForceY;
	  if (ag == 'd') force -= controlForceY;
  }

  vector<Vector3d>& attractionPoints = gEnv->GetAttractionPoints();
  int closestAPIndex = -1;
  double closestAPDist = 1e6;
  for (int i = 0; i < (int)attractionPoints.size(); i++) {
	  double thisDist = (attractionPoints[i] - pos).norm();
	  if (thisDist < closestAPDist) {
		  closestAPDist = thisDist;
		  closestAPIndex = i;
	  }
	  if (closestAPIndex != -1) {
		  Vector3d& apt = attractionPoints[closestAPIndex];
		  attractionPointForce = (apt - pos).normalize();
		  attractionPointForce *= 20.0;
	  }
  }

  vector<Vector3d>& avoidPoints = gEnv->GetAvoidPoints();
  int closestAPIndexA = -1;
  double closestAPDistA = 1e6;
  for (int i = 0; i < (int)avoidPoints.size(); i++) {
	  double thisDist = (avoidPoints[i] - pos).norm();
	  if (thisDist < closestAPDistA) {
		  closestAPDistA = thisDist;
		  closestAPIndexA = i;
	  }
	  if (closestAPIndexA != -1) {
		  Vector3d& aptA = avoidPoints[closestAPIndexA];
		  avoidPointForce = (pos - aptA).normalize();
		  avoidPointForce *= 20.0;
	  }
  }


  if( numSeen>0 ) {
    center /= 1.0*numSeen;
    double distToCenter = (center-pos).norm();
    cohesion = (center-pos).normalize()*cohesionComponent*(distToCenter/viewRadius);
    separation = separation * separationComponent;
    alignment = alignment.normalize() * alignmentComponent;

    if( agDebug ) cout << "component vectors for : " << id << endl;
    if(agDebug ) cout << "sep " << separation << endl;
    if( agDebug ) cout << "coh " << cohesion << endl;
    if( agDebug ) cout << "ali " << alignment << endl;

		force = separation + cohesion + alignment + attractionPointForce + avoidPointForce;

	if( force.norm()>maxAccel ) force.selfScale(maxAccel);
    separationForce = separation;
    cohesionForce = cohesion;
    alignmentForce = alignment;
    
    if( agDebug ) cout << "totalf " << force << endl;
    force /= mass;
  }
  else {
    if( agDebug ) cout << "No agents visible for agent:  " << id << endl;
  }
  if( agDebug ) cout << "origPos " << pos << endl;
  if( agDebug ) cout << "origVel " << vel << endl;
  pos = pos + vel*dt;
  vel = vel + force*dt;
  if( agDebug ) cout << "newPos " << pos << endl;
  if( agDebug ) cout << "newVel " << vel << endl;
  if( vel.norm() > maxVel ) {
    vel.selfScale(maxVel);
  }
  if( agDebug ) cout << "newVel(trunc) " << vel << endl;

  if (id == agName)
  {
	  agColor = true;
  }
  else
	  agColor = false;

  bool wrapWorld= true;
  if( wrapWorld ) {
    bool updated=false;
    Vector3d pNew = gEnv->GetWrappedPosition(pos, updated);
    if( updated )
      pos = pNew;
  }

}

void drawCircle(double radius, int divisions) {
  double deltaAng = TWOPI / divisions;
  double curAng = 0;


	  glColor3f(myRand(), myRand(), myRand());

	  glBegin(GL_POLYGON);
  for(int i=0; i<divisions; i++) {
	  double x = radius * cos(curAng);
	  double y = radius * sin(curAng);
	  if( agDebug ) cout << "Circle " << i << " x= " << x << " y= " << y << endl;
	  glVertex2f( x, y );
	  curAng += deltaAng;
	  if( agDebug ) cout << "curAng= " << curAng << " deltaAng= " << deltaAng << endl;

	}
  glEnd();
}

void Agent::Draw() {

  glColor3f(0.2, 0.2, 0.8);
  /*
  glPointSize(5);
  glBegin(GL_POINTS);
  glVertex2f( pos.GetX(), pos.GetY() );
  glEnd();
  */
  glPushMatrix();
  glTranslatef(pos.GetX(), pos.GetY(), 0);
  drawCircle(radius, 3);
  glPopMatrix();

  if( drawForce ) {
    glColor3f(1.0, 0.48, 0.0);
    glBegin(GL_LINES);
    glVertex2f( pos.GetX(),pos.GetY() );
    glVertex2f( pos.GetX()+separationForce.GetX(), pos.GetY()+separationForce.GetY() );
    glEnd();

    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2f( pos.GetX(),pos.GetY() );
    glVertex2f( pos.GetX()+cohesionForce.GetX(), pos.GetY()+cohesionForce.GetY() );
    glEnd();

    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f( pos.GetX(),pos.GetY() );
    glVertex2f( pos.GetX()+alignmentForce.GetX(), pos.GetY()+alignmentForce.GetY() );
    glEnd();
  }

  if( drawVelocity ) {
    //cout << "drawVelocity" << endl;
    glColor3f(1.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex2f( pos.GetX(),pos.GetY() );
    glVertex2f( pos.GetX()+vel.GetX(), pos.GetY()+vel.GetY() );
    glEnd();
  }
  else { /*cout << "NO drawVelocity. " << endl;*/}
}

void Agent::DrawAttractionPoint() {

	glColor3f(1.0, 0.0, 0.0);
	glPushMatrix();
	drawCircle(5, 20);
	glPopMatrix();
}

void Agent::DrawAvoidPoint() {

}

void Agent::toggleCohesion() {
	if (cohesionComponent > 0) cohesionComponent = 0;
	else cohesionComponent = 70;

}

void Agent::toggleAlignment() {
	if (alignmentComponent > 0) alignmentComponent = 0;
	else alignmentComponent = 40;
}
void Agent::toggleSeparation() {
	if (separationComponent > 0) separationComponent = 0;
	else separationComponent = 40;
}

void Agent::controlAgent(char c) {
	ag = c;
}
