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
class QueensSolver {
private:
    GeneticOperations op;
    BoardPrinter printer;

    int findBestIndex(Individual pop[max_solutions]) {
        int best = 0;
        for (int i = 1; i < max_solutions; i++) {
            if (pop[i].score > pop[best].score) best = i;
        }
        return best;
    }

public:
    void solve(int initialState[N], int caseNum, int methodChoice) {
        int target = 28;
        Individual pop[max_solutions];

        // وضع الحالة الابتدائية كأول عنصر
        for (int i = 0; i < N; i++) pop[0].rows[i] = initialState[i];
        pop[0].score = op.calculateScore(pop[0].rows);

        // توليد بقية الجيل عشوائياً
        for (int i = 1; i < max_solutions; i++) pop[i] = op.createRandom();

        Individual bestEver = pop[findBestIndex(pop)];
        int scoreHistory[change];
        int generationsUsed = change;

        // حلقة الأجيال
        for (int gen = 0; gen < change; gen++) {
            int bestIdx = findBestIndex(pop);
            if (pop[bestIdx].score > bestEver.score) bestEver = pop[bestIdx];

            scoreHistory[gen] = bestEver.score;

            if (bestEver.score == target) {
                generationsUsed = gen + 1;
                break;
            }

            Individual newPop[max_solutions];
            newPop[0] = pop[bestIdx]; // الحفاظ على الأفضل (Elitism)

            for (int i = 1; i < max_solutions; i++) {
                Individual p1, p2;
                if (methodChoice == 1) {
                    p1 = op.rouletteSelection(pop);
                    p2 = op.rouletteSelection(pop);
                } else {
                    p1 = op.tournamentSelection(pop);
                    p2 = op.tournamentSelection(pop);
                }

                Individual child = op.crossover(p1, p2);
                op.mutate(child);
                newPop[i] = child;
            }

            for (int i = 0; i < max_solutions; i++) pop[i] = newPop[i];
        }

        // طباعة مخرجات واضحة وبسيطة
        cout << "#  Test Case " << caseNum << "\n";
        cout << "Status        : " << (bestEver.score == target ? "SOLVED" : "NOT SOLVED") << "\n";
        cout << "Best score    : " << bestEver.score << " / " << target << "\n";
        cout << "Generations   : " << generationsUsed << "\n";
        cout << "Best solution : [";
        for (int i = 0; i < N; i++) cout << bestEver.rows[i] << (i < N - 1 ? "," : "");
        cout << "]\n\nBoard:\n";
        printer.print(bestEver.rows);

        cout << "\nScore progress (first 5 generations):\n";
        int limit = (generationsUsed < 5) ? generationsUsed : 5;
        for (int i = 0; i < limit; i++) {
            cout << "  Gen " << (i + 1) << "  ->  score = " << scoreHistory[i] << "\n";
        }
    }
};
int main() {
    // تثبيت الرقم داخل srand بديل لدالة الوقت،  يعطيك عشوائية ثابتة
    srand(42);

    QueensSolver solver;
    int methodChoice;
    int cases[5][N];

    cout << "N-Queens Solver using Genetic Algorithm\n";
    cout << "1: Roulette"<<endl;
    cout << "2: Tournament"<<endl;
    cout << "Select Selection Method:";
    cin >> methodChoice;

    cout << "\nEnter 5 test cases (8 numbers each):\n";
    for (int i = 0; i < 5; i++) {
        cout << "Enter case " << i + 1 << ": ";
        for (int j = 0; j < N; j++) {
            cin >> cases[i][j];
        }
    }

    // تشغيل الحالات الخمسة عبر الـ Solver
    for (int i = 0; i < 5; i++) {
        solver.solve(cases[i], i + 1, methodChoice);
    }

    cout << "Done!\n";
    return 0;
}