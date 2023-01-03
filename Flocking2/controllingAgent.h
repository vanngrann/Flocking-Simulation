class ControllingAgent {
public:
	ControllingAgent() {
	};

	double doubSeparation;
	double doubCohesion;
	double doubAlignment;

	bool singleAgent;
	bool groupAgent;
	bool attractPoint;
	bool avoidPoint;

	double getDoubSeparation() { return doubSeparation; }
	double getDoubCohesion() { return doubCohesion; }
	double getDoubAlignment() { return doubAlignment; }



};