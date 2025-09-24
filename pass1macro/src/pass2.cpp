#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Tables
struct MNTEntry {
    string name;
    int pp;
    int kp;
    int MDTP;
    int KPDT;
    int PNTP;
};

vector<MNTEntry> MNT;
vector<string> MDT;
vector<pair<string, string>> KPDT;  // keyword param, default
vector<string> PNT;
vector<string> output_lines;

vector<vector<string>> source_code;

// Utility functions
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t");
    return s.substr(start, end - start + 1);
}

vector<string> split(const string& s, char delim=' ') {
    vector<string> tokens;
    stringstream ss(s);
    string token;
    while (getline(ss, token, delim)) {
        token = trim(token);
        if (!token.empty()) tokens.push_back(token);
    }
    return tokens;
}

void read_lines(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error opening file: " << filename << "\n";
        exit(1);
    }
    string line;
    while (getline(fin, line)) {
        vector<string> tokens = split(line);
        source_code.push_back(tokens);
    }
    fin.close();
}

void analyze_macro_definition(const vector<string>& md_tokens) {
    string macro_name = md_tokens[0];
    int pp = 0;
    int kp = 0;
    int MDTP = (int)MDT.size() + 1;
    int KPDTP = (int)KPDT.size() + 1;
    int PNTP = (int)PNT.size() + 1;

    // Parse parameters
    for (size_t i = 1; i < md_tokens.size(); ++i) {
        string param = md_tokens[i];
        auto eqPos = param.find('=');
        if (eqPos != string::npos) {
            // keyword param
            string key = param.substr(0, eqPos);
            string defVal = param.substr(eqPos + 1);
            if (defVal.empty()) defVal = "_";
            kp++;
            PNT.push_back(key);
            KPDT.push_back(make_pair(key, defVal));
        } else {
            // positional param
            pp++;
            PNT.push_back(param);
        }
    }
    MNT.push_back({macro_name, pp, kp, MDTP, KPDTP, PNTP});
}

// Replace &param with (P, index) notation in line tokens, then add to MDT
void substitute_index_notations_for_argument(const vector<string>& line_tokens) {
    if (MNT.empty()) return;
    MNTEntry& last_mnt = MNT.back();
    int total_params = last_mnt.pp + last_mnt.kp;
    int pnt_start = last_mnt.PNTP - 1;

    string output_line;
    for (const string& token : line_tokens) {
        if (!token.empty() && token[0] == '&') {
            int index = -1;
            for (int j = 0; j < total_params; ++j) {
                if (token == PNT[pnt_start + j]) {
                    index = j + 1;  // index starts at 1
                    break;
                }
            }
            if (index != -1)
                output_line += "(P, " + to_string(index) + ") ";
            else
                output_line += token + " ";  // if not found, keep as is
        } else {
            output_line += token + " ";
        }
    }
    output_line = trim(output_line);
    MDT.push_back(output_line);
}

void print_output() {
    cout << "----- OUTPUT -----\n";
    for (const string& line : output_lines) {
        cout << line << "\n";
    }
}

void print_mnt() {
    cout << "----- MNT -----\n";
    cout << "Name\tpp\tkp\tMDTP\tKPDT\tPNTP\n";
    for (const auto& entry : MNT) {
        cout << entry.name << "\t" << entry.pp << "\t" << entry.kp << "\t"
             << entry.MDTP << "\t" << entry.KPDT << "\t" << entry.PNTP << "\n";
    }
}

void print_pnt() {
    cout << "----- PNT -----\n";
    for (const string& p : PNT) {
        cout << p << "\n";
    }
}

void print_kpdt() {
    cout << "----- KPDT -----\n";
    cout << "Name\tDefault\n";
    for (const auto& entry : KPDT) {
        cout << entry.first << "\t" << entry.second << "\n";
    }
}

void print_mdt() {
    cout << "----- MDT -----\n";
    for (const string& line : MDT) {
        cout << line << "\n";
    }
}

int main() {
    string source_location;
    cout << "Source input file: ";
    getline(cin, source_location);

    read_lines(source_location);

    int line_number = 0;
    while (line_number < (int)source_code.size()) {
        auto& line_tokens = source_code[line_number];
        if (find(line_tokens.begin(), line_tokens.end(), "MACRO") != line_tokens.end()) {
            line_number++;
            if (line_number >= (int)source_code.size()) {
                cerr << "Error: MACRO keyword at last line\n";
                return 1;
            }
            auto& macro_definition_tokens = source_code[line_number];
            analyze_macro_definition(macro_definition_tokens);

            line_number++;
            while (line_number < (int)source_code.size()) {
                auto& macro_line_tokens = source_code[line_number];
                substitute_index_notations_for_argument(macro_line_tokens);
                if (find(macro_line_tokens.begin(), macro_line_tokens.end(), "MEND") != macro_line_tokens.end()) {
                    break;
                }
                line_number++;
            }
            line_number++;
        } else {
            // Normal line, add to output
            string out_line;
            for (const string& t : line_tokens) {
                out_line += t + " ";
            }
            output_lines.push_back(trim(out_line));
            line_number++;
        }
    }

    print_output();
    print_mnt();
    print_pnt();
    print_kpdt();
    print_mdt();

    return 0;
}
