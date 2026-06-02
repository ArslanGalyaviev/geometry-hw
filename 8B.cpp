// Задача 8.B (ЛЭП)
#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct Point {
  ll x, y;
  Point() {}
  Point(ll new_x, ll new_y) : x(new_x), y(new_y) {}
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  ll operator*(const Point& p) const { return x * p.x + y * p.y; }
  ll operator%(const Point& p) const { return x * p.y - y * p.x; }
};

ll Orient(Point a, Point b, Point c) { return (b - a) % (c - a); }

long double InCircle(Point a, Point b, Point c, Point p) {
  long double ax = a.x - p.x, ay = a.y - p.y;
  long double bx = b.x - p.x, by = b.y - p.y;
  long double cx = c.x - p.x, cy = c.y - p.y;
  long double a2 = ax * ax + ay * ay;
  long double b2 = bx * bx + by * by;
  long double c2 = cx * cx + cy * cy;
  return ax * (by * c2 - b2 * cy) - ay * (bx * c2 - b2 * cx) +
         a2 * (bx * cy - by * cx);
}

void PrintColinear(vector<Point>& pts, int n) {
  vector<int> ord(n);
  for (int i = 0; i < n; i++) ord[i] = i;
  sort(ord.begin(), ord.end(), [&](int i, int j) {
    return make_pair(pts[i].x, pts[i].y) < make_pair(pts[j].x, pts[j].y);
  });
  cout << n - 1 << '\n';
  for (int i = 0; i + 1 < n; i++) {
    cout << ord[i] + 1 << ' ' << ord[i + 1] + 1 << '\n';
  }
}

struct Tr {
  int a, b, c;
};

vector<Tr> BuildDelaunay(vector<Point>& pts, int n) {
  const ll M = 4000000;
  pts.push_back({-M, -M});
  pts.push_back({M, -M});
  pts.push_back({0, M});

  vector<Tr> tris = {{n, n + 1, n + 2}};

  for (int i = 0; i < n; i++) {
    vector<bool> bad(tris.size(), false);
    map<pair<int, int>, int> edge_cnt;

    for (int t = 0; t < (int)tris.size(); t++) {
      int x = tris[t].a, y = tris[t].b, z = tris[t].c;
      if (InCircle(pts[x], pts[y], pts[z], pts[i]) > 1e-12) {
        bad[t] = true;
        int edges[3][2] = {{x, y}, {y, z}, {z, x}};
        for (int j = 0; j < 3; j++) {
          int u = edges[j][0], v = edges[j][1];
          if (u > v) swap(u, v);
          edge_cnt[{u, v}]++;
        }
      }
    }

    vector<Tr> next_tris;
    for (int t = 0; t < (int)tris.size(); t++) {
      if (!bad[t]) {
        next_tris.push_back(tris[t]);
      }
    }

    for (const auto& p : edge_cnt) {
      if (p.second == 1) {
        int u = p.first.first, v = p.first.second;
        if (Orient(pts[u], pts[v], pts[i]) > 0) {
          next_tris.push_back({u, v, i});
        } else {
          next_tris.push_back({v, u, i});
        }
      }
    }
    tris.swap(next_tris);
  }

  vector<Tr> good;
  for (auto& t : tris) {
    if (t.a < n && t.b < n && t.c < n) {
      good.push_back(t);
    }
  }
  return good;
}

bool AllAcute(const vector<Point>& pts, int i, int j, int n) {
  for (int k = 0; k < n; k++) {
    if (k == i || k == j) {
      continue;
    }
    Point ca = pts[i] - pts[k];
    Point cb = pts[j] - pts[k];
    if (ca * cb <= 0) return false;
  }
  return true;
}

vector<pair<int, int>> CheckEdges(const vector<Tr>& tris,
                                  const vector<Point>& pts, int n) {
  set<pair<int, int>> edges;
  for (auto& t : tris) {
    int e[3][2] = {{t.a, t.b}, {t.b, t.c}, {t.c, t.a}};
    for (int j = 0; j < 3; j++) {
      int u = e[j][0], v = e[j][1];
      if (u > v) swap(u, v);
      edges.insert({u, v});
    }
  }

  vector<pair<int, int>> ans;
  for (const auto& e : edges) {
    if (AllAcute(pts, e.first, e.second, n)) {
      ans.push_back({e.first + 1, e.second + 1});
    }
  }
  return ans;
}

void Solve() {
  int n;
  cin >> n;
  vector<Point> pts(n);
  for (int i = 0; i < n; i++) {
    cin >> pts[i].x >> pts[i].y;
  }

  if (n >= 3) {
    bool colinear = true;
    for (int i = 2; i < n; i++)
      if (Orient(pts[0], pts[1], pts[i]) != 0) {
        colinear = false;
        break;
      }
    if (colinear) {
      PrintColinear(pts, n);
      return;
    }
  }

  vector<Tr> tris = BuildDelaunay(pts, n);
  vector<pair<int, int>> res = CheckEdges(tris, pts, n);

  cout << res.size() << '\n';
  for (const auto& p : res) {
    cout << p.first << ' ' << p.second << '\n';
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
