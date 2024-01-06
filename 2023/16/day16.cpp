#include <fstream>
#include <iostream>
#include <queue>
#include <unordered_set>
#include <vector>

using namespace std;

//#define FILE "example.txt"
#define FILE "input.txt"
int PART = 2;

using LaserMap = vector<vector<char>>;
using Position = tuple<int, int, int, int>;

struct pair_hash {
    inline std::size_t operator()(const std::pair<int, int> &v) const {
        return v.first * 31 + v.second;
    }
};

struct pos_hash {
    inline std::size_t operator()(const Position &v) const {
        auto [x, y, dx, dy] = v;
        return x * 31 + y * 37 + dx * 43 + dy * 47;
    }
};

void readToVector(const string &fileName, LaserMap &map);
void printMap(LaserMap &map, const unordered_set<pair<int, int>, pair_hash> &visited);
bool isInvalid(Position &position, const LaserMap &map);

void shoot(const Position &position, const LaserMap &map, unordered_set<pair<int, int>, pair_hash> &visitedTiles);
void runPart1();
void runPart2();

int main(int argc, char *argv[]) {
    if (PART == 1) runPart1();
    if (PART == 2) runPart2();
    return 0;
}

void runPart1() {
    cout << "AoC 2023 Day 16 Part 1" << endl;
    LaserMap map;
    map.reserve(111);

    readToVector(FILE, map);

    auto start = chrono::high_resolution_clock::now();

    Position position = {0, 0, 1, 0};

    unordered_set<pair<int, int>, pair_hash> visitedTiles;

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

    unsigned long result = 0;
    unsigned long rows = map.size();
    unsigned long cols = map[0].size();
    unordered_set<pair<int, int>, pair_hash> visitedTiles;

    for (unsigned long x = 0; x < cols; ++x) {
        visitedTiles.clear();
        Position position = {x, 0, 0, 1};

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }

    for (unsigned long x = 0; x < cols; ++x) {
        visitedTiles.clear();
        Position position = {x, rows - 1, 0, -1};

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }


    for (unsigned long y = 0; y < rows; ++y) {
        visitedTiles.clear();
        Position position = {0, y, 1, 0};

        shoot(position, map, visitedTiles);

        if (visitedTiles.size() > result) {
            result = visitedTiles.size();
        }
    }


    for (unsigned long y = 0; y < rows; ++y) {
        visitedTiles.clear();
        Position position = {cols - 1, y, -1, 0};

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

void shoot(const Position &position, const LaserMap &map, unordered_set<pair<int, int>, pair_hash> &visitedTiles) {
    queue<Position> q;
    unordered_set<Position, pos_hash> visitedStates;
    q.push(position);
    Position nextPositions[2];

    while (!q.empty()) {
        auto [x, y, dx, dy] = q.front();
        q.pop();

        visitedTiles.insert({x, y});

        char pos = map[y][x];

        nextPositions[0] = {-1, -1, 0, 0};
        nextPositions[1] = {-1, -1, 0, 0};

        if (pos == '.') {
            nextPositions[0] = {x + dx, y + dy, dx, dy};
        }

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

        if (pos == '\\') {
            int ndy = dx;
            int ndx = dy;
            nextPositions[0] = {x + ndx, y + ndy, ndx, ndy};
        }

        if (pos == '/') {
            int ndx = -dy;
            int ndy = -dx;
            nextPositions[0] = {x + ndx, y + ndy, ndx, ndy};
        }

        for (auto np: nextPositions) {
            if (isInvalid(np, map)) {
                continue;
            }
            if (visitedStates.find(np) == visitedStates.end()) {
                visitedStates.insert(np);
                q.emplace(np);
            }
        }
    }
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

void printMap(LaserMap &map, const unordered_set<pair<int, int>, pair_hash> &visited) {
    for (int y = 0; y < map.size(); ++y) {
        auto row = map[y];
        for (int x = 0; x < row.size(); ++x) {
            cout << (visited.find({x, y}) != visited.end() ? 'o' : row[x]);
        }
        cout << endl;
    }
}
