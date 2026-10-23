#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Edge {
    int destination;
    int weight;
};

void dijkstra(const vector<vector<Edge>>& graph, int source, int destination) {
    int n = graph.size();
    vector<int> distance(n, INT_MAX);
    vector<int> parent(n, -1);

    // min-heap of {distance, vertex}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    distance[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {
        int dist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (dist > distance[u]) {
            continue;
        }

        for (const Edge& edge : graph[u]) {
            int v = edge.destination;
            int newDistance = distance[u] + edge.weight;

            if (newDistance < distance[v]) {
                distance[v] = newDistance;
                parent[v] = u;
                pq.push({newDistance, v});
            }
        }
    }

    if (distance[destination] == INT_MAX) {
        cout << "Customer location is not reachable." << endl;
        return;
    }

    vector<int> path;
    for (int current = destination; current != -1; current = parent[current]) {
        path.push_back(current);
    }
    reverse(path.begin(), path.end());

    cout << endl;
    cout << "Shortest Travel Time: " << distance[destination] << " minutes" << endl;

    cout << "Shortest Path: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i < path.size() - 1) {
            cout << " -> ";
        }
    }
    cout << endl;
}

int main() {
    string fileName = "food_delivery_graph.txt";
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "food_delivery_graph.txt file not found." << endl;
        return 1;
    }

    int vertices, edges;
    if (!(file >> vertices >> edges)) {
        cout << "Error while reading the dataset." << endl;
        return 1;
    }

    vector<vector<Edge>> graph(vertices);

    for (int i = 0; i < edges; i++) {
        int u, v, weight;
        file >> u >> v >> weight;

        graph[u].push_back({v, weight});
        graph[v].push_back({u, weight});
    }

    int source, destination;
    file >> source >> destination;

    cout << endl;
    cout << "Food Delivery Route Optimization" << endl;
    cout << endl;

    cout << "Number of Locations: " << vertices << endl;
    cout << "Number of Roads: " << edges << endl;
    cout << "Restaurant Location: " << source << endl;
    cout << "Customer Location: " << destination << endl;

    dijkstra(graph, source, destination);

    return 0;
}
