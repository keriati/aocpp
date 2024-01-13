#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;

// #define FILE "example.txt"
// int ROWS = 13;
// int COLS = 13;
//
#define FILE "input.txt"
int ROWS = 141;
int COLS = 141;

int PART = 2;

void parseMap(std::unique_ptr<std::unique_ptr<int[]>[]> &map, int rows, int cols);
int getLeastHeatLoss(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map, int minSteps = 1, int maxSteps = 3);
void runPart1();
void runPart2();

struct Step {
    int x;
    int y;
    int vx;
    int vy;
    int heatLoss;
    int steps;
    int prio;
};

int main(int argc, char *argv[]) {
    runPart1();
    runPart2();
    // if (PART == 1) runPart1();
    // if (PART == 2) runPart2();
    return 0;
}


void printMap(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map);

void runPart1() {
    cout << "AoC 2023 Day 17 Part 1" << endl;
    const auto start = chrono::high_resolution_clock::now();

    auto map = std::make_unique<std::unique_ptr<int[]>[]>(ROWS);
    for (int i = 0; i < ROWS; ++i) {
        map[i] = std::make_unique<int[]>(COLS);
    }

    parseMap(map, ROWS, COLS);

    // printMap(map);

    const int result = getLeastHeatLoss(map);

    const auto finish = chrono::high_resolution_clock::now();
    const chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << result << ((result != 102 && result != 916) ? " FAILING" : " PASSING") << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
};


void runPart2() {
    cout << "AoC 2023 Day 17 Part 2" << endl;
    const auto start = chrono::high_resolution_clock::now();

    auto map = std::make_unique<std::unique_ptr<int[]>[]>(ROWS);
    for (int i = 0; i < ROWS; ++i) {
        map[i] = std::make_unique<int[]>(COLS);
    }

    parseMap(map, ROWS, COLS);

    // printMap(map);

    const int result = getLeastHeatLoss(map, 4, 10);

    const auto finish = chrono::high_resolution_clock::now();
    const chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << result << ((result != 94 && result != 1067) ? " FAILING" : " PASSING") << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
}

void printMap(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map) {
    cout << "----" << endl;

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLS; ++x) {
            cout << map[y][x];
        }
        cout << endl;
    }
    cout << "----" << endl;
}


struct CompareSteps {
    bool operator()(Step const &s1, Step const &s2) const {
        return s1.prio >= s2.prio;
    }
};

int cacheKey(Step &step) {
    auto &[x, y, vx, vy, heatLoss, steps, prio] = step;

    // return "" + to_string(x) + "," + to_string(y) + "," + to_string(steps);
    // return "" + to_string(x) + "," + to_string(y) + "," + to_string(vx) + "," + to_string(vy) + "," + to_string(steps);
    // return (x << 16) | (y << 8) | heatLoss;
    return ((((((x * 1000 + y) * 10) + vx + 2) * 10) + vy + 2) * 1000) + steps;
}


int getLeastHeatLoss(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map, const int minSteps, const int maxSteps) {
    const int endX = COLS - 1;
    const int endY = ROWS - 1;

    Step startingEast = {0, 0, 1, 0, 0, 0, 0};
    Step staringSouth = {0, 0, 0, 1, 0, 0, 0};

    priority_queue<Step, vector<Step>, CompareSteps> q;
    q.push(startingEast);
    q.push(staringSouth);

    unordered_set<int> visited;
    visited.insert(cacheKey(staringSouth));
    visited.insert(cacheKey(startingEast));

    while (!q.empty()) {
        auto &[x, y, vx, vy, heatLoss, steps, prio] = q.top();
        q.pop();

        if (x == endX && y == endY) {
            if (steps < minSteps) continue;
            return heatLoss;
        }

        std::tuple<int, int, int, int> nextSteps[3];

        nextSteps[0] = {x + vx, y + vy, vx, vy};
        nextSteps[1] = {x + vy, y + vx, vy, vx};
        nextSteps[2] = {x - vy, y - vx, -vy, -vx};

        for (auto &[nx, ny, nvx, nvy]: nextSteps) {
            if (steps > maxSteps - 1 && vx == nvx && vy == nvy) continue;
            if (steps < minSteps && (vx != nvx || vy != nvy)) continue;
            if (nx < 0 || ny < 0 || nx > COLS - 1 || ny > ROWS - 1) continue;

            const int nhl = heatLoss + map[ny][nx];
            const int nSteps = (nvx == vx && nvy == vy) ? steps + 1 : 1;
            const int nPrio = nhl;

            Step next = {nx, ny, nvx, nvy, nhl, nSteps, nPrio};

            if (auto key = cacheKey(next); visited.find(key) == visited.end()) {
                visited.insert(key);
                q.push(next);
            }
        }
    }

    return -1;
}

void parseMap(std::unique_ptr<std::unique_ptr<int[]>[]> &map, int rows, int cols) {
    ifstream file(FILE);

    if (!file.is_open()) {
        cout << "Can't open: " << FILE << endl;
        exit(1);
    }

    string line;
    int mr = 0;

    while (getline(file, line)) {
        int mc = 0;
        for (const char &c: line) {
            map[mr][mc] = c - '0';
            mc++;
        }
        mr++;
    }

    file.close();
}
