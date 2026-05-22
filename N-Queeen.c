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
    public:

    int calculateScore(int rows[N]) {
        int maxPairs = N * (N - 1) / 2;
        int conflicts = 0;

        for (int i = 0; i < N; i++) {
            for (int j = i + 1; j < N; j++) {

                if (rows[i] == rows[j]) {
                    conflicts++;
                    continue;
                }
                int row_diff = absoluteValue(rows[i] - rows[j]);
                int col_diff = absoluteValue(i - j);

                if (row_diff == col_diff) {
                    conflicts++;
                }
            }
        }

        return maxPairs - conflicts;
    }

    Individual createRandom() {
        Individual ind;

        for (int i = 0; i < N; i++) {
            ind.rows[i] = 1 + myRandom(N);
        }

        ind.score = calculateScore(ind.rows);

        return ind;
    }

  
    Individual rouletteSelection(Individual pop[max_solutions]) {

        int total = 0;

        for (int i = 0; i < max_solutions; i++) {
            total += pop[i].score;
        }

        if (total == 0) {
            return pop[myRandom(max_solutions)];
        }

        int point = myRandom(total + 1);

        int cumulative = 0;

        for (int i = 0; i < max_solutions; i++) {

            cumulative += pop[i].score;

            if (cumulative >= point) {
                return pop[i];
            }
        }

        return pop[max_solutions - 1];
    }

    Individual tournamentSelection(Individual pop[max_solutions]) {

        Individual best = pop[myRandom(max_solutions)];

        for (int i = 1; i < sample; i++) {

            Individual candidate = pop[myRandom(max_solutions)];

            if (candidate.score > best.score) {
                best = candidate;
            }
        }

        return best;
    }

    Individual crossover(Individual p1, Individual p2) {

        Individual child;

        int cutPoint = 1 + myRandom(N - 1);

        for (int i = 0; i < cutPoint; i++) {
            child.rows[i] = p1.rows[i];
        }

        for (int i = cutPoint; i < N; i++) {
            child.rows[i] = p2.rows[i];
        }

        child.score = calculateScore(child.rows);

        return child;
    }

 
    void mutate(Individual &ind) {

        for (int i = 0; i < N; i++) {

            int r = myRandom(100);

            if (r < 5) {
                ind.rows[i] = 1 + myRandom(N);
            }
        }

        ind.score = calculateScore(ind.rows);
    }
};
class BoardPrinter {
public:

    void print(int rows[N]) {

        for (int row = 1; row <= N; row++) {

            cout << "  |";

            for (int col = 0; col < N; col++) {

                if (rows[col] == row) {
                    cout << " Q |";
                }
                else {
                    cout << " . |";
                }
            }

            cout << "\n";
        }

        for (int i = 0; i < N * 4 + 3; i++) {
            cout << "-";
        }

        cout << "\n";
    }
};