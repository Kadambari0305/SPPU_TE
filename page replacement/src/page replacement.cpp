#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <climits>   // For INT_MAX
using namespace std;

// FIFO Page Replacement
int fifoPageReplacement(const vector<int>& pages, int capacity) {
    unordered_set<int> s;
    queue<int> indexes;
    int page_faults = 0;

    for (int page : pages) {
        if (s.size() < (size_t)capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                indexes.push(page);
                page_faults++;
            }
        } else {
            if (s.find(page) == s.end()) {
                int val = indexes.front();
                indexes.pop();
                s.erase(val);

                s.insert(page);
                indexes.push(page);
                page_faults++;
            }
        }
    }
    return page_faults;
}

// LRU Page Replacement
int lruPageReplacement(const vector<int>& pages, int capacity) {
    unordered_set<int> s;
    unordered_map<int, int> indexes; // Page -> last used index
    int page_faults = 0;

    for (int i = 0; i < (int)pages.size(); i++) {
        int page = pages[i];
        if (s.size() < (size_t)capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
            }
            indexes[page] = i;
        } else {
            if (s.find(page) == s.end()) {
                int lru = INT_MAX;
                int val = -1;
                for (auto p : s) {
                    if (indexes[p] < lru) {
                        lru = indexes[p];
                        val = p;
                    }
                }
                s.erase(val);
                s.insert(page);
                page_faults++;
            }
            indexes[page] = i;
        }
    }
    return page_faults;
}

// MRU Page Replacement
int mruPageReplacement(const vector<int>& pages, int capacity) {
    unordered_set<int> s;
    unordered_map<int, int> indexes; // Page -> last used index
    int page_faults = 0;

    for (int i = 0; i < (int)pages.size(); i++) {
        int page = pages[i];
        if (s.size() < (size_t)capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
            }
            indexes[page] = i;
        } else {
            if (s.find(page) == s.end()) {
                int mru = -1;
                int val = -1;
                for (auto p : s) {
                    if (indexes[p] > mru) {
                        mru = indexes[p];
                        val = p;
                    }
                }
                s.erase(val);
                s.insert(page);
                page_faults++;
            }
            indexes[page] = i;
        }
    }
    return page_faults;
}

// Optimal Page Replacement
int optimalPageReplacement(const vector<int>& pages, int capacity) {
    unordered_set<int> s;
    int page_faults = 0;
    int n = pages.size();

    for (int i = 0; i < n; i++) {
        int page = pages[i];
        if (s.size() < (size_t)capacity) {
            if (s.find(page) == s.end()) {
                s.insert(page);
                page_faults++;
            }
        } else {
            if (s.find(page) == s.end()) {
                int farthest = i;
                int val = -1;

                for (auto p : s) {
                    int j;
                    for (j = i + 1; j < n; j++) {
                        if (pages[j] == p) break;
                    }
                    if (j == n) {
                        val = p;
                        break;
                    }
                    if (j > farthest) {
                        farthest = j;
                        val = p;
                    }
                }

                if (val == -1) {
                    val = *s.begin();
                }

                s.erase(val);
                s.insert(page);
                page_faults++;
            }
        }
    }
    return page_faults;
}

int main() {
    int num_pages, capacity;

    cout << "Enter number of pages: ";
    cin >> num_pages;

    vector<int> pages(num_pages);
    cout << "Enter the page reference string (space-separated): ";
    for (int i = 0; i < num_pages; i++) {
        cin >> pages[i];
    }

    cout << "Enter capacity of page frames: ";
    cin >> capacity;

    cout << "\nRunning page replacement algorithms...\n\n";

    cout << "FIFO Page Faults: " << fifoPageReplacement(pages, capacity) << "\n";
    cout << "LRU Page Faults: " << lruPageReplacement(pages, capacity) << "\n";
    cout << "MRU Page Faults: " << mruPageReplacement(pages, capacity) << "\n";
    cout << "Optimal Page Faults: " << optimalPageReplacement(pages, capacity) << "\n";

    return 0;
}
