//============================================================================
// Name        : PASS!.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>   // Added for std::remove

using namespace std;

// File paths
string ic_location;
string symbol_table_location;
string literal_table_location;
string pool_table_location;

// Data structures
vector<pair<string, int>> symbol_table;      // [symbol, location_counter]
vector<pair<string, int>> literal_table;     // [literal, location_counter]
vector<string> pool_table;                    // ["#1", "#2", ...]
vector<vector<string>> ic_code;               // Each line split into tokens

int location_counter = 0;
vector<string> output_code;

// Helper function to split string by delimiter
vector<string> split(const string& s, const string& delimiter) {
    vector<string> tokens;
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    string token;

    while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        if (!token.empty()) tokens.push_back(token);
        pos_start = pos_end + delim_len;
    }

    token = s.substr(pos_start);
    if (!token.empty()) tokens.push_back(token);

    return tokens;
}

// Reads symbol table file
void read_symbol_table() {
    ifstream file(symbol_table_location);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string sym; int loc;
        if (iss >> sym >> loc) {
            symbol_table.push_back({sym, loc});
        }
    }
}

// Reads literal table file
void read_literal_table() {
    ifstream file(literal_table_location);
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string lit; int loc;
        if (iss >> lit >> loc) {
            literal_table.push_back({lit, loc});
        }
    }
}

// Reads pool table file
void read_pool_table() {
    ifstream file(pool_table_location);
    string line;
    while (getline(file, line)) {
        if (!line.empty())
            pool_table.push_back(line);
    }
}

// Reads intermediate code file
void read_intermediate_code() {
    ifstream file(ic_location);
    string line;
    while (getline(file, line)) {
        // Split line into tokens like: (AD,01) (C,100)
        vector<string> parts;
        size_t start = 0, end = 0;
        while (end < line.size()) {
            if (line[end] == '(') {
                start = end;
            } else if (line[end] == ')') {
                parts.push_back(line.substr(start, end - start + 1));
            }
            end++;
        }
        ic_code.push_back(parts);
    }
}

// Parse opcode token, e.g. "(AD,01)" -> ("AD", 1)
pair<string, int> parse_opcode(const string& token) {
    string s = token;
    s.erase(std::remove(s.begin(), s.end(), '('), s.end());
    s.erase(std::remove(s.begin(), s.end(), ')'), s.end());

    auto tokens = split(s, ",");
    string opcode_type = tokens[0];
    int opcode_val = stoi(tokens[1]);
    return {opcode_type, opcode_val};
}

// Parse operand token, e.g. "(S,1)" or "(C,100)" or "(2)" for REG/COND
pair<string, int> parse_operand(const string& token) {
    string s = token;
    s.erase(std::remove(s.begin(), s.end(), '('), s.end());
    s.erase(std::remove(s.begin(), s.end(), ')'), s.end());

    auto tokens = split(s, ",");
    if (tokens.size() == 1) {
        // REG or COND, just return value
        return {"", stoi(tokens[0])};
    }

    string type = tokens[0];
    int val = stoi(tokens[1]);

    if (type == "S") {
        // Symbol table index to location
        int idx = val - 1;
        return {type, symbol_table[idx].second};
    }
    if (type == "L") {
        int idx = val - 1;
        return {type, literal_table[idx].second};
    }
    if (type == "C") {
        return {type, val};
    }

    return {"", -1}; // fallback (shouldn't happen)
}

void change_location_counter(int location) {
    location_counter = location;
}

void increment_location_counter() {
    location_counter++;
}

int main() {
    cout << "Enter Testcase folder : ";
    string testcase_location;
    getline(cin, testcase_location);

    literal_table_location = testcase_location + "/literal_table.txt";
    symbol_table_location = testcase_location + "/symbol_table.txt";
    pool_table_location = testcase_location + "/pool_table.txt";
    ic_location = testcase_location + "/intermediate_code.txt";

    // Read tables and intermediate code
    read_symbol_table();
    read_literal_table();
    read_pool_table();
    read_intermediate_code();

    for (const auto& card : ic_code) {
        if (card.size() == 1) {
            auto [opcode_type, opcode_val] = parse_opcode(card[0]);

            if (opcode_type == "AD") {
                // Assembler directive - no output
            } else {
                // IS or DL with no operand
                output_code.push_back(to_string(location_counter) + "\t" + to_string(opcode_val) + "\t0\t0");
                increment_location_counter();
            }

        } else if (card.size() == 2) {
            auto [opcode_type, opcode_val] = parse_opcode(card[0]);
            auto [operand_1_type, operand_1_val] = parse_operand(card[1]);

            if (opcode_type == "AD") {
                if (opcode_val == 1 || opcode_val == 3) {  // START or ORIGIN
                    change_location_counter(operand_1_val);
                }
            } else if (opcode_type == "DL") {
                if (opcode_val == 1) { // DC - define constant
                    output_code.push_back(to_string(location_counter) + "\t0\t0\t" + to_string(operand_1_val));
                    increment_location_counter();
                } else if (opcode_val == 2) { // DS - define space
                    for (int i = 0; i < operand_1_val; i++) {
                        output_code.push_back(to_string(location_counter) + "\t0\t0\t0");
                        increment_location_counter();
                    }
                }
            } else {
                if (operand_1_type == "") {
                    output_code.push_back(to_string(location_counter) + "\t" + to_string(opcode_val) + "\t" + to_string(operand_1_val) + "\t0");
                } else {
                    output_code.push_back(to_string(location_counter) + "\t" + to_string(opcode_val) + "\t0\t" + to_string(operand_1_val));
                }
                increment_location_counter();
            }

        } else if (card.size() == 3) {
            auto [opcode_type, opcode_val] = parse_opcode(card[0]);
            auto [operand_1_type, operand_1_val] = parse_operand(card[1]);
            auto [operand_2_type, operand_2_val] = parse_operand(card[2]);

            if (opcode_type == "AD") {
                // no output
            } else {
                output_code.push_back(to_string(location_counter) + "\t" + to_string(opcode_val) + "\t" + to_string(operand_1_val) + "\t" + to_string(operand_2_val));
                increment_location_counter();
            }
        }
    }

    // Print output code
    for (auto &line : output_code) {
        cout << line << "\n";
    }

    return 0;
}
