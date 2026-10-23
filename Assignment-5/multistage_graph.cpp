#include <cmath>
#include <cstdio>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int to;
    double baseCost;
    double curCost;
};

const double INF = 1e30;

vector<vector<Edge>> adj;
vector<vector<int>> revAdj;
vector<double> bestCost;
vector<int> nextNode;

// Recalculate the minimum cost of a node
double recomputeNode(int node) {
    double best = INF;
    int bestV = -1;

    for (const Edge& e : adj[node]) {
        if (bestCost[e.to] >= INF / 2)
            continue;

        double candidate = e.curCost + bestCost[e.to];

        if (candidate < best) {
            best = candidate;
            bestV = e.to;
        }
    }

    nextNode[node] = bestV;
    return best;
}

// Display minimum costs of source-stage nodes
void displayCosts(const vector<int>& stageStart, const vector<int>& stageCount) {
    cout << "\nBest costs from Stage-0 nodes:" << endl;

    for (int k = 0; k < stageCount[0]; k++) {
        int node = stageStart[0] + k;

        if (bestCost[node] >= INF / 2)
            cout << "Node " << node << ": unreachable" << endl;
        else
            printf("Node %d: cost = %.6f\n", node, bestCost[node]);
    }
}

// Display the optimal path from a source node
void printPath(int src) {
    if (bestCost[src] >= INF / 2) {
        cout << "No route from " << src << endl;
        return;
    }

    cout << "Path from " << src << " : ";

    int cur = src;
    double total = 0.0;

    while (cur != -1) {
        cout << cur;

        int nxt = nextNode[cur];

        if (nxt == -1)
            break;

        double edgeCost = INF;

        for (const Edge& e : adj[cur]) {
            if (e.to == nxt && e.curCost < edgeCost) {
                edgeCost = e.curCost;
            }
        }

        if (edgeCost == INF)
            break;

        total += edgeCost;
        cout << " -> ";
        cur = nxt;
    }

    printf("\nTotal route cost (sum edges): %.6f\n", total);
}

int main() {
    // Input stages
    cout << "Enter number of stages: ";
    int S;
    cin >> S;

    if (S <= 0) {
        cout << "Invalid number of stages" << endl;
        return 0;
    }

    vector<int> stageCount(S);
    vector<int> stageStart(S);
    int N = 0;

    cout << "Enter number of nodes in each stage (" << S << " values): ";

    for (int i = 0; i < S; i++) {
        cin >> stageCount[i];

        if (stageCount[i] <= 0) {
            cout << "Each stage must have at least one node" << endl;
            return 0;
        }

        stageStart[i] = N;
        N += stageCount[i];
    }

    // Create adjacency lists
    adj.assign(N, {});
    revAdj.assign(N, {});

    cout << "Enter number of edges: ";
    int M;
    cin >> M;

    cout << "Enter each edge as: u v cost" << endl;

    for (int i = 0; i < M; i++) {
        int u, v;
        double cost;
        cin >> u >> v >> cost;

        if (u < 0 || u >= N || v < 0 || v >= N) {
            cout << "Invalid edge node ID" << endl;
            return 0;
        }

        adj[u].push_back({v, cost, cost});
        revAdj[v].push_back(u);
    }

    // Dynamic Programming initialization
    bestCost.assign(N, INF);
    nextNode.assign(N, -1);

    // Destination-stage nodes have cost zero
    int lastStage = S - 1;

    for (int k = 0; k < stageCount[lastStage]; k++) {
        int node = stageStart[lastStage] + k;
        bestCost[node] = 0.0;
    }

    // Calculate minimum costs backward
    for (int st = S - 2; st >= 0; st--) {
        for (int k = 0; k < stageCount[st]; k++) {
            int u = stageStart[st] + k;
            bestCost[u] = recomputeNode(u);
        }
    }

    displayCosts(stageStart, stageCount);

    // Print path
    cout << "\nEnter a source node ID (in stage 0) to print path, or -1 to skip: ";

    int src;
    cin >> src;

    if (src >= stageStart[0] && src < stageStart[0] + stageCount[0]) {
        printPath(src);
    } else if (src != -1) {
        cout << "Invalid source node for Stage 0" << endl;
    }

    // Real-time edge cost updates
    cout << "\nEnter number of live updates to edge costs (0 to finish): ";

    int Q;
    cin >> Q;

    while (Q-- > 0) {
        cout << "Enter edge update (u v multiplier): ";

        int u, v;
        double multiplier;
        cin >> u >> v >> multiplier;

        if (u < 0 || u >= N || v < 0 || v >= N) {
            cout << "Invalid edge node ID" << endl;
            continue;
        }

        bool found = false;

        for (Edge& e : adj[u]) {
            if (e.to == v) {
                e.curCost = e.baseCost * multiplier;
                found = true;
            }
        }

        if (!found) {
            cout << "Edge not found" << endl;
            continue;
        }

        // Recalculate the updated node and its predecessors
        queue<int> q;

        double newCost = recomputeNode(u);

        if (fabs(newCost - bestCost[u]) > 1e-9) {
            bestCost[u] = newCost;
            q.push(u);
        }

        while (!q.empty()) {
            int node = q.front();
            q.pop();

            for (int pred : revAdj[node]) {
                double updatedCost = recomputeNode(pred);

                if (fabs(updatedCost - bestCost[pred]) > 1e-9) {
                    bestCost[pred] = updatedCost;
                    q.push(pred);
                }
            }
        }
    }

    cout << "\nAfter updates:" << endl;
    displayCosts(stageStart, stageCount);

    return 0;
}
