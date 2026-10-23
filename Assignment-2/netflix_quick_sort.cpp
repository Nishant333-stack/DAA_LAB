#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct NetflixShow {
    string showId;
    string type;
    string title;
    int releaseYear;
};

int partition(vector<NetflixShow>& shows, int low, int high) {
    int pivot = shows[high].releaseYear;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (shows[j].releaseYear <= pivot) {
            i++;
            swap(shows[i], shows[j]);
        }
    }

    swap(shows[i + 1], shows[high]);

    return i + 1;
}

void quickSort(vector<NetflixShow>& shows, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(shows, low, high);
        quickSort(shows, low, pivotIndex - 1);
        quickSort(shows, pivotIndex + 1, high);
    }
}

string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    if (start == string::npos) {
        return "";
    }
    size_t end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

int main() {
    vector<NetflixShow> shows = {
        {"s100", "Movie", "The Lost World", 1925},
        {"s200", "Movie", "The Great Dictator", 1940},
        {"s300", "Movie", "Double Indemnity", 1944},
        {"s400", "Movie", "Seven Samurai", 1954},
        {"s500", "Movie", "Pather Panchali", 1955},
        {"s600", "Movie", "The Ten Commandments", 1956},
        {"s700", "Movie", "Vertigo", 1958},
        {"s800", "Movie", "North by Northwest", 1959},
        {"s900", "Movie", "Psycho", 1960},
        {"s1000", "Movie", "Breakfast at Tiffany's", 1961}
    };

    cout << "Enter the column to sort by: ";
    string column;
    getline(cin, column);
    column = trim(column);

    if (toLower(column) != "release year") {
        cout << "Invalid column. Please enter Release Year." << endl;
        return 0;
    }

    quickSort(shows, 0, (int)shows.size() - 1);

    cout << "\nNetflix Content Sorted by Release Year\n" << endl;

    printf("%-12s %-12s %-45s %-12s\n", "Show ID", "Type", "Title", "Release Year");

    for (const NetflixShow& show : shows) {
        printf("%-12s %-12s %-45s %-12d\n",
               show.showId.c_str(), show.type.c_str(), show.title.c_str(), show.releaseYear);
    }

    return 0;
}
