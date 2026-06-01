// Задача 3.B (Точка в многоугольнике)
#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;

struct Point {
  ll x, y;

  Point() : x(0), y(0) {}
  Point(ll new_x, ll new_y) : x(new_x), y(new_y) {}

  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  ll operator*(const Point& p) const { return x * p.x + y * p.y; }
  ll operator%(const Point& p) const { return x * p.y - y * p.x; }
};

bool IsInPoly(const vector<Point>& poly, const Point& p) {
  int n = (int)poly.size();
  ld sum_angles = 0.0;
  for (int i = 0; i < n; ++i) {
    Point cur = poly[i] - p;
    Point next = poly[(i + 1) % n] - p;
    sum_angles += atan2(cur % next, cur * next);
  }
  return !(abs(sum_angles) < 0.5);
}

void Solve() {
  int n;
  cin >> n;
  Point p;
  cin >> p.x >> p.y;
  vector<Point> poly(n);
  for (int i = 0; i < n; ++i) {
    cin >> poly[i].x >> poly[i].y;
  }
  cout << (IsInPoly(poly, p) ? "YES" : "NO") << '\n';
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
