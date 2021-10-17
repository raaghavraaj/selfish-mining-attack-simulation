#include <bits/stdc++.h>
#include "utils.h"
#define ll unsigned long long
using namespace std;
default_random_engine generator;
uniform_real_distribution<double> unif_dist(0.0, 1.0);
///// INITIALISES UTILS DATA STRUCTURES /////
void init_utils(int seed){
  srand(seed);
  generator.seed(seed);
}
////// GENERATES A RANDOM NUMBER FROM EXPO DIST WITH GIVEN MEAN ////
double get_expo_dist(double mean)
{
  exponential_distribution<double> expo_dist(1.0 / mean);
  return expo_dist(generator);
}
///// GENERATES A RANDOM NUMBER BETWEEN 0 AND 1 //////
double get_uniform_0to1()
{
  return unif_dist(generator);
}
////// GENERATES A RANDOM INTEGER BETWEEN 0 AND S-1 //////
int get_random_int(int s)
{
  return rand() % s;
}
/////// OUTPUTS CURRENT TIME IN MS ///////////
ll get_time_in_ms(){
  return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();  
}