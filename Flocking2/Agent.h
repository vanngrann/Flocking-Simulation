#ifndef AGENT
#define AGENT
#include "Vector.h"
using namespace mathtool;
#include <vector>
using namespace std;

class Agent {
public:
  Agent();
  Agent(const Agent& other);

  void Init(int _id, Vector3d _pos, Vector3d _vel, double _mass, double _maxVel, double _maxAccel, double _viewRadius);
  Vector3d& GetPos() { return pos; }
  Vector3d& GetVel() { return vel; }

  void Update(vector<Agent>& agents, double dt);
  void Draw();
  void DrawAttractionPoint();
  void DrawAvoidPoint();
  void toggleCohesion();
  void toggleAlignment();
  void toggleSeparation();

  void controlAgent(char c);

  double separationComponent;
  double cohesionComponent;
  double alignmentComponent;

private:

	vector<Agent> agents;
	vector<Agent> points;
  int id;
  Vector3d pos;
  Vector3d vel;
  
  Vector3d separationForce;
  Vector3d cohesionForce;
  Vector3d alignmentForce;
  
  double radius;
  double mass;
  double maxVel;
  double maxAccel;
  double viewRadius;

  bool initialized;


  
  pair<int,int> gridCell;

  bool drawForce;
  bool drawVelocity;
  bool drawVR;
};

#endif
