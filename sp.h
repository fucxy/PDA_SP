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
    void set_long_height(int id);
    void set_long_weight(int id);
    void show_pair();
    void place_module(int mod_id);
    bool legal();
    void clear();
    
    //Auxilary function
    void add_changed_nodes(int n);
    
    void single_swap(int n1,int n2,int i);
    void double_swap(int n1,int n2);
    vecotr<int> hi_nodes,lo_nodes; 
  private:
    struct Solution{
      vector<int> hi_nodes,lo_nodes;
      double cost;
      Solution() {cost = 1;}
      void clear() { cost = 1, hi_nodes.clear(),lo_nodes.clear();}
    }
    void get_solution(Solution &sol);
    void recover(Solution &sol);
    
    Solution best_sol,last_sol;
    //for partial recover
    vector<int> changed_hi_nodes,changed_lo_nodes;
}
//--------------------------------------------------------------------------
#endif
