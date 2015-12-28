#ifndef fplanH
#define fplanH
//--------------------------------------------------------------------------
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cstdio>
//--------------------------------------------------------------------------
using namespace std;

struct Pin{
  int mod;
  int net;
  int x,y;
  int ax,ay;
  Pin(int x_=-1,int y_=-1){ x=x_,y=y_; }
};
typedef vector<Pin> Pins;
typedef Pin* Pin_p;
typedef vector<Pin_p> Net;
typedef vector <Net> Nets;

struct Module{
  int id;
  char name[20];
  int width,height;
  int x=0,y=0;
  int area;
  Pins pins;
};
typedef vector<Module> Modules;

struct Module_Info{
  bool rotate;
  int x,y;
  int rx,ry;
};

typedef vector<Module_Info> Modules_Info;

