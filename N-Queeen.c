#include <iostream>
using namespace std;

const int N = 8;                 
const int max_solutions = 200;   
const int change = 2000;         
const int sample = 5;            
const double MUT_RATE = 0.05;
struct Individual {
    int rows[N];
    int score;
};
