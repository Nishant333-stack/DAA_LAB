#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    cout << "\n==============================================" << endl;
    cout << "     DISASTER RELIEF RESOURCE ALLOCATION" << endl;
    cout << "           0/1 KNAPSACK PROBLEM" << endl;
    cout << "==============================================" << endl;

    cout << "\nEnter number of items: ";
    int n;
    cin >> n;

    vector<string> itemNames(n);
    vector<int> weight(n);
    vector<int> value(n);

    cout << "\nEnter the item details:" << endl;

    for (int i = 0; i < n; i++) {
        cout << "\nItem " << (i + 1) << endl;

        cout << "Item Name: ";
        cin >> itemNames[i];

        cout << "Weight (kg): ";
        cin >> weight[i];

        cout << "Utility Score: ";
        cin >> value[i];
    }

    cout << "\nEnter truck capacity (kg): ";
    int W;
    cin >> W;

    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // Dynamic Programming
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            if (weight[i - 1] <= w) {
                int include = value[i - 1] + dp[i - 1][w - weight[i - 1]];
                int exclude = dp[i - 1][w];

                dp[i][w] = max(include, exclude);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Trace back to find selected items
    vector<bool> selected(n, false);
    int remainingCapacity = W;
    int totalWeight = 0;

    for (int i = n; i > 0; i--) {
        if (dp[i][remainingCapacity] != dp[i - 1][remainingCapacity]) {
            selected[i - 1] = true;
            totalWeight += weight[i - 1];
            remainingCapacity -= weight[i - 1];
        }
    }

    // Display results
    cout << "\n\n==============================================================" << endl;
    cout << "                 OPTIMIZATION RESULTS" << endl;
    cout << "==============================================================" << endl;

    printf("%-18s %-12s %-15s %-15s\n", "ITEM NAME", "WEIGHT (KG)", "UTILITY", "STATUS");

    cout << "--------------------------------------------------------------" << endl;

    for (int i = 0; i < n; i++) {
        printf("%-18s %-12d %-15d %-15s\n",
               itemNames[i].c_str(), weight[i], value[i],
               selected[i] ? "SELECTED" : "NOT SELECTED");
    }

    cout << "--------------------------------------------------------------" << endl;

    cout << "\n                 FINAL SUMMARY" << endl;
    cout << "--------------------------------------------------------------" << endl;
    cout << "Truck Capacity       : " << W << " kg" << endl;
    cout << "Total Weight Loaded  : " << totalWeight << " kg" << endl;
    cout << "Remaining Capacity   : " << (W - totalWeight) << " kg" << endl;
    cout << "Maximum Total Utility: " << dp[n][W] << endl;

    cout << "\nSelected Relief Items:" << endl;

    bool found = false;

    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            cout << "  [+] " << itemNames[i]
                 << " | Weight: " << weight[i]
                 << " kg | Utility: " << value[i] << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "  No items selected." << endl;
    }

    cout << "\n==============================================================" << endl;
    cout << "   Optimal resource allocation completed successfully!" << endl;
    cout << "==============================================================" << endl;

    return 0;
}
