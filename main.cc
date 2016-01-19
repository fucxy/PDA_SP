#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include "sa.h"
#include "sp.h"
using namespace std;
string temp_str;
float a=0.0;
int times=400,local=7;
float init_temp=0.9,term_temp;
int main(int argc,char * argv[]){
  if(argc!=5){
    cout<<"Usage:./FP a XXX.block XXX.net XXX.output"<<endl;
    return 0;
  }
  a=atof(argv[1]);
  try{
    double time = seconds();
    Spair fp(a);
    fp.read(argv[2],argv[3],argv[4]);
    fp.init();
    double last_time = SA_Floorplan(fp,time,local,term_temp);
    fp.set_time(last_time);
    fp.list_information();
  }catch(...){}
  return 1;
}
