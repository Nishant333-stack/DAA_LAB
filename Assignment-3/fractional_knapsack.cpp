#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Item {
    string name;
    double weight;
    double price;
    double ratio;
};

void sortItems(vector<Item>& items) {
    stable_sort(items.begin(), items.end(),
                [](const Item& a, const Item& b) { return a.ratio > b.ratio; });
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

vector<string> parseCSV(const string& line) {
    vector<string> fields;
    string field;
    bool quotes = false;

    for (char c : line) {
        if (c == '"') {
            quotes = !quotes;
        } else if (c == ',' && !quotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }

    fields.push_back(trim(field));
    return fields;
}

int main() {
    string fileName = "shipping_data.csv";
    double capacity = 1000.0;

    vector<Item> items;

    cout << "\n==============================================================" << endl;
    cout << "              FRACTIONAL KNAPSACK ALGORITHM" << endl;
    cout << "                  CARGO SHIPMENT SYSTEM" << endl;
    cout << "==============================================================" << endl;

    printf("Container Capacity : %.2f kg\n", capacity);

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "\nERROR: shipping_data.csv was not found." << endl;
        cout << "Place it in the program's working directory." << endl;
        return 1;
    }

    string line;
    getline(file, line);  // skip header

    while (getline(file, line)) {
        vector<string> data = parseCSV(line);

        if (data.size() < 3) {
            continue;
        }

        try {
            string name = data[0];
            double price = stod(data[1]);
            double weight = stod(data[2]);

            if (price > 0 && weight > 0) {
                items.push_back({name, weight, price, price / weight});
            }
        } catch (const exception&) {
            cout << "Skipping invalid CSV row." << endl;
        }
    }

    if (items.empty()) {
        cout << "\nNo valid products found." << endl;
        return 0;
    }

    sortItems(items);

    cout << "\nPRODUCTS SORTED BY VALUE PER KG" << endl;
    cout << "==============================================================" << endl;

    printf("%-22s %12s %12s %12s\n", "PRODUCT NAME", "WEIGHT (KG)", "PRICE", "VALUE/KG");

    cout << "--------------------------------------------------------------" << endl;

    for (const Item& item : items) {
        printf("%-22.22s %12.2f %12.2f %12.2f\n",
               item.name.c_str(), item.weight, item.price, item.ratio);
    }

    double remainingCapacity = capacity;
    double totalWeight = 0;
    double totalValue = 0;

    cout << "\nCARGO ALLOCATION REPORT" << endl;
    cout << "==========================================================================" << endl;

    printf("%-22s %12s %12s %14s %12s\n",
           "PRODUCT NAME", "TAKEN (KG)", "VALUE/KG", "VALUE GAINED", "STATUS");

    cout << "--------------------------------------------------------------------------" << endl;

    for (const Item& item : items) {
        if (remainingCapacity <= 0) {
            break;
        }

        double takenWeight;
        double obtainedValue;
        string status;

        if (item.weight <= remainingCapacity) {
            takenWeight = item.weight;
            obtainedValue = item.price;
            status = "FULL";
        } else {
            takenWeight = remainingCapacity;
            obtainedValue = item.ratio * takenWeight;
            status = "PARTIAL";
        }

        totalWeight += takenWeight;
        totalValue += obtainedValue;
        remainingCapacity -= takenWeight;

        printf("%-22.22s %12.2f %12.2f %14.2f %12s\n",
               item.name.c_str(), takenWeight, item.ratio, obtainedValue, status.c_str());
    }

    cout << "--------------------------------------------------------------------------" << endl;

    cout << "\nFINAL OPTIMIZATION SUMMARY" << endl;
    cout << "==============================================================" << endl;

    printf("Total Products Available : %d\n", (int)items.size());
    printf("Container Capacity       : %.2f kg\n", capacity);
    printf("Total Weight Loaded      : %.2f kg\n", totalWeight);
    printf("Remaining Capacity       : %.2f kg\n", max(0.0, remainingCapacity));
    printf("Capacity Utilization     : %.2f%%\n", totalWeight / capacity * 100);
    printf("Maximum Total Value      : Rs. %.2f\n", totalValue);

    cout << "==============================================================" << endl;
    cout << "    OPTIMAL CARGO ALLOCATION COMPLETED SUCCESSFULLY" << endl;
    cout << "==============================================================" << endl;

    return 0;
}
