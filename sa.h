//---------------------------------------------------------------------
#ifndef sa_spairH
#define sa_spairH
//---------------------------------------------------------------------
#include "fplan.h"
extern float init_avg;
extern float avg_ratio;
extern float lamda;

double SA_Floorplan(FPlan &fp, int k, int local ,float Term_T);
double Random_Floorplan(FPlan &fp,int time);
//---------------------------------------------------------------------
#endif
