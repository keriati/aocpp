#include <fstream>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;

//#define FILE "example.txt"
#define FILE "input.txt"
int PART = 2;

using LaserMap = vector<vector<char>>;
using Position = tuple<int, int, int, int>;

void readToVector(const string &fileName, LaserMap &map);
void printMap(LaserMap &map, const set<pair<int, int>> &visited);
bool isInvalid(Position &position, LaserMap &map);

void shoot(const Position &position, LaserMap &map, set<pair<int, int>> &visitedTiles);
void runPart1();
void runPart2();

int main(int argc, char *argv[]) {
    if (PART == 1) runPart1();
    if (PART == 2) runPart2();
    return -1;
}

void runPart1() {
    cout << "AoC 2023 Day 16 Part 1" << endl;
    LaserMap map;

    readToVector(FILE, map);

    auto start = chrono::high_resolution_clock::now();

    Position position = {0, 0, 1, 0};

    set<pair<int, int>> visitedTiles;

    shoot(position, map, visitedTiles);

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    printMap(map, visitedTiles);

    cout << "Result: " << visitedTiles.size() << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
};

void runPart2() {
    cout << "AoC 2023 Day 16 Part 2" << endl;

    LaserMap map;

    readToVector(FILE, map);

    auto start = chrono::high_resolution_clock::now();

    int result = 0;
    int rows = map.size();
    int cols = map[0].size();

    for (int x = 0; x < cols; ++x) {
        Position position = {x, 0, 0, 1};
        set<pair<int, int>> visitedTiles;

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }

    for (int x = cols; x < cols; ++x) {
        Position position = {x, rows - 1, 0, -1};
        set<pair<int, int>> visitedTiles;

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }

    for (int y = 0; y < rows; ++y) {
        Position position = {0, y, 1, 0};
        set<pair<int, int>> visitedTiles;

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }

    for (int y = 0; y < rows; ++y) {
        Position position = {cols - 1, y, -1, 0};
        set<pair<int, int>> visitedTiles;

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = finish - start;

    cout << "Result: " << result << endl;
    cout << "Elapsed: " << elapsed.count() << " ms" << endl;
}

void shoot(const Position &position, LaserMap &map, set<pair<int, int>> &visitedTiles) {
    queue<Position> q;
    set<Position> visitedStates;
    q.push(position);

    while (!q.empty()) {
        if (isInvalid(q.front(), map)) {
            q.pop();
            continue;
        }
        auto [x, y, dx, dy] = q.front();
        q.pop();

        visitedTiles.insert({x, y});

        char pos = map[y][x];

        vector<Position> nextPositions;


        if (pos == '.') {
            nextPositions.emplace_back(x + dx, y + dy, dx, dy);
        }

        if (pos == '|') {
            if (dx != 0) {
                nextPositions.emplace_back(x, y + 1, 0, 1);
                nextPositions.emplace_back(x, y - 1, 0, -1);
            } else {
                nextPositions.emplace_back(x + dx, y + dy, dx, dy);
            }
        }

        if (pos == '-') {
            if (dy != 0) {
                nextPositions.emplace_back(x + 1, y, 1, 0);
                nextPositions.emplace_back(x - 1, y, -1, 0);
            } else {
                nextPositions.emplace_back(x + dx, y + dy, dx, dy);
            }
        }

        if (pos == '\\') {
            int ndy = dx;
            int ndx = dy;
            nextPositions.emplace_back(x + ndx, y + ndy, ndx, ndy);
        }

        if (pos == '/') {
            int ndx = -dy;
            int ndy = -dx;
            nextPositions.emplace_back(x + ndx, y + ndy, ndx, ndy);
        }

        for (auto np: nextPositions) {
            if (visitedStates.find(np) == visitedStates.end()) {
                visitedStates.insert(np);
                q.emplace(np);
            }
        }
    }
}
bool isInvalid(Position &position, LaserMap &map) {
    auto [x, y, dx, dy] = position;
    int rows = map.size();
    int cols = map[0].size();
    return x < 0 || y < 0 || x >= cols || y >= rows;
}


void readToVector(const string &fileName, LaserMap &map) {
    ifstream file(FILE);

    if (!file.is_open()) {
        cout << "Can't open: " << FILE << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        vector<char> row;
        for (char &c: line) {
            row.push_back(c);
        }
        map.push_back(row);
    }
    file.close();
}

void printMap(LaserMap &map, const set<pair<int, int>> &visited) {
    for (int y = 0; y < map.size(); ++y) {
        auto row = map[y];
        for (int x = 0; x < row.size(); ++x) {
            cout << (visited.find({x, y}) != visited.end() ? 'o' : row[x]);
        }
        cout << endl;
    }
}
