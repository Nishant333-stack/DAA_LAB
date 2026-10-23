#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

const int INF = 1000000000;

struct Node {
    vector<vector<int>> matrix;
    vector<int> path;
    int bound;
};

struct CompareBound {
    bool operator()(const Node& a, const Node& b) const {
        return a.bound > b.bound;
    }
};

int reduceMatrix(vector<vector<int>>& matrix) {
    int n = matrix.size();
    int reduction = 0;

    for (int i = 0; i < n; i++) {
        int mn = INF;

        for (int j = 0; j < n; j++) {
            mn = min(mn, matrix[i][j]);
        }

        if (mn == INF) {
            continue;
        }

        reduction += mn;

        for (int j = 0; j < n; j++) {
            if (matrix[i][j] != INF) {
                matrix[i][j] -= mn;
            }
        }
    }

    for (int j = 0; j < n; j++) {
        int mn = INF;

        for (int i = 0; i < n; i++) {
            mn = min(mn, matrix[i][j]);
        }

        if (mn == INF) {
            continue;
        }

        reduction += mn;

        for (int i = 0; i < n; i++) {
            if (matrix[i][j] != INF) {
                matrix[i][j] -= mn;
            }
        }
    }

    return reduction;
}

int calculateCost(const vector<vector<int>>& cost, const vector<int>& path) {
    int total = 0;

    for (size_t i = 1; i < path.size(); i++) {
        total += cost[path[i - 1]][path[i]];
    }

    total += cost[path.back()][0];

    return total;
}

void solveTSP(const vector<vector<int>>& cost) {
    int n = cost.size();

    Node root{cost, {0}, 0};
    root.bound = reduceMatrix(root.matrix);

    priority_queue<Node, vector<Node>, CompareBound> pq;
    pq.push(root);

    int bestCost = INF;
    vector<int> bestPath;

    while (!pq.empty()) {
        Node current = pq.top();
        pq.pop();

        if (current.bound >= bestCost) {
            continue;
        }

        int u = current.path.back();

        if ((int)current.path.size() == n) {
            if (cost[u][0] == INF) {
                continue;
            }

            int total = calculateCost(cost, current.path);

            if (total < bestCost) {
                bestCost = total;
                bestPath = current.path;
            }

            continue;
        }

        for (int v = 1; v < n; v++) {
            if (find(current.path.begin(), current.path.end(), v) != current.path.end()) {
                continue;
            }

            if (current.matrix[u][v] == INF) {
                continue;
            }

            Node child = current;
            child.path.push_back(v);

            child.bound += child.matrix[u][v];

            for (int j = 0; j < n; j++) {
                child.matrix[u][j] = INF;
            }

            for (int i = 0; i < n; i++) {
                child.matrix[i][v] = INF;
            }

            if ((int)child.path.size() < n) {
                child.matrix[v][0] = INF;
            }

            child.bound += reduceMatrix(child.matrix);

            if (child.bound < bestCost) {
                pq.push(child);
            }
        }
    }

    if (bestPath.empty()) {
        cout << "No possible tour" << endl;
    } else {
        cout << "\nMinimum cost: " << bestCost << endl;
        cout << "Route: ";

        for (int city : bestPath) {
            cout << city << " -> ";
        }

        cout << "0" << endl;
    }
}

int main() {
    cout << "Enter number of cities: ";
    int n;
    cin >> n;

    if (n < 2) {
        cout << "Number of cities must be at least 2." << endl;
        return 0;
    }

    vector<vector<int>> cost(n, vector<int>(n));

    cout << "Enter cost matrix (0 for no road):" << endl;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int value;
            cin >> value;

            if (i == j || value <= 0) {
                cost[i][j] = INF;
            } else {
                cost[i][j] = value;
            }
        }
    }

    solveTSP(cost);

    return 0;
}
