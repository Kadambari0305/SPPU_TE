#include <iostream>
#include <algorithm>
using namespace std;

struct Job {
    string name;
    int deadline;
    int profit;
};

bool compare(Job a, Job b) {
    return a.profit > b.profit;
}

int main() {
    int n;
    cout << "Enter number of projects: ";
    cin >> n;

    Job jobs[n];

    for(int i=0;i<n;i++) {
        cout << "\nProject Name: ";
        cin >> jobs[i].name;
        cout << "Deadline: ";
        cin >> jobs[i].deadline;
        cout << "Profit: ";
        cin >> jobs[i].profit;
    }

    sort(jobs, jobs+n, compare);

    int maxDeadline = 0;
    for(int i=0;i<n;i++)
        maxDeadline = max(maxDeadline, jobs[i].deadline);

    string result[maxDeadline];
    bool slot[maxDeadline] = {false};
    int totalProfit = 0;

    for(int i=0;i<n;i++) {
        for(int j=jobs[i].deadline-1;j>=0;j--) {
            if(!slot[j]) {
                slot[j] = true;
                result[j] = jobs[i].name;
                totalProfit += jobs[i].profit;
                break;
            }
        }
    }

    cout << "\nScheduled Projects:\n";
    for(int i=0;i<maxDeadline;i++)
        if(slot[i])
            cout << result[i] << " ";

    cout << "\nTotal Profit Earned: " << totalProfit;

    return 0;
}
/*-------
company is sch project tp max my profit
Enter number of projects: 4

Project Name: A
Deadline: 2
Profit: 100

Project Name: B
Deadline: 1
Profit: 19

Project Name: C
Deadline: 2
Profit: 27

Project Name: D
Deadline: 1
Profit: 25
Let:

n = number of jobs

d = max deadline

time Complexity

Sorting jobs by profit → O(n log n)

Slot checking loop → O(n × d)

So total:

O(n log n + n·d)


If deadlines ≈ n:

O(n²)

 Space Complexity

Job array → O(n)

Slot array → O(d)
*/
