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
  vector<int> H_child,V_child,H_parent,V_parent;
};

typedef vector<Module_Info> Modules_Info;

class FPlan{
  public:
    FPlan(float calpha);
    //read file and parsing
    void read(const char*,const char *,const char *);
    virtual void init()         =0;
    virtual void packing();
    virtual void perturb()      =0;
    virtual void keep_sol()     =0;
    virtual void keep_best      =0;
    virtual void recover()      =0;
    virtual void recover_best() =0;
    //basic function
    int    size()          { return modules_N;  }
    double getTotalArea()  { return TotalArea;  }
    double getArea()       { return Area;       }
    int    getWireLength() { return WireLength; }
    double getWidth()      { return Width;      }
    double getHeight()     { return Height;     }
    float  getDeadSpace();

    //list information
    void list_information();
    void show_modules();
    void normalize_cost(int);
    void set_time(double);
    
  protected:
    void clear();
    double calcWireLength();
    void scaleIOPad();
    
    //parameter
    double Area;
    double Width,Height;
    int WireLength;
    double TotalArea;
    //module number
    int modules_N;
    Modules modules;
    Module root_module;
    Modules_Info modules_info;
    Nets network;
    double norm_area,norm_wire;
    float cost_alpha;
    double time;
    vector<vector<int>> connection;
    int max_Height,max_Width;
  private:
    map<string,int> net_table;
    string filename;
};
