// Задача 9.A (Друзья Оушена)
#include <bits/stdc++.h>

using namespace std;

using ld = long double;

struct Point {
  ld x, y;
  Point() : x(0), y(0) {}
  Point(ld new_x, ld new_y) : x(new_x), y(new_y) {}

  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  Point operator*(ld k) const { return Point(x * k, y * k); }
  ld Dot(const Point& p) const { return x * p.x + y * p.y; }
  ld Cross(const Point& p) const { return x * p.y - y * p.x; }
  ld Len2() const { return x * x + y * y; }
};

const ld kEps = 1e-9;

bool Closer(const Point& p, const Point& a, const Point& b) {
  return (p - a).Len2() <= (p - b).Len2() + kEps;
}

Point IntersectBisector(const Point& p1, const Point& p2, const Point& a,
                        const Point& b) {
  Point dir = b - a;
  ld c = (b.Len2() - a.Len2()) / 2.0;
  Point seg = p2 - p1;
  ld denom = seg.Dot(dir);
  if (abs(denom) < kEps) {
    return Point((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
  }
  ld t = (c - p1.Dot(dir)) / denom;
  t = max((ld)0.0, min((ld)1.0, t));
  return p1 + seg * t;
}

vector<Point> ClipPolygon(const vector<Point>& poly, const Point& a,
                          const Point& b) {
  vector<Point> res;
  int n = poly.size();
  Point S = poly.back();
  bool S_in = Closer(S, a, b);
  for (int i = 0; i < n; ++i) {
    Point E = poly[i];
    bool E_in = Closer(E, a, b);
    if (E_in) {
      if (!S_in) {
        res.push_back(IntersectBisector(S, E, a, b));
      }
      res.push_back(E);
    } else if (S_in) {
      res.push_back(IntersectBisector(S, E, a, b));
    }
    S = E;
    S_in = E_in;
  }
  return res;
}

ld Area(const vector<Point>& poly) {
  ld s = 0;
  int n = poly.size();
  for (int i = 0; i < n; ++i) {
    s += poly[i].Cross(poly[(i + 1) % n]);
  }
  return fabs(s) / 2.0;
}

vector<Point> Clean(const vector<Point>& poly) {
  vector<Point> res;
  for (const auto& p : poly) {
    if (res.empty() || (p - res.back()).Len2() > kEps * kEps) {
      res.push_back(p);
    }
  }
  if (res.size() > 1 && (res.front() - res.back()).Len2() < kEps * kEps) {
    res.pop_back();
  }
  return res;
}

void Solve() {
  int n;
  ld w, h;
  cin >> n >> w >> h;
  vector<Point> sites(n);
  for (int i = 0; i < n; ++i) {
    cin >> sites[i].x >> sites[i].y;
  }

  for (int i = 0; i < n; ++i) {
    vector<Point> poly = {Point(0, 0), Point(w, 0), Point(w, h), Point(0, h)};
    for (int j = 0; j < n; ++j) {
      if (i == j) {
        continue;
      }
      if ((sites[i] - sites[j]).Len2() < kEps * kEps) {
        continue;
      }
      poly = ClipPolygon(poly, sites[i], sites[j]);
      poly = Clean(poly);
      if (poly.size() < 3) {
        break;
      }
    }
    cout << fixed << setprecision(10) << Area(poly) << '\n';
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
