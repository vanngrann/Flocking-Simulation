#ifndef SIMULATOR
#define SIMULATOR

#include "Vector.h"
using namespace mathtool;

#include "Agent.h"

#include <vector>
using namespace std;

class Simulator {
   public:
      Simulator(); 

      void Draw();
      void Update();
	  void AddAgent(double x, double y);
	  void AddAttractionPoint(double x, double y);
	  void AddAvoidPoint(double x, double y);
	  void ToggleCohesion();
	  void ToggleAlignment();
	  void ToggleSeparation();

	  void changeAgent(char c);

	  vector<Agent> agents;
	  vector<Agent> points;
   private:


};

#endif
