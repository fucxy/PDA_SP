// Project: Sequence Pair floorplanning


//------------------------------------------------------------------------
#include "fplan.h"
#include <fstream>
#include <cstdio>
#include <cstring>
#include <climits>
#include <sys/time.h>
#include <sys/resource.h>
#include <cassert>
#include <sstream>
#include <iostream>
#include <cstdlib>
using namespace std;
//------------------------------------------------------------------------
char line[100],t1[40],t2[40]; 
ifstream fs;

//initial function
FPlan::FPlan(float calpha){
  norm_area = 1;
  norm_wire = 1;
  cost_alpha = calpha;
}

void FPlan::packing(){
  if(cost_alpha!=1)
    calcWireLength();
}

void FPlan::clear(){
  Area = 0;
  WireLength = 0;
}

double FPlan::getCost(){
  //debug message
  cout<<"max_width:"<<max_Width<<" ,max_Height:"<<max_Heigth<<" ,width:"<<Width<<" ,Height:"<<Heigth<<endl;

  return (cost_alpha*(Area/norm_area)+(1-cost_alpha)*(WireLength/norm_wire));
}

float FPlan::getDeadSpace(){
  return 100*(Area-TotalArea)/float(Area);
}

//func that you can normalize cost
void FPlan::normalize_cost(int t){

}
//------------------
//Read
//------------------

void FPlan::read(const char *block_file,const char *net_file,const char* rf_file){
  cout<<"Read file"<<endl;
  int ta,tb,th,tw;
  filename = rt_file;
  fs.open(block_file);
}

//-----------------
//Extra Func
//-----------------

void error(char *msg,char *msg2=(char *)""){
  printf(msg,msg2);
  cout<<endl;
  throw 1;
}

bool rand_bool(){
  return bool(rand()%2);
}

float rand_01(){
  return float(rand()%10000)/10000;
}

double seconds(){
   rusage time;
   getrusage(RUSAGE_SELF,&time);
   return (double)(1.0*time.ru_utime.tv_sec+0.000001*time.ru_utime.tv_usec);
}
