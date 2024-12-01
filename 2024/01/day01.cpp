#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
    int a, b, i = 0, result = 0;
    int numbers[1000];
    int_fast8_t occurrences[99999] = {};
    string line;
    ifstream file(argv[1]);

    while (getline(file, line)) {
        istringstream lineStream(line);
        lineStream >> a >> b;
        numbers[i] = a;
        occurrences[b]++;
        i++;
    }

    file.close();

    for (int n: numbers) {
        result += n * occurrences[n];
    }

    cout << "Result: " << result << endl;
    return 0;
}
