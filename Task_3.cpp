#include <iostream>
#include <string>
using namespace std;

struct Cell {
	int val;
	Cell *nx, *pv;
	Cell(int v) : val(v), nx(nullptr), pv(nullptr) {}
};

// hd = MSB, tl = LSB
class BinList {
	Cell *hd, *tl;
	int cnt;

	void append(int v) {
		Cell *c = new Cell(v);
		if (!tl) hd = tl = c;
		else { tl->nx = c; c->pv = tl; tl = c; }
		cnt++;
	}
	void prepend(int v) {
		Cell *c = new Cell(v);
		if (!hd) hd = tl = c;
		else { c->nx = hd; hd->pv = c; hd = c; }
		cnt++;
	}
	void dropFirst() {
		Cell *tmp = hd;
		hd = hd->nx;
		if (hd) hd->pv = nullptr; else tl = nullptr;
		delete tmp;
		cnt--;
	}
	void wipe() {
		while (hd) dropFirst();
	}
	// strip leading zeros, pad to 8
	void tidy() {
		while (cnt > 1 && hd->val == 0) dropFirst();
		while (cnt % 8 != 0) prepend(0);
	}

public:
	BinList() : hd(nullptr), tl(nullptr), cnt(0) {}
	BinList(const BinList &o) : hd(nullptr), tl(nullptr), cnt(0) {
		for (Cell *c = o.hd; c; c = c->nx) append(c->val);
	}
	BinList &operator=(const BinList &o) {
		if (this != &o) {
			wipe();
			for (Cell *c = o.hd; c; c = c->nx) append(c->val);
		}
		return *this;
	}
	~BinList() { wipe(); }

	bool empty() const { return cnt == 0; }

	// store as 8-bit groups
	bool load(const string &s) {
		if (s.empty()) return false;
		for (char ch : s) if (ch != '0' && ch != '1') return false;
		wipe();
		for (char ch : s) append(ch - '0');
		while (cnt % 8 != 0) prepend(0);
		return true;
	}

	void print() const {
		if (!hd) { cout << "(empty)"; return; }
		int k = 0;
		for (Cell *c = hd; c; c = c->nx, k++) {
			if (k > 0 && k % 8 == 0) cout << " ";
			cout << c->val;
		}
	}

	// 1's comp
	void flipBits() {
		for (Cell *c = hd; c; c = c->nx) c->val ^= 1;
	}

	// 2's comp
	void negate() {
		flipBits();
		int cy = 1;
		for (Cell *c = tl; c && cy; c = c->pv) {
			if (c->val == 1) c->val = 0;
			else { c->val = 1; cy = 0; }
		}
	}

	// addition
	static BinList plus(const BinList &a, const BinList &b) {
		BinList out;
		Cell *pa = a.tl, *pb = b.tl;
		int cy = 0;
		while (pa || pb || cy) {
			int tot = cy;
			if (pa) { tot += pa->val; pa = pa->pv; }
			if (pb) { tot += pb->val; pb = pb->pv; }
			out.prepend(tot % 2);
			cy = tot / 2;
		}
		if (out.empty()) out.prepend(0);
		out.tidy();
		return out;
	}

	// multiplication (add + shift)
	static BinList times(const BinList &a, const BinList &b) {
		BinList acc;
		acc.load("0");
		BinList sh(a);                       // shifted copy of a
		for (Cell *c = b.tl; c; c = c->pv) { // LSB to MSB
			if (c->val == 1) acc = plus(acc, sh);
			sh.append(0);                    // shift left
		}
		acc.tidy();
		return acc;
	}

	// to decimal
	unsigned long long toDec() const {
		unsigned long long r = 0;
		for (Cell *c = hd; c; c = c->nx) r = r * 2 + c->val;
		return r;
	}
	int len() const { return cnt; }
};

void getNum(BinList &x, const string &tag) {
	string s;
	cout << "Enter binary number " << tag << ": ";
	cin >> s;
	if (!x.load(s)) cout << "Invalid binary input.\n";
	else { cout << tag << " = "; x.print(); cout << "\n"; }
}

int main() {
	BinList A, B;
	int sel;
	do {
		cout << "\n===== BINARY DLL MENU =====\n"
			<< "1. Store number A\n2. Store number B\n3. 1's Complement of A\n"
			<< "4. 2's Complement of A\n5. A + B\n6. A * B\n"
			<< "7. Convert A and B to Decimal\n8. Display A and B\n0. Exit\nChoice: ";
		if (!(cin >> sel)) break;

		if (sel == 1) getNum(A, "A");
		else if (sel == 2) getNum(B, "B");
		else if (sel == 3 || sel == 4) {
			if (A.empty()) { cout << "Store A first.\n"; continue; }
			cout << "A       = "; A.print(); cout << "\n";
			if (sel == 3) A.flipBits(); else A.negate();
			cout << (sel == 3 ? "1's comp = " : "2's comp = ");
			A.print(); cout << "\n(A has been updated)\n";
		}
		else if (sel == 5 || sel == 6) {
			if (A.empty() || B.empty()) { cout << "Store both A and B first.\n"; continue; }
			BinList R = (sel == 5) ? BinList::plus(A, B) : BinList::times(A, B);
			cout << "A = "; A.print(); cout << "\nB = "; B.print();
			cout << "\nResult = "; R.print();
			cout << "\nDecimal = " << R.toDec() << "\n";
		}
		else if (sel == 7) {
			if (!A.empty()) { A.print(); cout << " = " << A.toDec() << "\n"; }
			if (!B.empty()) { B.print(); cout << " = " << B.toDec() << "\n"; }
			if (A.empty() && B.empty()) cout << "Nothing stored.\n";
		}
		else if (sel == 8) {
			cout << "A = "; A.print(); cout << "\nB = "; B.print(); cout << "\n";
		}
		else if (sel != 0) cout << "Invalid choice.\n";
	} while (sel != 0);

	return 0;
}