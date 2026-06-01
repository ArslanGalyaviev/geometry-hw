// Задача 3.C (Диаметр точек)
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
  ll Len2() const { return *this * *this; }
  bool operator<(const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }
};

vector<Point> ConvexHull(vector<Point>& points) {
  int n = (int)points.size();
  sort(points.begin(), points.end());
  vector<Point> ans;
  int ans_size = 0;
  for (int i = 0; i < n; ++i) {
    while (ans_size >= 2 && (ans[ans_size - 1] - ans[ans_size - 2]) %
                                    (points[i] - ans[ans_size - 2]) <=
                                0) {
      ans.pop_back();
      ans_size--;
    }
    ans.emplace_back(points[i]);
    ans_size++;
  }
  int lower_size = ans_size;
  for (int i = n - 2; i > -1; --i) {
    while (ans_size > lower_size && (ans[ans_size - 1] - ans[ans_size - 2]) %
                                            (points[i] - ans[ans_size - 2]) <=
                                        0) {
      ans.pop_back();
      ans_size--;
    }
    ans.emplace_back(points[i]);
    ans_size++;
  }
  if (ans_size > 1) ans.pop_back();
  return ans;
}

ll GetMaxDist(const vector<Point>& points) {
  int n = (int)points.size();
  ll ans = 0;
  int j = 1;
  for (int i = 0; i < n; ++i) {
    int next_i = (i + 1) % n;
    while (true) {
      ll cur = abs((points[next_i] - points[i]) % (points[j] - points[i]));
      ll nxt =
          abs((points[next_i] - points[i]) % (points[(j + 1) % n] - points[i]));
      if (nxt > cur) {
        j = (j + 1) % n;
      } else {
        break;
      }
    }
    ans = max(ans, (points[i] - points[j]).Len2());
    ans = max(ans, (points[next_i] - points[(j + 1) % n]).Len2());
  }
  return ans;
}

void Solve() {
  int n;
  cin >> n;
  vector<Point> poly(n);
  for (int i = 0; i < n; ++i) {
    cin >> poly[i].x >> poly[i].y;
  }
  vector<Point> hull = ConvexHull(poly);
  ll ans = GetMaxDist(hull);
  cout << ans << '\n';
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
