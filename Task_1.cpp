// playlist prog
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Track {
    int uid;
    string title;
    int mn, sc;
    Track *nxt, *prv;
    Track(int u, const string &t, int m, int s)
       : uid(u), title(t), mn(m), sc(s), nxt(nullptr), prv(nullptr) {}
};

class MusicList {
    Track *first, *last, *cur;   // cur = playing now

    Track *locate(int uid) const {
      for (Track *t = first; t; t = t->nxt)
        if (t->uid == uid) return t;
      return nullptr;
    }
    static void show(const Track *t) {
        cout << "[" << t->uid << "] " << t->title << " - "
           << t->mn << ":" << (t->sc < 10 ? "0" : "") << t->sc << "\n";
    }

public:
    MusicList() : first(nullptr), last(nullptr), cur(nullptr) {}
    ~MusicList() {
        while (first) {
          Track *tmp = first;
          first = first->nxt;
          delete tmp;
        }
    }
    MusicList(const MusicList &) = delete;
    MusicList &operator=(const MusicList &) = delete;

    // add at end
    void add(int uid, const string &title, int m, int s) {
        if (locate(uid)) {
            cout << "A song with ID " << uid << " already exists.\n";
            return;
        }
        Track *nw = new Track(uid, title, m, s);
        if (!first) first = last = nw;
        else {
          last->nxt = nw;
          nw->prv = last;
          last = nw;
        }
        cout << "Song added.\n";
    }

    // delete by id
    void remove(int uid) {
        Track *t = locate(uid);
        if (!t) { cout << "Song not found.\n"; return; }
        if (cur == t) cur = t->nxt ? t->nxt : t->prv;
        if (t->prv) t->prv->nxt = t->nxt; else first = t->nxt;
        if (t->nxt) t->nxt->prv = t->prv; else last = t->prv;
        delete t;
          cout << "Song deleted.\n";
    }

    // print fwd
    void showFwd() const {
        if (!first) { cout << "Playlist is empty.\n"; return; }
        cout << "--- Playlist (Forward) ---\n";
        for (Track *t = first; t; t = t->nxt) show(t);
    }

    // print bwd
  void showBwd() const {
        if (!last) { cout << "Playlist is empty.\n"; return; }
        cout << "--- Playlist (Backward) ---\n";
        for (Track *t = last; t; t = t->prv) show(t);
    }

    // find
    void find(int uid) const {
        Track *t = locate(uid);
        if (!t) { cout << "Song not found.\n"; return; }
        cout << "Found: ";
        show(t);
    }

    // next / prev
    void next() {
        if (!first) { cout << "Playlist is empty.\n"; return; }
        if (!cur) cur = first;
        else if (cur->nxt) cur = cur->nxt;
        else { cout << "Already at the last song.\n"; }
        cout << "Now playing: ";
        show(cur);
    }
    void prev() {
      if (!first) { cout << "Playlist is empty.\n"; return; }
      if (!cur) cur = last;
      else if (cur->prv) cur = cur->prv;
      else { cout << "Already at the first song.\n"; }
      cout << "Now playing: ";
      show(cur);
    }

    // reverse
    void flip() {
        if (!first) { cout << "Playlist is empty.\n"; return; }
        Track *t = first;
        while (t) {
            Track *sv = t->nxt;
            t->nxt = t->prv;
            t->prv = sv;
            t = sv;
        }
        Track *sw = first;
        first = last;
          last = sw;
        cout << "Playlist reversed.\n";
    }
};

int main() {
    MusicList ml;
    int opt;
    do {
        cout << "\n===== PLAYLIST MENU =====\n"
           << "1. Add Song\n2. Delete Song\n3. Display Forward\n4. Display Backward\n"
           << "5. Search Song\n6. Play Next\n7. Play Previous\n8. Reverse Playlist\n0. Exit\n"
           << "Choice: ";
        if (!(cin >> opt)) break;

        if (opt == 1) {
            int uid, m, s;
            string title;
            cout << "Song ID: "; cin >> uid;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Song Name: "; getline(cin, title);
            cout << "Duration minutes: "; cin >> m;
            cout << "Duration seconds: "; cin >> s;
            if (m < 0 || s < 0 || s > 59) cout << "Invalid duration.\n";
            else ml.add(uid, title, m, s);
        } else if (opt == 2) {
           int uid; cout << "Song ID to delete: "; cin >> uid; ml.remove(uid);
        } else if (opt == 3) ml.showFwd();
        else if (opt == 4) ml.showBwd();
        else if (opt == 5) {
            int uid; cout << "Song ID to search: "; cin >> uid; ml.find(uid);
        } else if (opt == 6) ml.next();
        else if (opt == 7) ml.prev();
          else if (opt == 8) ml.flip();
        else if (opt != 0) cout << "Invalid choice.\n";
    } while (opt != 0);

    return 0;
}