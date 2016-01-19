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
    cout<<hi_nodes[i].id<<" ";
  }
  cout<<endl;
  cout<<"lo_nodes:";
  for(int i;i<lo_nodes.size();i++){
    cout<<lo_nodes[i].id<<" ";
  }
  cout<<endl;
}

//--------------------------------------------------------------------------
//Placement modules
//--------------------------------------------------------------------------

//get Outline 
void Spair::packing(){
  
  void set_long_width(int id)
  {
  	if(Modules_Info[id].H_parent.size()==0)
  	{
  		Modules_Info[id].x = 0;
  		Modules_Info[id].rx = Module[id].width;
  	}
  	else 
  	{
  		int max=0;
  		for(int j=0;j<Modules_Info[id].H_parent.size();j++)
  		{
  			if(Modules_Info[Modules_Info[id].H_parent[j]].rx > max)
  			{
  				max = Modules_Info[Modules_Info[id].H_parent[j]].rx;
  			}
  		}
  		Modules_Info[id].x = max;
  		Modules_Info[id].rx = Modules_Info[id].x + Module[id].width;
  	}
  }

  void set_long_height(int id)
  {
  	if(Modules_Info[id].H_parent.size()==0)
  	{
  		Modules_Info[id].y = 0;
  		Modules_Info[id].ry = Module[id].height;
  	}
  	else 
  	{
  		int max=0;
  		for(int j=0;j<Modules_Info[id].V_parent.size();j++)
  		{
  			if(Modules_Info[Modules_Info[id].V_parent[j]].ry > max)
  			{
  				max = Modules_Info[Modules_Info[id].V_parent[j]].ry;
  			}
  		}
  		Modules_Info[id].y = max;
  		Modules_Info[id].ry = Modules_Info[id].y + Module[id].height;
  	}
  }
  
  double max_x=-1,max_y=-1;
  //child parent initialize
  for(int i=0;i<hi_nodes.size();i++){
    modules_info[i].H_child.clear();
    modules_info[i].V_child.clear();
    modules_info[i].H_parent.clear();
    modules_info[i].H_parent.clear();
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
  				Modules_Info[hi_nodes[i]].H_child.push_back(hi_nodes[j]);
  				Modules_Info[hi_nodes[j]].H_parent.push_back(hi_nodes[i]);
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
  				Modules_Info[hi_nodes[i]].V_child.push_back(hi_nodes[j]);
  				Modules_Info[hi_nodes[j]].V_parent.push_back(hi_nodes[i]);
  				break;
  			}
  		}
  	}
  }
  
  //place...
  vector <int> H_candidate, V_candidate, Eliminate;

  for(int i=0;i<Modules_Info.size();i++)
  {
  	if(Modules_Info[i].H_parent.size()==0)
  		H_candidate.push_back(i);
  	if(Modules_Info[i].V_parent.size()==0)
  		V_candidate.push_back(i);
  }
  
  while(H_candidate.size()!=0 && V_candidate.size()!=0)
  {
  	for(int i=0;i<H_candidate.size();i++)
  	{
  		vector<int>::iterator it;
  		it = find (V_candidate.begin(), V_candidate.end(), H_candidate[i]);
  		if (it != V_candidate.end())//found element
  		{
  			set_long_width(H_candidate[i]);
  			set_long_height(H_candidate[i]);
  			break;
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
    modules_info[n].rotate=!modules_info[n].rotate;
  }
  else{
    if(single_swap_rate>rand_01()){
      do{ 
        p = rand()%modules_N;
      }while(n==p)
      if(rand()%2==0){
        single_swap(n,p,0);
      }else{
        single_swap(n,p,1);
      }
    }else{
      do{
        p = rand()%modules_N;
      }while(n==p)
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
  single_swap(n,p,1);
  single_swap(n,p,0);
}
