// Project: Sequence pair floorplanning 

//--------------------------------------------------------------------------
#include <stack>
#include <algorithm>
#include "sp.h"
#include <iostream>
#include <climits>
using namespace std;
//--------------------------------------------------------------------------
float rotate_rate = 0.3;
float single_swap_rate = 0.5;

//--------------------------------------------------------------------------

void Spair::clear(){
  FPlan::clear();
}

//initialize Spair 
void Spair::init(){
  hi_nodes.resize(modules_N);
  lo_nodes.resize(modules_N);
  for(int i=0;i < modules_N;i++){
    hi_nodes[i] = i;
    lo_nodes[i] = i;   
    modules_info[i].V_child.clear();
    modules_info[i].H_child.clear();
    modules_info[i].V_parent.clear();
    modules_info[i].H_parent.clear();
  }
  best_sol.clear();
  last_sol.clear();
  clear();
  normalize_cost(10);
}
//--------------------------------------------------------------------------
//Testing, deubug function
//--------------------------------------------------------------------------
bool Spair::legal(){
  
}

void Spair::testing(){


}

//display pair
void Spair::show_pair(){
  cout<<"hi_nodes:";
  for(int i;i<hi_nodes.size();i++){
    cout<<hi_nodes[i]<<" ";
  }
  cout<<endl;
  cout<<"lo_nodes:";
  for(int i;i<lo_nodes.size();i++){
    cout<<lo_nodes[i]<<" ";
  }
  cout<<endl;
}

//--------------------------------------------------------------------------
//Placement modules
//--------------------------------------------------------------------------

//get Outline 
void Spair::packing(){
  Width  = -1;
  Height = -1;
  //child parent initialize
  for(int i=0;i<modules_info.size();i++){
    modules_info[i].H_child.clear();
    modules_info[i].V_child.clear();
    modules_info[i].H_parent.clear();
    modules_info[i].V_parent.clear();
  }
  //H_child H_parent
  for(int i=0;i<hi_nodes.size();i++)
  {
  	int lo_index;
  	for(int j=0;j<lo_nodes.size();j++)
  	{
  		if(lo_nodes[j]==hi_nodes[i])
  		{
  			lo_index = j;
  			break;
  		}
  	}
  	for(int j=i+1;j<hi_nodes.size();j++)
  	{
  		for(int k=lo_index+1;k<lo_nodes.size();k++)
  		{
  			if(hi_nodes[j]==lo_nodes[k])
  			{
  				modules_info[hi_nodes[i]].H_child.push_back(hi_nodes[j]);
  				modules_info[hi_nodes[j]].H_parent.push_back(hi_nodes[i]);
  			}
  		}
  	}
  }
  //V_child V_parent
  for(int i=0;i<hi_nodes.size();i++)
  {
  	int lo_index;
  	for(int j=0;j<lo_nodes.size();j++)
  	{
  		if(lo_nodes[j]==hi_nodes[i])
  		{
  			lo_index = j;
  			break;
  		}
  	}
  	for(int j=i+1;j<hi_nodes.size();j++)
  	{
  		for(int k=lo_index-1;k>=0;k--)
  		{
  			if(hi_nodes[j]==lo_nodes[k])
  			{
  				modules_info[hi_nodes[i]].V_child.push_back(hi_nodes[j]);
  				modules_info[hi_nodes[j]].V_parent.push_back(hi_nodes[i]);
  			}
  		}
  	}
  } 
  //place...
  bool not_end;
  vector<bool> exist,H_can,V_can;
  int root;
  double floor_x=0,floor_y=0,max_x=0,max_y=0,local_h,local_w,counter_x,counter_y;
  for(int i=0;i<modules_N;i++){
    exist.push_back(true);
    H_can.push_back(false);
    V_can.push_back(false);
  }
  do{
    //start
    not_end=false; 
    local_h=0;
    local_w=0;
    counter_x=0;
    counter_y=0;
    for(int i=0;i<modules_N;i++){
      H_can[i]=false;
      V_can[i]=false;
    }
    for(int i=0;i<modules_N;i++){
      if(exist[i]){
         H_can[i]=true;
         if(modules_info[i].H_parent.size()!=0)
           for(int j=0;j<modules_info[i].H_parent.size();j++){
             if(exist[modules_info[i].H_parent[j]]){
               H_can[i]=false;
               break;
             }
           }
         V_can[i]=true;
         if(modules_info[i].V_parent.size()!=0)
           for(int j=0;j<modules_info[i].V_parent.size();j++){
             if(exist[modules_info[i].V_parent[j]]){
               V_can[i]=false;
               break;
             }
           }
         if(V_can[i]&&H_can[i]){
           root=i;
         }
           
      }
    }
    int temp_w,temp_h;
    //add Root
    temp_w=modules[root].width;
    temp_h=modules[root].height;
    if(modules_info[root].rotate)
      swap(temp_w,temp_h);
    modules_info[root].x=floor_x;
    modules_info[root].y=floor_y;
    modules_info[root].rx=floor_x+temp_w;
    if(modules_info[root].rx>max_x)
      max_x=modules_info[root].rx;
    modules_info[root].ry=floor_y+temp_h;
    if(modules_info[root].ry>max_y)
      max_y=modules_info[root].ry;
    local_w=temp_w;
    local_h=temp_h;
    counter_x+=temp_w;
    counter_y+=temp_h;
    //add H_can
    for(int i=0;i<modules_N;i++)
      if(i!=root&&H_can[i]&&exist[i]){
        temp_w=modules[i].width;
        temp_h=modules[i].height;
        if(modules_info[i].rotate)
          swap(temp_w,temp_h);
        modules_info[i].x=floor_x+counter_x;
        modules_info[i].y=floor_y;
        modules_info[i].rx=modules_info[i].x+temp_w;
        if(modules_info[i].rx>max_x)
          max_x=modules_info[i].rx;
        modules_info[i].ry=floor_y+temp_h;
        if(modules_info[i].ry>max_y)
          max_y=modules_info[i].ry;
        if(temp_h>local_h)
          local_h=temp_h;
        counter_x+=temp_w;
      }
    //add V_can
    for(int i=0;i<modules_N;i++)
      if(i!=root&&V_can[i]&&exist[i]){
        temp_w=modules[i].width;
        temp_h=modules[i].height;
        if(modules_info[i].rotate)
          swap(temp_w,temp_h);
        modules_info[i].x=floor_x;
        modules_info[i].y=floor_y+counter_y;
        modules_info[i].rx=modules_info[i].x+temp_w;
        if(modules_info[i].rx>max_x)
          max_x=modules_info[i].rx;
        modules_info[i].ry=modules_info[i].y+temp_h;
        if(modules_info[i].ry>max_y)
          max_y=modules_info[i].ry;
        if(temp_w>local_w)
          local_w=temp_w;
        counter_y+=temp_h;
      }
    //remove node
    for(int i=0;i<modules_N;i++){
      if(H_can[i]||V_can[i]){
        exist[i]=false;
      }
    }
    //valid node
    for(int i=0;i<modules_N;i++){
      if(exist[i]){
        not_end=true;
        break;
      }
    }
    //calc floor
    floor_x+=local_w;
    floor_y+=local_h;
  }while(not_end);
  Height=max_y;
  Width=max_x;
  Area   = Height*Width;
  
  //for wirelength
  FPlan::packing(); 
}

void Spair::place_module(int mod_id){
  Module_Info &mod_mf = modules_info[mod_id];
  mod_mf.rotate = modules_info[mod_id].rotate;

  int w = modules[mod_id].width;
  int h = modules[mod_id].height;
  if(mod_mf.rotate)
    swap(w,h);
  //place your modules and uodate to Module_Info

}

//--------------------------------------------------------------------------
//auxilary function
//--------------------------------------------------------------------------
//add youself

//--------------------------------------------------------------------------
//SA Temporal Solution
//--------------------------------------------------------------------------

void Spair::get_solution(Solution &sol){
  sol.cost = getCost();
  sol.lo_nodes = lo_nodes;
  sol.hi_nodes = hi_nodes;
}

void Spair::keep_sol(){
  get_solution(last_sol);
}
void Spair::keep_best(){
  get_solution(best_sol);
}
void Spair::recover(){
  recover(last_sol);
}
void Spair::recover_best(){
  recover(best_sol);
}
void Spair::recover(Solution &sol){
  lo_nodes=sol.lo_nodes;
  hi_nodes=sol.hi_nodes;
}

//--------------------------------------------------------------------------
// SA Permutation Operations
//--------------------------------------------------------------------------
//SA operation
void Spair::perturb(){
  int p,n;
  n = rand()%modules_N;
  if(rotate_rate > rand_01()){
    modules_info[n].rotate=!modules_info[n].rotate;
  }
  else{
    if(single_swap_rate>rand_01()){
      do{ 
        p = rand()%modules_N;
      }while(n==p);
      if(rand()%2==0){
        single_swap(n,p,0);
      }else{
        single_swap(n,p,1);
      }
    }else{
      do{
        p = rand()%modules_N;
      }while(n==p);
      double_swap(n,p);
    }
  }
}

void Spair::single_swap(int n1,int n2,int i){
  if(i==0)
    swap(lo_nodes[n1],lo_nodes[n2]);
  else
    swap(hi_nodes[n1],hi_nodes[n2]);
}

void Spair::double_swap(int n1,int n2){
  single_swap(n1,n2,1);
  single_swap(n1,n2,0);
}
