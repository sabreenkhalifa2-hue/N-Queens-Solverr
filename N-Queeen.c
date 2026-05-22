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
class GeneticOperations {
private:
    int seed = 37;

    // مولد أرقام شبه عشوائي يدوي
    int myRandom(int limit) {
        seed = (seed * 17 + 11) % 100;
        return seed % limit;
    }

    // قيمة مطلقة يدوية
    int absoluteValue(int value) {
        if (value < 0) {
            return -value;
        }
        return value;
    }
