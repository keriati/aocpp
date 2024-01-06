#include <fstream>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

//#define FILE "example.txt"
#define FILE "input.txt"

int PART = 2;

using LaserMap = vector<vector<char>>;
using Position = tuple<int, int, int, int>;

void readToVector(const string &fileName, LaserMap &map);
bool isInvalid(Position &position, const LaserMap &map);
void resetBoolArray(bool *tiles, int totalSize);
int shoot(const Position &position, const LaserMap &map);
void runPart1();
void runPart2();

int main(int argc, char *argv[]) {
    if (PART == 1) runPart1();
    if (PART == 2) runPart2();
    return 0;
}

void runPart1() {
    cout << "AoC 2023 Day 16 Part 1" << endl;
    auto start = chrono::high_resolution_clock::now();

    LaserMap map;

    map.reserve(111);

    readToVector(FILE, map);

    Position position = {0, 0, 1, 0};

    int tilesCount = shoot(position, map);

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << tilesCount << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
};

void runPart2() {
    cout << "AoC 2023 Day 16 Part 2" << endl;
    auto start = chrono::high_resolution_clock::now();

    LaserMap map;

    readToVector(FILE, map);


    unsigned long result = 0;
    unsigned long rows = map.size();
    unsigned long cols = map[0].size();

    for (unsigned long x = 0; x < cols; ++x) {
        Position position = {x, 0, 0, 1};

        int tileCount = shoot(position, map);
        result = (tileCount > result) ? tileCount : result;
    }

    for (unsigned long x = 0; x < cols; ++x) {
        Position position = {x, rows - 1, 0, -1};

        int tileCount = shoot(position, map);
        result = (tileCount > result) ? tileCount : result;
    }

    for (unsigned long y = 0; y < rows; ++y) {
        Position position = {0, y, 1, 0};

        int tileCount = shoot(position, map);
        result = (tileCount > result) ? tileCount : result;
    }

    for (unsigned long y = 0; y < rows; ++y) {
        Position position = {cols - 1, y, -1, 0};

        int tileCount = shoot(position, map);
        result = (tileCount > result) ? tileCount : result;
    }

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << result << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
}

void resetBoolArray(bool *tiles, int totalSize) {
    fill(tiles, tiles + totalSize, false);
}

int shoot(const Position &position, const LaserMap &map) {
    bool visitedTiles[map.size()][map[0].size()];
    resetBoolArray(&visitedTiles[0][0], map.size() * map[0].size());

    queue<Position> q;

    bool visitedStates[map.size()][map[0].size()][3][3];
    resetBoolArray(&visitedStates[0][0][0][0], map.size() * map[0].size() * 3 * 3);

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
            if (isInvalid(np, map)) continue;

            auto [nx, ny, ndx, ndy] = np;

            if (visitedStates[nx][ny][ndx + 1][ndy + 1]) continue;

            visitedStates[nx][ny][ndx + 1][ndy + 1] = true;
            q.emplace(np);
        }
    }

    int result = 0;

    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[0].size(); ++j) {
            if (visitedTiles[i][j]) result++;
        }
    }
    return result;
}

bool isInvalid(Position &position, const LaserMap &map) {
    auto [x, y, dx, dy] = position;
    unsigned long rows = map.size();
    unsigned long cols = map[0].size();
    return x < 0 || y < 0 || x >= cols || y >= rows;
}


void readToVector(const string &fileName, LaserMap &map) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Can't open: " << FILE << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<char> row;
        row.reserve(111);
        for (char &c: line) {
            row.push_back(c);
        }
        map.push_back(row);
    }
    file.close();
}
