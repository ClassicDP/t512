#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <vector>

using namespace std;

void printArr(int *a, int n) {
    for (int i = 0; i < n; i++)
        cout << a[i] << " ";
    cout << endl;
}

int cNM(int n, int m) {
    int res = 1;
    for (int i = m + 1; i <= n; i++)
        res *= i;
    int p = 1;
    for (int i = 2; i <= (n - m); i++)
        p *= i;
    return res / p;
}

int aNM(int n, int m) {
    int res = 1;
    for (int i = n - m + 1; i <= n; i++)
        res *= i;
    return res;
}

int factor(int n) {
    int p = 1;
    for (int i = 2; i <= n; i++)
        p *= i;
    return p;
}

class Permutations {
    vector<vector<int>> list;
    int listP = 0;
    vector<int> temp;
    vector<int> used;
    int n;


    void generate(vector<int> &v, int ind = 0) {

        for (int i = 0; i < v.size(); i++) {
            if (!used[i]) {
                temp[ind] = v[i];
                if (++ind < n) {
                    used[i] = 1;
                    generate(v, ind);
                    used[i] = 0;
                    ind--;
                } else list[listP++] = temp;
            }
        }
    }

public:
    Permutations(int n) : n(n) {
        list.resize(factor(n));
        temp.resize(n);
        for (auto &it: list) it.resize(n);
        for (int i = 0; i < n; i++) list[0][i] = i;
        used.resize(n);
        fill_n(used.begin(), n, 0);
        generate(list[0]);
    }
};

class Combinations {
public:
    vector<vector<int>> table;
    int size;

    Combinations(int n, int k) {
        int p = 1;
        int kN = 1, j = 1;
        for (int i = n; i > n - k; i--) {
            p *= i;
            kN *= j++;
        }
        table.resize(size = p / kN);
        for (auto &item: table) item.resize(k);
        for (int i = 0; i < k; i++)
            table[0][i] = i;
        int i = 0;
        do {
            int j = k - 1;
            while (table[i][j] == n - 1 - ((k - 1) - j) && j >= 0) j--;
            if (j < 0) return;
            i++;
            table[i] = table[i - 1];
            table[i][j]++;
            for (int jj = j + 1; jj < k; jj++) table[i][jj] = table[i][jj - 1] + 1;
        } while (1);
    }
};

template<typename T1, typename T2>
class Pair {
public:
    T1 x;
    T2 y;
    Pair() = default;
    Pair(T1 x, T2 y) {
        this->x = x;
        this->y = y;
    }
    bool operator<(Pair const &p) const {
        return x < p.x || (x == p.x && y < p.y);
    }
};

struct Pos {
    int i, j;
};

struct NextResult {
    bool ok;
    vector<int> result;
};

struct ZPos {
    int r, c;
};

class Square {
    vector<vector<int>> &t;
    int n;
    vector<int> used, path, unUsed, toNewPath;
    vector<ZPos> zeros;

    int getNearest(int k) {
        auto it =
                find_if(toNewPath.begin(), toNewPath.end(), [k](const int &it) { return it >= k; });
        if (it != toNewPath.end()) {
            int res = *it;
            toNewPath.erase(it);
            return res;
        }
        return -1;
    }

    bool pathZCheck() {
        for (int k = n - unUsed.size(); k < n; k++)
            if (!t[k][path[k]]) return true;
        return false;
    }


    bool zeroCombinations() {
        bool res = false;
        unUsed.clear();
        zeros.clear();
        for (int l = 0; l < n; l++) if (!used[l]) unUsed.push_back(l);
        bool solve;
        do {
            toNewPath = unUsed;
            int k = n - unUsed.size();
            int pColumn;
            while (k < n) {
                do {
                    pColumn = getNearest(path[k]);
                    if (pColumn < 0) {
                        path[k--] = 0;
                        if (k < n - unUsed.size()) return false;
                        int ret = path[k]++;
                        toNewPath.insert(find_if(toNewPath.begin(), toNewPath.end(),
                                                 [ret](const int &r) { return ret < r; }), ret);
                    } else {
                        path[k] = pColumn;
                        k++;
                    }
                } while (pColumn < 0);
            }
            solve = pathZCheck();
            if (!solve) path[n - 1]++;
        } while (!solve);
        return true;
    }

    NextResult findNext(int lookFor1) {
        int i, j;
        if (!lookFor1) {
            i = n - 2;
            j = path[i] + 1;
        } else {
            for (int k = 0; k < n; k++) {
                if (!t[k][path[k]]) {
                    i = k;
                    j = path[k] + 1;
                    break;
                }
            }
        }
        fill(used.begin(), used.end(), 0);
        for (int k = 0; k < (lookFor1 ? i : n - 2); k++) used[path[k]] = 1;
        while (i >= 0 && i < n) {
            while (j < n && (used[j] || (lookFor1 ? t[i][j] == 0 : !zeroCombinations()))) j++;
            if (j < n) {
                if (lookFor1) {
                    path[i++] = j;
                    used[j] = 1;
                } else {
                    return {true, path};
                }
                j = 0;
            } else {
                used[path[i]] = 0;
                i--;
                j = path[i] + 1;
                used[path[i]] = 0;
            }
        }
        return {i >= 0, path};
    }

public:
    Square(vector<vector<int>> &t) : t(t), n(t.size()) {
        used.resize(n);
        path.resize(n);
        unUsed.reserve(n);
        zeros.reserve(n);
    }

    vector<vector<int>> getList() {
        int lookFor = 1;
        vector<vector<int>> res;
        for (int i = 0; i < n; i++) {
            path[i] = i;
            lookFor *= t[i][i];
        }
        bool ok = true;
        do {
            lookFor = !lookFor;
            auto x = findNext(lookFor);
            if (x.ok) printArr(x.result.data(), n);
            ok = x.ok;
            if (ok) res.push_back(x.result);
        } while (ok);
        return res;
    }
};

class Table {
public:
    int n, m, k;
    vector<vector<int>> t;

    void printList(vector<vector<Pair<int, vector<Pair<int, int>>>>> &list) {
        for (auto it1: list) {
            for (auto it2: it1) {
                cout << it2.x << ": ";
                for (auto i3: it2.y) {
                    cout << i3.y << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
    }

    void findNext() {
        vector<vector<int>> sq(k);
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
                auto list = square.getList();
            }
        }
    }

    vector<vector<Pair<int, vector<Pair<int, int>>>>> makeList() {
        vector<vector<Pair<int, vector<Pair<int, int>>>>> res;
        Combinations nK(n, k);
        Combinations mK(m, k);
        res.reserve(nK.size * mK.size);


        for (auto b: nK.table) {
            for (auto g: mK.table) {
                vector<Pair<int, vector<Pair<int, int>>>> vList;
                int p0 = 1;
                do {
                    int p = 1;
                    vector<Pair<int, int>> v0;
                    v0.reserve(k);
                    for (int i = 0; i < k; i++) {
                        v0.emplace_back(b[i], g[i]);
                        p *= t[b[i]][g[i]];
                    }
                    if (p != p0) {
                        p0 = p;
                        vList.emplace_back(p, v0);
                    }
                } while (next_permutation(g.begin(), g.end()));
                res.push_back(vList);
            }
        }
        return res;
    }

    int calc() {
        int cnt = 0;
        if (n < k || m < k) return 0;
        Combinations nK(n, k);
        Combinations mK(m, k);
        for (auto &boys: nK.table) {
            for (auto &girls: mK.table) {
                do {
                    int p = 1;
                    for (int i = 0; i < k; i++) {
                        p *= t[boys[i]][girls[i]];
                        if (!p) break;
                    }
                    cnt += p;
                } while (next_permutation(girls.begin(), girls.end()));
            }
        }
        return cnt;

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
    int m, n, k;
    ifstream file("INPUT.TXT");
    ofstream output("OUTPUT.TXT");
    stringstream buff;
    buff << file.rdbuf();
    Table table(buff);
    output << table.calc() << endl;
    using namespace std::chrono;
    vector<int> v0(10);
    for (int i = 0; i < 10; i++) v0[i] = i;
    milliseconds t0 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    auto list = table.makeList();
    table.printList(list);
    milliseconds t1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    table.findNext();
    cout << t1.count() - t0.count() << endl;
    output.close();
    return 0;
}