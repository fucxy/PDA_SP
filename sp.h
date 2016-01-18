// Project: Sequense pair floorplanning

//--------------------------------------------------------------------------
#ifndef spairH
#define spairH
//--------------------------------------------------------------------------
#include <vecotr>
#include <cassert>
#include "fplan.h"
//--------------------------------------------------------------------------
const int NIL = -1;
typedef bool DIR;
const bool H=0,V=1;
struct Node{
  int id;
  vector<int> H_child,V_clild,H_parent,V_parent;
  bool rotate;
}

class Spair: public FPlain{
  public:
    Spair(float calpha=1):FPlan(calpha){}
    virtual void init();
    virtual void packing();
    virtual void perturb();
    virtual void keep_sol();
    virtual void keep_best();
    virtual void recover();
    virtual void recover_best();
    
    //debug
    void testing();

  protected:
  
    void show_pair();
    void place_module(int mod_id);
    bool legal();
    void clear();
    
    //Auxilary function
    void add_changed_nodes(int n);
    
    void single_swap(Node &n1,Node &n2);
    void double_swap(int n1,int n2);
    
    Node start,term;
    vecotr<Node> hi_nodes,lo_nodes; 
  private:
    struct Solution{
      vector<Node> hi_nodes,lo_nodes;
      double cost;
      Solution() {cost = 1;}
      void clear() { cost = 1, hi_nodes.clear(),lo_nodes.clear();}
    }
    void get_solution(Solution &sol);
    void recover(Solution &sol);
    
    Solution best_sol,last_sol;
    //for partial recover
    vector<Node> changed_hi_nodes,changed_lo_nodes;
}
//--------------------------------------------------------------------------
#endif
