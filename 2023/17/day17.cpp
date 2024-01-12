#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;

// #define FILE "example.txt"
// int ROWS = 13;
// int COLS = 13;

#define FILE "input.txt"
int ROWS = 141;
int COLS = 141;

int PART = 1;

void parseMap(std::unique_ptr<std::unique_ptr<int[]>[]> &map, int rows, int cols);
int getLeastHeatLoss(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map);
void runPart1();
void runPart2();

struct Step {
    int x;
    int y;
    int vx;
    int vy;
    int heatLoss;
    int steps;
};

int main(int argc, char *argv[]) {
    if (PART == 1) runPart1();
    if (PART == 2) runPart2();
    return 0;
}


void printMap(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map);

void runPart1() {
    cout << "AoC 2023 Day 17 Part 1" << endl;
    auto start = chrono::high_resolution_clock::now();

    int result = 0;

    int rows = ROWS;
    int cols = COLS;

    auto map = std::make_unique<std::unique_ptr<int[]>[]>(rows);
    for (int i = 0; i < rows; ++i) {
        map[i] = std::make_unique<int[]>(cols);
    }

    parseMap(map, rows, cols);

    // printMap(map);

    result = getLeastHeatLoss(map);

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << result << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
};

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
    bool operator()(Step const &s1, Step const &s2) {
        return s1.heatLoss > s2.heatLoss;
    }
};

int cacheKey(Step &step) {
    auto &[x, y, vx, vy, heatLoss, steps] = step;

    return (x << 16) | (y << 8) | ((vx + 2) << 4) | ((vy + 2) << 2) | steps;
}


int getLeastHeatLoss(const __unique_if<std::__1::unique_ptr<int[]>[]>::__unique_array_unknown_bound &map) {
    int endX = ROWS - 1;
    int endY = COLS - 1;

    Step startingEast = {0, 0, 1, 0, 0, 0};
    Step staringSouth = {0, 0, 0, 1, 0, 0};

    priority_queue<Step, vector<Step>, CompareSteps> q;
    // deque<Step> q[10];
    // q[0] = deque<Step>{};
    // q[1] = deque<Step>{};
    // q[2] = deque<Step>{};
    // q[3] = deque<Step>{};
    // q[4] = deque<Step>{};
    // q[5] = deque<Step>{};
    // q[6] = deque<Step>{};
    // q[7] = deque<Step>{};
    // q[8] = deque<Step>{};
    // q[9] = deque<Step>{};

    q.push(startingEast);
    q.push(staringSouth);

    unordered_set<int> visited;
    visited.insert(cacheKey(staringSouth));
    visited.insert(cacheKey(startingEast));

    int minPrio = 0;

    while (minPrio < 10) {
        while (!q.empty()) {
            auto &[x, y, vx, vy, heatLoss, steps] = q.top();

            // q[minPrio].pop_front();
            q.pop();
            if (x == endX && y == endY) {
                cout << "visited: " << visited.size() << endl;
                return heatLoss;
            }

            std::tuple<int, int, int, int> nextSteps[3];

            nextSteps[0] = {x + vx, y + vy, vx, vy};
            nextSteps[1] = {x + vy, y + vx, vy, vx};
            nextSteps[2] = {x - vy, y - vx, -vy, -vx};

            for (auto &[nx, ny, nvx, nvy]: nextSteps) {
                if (steps > 2 && vx == nvx && vy == nvy) continue;

                if (nx < 0 || ny < 0 || nx > ROWS - 1 || ny > COLS - 1) continue;

                int nhl = heatLoss + map[ny][nx];
                int nSteps = (nvx == vx && nvy == vy) ? steps + 1 : 1;

                Step next = {nx, ny, nvx, nvy, nhl, nSteps};

                auto key = cacheKey(next);

                if (visited.find(key) == visited.end()) {
                    visited.insert(key);
                    // q[map[ny][nx]].push_back(next);
                    q.push(next);
                }
            }
            minPrio = 0;
        }

        minPrio++;
    }

    cout << "visited size: " << visited.size() << endl;
    return -1;
}

void runPart2() {
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
