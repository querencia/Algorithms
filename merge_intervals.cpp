/***************************************
 * Find Union of given intervals       *
 *                                     *
 * By Euxhen Hasanaj                   *
 * ************************************/

#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>

using namespace std;

bool sort_by_1_then_2(const pair<int, int>& p1, const pair<int, int>& p2) {
    if (p1.first != p2.first) return p1.first < p2.first;
    else return p1.second < p2.second;
}

void solve(vector<pair<int, int>>& v, vector<pair<int, int>>& result) {
    vector<pair<int, bool>> extremas;
    for (auto p : v) {
        extremas.push_back(make_pair(p.first, 0));
        extremas.push_back(make_pair(p.second, 1));
    }
    sort(extremas.begin(), extremas.end(), sort_by_1_then_2);

    int counter = 0, start;
    for (auto p : extremas) {
        if (p.second == 0) {
            if (counter++ == 0) start = p.first;
        } else {
            if (--counter == 0) result.push_back(make_pair(start, p.first));
        }
    }
}

int main() {
    vector<pair<int, int>> v;
    v.push_back(make_pair(1, 2));
    v.push_back(make_pair(1, 4));
    v.push_back(make_pair(2, 3));
    v.push_back(make_pair(6, 6));
    v.push_back(make_pair(9, 12));
    vector<pair<int, int>> result;
    solve(v, result);
    for (int i = 0; i < result.size(); ++i) {
        cout << "[" << result[i].first << ", " << result[i].second << "]\n";
    }
    return 0;
}
