//
//  main.cpp
//  HW3
//
//  Created by 王吉朗 on 2023/4/10.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

class Employee {
public:
    Employee(string id) : id_(id), overloading_days_(0), sign_forget_days_(0) {}

    void add_signin(string timestamp) {
        signin_ = timestamp;
        signed_in_ = true;
    }

    void add_signout(string timestamp) {
        signout_ = timestamp;
        signed_out_ = true;
    }

    void calculate_workdays() {
        if (signed_in_ && signed_out_) {
            int minutes_worked = calculate_minutes_worked(signin_, signout_);
            if (minutes_worked > 8 * 60) {
                overloading_days_++;
            }
        } else {
            sign_forget_days_++;
        }
    }

    void print_results() const {
        cout << id_ << ", " << overloading_days_ << ", " << sign_forget_days_ << endl;
    }

private:
    string id_;
    string signin_;
    string signout_;
    bool signed_in_ = false;
    bool signed_out_ = false;
    int overloading_days_;
    int sign_forget_days_;

    int calculate_minutes_worked(string signin, string signout) const {
        // Convert timestamps to minutes since midnight
        int signin_minutes = convert_to_minutes(signin);
        int signout_minutes = convert_to_minutes(signout);

        // Calculate difference in minutes
        int minutes_worked = signout_minutes - signin_minutes;

        // Handle cases where sign-out is on the next day
        if (minutes_worked < 0) {
            minutes_worked += 24 * 60;
        }

        return minutes_worked;
    }

    int convert_to_minutes(string timestamp) const {
        // Parse hours and minutes from timestamp
        int year, month, day, hour, minute;
        sscanf(timestamp.c_str(), "%4d-%2d-%2d %2d:%2d", &year, &month, &day, &hour, &minute);

        // Convert to minutes since midnight
        return (hour * 60) + minute;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " input_file.csv" << endl;
        return 1;
    }

    string input_file = argv[1];

    unordered_map<string, Employee> employees;
    ifstream fin(input_file);
    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string id, sign_type, timestamp;
        getline(ss, id, ',');
        getline(ss, sign_type, ',');
        getline(ss, timestamp, ',');

        if (employees.find(id) == employees.end()) {
            employees[id] = Employee(id);
        }

        if (sign_type == "sign-in") {
            employees[id].add_signin(timestamp);
        } else if (sign_type == "sign-out") {
            employees[id].add_signout(timestamp);
        }
    }

    fin.close();

    for (auto& kv : employees) {
        kv.second.calculate_workdays();
        kv.second.print_results();
    }

    return 0;
}
