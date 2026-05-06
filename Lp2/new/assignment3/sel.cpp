#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter number of products: ";
    cin >> n;

    int price[n];
    string name[n];

    cout << "Enter product name and price:\n";
    for(int i = 0; i < n; i++) {
        cin >> name[i] >> price[i];
    }

    int ch;
    cout << "\n1. Ascending order\n2. Descending order\nEnter choice: ";
    cin >> ch;

    switch (ch) {
        case 1:
            // Selection Sort Ascending
            for(int i = 0; i < n - 1; i++) {
                int minIndex = i;
                for(int j = i + 1; j < n; j++) {
                    if(price[j] < price[minIndex])
                        minIndex = j;
                }
                swap(price[i], price[minIndex]);
                swap(name[i], name[minIndex]);
            }

            cout << "\nProducts in Ascending Order:\n";
            break;

        case 2:
            // Selection Sort Descending
            for(int i = 0; i < n - 1; i++) {
                int maxIndex = i;
                for(int j = i + 1; j < n; j++) {
                    if(price[j] > price[maxIndex])
                        maxIndex = j;
                }
                swap(price[i], price[maxIndex]);
                swap(name[i], name[maxIndex]);
            }

            cout << "\nProducts in Descending Order:\n";
            break;

        default:
            cout << "Invalid choice";
            return 0;
    }

    for(int i = 0; i < n; i++) {
        cout << name[i] << " -> " << price[i] << endl;
    }

    return 0;
}
