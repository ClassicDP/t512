#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <chrono>
#include <vector>
#include "map"


using namespace std;

class Combinations {
public:
    vector<vector<int>> table;
    Combinations(int n, int k) {
        int p = 1;
        int kN = 1, j = 1;
        for (int i = n; i > n - k; i--) {
            p *= i;
            kN *= j++;
        }
        table.resize(p / kN);
        for (auto &item: table) item.resize(k);
        for (int i = 0; i < k; i++)
            table[0][i] = i;
        int i = 0;
        do {
            int j = k - 1;
            while (j >= 0 && table[i][j] == n - 1 - ((k - 1) - j)) j--;
            if (j < 0) return;
            i++;
            table[i] = table[i - 1];
            table[i][j]++;
            for (int jj = j + 1; jj < k; jj++) table[i][jj] = table[i][jj - 1] + 1;
        } while (1);
    }
};


class Square {
    vector<vector<int>> &t;
    int n;
    vector<int> used, path, unUsed;

public:
    Square(vector<vector<int>> &t) : t(t), n(t.size()) {
        used.resize(n);
        path.resize(n);
        unUsed.reserve(n);
    }


    int vectorToBin(vector<int> &v) {
        int res = 0;
        for (auto &it: v) {
            res += 1 << it;
        }
        return res;
    }

    uint64_t calcSet() {
        map<int, uint64_t> bg;
        for (int j = 0; j < n; j++)
            if (t[0][j]) {
                vector<int> v(1);
                v[0] = j;
                bg[vectorToBin(v)] = 1;
            }
        for (int i = 1; i < n; i++) {
            map<int, uint64_t> newBG;
            for (int j = 0; j < n; j++) {
                if (t[i][j])
                    for (auto &it: bg) {
                        if (it.first & (1 << j)) continue;
                        auto newBits = it.first | (1 << j);
                        auto bin = newBG.find(newBits);
                        if (bin != newBG.end()) bin->second += it.second; else newBG[newBits] = it.second;
                    }
            }
            bg = newBG;
            if (bg.empty()) return 0;
        }
        return bg.begin()->second;
    }
};

class Table {
public:
    int n, m, k;
    vector<vector<int>> t;


    uint64_t calcAll() {
        uint64_t res = 0;
        vector<vector<int>> sq(k);
        if (k > m || k > n) return 0;
        Combinations nK(n, k);
        Combinations mK(m, k);
        for (auto &it: sq) it.resize(k);
        for (auto b: nK.table) {
            for (auto g: mK.table) {
                for (int i = 0; i < k; i++) {
                    for (int j = 0; j < k; j++) {
                        sq[i][j] = t[b[i]][g[j]];
                    }
                }
                Square square(sq);
                res += square.calcSet();
            }
        }
        return res;
    }


    Table(stringstream &buff) {
        buff >> n >> m >> k;
        t.resize(n);
        for (auto &it: t) {
            it.resize(m);
            for (int j = 0; j < m; j++) {
                char c;
                buff >> c;
                it[j] = c == 'Y' || c == '1' ? 1 : 0;
            }
        }
    }

};


int main(int argc, char const *argv[]) {
    ifstream file("INPUT.TXT");
    ofstream output("OUTPUT.TXT");
    stringstream buff;
    buff << file.rdbuf();
    Table table(buff);
    using namespace std::chrono;
    milliseconds t0 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    auto n1 = table.calcAll();
    milliseconds t1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << t1.count() - t0.count() << endl;
    cout << n1 << endl;
    output << n1 << endl;
    output.close();
    return 0;
}
