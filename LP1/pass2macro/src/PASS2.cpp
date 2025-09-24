#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>

using namespace std;

// Global data structures
vector<vector<string>> src_lines;            // Source lines from src.txt
vector<vector<string>> mnt;                  // Macro Name Table: [name, pp, kp, mdtp, kpdtp]
vector<vector<string>> mdt;                  // Macro Definition Table (lines of macro body)
vector<vector<string>> kpdt;                 // Keyword Param Default Table: [param, default]

vector<string> apt;                          // Argument Parameter Table for expansion
vector<string> output_lines;                 // Final output lines

// Utility functions
vector<string> split(const string& line, char delim = ' ') {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, delim)) {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

void read_file_into_lines(const string& filepath, vector<vector<string>>& container) {
    ifstream fin(filepath);
    if (!fin) {
        cerr << "Failed to open file: " << filepath << endl;
        exit(1);
    }
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        container.push_back(split(line));
    }
    fin.close();
}

void read_mnt_file(const string& filepath) {
    ifstream fin(filepath);
    if (!fin) {
        cerr << "Failed to open MNT file: " << filepath << endl;
        exit(1);
    }
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        vector<string> tokens = split(line);
        // MNT fields: name, pp, kp, mdtp, kpdtp
        if (tokens.size() < 5) {
            cerr << "Invalid MNT entry: " << line << endl;
            exit(1);
        }
        mnt.push_back(tokens);
    }
    fin.close();
}

// Check if a line is a macro call
pair<bool, vector<string>> is_macro_call(const vector<string>& line) {
    if (line.empty()) return {false, {}};
    int pp = 0, kp = 0;
    // Count positional and keyword args in call
    for (size_t i = 1; i < line.size(); ++i) {
        if (line[i].find('=') != string::npos) kp++;
        else pp++;
    }

    for (const auto& mnt_entry : mnt) {
        string macro_name = mnt_entry[0];
        int mnt_pp = stoi(mnt_entry[1]);
        int mnt_kp = stoi(mnt_entry[2]);
        if (line[0] == macro_name && pp == mnt_pp && kp <= mnt_kp) {
            return {true, mnt_entry};
        }
    }
    return {false, {}};
}

// Get parameters for macro call and fill APT accordingly
vector<string> get_parameters(const vector<string>& called_line, const vector<string>& mnt_entry) {
    vector<string> params;

    int kp = stoi(mnt_entry[2]);
    int kpdt_start = stoi(mnt_entry[4]) - 1;

    // Get keyword params from KPDT with default values
    vector<pair<string, string>> keyword_params;
    for (int i = 0; i < kp; ++i) {
        vector<string> kpdt_entry = kpdt[kpdt_start + i];
        if (kpdt_entry.size() < 2) {
            cerr << "Invalid KPDT entry at index " << kpdt_start + i << endl;
            exit(1);
        }
        keyword_params.push_back({kpdt_entry[0], kpdt_entry[1]});
    }

    // Add positional params in order
    for (size_t i = 1; i < called_line.size(); ++i) {
        if (called_line[i].find('=') == string::npos) {
            params.push_back(called_line[i]);
        }
    }

    // Update keyword params with actual arguments if provided
    for (size_t i = 1; i < called_line.size(); ++i) {
        if (called_line[i].find('=') != string::npos) {
            auto eqPos = called_line[i].find('=');
            string param_name = called_line[i].substr(0, eqPos);
            string arg_value = called_line[i].substr(eqPos + 1);
            for (auto& kp_param : keyword_params) {
                if (kp_param.first == param_name) {
                    kp_param.second = arg_value;
                    break;
                }
            }
        }
    }

    // Add keyword params to params list, check for missing arguments
    for (auto& kp_param : keyword_params) {
        if (kp_param.second != "_") {
            params.push_back(kp_param.second);
        } else {
            throw runtime_error("Argument not provided for parameter " + kp_param.first);
        }
    }

    return params;
}

// Expand macro call starting from MDT pointer
void expand_macro_call(int mdt_start) {
    int i = mdt_start;
    while (i < (int)mdt.size() && find(mdt[i].begin(), mdt[i].end(), "MEND") == mdt[i].end()) {
        string output_line;
        for (const string& token : mdt[i]) {
            if (token.find('(') != string::npos && token.find(')') != string::npos) {
                // token like (P, 1)
                size_t pos1 = token.find('(');
                size_t pos2 = token.find(')');
                string inside = token.substr(pos1 + 1, pos2 - pos1 - 1); // e.g. "P, 1"
                size_t comma_pos = inside.find(',');
                if (comma_pos == string::npos) {
                    output_line += token + " ";
                } else {
                    string index_str = inside.substr(comma_pos + 1);
                    int index = stoi(index_str) - 1;
                    if (index >= 0 && index < (int)apt.size()) {
                        output_line += apt[index] + " ";
                    } else {
                        cerr << "APT index out of range: " << index << endl;
                        output_line += token + " ";
                    }
                }
            } else {
                output_line += token + " ";
            }
        }
        output_lines.push_back(output_line);
        i++;
    }
}

int main() {
    string testcase_folder;
    cout << "Enter Testcase folder: ";
    getline(cin, testcase_folder);

    read_file_into_lines(testcase_folder + "/src.txt", src_lines);
    read_mnt_file(testcase_folder + "/MNT.txt");
    read_file_into_lines(testcase_folder + "/MDT.txt", mdt);
    read_file_into_lines(testcase_folder + "/KPDT.txt", kpdt);

    for (const auto& line : src_lines) {
        auto [is_macro, mnt_entry] = is_macro_call(line);
        if (is_macro) {
            try {
                apt = get_parameters(line, mnt_entry);
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
                return 1;
            }
            int mdt_start = stoi(mnt_entry[3]) - 1;
            expand_macro_call(mdt_start);
        } else {
            // Normal line, just append
            string output_line;
            for (const string& t : line) {
                output_line += t + " ";
            }
            output_lines.push_back(output_line);
        }
    }

    // Print output lines
    for (const string& line : output_lines) {
        cout << line << endl;
    }

    return 0;
}
