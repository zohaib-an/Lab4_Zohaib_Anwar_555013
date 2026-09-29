#include <iostream>
using namespace std;

struct Member {
    int num;
    Member *link;
    Member(int x) : num(x), link(nullptr) {}
};

// build ring of n members
Member *buildRing(int n) {
    Member *start = new Member(1);
    Member *tail = start;
    for (int i = 2; i <= n; i++) {
      tail->link = new Member(i);
      tail = tail->link;
    }
    tail->link = start;          // close ring
    return start;
}

// kill every step-th member, return winner
int solve(Member *start, int step) {
    Member *now = start;
    Member *before = start;
    while (before->link != start) before = before->link;   // last node

    cout << "Elimination order: ";
    while (now->link != now) {                 // until 1 left
       for (int i = 1; i < step; i++) {        // step-1 moves
            before = now;
            now = now->link;
        }
        cout << now->num << " ";
        before->link = now->link;              // unlink
        Member *gone = now;
          now = now->link;
        delete gone;
    }
    cout << "\n";

    int winner = now->num;
    delete now;                                // free last
    return winner;
}

int main() {
    int total, step;
    cout << "Enter number of people (N): ";
    cin >> total;
    cout << "Enter step count (k): ";
    cin >> step;

    if (total <= 0 || step <= 0) {
      cout << "N and k must be positive integers.\n";
      return 1;
    }

    Member *ring = buildRing(total);
    int winner = solve(ring, step);
    cout << "Survivor: Person " << winner << "\n";
    return 0;
}