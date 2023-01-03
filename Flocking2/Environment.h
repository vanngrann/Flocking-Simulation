#ifndef ENVIRONMENT
#define ENVIRONMENT

#include "Point.h"
#include "Vector.h"
using namespace mathtool;
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Cell {
public:
  Cell();
  void Init(int _i, int _j, Point _center, double _dx, double _dy, bool _blocked);
  void Draw();

  int xi;
  int xj;
  double dx;
  double dy;
  Point center;
  Point minPt;
  Point maxPt;
  bool blocked;
};

class Environment {
public:
  Environment();
  Environment(string _inputFile, double _xmin, double _xmax, double _ymin, double _ymax );
  void Draw();
  Vector3d GetWrappedPosition(Vector3d p, bool& updated);

  void AddAttractionPoint(double x, double y);
  vector<Vector3d> attractionPoints;
  vector<Vector3d>& GetAttractionPoints() { return attractionPoints; }

  void AddAvoidPoint(double x, double y);
  vector<Vector3d> avoidPoints;
  vector<Vector3d>& GetAvoidPoints() { return avoidPoints; }

  Cell** cells;

  double xmin, xmax, ymin, ymax; 
  int rows, columns;
  string inputFile;
};

#endif
