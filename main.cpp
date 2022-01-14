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

void printArr(const vector<int> &v) {
    for (auto x: v)
        cout << x << " ";
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

class PermutationNumber {

public:

    PermutationNumber(int n) {
        factorial.resize(n + 1);
        factorial[0] = 1;
        for (int i = 1; i <= n; i++) factorial[i] = i * factorial[i - 1];
    }

    vector<int> getMess(const vector<int> &v) {
        vector<int> s = v;
        int k = 1;
        for (auto a: v) {
            for (int i = k; i < v.size(); i++) {
                if (v[i] > a) s[i]--;
            }
            k++;
        }
        return s;
    }

    u_int64_t get(const vector<int> &v) {
        auto s = getMess(v);
        u_int64_t res = 0;
        for (int i = 0; i < v.size(); i++) {
            res += s[i] * factorial[v.size() - i - 1];
        }
        return res;
    }

    vector<uint64_t> factorial;
};

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

struct NextResult {
    bool ok;
    vector<int> result;
};
struct Zeros {
    int row, col;
};

class Square {
    vector<vector<int>> &t;
    int n;
    vector<int> used, path, unUsed;
    bool nearestTopChange;
    PermutationNumber permutationNumber;


    int takeNearest(vector<int> &v, int k) {
        if (nearestTopChange) k = -1;
        auto it =
                find_if(v.begin(), v.end(), [k](const int &it) { return it > k; });
        if (it != v.end()) {
            int res = *it;
            nearestTopChange = nearestTopChange || res > k;
            v.erase(it);
            return res;
        }
        return -1;
    }


    int nextUnUsed(int i) {
        auto it = find_if(unUsed.begin(), unUsed.end(), [i](int j) { return j > i; });
        if (it == unUsed.end()) return n;
        return *it;
    }

    void insertAscending(vector<int> &l, int i) {
        for (auto it = l.begin(); it != l.end(); it++) {
            if (i < *it) {
                l.insert(it, i);
                return;
            }
        }
        l.insert(l.end(), i);
    }

    void delValue(vector<int> &l, int i) {
        auto it = find(l.begin(), l.end(), i);
        if (it != l.end()) l.erase(it);
    }

    NextResult findNext(int lookFor1) {
        int i, j;
        vector<vector<int>> pull;
        vector<Zeros> zeros;
        if (!lookFor1) {
            i = n - 2;
            unUsed.clear();
            insertAscending(unUsed, path[n - 1]);
            while (i >= 0) {
                insertAscending(unUsed, path[i]);
                for (auto it: unUsed) {
                    if (it > path[i] && !t[i][it]) zeros.push_back({i, it});
                    for (int k = i + 1; k < n; k++) if (!t[k][it]) zeros.push_back({k, it});
                }

                auto savePath = path;
                auto saveUnused = unUsed;
                for (auto &it: zeros) {
                    delValue(unUsed, it.col);
                    int zeroChangeRow = path[it.row] < it.col ? it.row : n;
                    path[it.row] = it.col;
                    nearestTopChange = i == zeroChangeRow;
                    if (i != it.row) path[i] = takeNearest(unUsed, path[i]);
                    bool ok = path[i] >= 0;
                    if (ok)
                        for (int k = i + 1; k < n; k++)
                            if (k != it.row) {
                                ok &= ((path[k] = takeNearest(unUsed, path[k])) >= 0);
                                if (!ok) break;
                            } else { nearestTopChange |= (k == zeroChangeRow); }
                    if (ok) pull.push_back(path);
                    path = savePath;
                    unUsed = saveUnused;
                }
                auto cmp = [](vector<int> &a, vector<int> &b) {
                    for (int i = 0; i < min(a.size(), b.size()); i++) {
                        if (a[i] < b[i]) return true;
                        if (a[i] > b[i]) return false;
                    }
                };
                if (!pull.empty()) {
                    sort(pull.begin(), pull.end(), cmp);
                    path = pull[0];
                    return {true, path};
                }
                pull.clear();
                i--;
            }
            return {false, path};
        }

        for (int k = 0; k < n; k++) {
            if (!t[k][path[k]]) {
                i = k;
                j = path[k] + 1;
                break;
            }
        }

        fill(used.begin(), used.end(), 0);
        for (int k = 0; k < i; k++) used[path[k]] = 1;
        while (i >= 0 && i < n) {
            while (j < n && (used[j] || t[i][j] == 0)) j++;
            if (j < n) {
                path[i++] = j;
                used[j] = 1;
                j = 0;
            } else {
//                used[path[i]] = 0;
                i--;
                j = path[i] + 1;
                used[path[i]] = 0;
            }
        }
        return {i >= 0, path};
    }

public:
    Square(vector<vector<int>>
           &t) :

            t(t), n(t.size()), permutationNumber(n) {
        used.resize(n);
        path.resize(n);
        unUsed.reserve(n);
    }

    struct PermListItem {
        int bit;
        vector<int> permutation;
        uint64_t number;
    };

    vector<PermListItem> getList() {
        int lookFor = 1;
        vector<PermListItem> res;
        for (int i = 0; i < n; i++) {
            path[i] = i;
            lookFor *= t[i][i];
        }
        res.push_back({lookFor, path, permutationNumber.get(path)});
        NextResult x;
        do {
            lookFor = !lookFor;
            x = findNext(lookFor);
            if (x.ok) {
                res.push_back({lookFor, x.result, permutationNumber.get(x.result)});
            }
        } while (x.ok);
        auto last = res[0];
//        reverse(last.permutation.begin(), last.permutation.end());
        last.number = permutationNumber.factorial[last.permutation.size()];
        last.bit = !(res.end()-1)->bit;
        res.push_back(last);
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

    int calcAll() {
        int res = 0;
        vector<vector<int>> sq(k);
        Combinations nK(n, k);
        Combinations mK(m, k);
        for (auto &it: sq) it.resize(k);
        for (auto b: nK.table) {
            auto lastRes = res;
            for (auto g: mK.table) {
                for (int i = 0; i < k; i++) {
                    for (int j = 0; j < k; j++) {
                        sq[i][j] = t[b[i]][g[j]];
                    }
                }
                Square square(sq);
                auto list = square.getList();
                auto getI = [&b](int ii) { return b[ii]; };
                auto getJ = [&g](int ii) { return g[ii]; };
                auto convertVector = [&getJ](vector<int> &v) {
                    vector<int> res(v.size());
                    for (int i = 0; i < v.size(); i++) res[i] = getJ(v[i]);
                    return res;
                };
//                // todo
//                for (auto &it: list) printArr(convertVector(it.permutation).data(), k);
                for (int i = list[0].bit ? 1 : 2; i < list.size(); i += 2) {
                    res += list[i].number - list[i - 1].number;
                }

            lastRes = res;
        }
//        //todo
//        cout << endl;
        return res;
    }

    vector<vector<Pair<int, vector<Pair<int, int>>>>> makeList() {
        vector<vector<Pair<int, vector<Pair<int, int>>>>> res;
        Combinations nK(n, k);
        Combinations mK(m, k);
        res.reserve(nK.size * mK.size);


        for (auto b: nK.table) {
            for (auto g: mK.table) {
                vector<Pair<int, vector<Pair<int, int>>>> vList;
                int p0 = -1;
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
        int lastCnt = 0;
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
            cout << cnt - lastCnt << " " ;
            lastCnt = cnt;
        }
        cout << endl;
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
    using namespace std::chrono;
    milliseconds t0 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    int n1 = table.calcAll();
    milliseconds t1 = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    cout << t1.count() - t0.count() << endl;
    cout << n1 << endl;
    output << n1 << endl;
    output.close();
    return 0;
}
