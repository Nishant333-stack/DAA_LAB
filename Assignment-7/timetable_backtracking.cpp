#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<vector<bool>> graph;
vector<int> slot;
int n;

// Check whether subject u can be given colour c
bool isSafe(int u, int c) {
    for (int v = 0; v < n; v++) {
        if (graph[u][v] && slot[v] == c) {
            return false;
        }
    }
    return true;
}

// Backtracking: try to colour subjects u..n-1 using colours 0..m-1
bool colourGraph(int u, int m) {
    if (u == n) {
        return true;
    }

    for (int c = 0; c < m; c++) {
        if (isSafe(u, c)) {
            slot[u] = c;

            if (colourGraph(u + 1, m)) {
                return true;
            }

            slot[u] = -1;  // backtrack
        }
    }

    return false;
}

int main() {
    vector<string> subjects = {"DAA", "AIML", "SPM", "EAC", "MDM-3"};
    n = subjects.size();

    graph.assign(n, vector<bool>(n, false));

    cout << "Subjects:" << endl;
    for (int i = 0; i < n; i++) {
        cout << i << " - " << subjects[i] << endl;
    }

    cout << "Enter number of conflicts: ";
    int m;
    cin >> m;

    cout << "Enter conflicting pairs (u v):" << endl;

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;

        if (u < 0 || u >= n || v < 0 || v >= n || u == v) {
            cout << "Invalid conflict!" << endl;
            return 0;
        }

        graph[u][v] = true;
        graph[v][u] = true;
    }

    // Find the minimum number of slots: try m = 1, 2, ... until backtracking succeeds
    int totalSlots = 0;

    for (int colours = 1; colours <= n; colours++) {
        slot.assign(n, -1);

        if (colourGraph(0, colours)) {
            totalSlots = colours;
            break;
        }
    }

    cout << "Enter number of rooms: ";
    int roomCount;
    cin >> roomCount;

    if (roomCount <= 0) {
        cout << "Number of rooms must be positive." << endl;
        return 0;
    }

    vector<int> capacity(roomCount);

    cout << "Enter capacity of each room:" << endl;
    for (int i = 0; i < roomCount; i++) {
        cin >> capacity[i];
    }

    vector<int> students(n);

    cout << "Enter student count for DAA, AIML, SPM, EAC, MDM-3:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> students[i];
    }

    vector<string> colors = {"Red", "Green", "Yellow", "Blue", "Orange"};

    cout << "\n----- EXAM TIMETABLE (Backtracking Graph Colouring) -----" << endl;

    bool allocationFailed = false;

    for (int t = 0; t < totalSlots; t++) {
        cout << "\nColor: " << colors[t] << " (Slot " << (t + 1) << "):" << endl;

        vector<bool> occupied(roomCount, false);

        for (int i = 0; i < n; i++) {
            if (slot[i] != t) {
                continue;
            }

            // Best fit: smallest free room that can hold the class
            int chosen = -1;

            for (int r = 0; r < roomCount; r++) {
                if (!occupied[r] && capacity[r] >= students[i] &&
                    (chosen == -1 || capacity[r] < capacity[chosen])) {
                    chosen = r;
                }
            }

            cout << subjects[i] << " (" << students[i] << " students) - ";

            if (chosen == -1) {
                cout << "NO SUITABLE ROOM" << endl;
                allocationFailed = true;
            } else {
                occupied[chosen] = true;
                cout << "Room " << (chosen + 1) << endl;
            }
        }
    }

    cout << "\nMinimum exam slots (chromatic number): " << totalSlots << endl;

    if (allocationFailed) {
        cout << "Warning: Complete room allocation was not possible." << endl;
    } else {
        cout << "All examinations have suitable rooms." << endl;
    }

    return 0;
}
