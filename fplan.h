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
