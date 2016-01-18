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
    hi_nodes[i].id = i;
    lo_nodes[i].id = i;   
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

}

//--------------------------------------------------------------------------
//Placement modules
//--------------------------------------------------------------------------

//get Outline 
void Spair::packing(){
  double max_x=-1,max_y=-1;
  //child parent initialize
  for(int i=0;i<hi_nodes.size();i++){
    hi_nodes[i].H_child.clear();
    hi_nodes[i].V_child.clear();
    hi_nodes[i].H_parent.clear();
    hi_nodes[i].H_parent.clear();
    lo_nodes[i].H_child.clear();
    lo_nodes[i].V_child.clear();
    lo_nodes[i].H_parent.clear();
    lo_nodes[i].H_parent.clear();
  }
  //H_child H_parent
  for(int i=0;i<hi_nodes.size();i++)
  {
  	int lo_index;
  	for(int j=0;j<lo_nodes.size();j++)
  	{
  		if(lo_nodes[j].id==hi_nodes[i].id)
  		{
  			lo_index = j;
  			break;
  		}
  	}
  	for(int j=i+1;j<hi_nodes.size();j++)
  	{
  		for(int k=lo_index+1;k<lo_nodes.size();k++)
  		{
  			if(hi_nodes[j].id==lo_nodes[k].id)
  			{
  				hi_nodes[i].H_child.push_back(hi_nodes[j].id);
  				hi_nodes[j].H_parent.push_back(hi_nodes[i].id);
  				break;
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
  		if(lo_nodes[j].id==hi_nodes[i].id)
  		{
  			lo_index = j;
  			break;
  		}
  	}
  	for(int j=i+1;j<hi_nodes.size();j++)
  	{
  		for(int k=lo_index-1;k>=0;k--)
  		{
  			if(hi_nodes[j].id==lo_nodes[k].id)
  			{
  				hi_nodes[i].V_child.push_back(hi_nodes[j].id);
  				hi_nodes[j].V_parent.push_back(hi_nodes[i].id);
  				break;
  			}
  		}
  	}
  }

  //place_module XXXX
  Width  = max_x;
  Height = max_y;
  Area   = Height*Width;
  
  //for wirelength
  FPan::packing(); 
}

void Spair::place_module(int mod_id){
  Module_Info &mod_mf = modules_info[mod];
  mod_mf.rotate = hi_nodes[mod].rotate;

  int w = modules[mod].width;
  int h = modules[mod].height;
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
    lo_nodes[n].rotate=!lo_nodes[n].rotate;
  }
  else{
    if(single_swap_rate>rand_01()){
      do{ 
        p = rand()%modules_N;
      }while(n==p)
      if(rand()%2==0){
        single_swap(hi_nodes[n],hi_nodes[p]);
      }else{
        single_swap(lo_nodes[n],lo_nodes[p]);
      }
    }else{
      do{
        p = rand()%modules_N;
      }while(n==p)
      double_swap(n,p);
    }
  }
}

void single_swap(Node &n1,Node &n2){
  swap(n1.id,n2.id);
  swap(n1.V_parent,n2.V_parent);
  swap(n1.H_parent,n2.H_parent);
  swap(n1.rotate,n2.rotate);
  swap(n1.H_child,n2.H_child);
  swap(n1.V_child,n2.V_child);
}

void double_swap(int n1,int n2){
  single_swap(lo_nodes[n],lo_nodes[p]);
  single_swap(hi_nodes[n],hi_nodes[p]);
}
