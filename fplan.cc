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
  cout<<"max_width:"<<max_Width<<" ,max_Height:"<<max_Height<<" ,width:"<<Width<<" ,Height:"<<Height<<endl;
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

void FPlan::read(const char *block_file,const char *net_file,const char* rt_file){
  cout<<"Read file"<<endl;
  int ta,tb,th,tw;
  filename = rt_file;
  fs.open(block_file,ios::in);
  if(fs==NULL)
    error("unable to open block file","");
  fs >> t1 >> max_Width >> max_Height;
  fs >> t1 >>ta;
  fs >> t1 >>tb;
  bool final = false;
  Module dummy_mod;
  for(int i=0;i<ta;i++){
    modules.push_back(dummy_mod);
    Module &mod = modules.back();
    mod.id = i;
    mod.pins.clear();
    fs >> t1 >> mod.width >> mod.height;
    strcpy(mod.name,t1);
    mod.area = mod.width * mod.height;
    Pin p;
    p.mod = i;
    p.x = mod.width/2;
    p.y = mod.height/2;
    p.net = INT_MAX;
    mod.pins.push_back(p);
  }
  //root terminal module
  root_module.id = ta;
  root_module.pins.clear();
  strcpy(root_module.name,"terminal");
  root_module.width = max_Width;
  root_module.height = max_Height;
  root_module.area = max_Width * max_Height;
  if(tb > 0)
    for(int i=0;i<tb;i++){
      fs >> t1 >> t2 >> tw >>th;
      Pin p;
      p.mod = ta;
      p.x = tw;
      p.y = th;
      p.net = INT_MAX;
      root_module.pins.push_back(p);
      net_table.insert(make_pair(string(t1),net_table.size()));
    }
  modules.push_back(root_module);
  fs.close();
  //finish block file

  //read .net
  fs.open(net_file,ios::in);
  fs >> t1 >> ta;
  for(int i=0;i<ta;i++){
    fs >> t1 >> tb;
    int m_id,n_id=net_table.size();
    ostringstream ss;
    ss << i;
    net_table.insert(make_pair(ss.str()+"_fucxy",net_table.size()));
    for(int j=0;j<tb;j++){
      fs >>t1;
      for(m_id=0; m_id < modules.size()-1;m_id++)
        if(!strcmp(modules[m_id].name,t1)){
          break;
        }
      if(m_id != modules.size()-1){
        Pin p=modules[m_id].pins[0];
        p.net=n_id;
        modules[m_id].pins.push_back(p);
      }
      else{
        Pin p=modules[m_id].pins[net_table[t1]] ;
        p.net=n_id;
        modules[m_id].pins.push_back(p);
      }
    } 
  }
  ta=modules.size()-1;
  for(int i=0;i<ta;i++)
   if(modules[i].pins.size()!=0)
     modules[i].pins.erase(modules[i].pins.begin());
  if(modules[ta].pins.size()!=0)
  while( modules[ta].pins[0].net==INT_MAX){
   modules[ta].pins.erase(modules[ta].pins.begin());
  }
  fs.close();
//net read end

  root_module = modules.back();
  modules.pop_back();
  modules_N = modules.size();
  modules_info.resize(modules_N);
  modules.resize(modules_N);
  network.resize(net_table.size());
  for(int i=0;i<0;i++)
   network[i].clear();
  for(int i=0; i < modules_N; i++){
    for(int j=0; j < modules[i].pins.size(); j++){
      Pin &p = modules[i].pins[j];
      network[p.net].push_back(&p);
    }
  }
  for(int i=0; i < root_module.pins.size(); i++){
    Pin &p = root_module.pins[i];
    network[p.net].push_back(&p);
  }
  
  connection.resize(modules_N+1);
  for(int i=0; i < modules_N+1; i++){
    connection[i].resize(modules_N+1);
    fill(connection[i].begin(), connection[i].end(), 0);
  }
  for(int i=0; i < network.size(); i++){
    if( network[i].size()>1)
    for(int j=0; j < network[i].size()-1; j++){
      int p= network[i][j]->mod;
      for(int k=j+1 ; k < network[i].size(); k++){
        int q= network[i][k]->mod;
        connection[p][q]++;
        connection[q][p]++;
      }
    }
  }
  
  TotalArea = 0;
  for(int i=0; i < modules_N; i++)
    TotalArea += modules[i].area;
}
//-----------------
//WireLength
//-----------------

void FPlan::scaleIOPad(){
  float px = Width/float(root_module.width);
  float py = Height/float(root_module.height);
  for(int i=0; i < root_module.pins.size(); i++){
    Pin &p = root_module.pins[i];
    p.ax =  int(px * p.x);
    p.ay =  int(py * p.y);

  }
}

double FPlan::calcWireLength(){
  WireLength=0;
  // scaleIOPad();
  // compute absolute position
  for(int i=0; i < modules_N; i++){
    int mx= modules_info[i].x, my= modules_info[i].y;
    bool rotate= modules_info[i].rotate;
    int w= modules[i].width;
    for(int j=0; j < modules[i].pins.size(); j++){
      Pin &p = modules[i].pins[j];
      if(!rotate){
        p.ax= p.x+mx, p.ay= p.y+my;
      }
      else{ // Y' = W - X, X' = Y
        p.ax= p.y+mx, p.ay= (w-p.x)+my;
      }
    }
  }

  for(int i=0; i < network.size(); i++){
    int max_x= INT_MIN, max_y= INT_MIN;
    int min_x= INT_MAX, min_y= INT_MAX;

    assert(network[i].size() > 0);
    for(int j=0; j < network[i].size(); j++){
      Pin &p = *network[i][j];
      max_x= max(max_x, p.ax), max_y= max(max_y, p.ay);
      min_x= min(min_x, p.ax), min_y= min(min_y, p.ay);
    }
//    printf("%d %d %d %d\n",max_x,min_x,max_y,min_y);
    WireLength += (max_x-min_x)+(max_y-min_y);
  }
  return WireLength;
}
double FPlan::calcWireLength1(){
  WireLength=0;
   for(int i=0; i < root_module.pins.size(); i++){
    Pin &p = root_module.pins[i];
    p.ax =  p.x;
    p.ay =  p.y;

  }

  // compute absolute position
  for(int i=0; i < modules_N; i++){
    int mx= modules_info[i].x, my= modules_info[i].y;
    bool rotate= modules_info[i].rotate;
    int w= modules[i].width;

    for(int j=0; j < modules[i].pins.size(); j++){
      Pin &p = modules[i].pins[j];
      if(!rotate){
        p.ax= p.x+mx, p.ay= p.y+my;
      }
      else{ // Y' = W - X, X' = Y
        p.ax= p.y+mx, p.ay= (w-p.x)+my;
      }
    }
  }

  for(int i=0; i < network.size(); i++){
    int max_x= INT_MIN, max_y= INT_MIN;
    int min_x= INT_MAX, min_y= INT_MAX;

    assert(network[i].size() > 0);
    for(int j=0; j < network[i].size(); j++){
      Pin &p = *network[i][j];
      max_x= max(max_x, p.ax), max_y= max(max_y, p.ay);
      min_x= min(min_x, p.ax), min_y= min(min_y, p.ay);
    }
//    printf("%d %d %d %d\n",max_x,min_x,max_y,min_y);
    WireLength += (max_x-min_x)+(max_y-min_y);
  }
  return WireLength;
}

//-----------------
//Show Info
//-----------------

string query_map(map<string,int> M,int value){
  for(map<string,int>::iterator p=M.begin(); p != M.end(); p++){
    if(p->second == value)
      return p->first;
  }
  return "";
}

void FPlan::show_modules()
{
  for(int i=0; i < modules.size();i++){
    cout << "Module: " << modules[i].name << endl;
    cout << "  Width = " << modules[i].width;
    cout << "  Height= " << modules[i].height << endl;
    cout << "  Area  = " << modules[i].area << endl;
    cout << modules[i].pins.size() << " Pins:\n";
    for(int j=0; j < modules[i].pins.size(); j++){
      cout << query_map(net_table,modules[i].pins[j].net) << " ";
      cout << modules[i].pins[j].x << " " << modules[i].pins[j].y << endl;
    }
  }
}

void FPlan::list_information(){

  /*if(inv){
    swap(Width,Height);
    for(int i=0;i<modules_N;i++){
      swap(modules_info[i].x,modules_info[i].y);
      swap(modules_info[i].rx,modules_info[i].ry);
    }
    for(int i=0;i<modules_N;i++)
      for(int j=0; j < modules[i].pins.size(); j++){
        Pin &p = modules[i].pins[j];
        swap(p.x,p.y) ;
      }
    }*/
  fstream of(filename.c_str(),ios::out|ios::trunc);
  double wirel=calcWireLength1();
  of <<  cost_alpha*Area+(1-cost_alpha)*wirel << endl;
  of << wirel <<endl;
  of << Width * Height<<endl;
  of << Width << "\t" << Height << endl;
  of << time << endl;
  for(int i=0; i < modules_N; i++){
    of <<modules[i].name<<"\t"<< modules_info[i].x  << "\t" << modules_info[i].y  << "\t";
    of << modules_info[i].rx << "\t" << modules_info[i].ry << endl;
  }
  calcWireLength();
  int x,y,rx,ry;
  /*
  for(int i=0; i < network.size(); i++){
    cout<<network[i].size()<<endl;
    if(network[i].size()>0){
      x = network[i][0]->ax;
      y = network[i][0]->ay;
      for(int j=1; j < network[i].size(); j++){
        rx = network[i][j]->ax;
        ry = network[i][j]->ay;
        of << x << " " << y << " " << rx << " " << ry << endl;
        x = rx, y = ry;
      }
    }
  }
  */
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

void FPlan::set_time(double t){
  time=t;
}
