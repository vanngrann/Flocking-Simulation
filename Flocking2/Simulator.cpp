#include "Simulator.h"
#include <stdlib.h>

#include <GL/glut.h>  // (or others, depending on the system in use)
#include "Basic.h"
#include"controllingAgent.h"
bool cohesion = true;
bool separation = true;
bool alignment = true;

ControllingAgent a;
extern int agName;


Simulator::Simulator() {
	for (int i = 0; i<70; i++) {
		//pos
		double x = 50 + 200 * (myRand() - 0.5);
		double y = 50 + 200 * (myRand() - 0.5);
		double z = 0;
		Vector3d pos(x, y, z);
		//vel
		double vx = 10 * (myRand() - 0.5);
		double vy = 10 * (myRand() - 0.5);
		double vz = 0;
		Vector3d vel(vx, vy, vz);
		double mass = 4 + myRand() * 10; //random between 4 and 14
		double maxVel = 8 + myRand() * 5; //random between 8 and 13
		double maxAccel = 20 + myRand() * 10; //random between 20 and 30
		double viewRadius = 40 + myRand() * 10; //random between 40 and 50


		Agent a_i;
		a_i.Init(i, pos, vel, mass, maxVel, maxAccel, viewRadius);
		agents.push_back(a_i);
	}
}

void Simulator::Draw() {
  for(int i=0; i<(int)agents.size(); i++) {
    agents[i].Draw();
  }
}

void Simulator::Update() {
  double dt = 0.075;
  for(int i=0; i<(int)agents.size(); i++) {
    agents[i].Update(agents, dt);
  }


}

void Simulator::AddAgent(double a, double b) {

    //pos
	double x = a;
	double y = b;
    double z = 0;
    Vector3d pos(x,y,z);
    //vel
	double vx = 10*(myRand()-0.5);
	double vy = 10 * (myRand() - 0.5);
    double vz = 0;
    Vector3d vel(vx,vy,vz);
    double mass = 4 + myRand() * 10; //random between 4 and 14
    double maxVel = 8 + myRand() * 5; //random between 8 and 13
	double maxAccel = 20 + myRand() * 10; //random between 20 and 30
    double viewRadius = 40 + myRand() * 10; //random between 40 and 50


	Agent a_i;

    a_i.Init(0,pos,vel,mass,maxVel,maxAccel,viewRadius);
    agents.push_back( a_i );


}

void Simulator::ToggleCohesion() {
	for (int i = 0; i < (int)agents.size(); i++) {
		agents[i].toggleCohesion();
	}
}


void Simulator::ToggleAlignment() {
	for (int i = 0; i < (int)agents.size(); i++) {
		agents[i].toggleAlignment();
	}
}

void Simulator::ToggleSeparation() {
	for (int i = 0; i < (int)agents.size(); i++) {
		agents[i].toggleSeparation();
	}
}

void Simulator::changeAgent(char c) {
	agents[agName].controlAgent(c);
}