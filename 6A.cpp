// Задача 6.A (Возможно глупая ссора)
#include <bits/stdc++.h>

using namespace std;

using ll = long long;
using ld = long double;

const ld kEps = 1e-12;

struct Point {
  ld x, y;

  Point() : x(0), y(0) {}
  Point(ld new_x, ld new_y) : x(new_x), y(new_y) {}

  Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
  Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
  ld operator*(const Point& p) const { return x * p.x + y * p.y; }
  ld operator%(const Point& p) const { return x * p.y - y * p.x; }
  bool operator==(const Point& p) const {
    return std::abs(x - p.x) < kEps && std::abs(y - p.y) < kEps;
  }
};

vector<Point> MinkSum(vector<Point>& a, vector<Point>& b) {
  int n = a.size(), m = b.size();

  auto find_start = [&](const vector<Point>& p) {
    int idx = 0;
    for (int i = 1; i < (int)p.size(); ++i) {
      if (p[i].y < p[idx].y - kEps ||
          (fabs(p[i].y - p[idx].y) < kEps && p[i].x < p[idx].x - kEps))
        idx = i;
    }
    return idx;
  };

  int i = find_start(a), j = find_start(b);
  vector<Point> res;
  res.push_back(a[i] + b[j]);

  int cnt_a = 0, cnt_b = 0;
  while (cnt_a < n || cnt_b < m) {
    Point edge_a = a[(i + 1) % n] - a[i];
    Point edge_b = b[(j + 1) % m] - b[j];
    ld cross = edge_a % edge_b;

    if (cnt_a < n && (cnt_b == m || cross > kEps)) {
      res.push_back(res.back() + edge_a);
      i = (i + 1) % n;
      cnt_a++;
    } else if (cnt_b < m && (cnt_a == n || cross < -kEps)) {
      res.push_back(res.back() + edge_b);
      j = (j + 1) % m;
      cnt_b++;
    } else {
      if (cnt_a < n) {
        res.push_back(res.back() + edge_a);
        i = (i + 1) % n;
        cnt_a++;
      }
      if (cnt_b < m) {
        res.push_back(res.back() + edge_b);
        j = (j + 1) % m;
        cnt_b++;
      }
    }
  }
  if (res.size() > 1 && res.back() == res.front()) {
    res.pop_back();
  }
  return res;
}

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

bool PolyInter(vector<Point>& p, vector<Point>& q) {
  reverse(p.begin(), p.end());
  reverse(q.begin(), q.end());
  for (auto& point : q) {
    point.x *= -1;
    point.y *= -1;
  }
  auto mink_sum_res = MinkSum(p, q);
  Point start = Point(0, 0);
  return IsInPoly(mink_sum_res, start);
}

void Solve() {
  int n;
  cin >> n;
  vector<Point> poly1(n);
  for (int i = 0; i < n; ++i) {
    cin >> poly1[i].x >> poly1[i].y;
  }
  int m;
  cin >> m;
  vector<Point> poly2(m);
  for (int i = 0; i < m; ++i) {
    cin >> poly2[i].x >> poly2[i].y;
  }
  cout << (PolyInter(poly1, poly2) ? "YES" : "NO") << '\n';
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
