#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Student {
    int id;
    int age;
    int grade;
};

void merge(vector<Student>& students, int low, int mid, int high) {
    int n1 = mid - low + 1;
    int n2 = high - mid;

    vector<Student> left(students.begin() + low, students.begin() + mid + 1);
    vector<Student> right(students.begin() + mid + 1, students.begin() + high + 1);

    int i = 0;
    int j = 0;
    int k = low;

    while (i < n1 && j < n2) {
        if (left[i].grade <= right[j].grade) {
            students[k] = left[i];
            i++;
        } else {
            students[k] = right[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        students[k] = left[i];
        i++;
        k++;
    }

    while (j < n2) {
        students[k] = right[j];
        j++;
        k++;
    }
}

void mergeSort(vector<Student>& students, int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;

        mergeSort(students, low, mid);
        mergeSort(students, mid + 1, high);

        merge(students, low, mid, high);
    }
}

string stripQuotes(string s) {
    string out;
    for (char c : s) {
        if (c != '"' && c != ' ' && c != '\r') {
            out += c;
        }
    }
    return out;
}

int main() {
    string fileName = "student-mat.csv";
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "student-mat.csv file not found." << endl;
        return 1;
    }

    vector<Student> students;
    string line;
    getline(file, line);  // skip header

    int id = 1;

    try {
        while (getline(file, line)) {
            if (line.empty()) {
                continue;
            }

            vector<string> data;
            stringstream ss(line);
            string field;

            while (getline(ss, field, ';')) {
                data.push_back(field);
            }

            int age = stoi(stripQuotes(data[2]));
            int grade = stoi(stripQuotes(data[data.size() - 1]));

            students.push_back({id, age, grade});
            id++;
        }
    } catch (const exception&) {
        cout << "Invalid data found in dataset." << endl;
        return 1;
    }

    cout << endl;
    cout << "Students Sorted by Final Grade" << endl;
    cout << endl;
    cout << "---------------------------------------------" << endl;
    printf("%-15s %-10s %-15s\n", "Student No.", "Age", "Final Grade");
    cout << "---------------------------------------------" << endl;

    mergeSort(students, 0, (int)students.size() - 1);

    for (const Student& student : students) {
        printf("%-15d %-10d %-15d\n", student.id, student.age, student.grade);
    }

    cout << "---------------------------------------------" << endl;

    return 0;
}
