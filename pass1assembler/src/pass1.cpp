//============================================================================
// Name        : pass1.cpp
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
#include <map>
#include <algorithm>

using namespace std;

// Data structures to hold opcode tables, registers, etc.
map<string, pair<string, int>> optab = {
    {"STOP", {"IS", 0}},
    {"ADD", {"IS", 1}},
    {"SUB", {"IS", 2}},
    {"MULT", {"IS", 3}},
    {"MOVER", {"IS", 4}},
    {"MOVEM", {"IS", 5}},
    {"COMP", {"IS", 6}},
    {"BC", {"IS", 7}},
    {"DIV", {"IS", 8}},
    {"READ", {"IS", 9}},
    {"PRINT", {"IS", 10}},
    {"START", {"AD", 1}},
    {"END", {"AD", 2}},
    {"ORIGIN", {"AD", 3}},
    {"EQU", {"AD", 4}},
    {"LTORG", {"AD", 5}},
    {"DC", {"DL", 1}},
    {"DS", {"DL", 2}}
};

map<string, int> register_codes = {
    {"AREG", 1},
    {"BREG", 2},
    {"CREG", 3},
    {"DREG", 4}
};

map<string, int> cond_codes = {
    {"LT", 1},
    {"LE", 2},
    {"EQ", 3},
    {"GT", 4},
    {"GE", 5},
    {"ANY", 6}
};

vector<string> ic_code;
int location_counter = 0;
vector<vector<string>> source_lines;

// Symbol Table, Literal Table, Pool Table
// Symbol Table Entry: symbol, location
vector<pair<string, int>> symbol_table;
// Literal Table Entry: literal, location, pool
struct LiteralEntry {
    string literal;
    int location;
    string pool;
};
vector<LiteralEntry> literal_table;
vector<string> pool_table = {"#1"};

// Function declarations
void read_file();
vector<string> split(const string &s, char delimiter);
tuple<string, string, string, string> tokenize_line(const vector<string>& line);
void increment_location_counter(int val = 1);
void set_location_counter(int lc);
int get_symbol_position(const string& symbol);
int get_literal_position(const string& literal);
int get_symbol_location_counter(const string& symbol);
void insert_into_symbol_table(const string& symbol, int lc = -1, bool forward_ref = true);
void insert_into_literal_table(const string& literal);
pair<bool, int> get_constant_value(const string& operand);
pair<string, string> parse_operand(const string& operand);
string get_operand_str(const string& operand);
void create_new_pool();
void print_ic();
void print_symbol_table();
void print_literal_table();
void print_pool_table();
void handle_imperative_statement(const string& label, const string& opcode, const string& operand1, const string& operand2);
void handle_assembler_directive(const string& label, const string& opcode, const string& operand1, const string& operand2);
void handle_declarative(const string& label, const string& opcode, const string& operand1, const string& operand2);

int main() {
    read_file();

    location_counter = 0;

    for (auto &line : source_lines) {
        auto [label, opcode, operand1, operand2] = tokenize_line(line);

        if (!label.empty()) {
            insert_into_symbol_table(label, -1, false);
        }

        if (!opcode.empty()) {
            auto it = optab.find(opcode);
            if (it == optab.end()) {
                cerr << "Invalid opcode: " << opcode << endl;
                continue;
            }

            string opcode_type = it->second.first;

            if (opcode_type == "IS") {
                handle_imperative_statement(label, opcode, operand1, operand2);
            } else if (opcode_type == "AD") {
                handle_assembler_directive(label, opcode, operand1, operand2);
            } else if (opcode_type == "DL") {
                handle_declarative(label, opcode, operand1, operand2);
            }
        }
    }

    print_ic();
    print_symbol_table();
    print_literal_table();
    print_pool_table();

    return 0;
}

// Function definitions

void read_file() {
    string source_file_location;
    cout << "Enter Source File Location: ";
    getline(cin, source_file_location);

    ifstream file(source_file_location);
    if (!file.is_open()) {
        cerr << "Failed to open file " << source_file_location << endl;
        exit(1);
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        source_lines.push_back(split(line, ' '));
    }

    file.close();
}

vector<string> split(const string &s, char delimiter) {
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);
    while (getline(tokenStream, token, delimiter)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

tuple<string, string, string, string> tokenize_line(const vector<string>& line) {
    string label = "", opcode = "", operand1 = "", operand2 = "";

    if (line.size() == 1 && optab.find(line[0]) != optab.end()) {
        opcode = line[0];
    } else if (line.size() == 1) {
        label = line[0];
    } else if (line.size() == 2 && optab.find(line[0]) != optab.end()) {
        opcode = line[0];
        operand1 = line[1];
    } else if (line.size() == 2) {
        label = line[0];
        opcode = line[1];
    } else if (line.size() == 3 && optab.find(line[0]) != optab.end()) {
        opcode = line[0];
        operand1 = line[1];
        operand2 = line[2];
    } else if (line.size() == 3) {
        label = line[0];
        opcode = line[1];
        operand1 = line[2];
    } else if (line.size() == 4) {
        label = line[0];
        opcode = line[1];
        operand1 = line[2];
        operand2 = line[3];
    }

    return {label, opcode, operand1, operand2};
}

void increment_location_counter(int val) {
    location_counter += val;
}

void set_location_counter(int lc) {
    location_counter = lc;
}

int get_symbol_position(const string& symbol) {
    for (int i = 0; i < (int)symbol_table.size(); i++) {
        if (symbol_table[i].first == symbol) return i + 1;
    }
    return -1;
}

int get_literal_position(const string& literal) {
    string current_pool = pool_table.back();
    for (int i = 0; i < (int)literal_table.size(); i++) {
        if (literal_table[i].literal == literal && literal_table[i].pool == current_pool)
            return i + 1;
    }
    return -1;
}

int get_symbol_location_counter(const string& symbol) {
    int idx = get_symbol_position(symbol) - 1;
    if (idx < 0) return -1;
    return symbol_table[idx].second;
}

void insert_into_symbol_table(const string& symbol, int lc, bool forward_ref) {
    int position = get_symbol_position(symbol);

    if (lc == -1) {
        lc = location_counter;
    }

    if (forward_ref) {
        if (position == -1) {
            symbol_table.push_back({symbol, -1});
        }
    } else {
        if (position == -1) {
            symbol_table.push_back({symbol, lc});
        } else {
            symbol_table[position - 1].second = lc;
        }
    }
}

void insert_into_literal_table(const string& literal) {
    int position = get_literal_position(literal);

    if (position == -1) {
        string current_pool = pool_table.back();
        literal_table.push_back({literal, -1, current_pool});
    }
}

pair<bool, int> get_constant_value(const string& operand) {
    if (operand.empty()) return {false, -1};

    if (all_of(operand.begin(), operand.end(), ::isdigit)) {
        return {true, stoi(operand)};
    }

    size_t plus_pos = operand.find('+');
    if (plus_pos != string::npos) {
        string symbol = operand.substr(0, plus_pos);
        int offset = stoi(operand.substr(plus_pos + 1));
        int symbol_lc = get_symbol_location_counter(symbol);
        return {true, symbol_lc + offset};
    }

    size_t minus_pos = operand.find('-');
    if (minus_pos != string::npos) {
        string symbol = operand.substr(0, minus_pos);
        int offset = stoi(operand.substr(minus_pos + 1));
        int symbol_lc = get_symbol_location_counter(symbol);
        return {true, symbol_lc - offset};
    }

    return {false, -1};
}

pair<string, string> parse_operand(const string& operand) {
    if (operand.empty()) return {"", ""};

    if (operand[0] == '=') {
        string literal = operand.substr(1);
        insert_into_literal_table(literal);
        int pos = get_literal_position(literal);
        return {"L", to_string(pos)};
    }

    if (cond_codes.find(operand) != cond_codes.end()) {
        return {"COND", to_string(cond_codes[operand])};
    }

    if (register_codes.find(operand) != register_codes.end()) {
        return {"REG", to_string(register_codes[operand])};
    }

    auto [is_const, val] = get_constant_value(operand);
    if (is_const) {
        return {"C", to_string(val)};
    }

    // Otherwise symbol
    insert_into_symbol_table(operand, -1, true);
    int pos = get_symbol_position(operand);
    return {"S", to_string(pos)};
}

string get_operand_str(const string& operand) {
    if (operand.empty()) return "";

    auto [type, val] = parse_operand(operand);
    if (type == "REG" || type == "COND") {
        return " (" + val + ")";
    } else {
        return " (" + type + ", " + val + ")";
    }
}

void create_new_pool() {
    string current_pool = pool_table.back();
    int pool_number = stoi(current_pool.substr(1));

    for (auto &literal : literal_table) {
        if (literal.location == -1 && literal.pool == current_pool) {
            literal.location = location_counter;
            string output_line = "(DL, 01) (C, " + literal.literal.substr(1) + ")";
            ic_code.push_back(output_line);
            increment_location_counter();
        }
    }

    pool_table.push_back("#" + to_string(pool_number + 1));
}

void print_ic() {
    for (auto &line : ic_code) {
        cout << line << "\n";
    }
}

void print_symbol_table() {
    cout << "\nSymbol Table:\n";
    cout << "Index\tSymbol\tLocation\n";
    for (size_t i = 0; i < symbol_table.size(); i++) {
        cout << i + 1 << "\t" << symbol_table[i].first << "\t" << symbol_table[i].second << "\n";
    }
}

void print_literal_table() {
    cout << "\nLiteral Table:\n";
    cout << "Index\tLiteral\tLocation\n";
    string pool = "#1";
    int i = 1;
    for (auto &literal_entry : literal_table) {
        if (literal_entry.pool != pool) {
            pool = literal_entry.pool;
            cout << "\n";
        }
        cout << i++ << "\t" << literal_entry.literal << "\t" << literal_entry.location << "\n";
    }
}

void print_pool_table() {
    cout << "\nPool Table:\n";
    for (size_t i = 0; i < pool_table.size() - 1; i++) {
        cout << pool_table[i] << "\n";
    }
}

void handle_imperative_statement(const string& label, const string& opcode, const string& operand1, const string& operand2) {
    auto [_, opcode_ic] = optab[opcode];
    string output_line = "(IS, " + to_string(opcode_ic) + ")";
    output_line += get_operand_str(operand1);
    output_line += get_operand_str(operand2);
    ic_code.push_back(output_line);
    increment_location_counter();
}

void handle_assembler_directive(const string& label, const string& opcode, const string& operand1, const string& operand2) {
    auto [_, opcode_ic] = optab[opcode];
    string output_line = "(AD, " + to_string(opcode_ic) + ")";

    if (opcode == "START" || opcode == "ORIGIN") {
        auto [is_const, constant_value] = get_constant_value(operand1);
        if (!is_const) {
            cerr << "START and ORIGIN must have operand (constant type)\n";
            exit(1);
        }
        output_line += get_operand_str(operand1);
        ic_code.push_back(output_line);
        set_location_counter(constant_value);
    } else if (opcode == "END" || opcode == "LTORG") {
        if (opcode == "END") {
            output_line += get_operand_str(operand1);
            ic_code.push_back(output_line);
        }
        create_new_pool();
    } else if (opcode == "EQU") {
        auto [operand_type, operand_val] = parse_operand(operand1);

        if (operand_type != "S" && operand_type != "C") {
            cerr << "EQU must have operand (constant or symbol or equivalent)\n";
            exit(1);
        }

        if (operand_type == "C") {
            insert_into_symbol_table(label, stoi(operand_val), false);
        } else {
            int pos = stoi(operand_val);
            if (pos == -1) {
                cerr << "Symbol must be initialized before using in EQU\n";
                exit(1);
            }
            int symbol_lc = symbol_table[pos - 1].second;
            insert_into_symbol_table(label, symbol_lc, false);
        }
        output_line += get_operand_str(operand1);
        ic_code.push_back(output_line);
    }
}

void handle_declarative(const string& label, const string& opcode, const string& operand1, const string& operand2) {
    auto [_, opcode_ic] = optab[opcode];
    string output_line = "(DL, " + to_string(opcode_ic) + ")";
    output_line += get_operand_str(operand1);
    ic_code.push_back(output_line);

    if (opcode == "DC") {
        increment_location_counter(1);
    } else if (opcode == "DS") {
        auto [is_const, const_value] = get_constant_value(operand1);
        if (!is_const) {
            cerr << "Declare Space must have operand (constant type or equivalent)\n";
            exit(1);
        }
        increment_location_counter(const_value);
    }
}
