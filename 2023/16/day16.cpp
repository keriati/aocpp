#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

//#define FILE "example.txt"
#define FILE "input.txt"
int PART = 2;

using Position = tuple<int, int, int, int>;

void runPart1();
void runPart2();
int shoot(const Position &position, char **map, int rows, int cols);
void parseMap(char **map, int rows, int cols);
bool isInvalid(Position &position, int rows, int cols);

int main(int argc, char *argv[]) {
    if (PART == 1) runPart1();
    if (PART == 2) runPart2();
    return 0;
}

void runPart1() {
    cout << "AoC 2023 Day 16 Part 1" << endl;
    auto start = chrono::high_resolution_clock::now();

    int rows = 110;
    int cols = 110;

    char **map = new char *[rows];
    parseMap(map, rows, cols);

    Position position = {0, 0, 1, 0};

    int tilesCount = shoot(position, map, rows, cols);

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    for (int i = 0; i < rows; i++) {
        delete[] map[i];
    }

    delete[] map;

    cout << "Result: " << tilesCount << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
};

void runPart2() {
    cout << "AoC 2023 Day 16 Part 2" << endl;
    auto start = chrono::high_resolution_clock::now();

    int rows = 110;
    int cols = 110;

    char **map = new char *[rows];
    parseMap(map, rows, cols);

    unsigned long result = 0;

    for (unsigned long x = 0; x < cols; ++x) {
        Position position = {x, 0, 0, 1};

        int tileCount = shoot(position, map, rows, cols);
        result = (tileCount > result) ? tileCount : result;
    }

    for (unsigned long x = 0; x < cols; ++x) {
        Position position = {x, rows - 1, 0, -1};

        int tileCount = shoot(position, map, rows, cols);
        result = (tileCount > result) ? tileCount : result;
    }

    for (unsigned long y = 0; y < rows; ++y) {
        Position position = {0, y, 1, 0};

        int tileCount = shoot(position, map, rows, cols);
        result = (tileCount > result) ? tileCount : result;
    }

    for (unsigned long y = 0; y < rows; ++y) {
        Position position = {cols - 1, y, -1, 0};

        int tileCount = shoot(position, map, rows, cols);
        result = (tileCount > result) ? tileCount : result;
    }

    for (int i = 0; i < rows; i++) {
        delete[] map[i];
    }

    delete[] map;

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << result << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
}

void parseMap(char **map, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        map[i] = new char[cols];
    }

    ifstream file(FILE);

    if (!file.is_open()) {
        cout << "Can't open: " << FILE << endl;
        exit(1);
    }

    string line;
    int mr = 0;

    while (getline(file, line)) {
        int mc = 0;
        for (char &c: line) {
            map[mr][mc] = c;
            mc++;
        }
        mr++;
    }
    file.close();
}

int shoot(const Position &position, char **map, int rows, int cols) {
    bool visitedTiles[rows][cols];
    memset(visitedTiles, 0, sizeof(visitedTiles));

    queue<Position> q;

    bool visitedStates[rows][cols][3][3];
    memset(visitedStates, 0, sizeof(visitedStates));

    auto [sx, sy, sdx, sdy] = position;
    visitedStates[sx][sy][sdx][sdy] = true;

    q.push(position);
    Position nextPositions[2];

    while (!q.empty()) {
        auto [x, y, dx, dy] = q.front();
        q.pop();

        visitedTiles[y][x] = true;

        char pos = map[y][x];

        nextPositions[0] = {-1, -1, 0, 0};
        nextPositions[1] = {-1, -1, 0, 0};

        if (pos == '.') nextPositions[0] = {x + dx, y + dy, dx, dy};
        if (pos == '\\') nextPositions[0] = {x + dy, y + dx, dy, dx};
        if (pos == '/') nextPositions[0] = {x - dy, y - dx, -dy, -dx};

        if (pos == '|') {
            if (dx != 0) {
                nextPositions[0] = {x, y + 1, 0, 1};
                nextPositions[1] = {x, y - 1, 0, -1};
            } else {
                nextPositions[0] = {x + dx, y + dy, dx, dy};
            }
        }

        if (pos == '-') {
            if (dy != 0) {
                nextPositions[0] = {x + 1, y, 1, 0};
                nextPositions[1] = {x - 1, y, -1, 0};
            } else {
                nextPositions[0] = {x + dx, y + dy, dx, dy};
            }
        }

        for (auto np: nextPositions) {
            if (isInvalid(np, rows, cols)) continue;

            auto [nx, ny, ndx, ndy] = np;

            if (visitedStates[nx][ny][ndx + 1][ndy + 1]) continue;

            visitedStates[nx][ny][ndx + 1][ndy + 1] = true;
            q.emplace(np);
        }
    }

    int result = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (visitedTiles[i][j]) result++;
        }
    }

    return result;
}

bool isInvalid(Position &position, int rows, int cols) {
    auto [x, y, dx, dy] = position;
    return x < 0 || y < 0 || x >= cols || y >= rows;
}