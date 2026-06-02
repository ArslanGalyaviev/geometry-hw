// Задача 7.B (Шарик)
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
    return fabsl(x - p.x) < kEps && fabsl(y - p.y) < kEps;
  }
};

ld Length(const Point& a, const Point& b) {
  return sqrtl((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

ld Orient(const Point& a, const Point& b, const Point& c) {
  return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

pair<Point, ld> Circumcircle(const Point& a, const Point& b, const Point& c) {
  ld denom = 2.0L * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
  ld cx = ((a.x * a.x + a.y * a.y) * (b.y - c.y) +
           (b.x * b.x + b.y * b.y) * (c.y - a.y) +
           (c.x * c.x + c.y * c.y) * (a.y - b.y)) /
          denom;
  ld cy = ((a.x * a.x + a.y * a.y) * (c.x - b.x) +
           (b.x * b.x + b.y * b.y) * (a.x - c.x) +
           (c.x * c.x + c.y * c.y) * (b.x - a.x)) /
          denom;
  Point center(cx, cy);
  return {center, Length(center, a)};
}

bool PointInCircumcircle(const Point& a, const Point& b, const Point& c,
                         const Point& d) {
  ld ax = a.x - d.x, ay = a.y - d.y;
  ld bx = b.x - d.x, by = b.y - d.y;
  ld cx = c.x - d.x, cy = c.y - d.y;
  ld det = ax * (by * (cx * cx + cy * cy) - cy * (bx * bx + by * by)) -
           bx * (ay * (cx * cx + cy * cy) - cy * (ax * ax + ay * ay)) +
           cx * (ay * (bx * bx + by * by) - by * (ax * ax + ay * ay));
  return det > kEps;
}

void FlipEdge(int u, int v, int new_pt, const vector<Point>& coords,
              map<pair<int, int>, int>& opp_v) {
  auto it = opp_v.find({u, v});
  if (it == opp_v.end()) return;
  int opp = it->second;

  bool need_flip;
  if (Orient(coords[new_pt], coords[u], coords[v]) > 0) {
    need_flip =
        PointInCircumcircle(coords[new_pt], coords[u], coords[v], coords[opp]);
  } else {
    need_flip =
        PointInCircumcircle(coords[new_pt], coords[v], coords[u], coords[opp]);
  }

  if (!need_flip) return;

  opp_v.erase({v, u});
  opp_v.erase({u, new_pt});
  opp_v.erase({new_pt, v});
  opp_v.erase({u, v});
  opp_v.erase({v, opp});
  opp_v.erase({opp, u});

  opp_v[{new_pt, opp}] = u;
  opp_v[{opp, u}] = new_pt;
  opp_v[{u, new_pt}] = opp;
  opp_v[{opp, new_pt}] = v;
  opp_v[{new_pt, v}] = opp;
  opp_v[{v, opp}] = new_pt;

  FlipEdge(u, opp, new_pt, coords, opp_v);
  FlipEdge(opp, v, new_pt, coords, opp_v);
}

void Solve() {
  int n;
  cin >> n;
  vector<Point> coords(n);
  for (int i = 0; i < n; ++i) {
    cin >> coords[i].x >> coords[i].y;
  }

  vector<int> order(n);
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int i, int j) {
    if (fabsl(coords[i].x - coords[j].x) < kEps)
      return coords[i].y < coords[j].y;
    return coords[i].x < coords[j].x;
  });
  map<pair<int, int>, int> opp_v;
  vector<int> next_on_hull(n), prev_on_hull(n);

  int p0 = order[0], p1 = order[1], p2 = order[2];

  if (Orient(coords[p0], coords[p1], coords[p2]) > 0) {
    next_on_hull[p0] = p1;
    next_on_hull[p1] = p2;
    next_on_hull[p2] = p0;
    prev_on_hull[p0] = p2;
    prev_on_hull[p1] = p0;
    prev_on_hull[p2] = p1;
    opp_v[{p0, p1}] = p2;
    opp_v[{p1, p2}] = p0;
    opp_v[{p2, p0}] = p1;
  } else {
    next_on_hull[p0] = p2;
    next_on_hull[p2] = p1;
    next_on_hull[p1] = p0;
    prev_on_hull[p0] = p1;
    prev_on_hull[p2] = p0;
    prev_on_hull[p1] = p2;
    opp_v[{p0, p2}] = p1;
    opp_v[{p2, p1}] = p0;
    opp_v[{p1, p0}] = p2;
  }

  int last = p2;

  for (int idx = 3; idx < n; ++idx) {
    int cur_pt = order[idx];
    vector<pair<int, int>> edges;

    int walker = last;
    while (Orient(coords[cur_pt], coords[walker],
                  coords[next_on_hull[walker]]) < 0) {
      edges.emplace_back(walker, next_on_hull[walker]);
      walker = next_on_hull[walker];
    }
    int rb = walker;

    walker = last;
    while (Orient(coords[cur_pt], coords[prev_on_hull[walker]],
                  coords[walker]) < 0) {
      edges.emplace_back(prev_on_hull[walker], walker);
      walker = prev_on_hull[walker];
    }
    int lb = walker;

    for (auto& [u, v] : edges) {
      opp_v[{v, u}] = cur_pt;
      opp_v[{u, cur_pt}] = v;
      opp_v[{cur_pt, v}] = u;
    }

    next_on_hull[lb] = cur_pt;
    prev_on_hull[cur_pt] = lb;
    next_on_hull[cur_pt] = rb;
    prev_on_hull[rb] = cur_pt;

    for (auto& [u, v] : edges) {
      FlipEdge(u, v, cur_pt, coords, opp_v);
    }

    last = cur_pt;
  }

  ld answer = 0;
  set<tuple<int, int, int>> processed;

  for (const auto& [edge, third] : opp_v) {
    auto [a, b] = edge;
    int c = third;
    vector<int> tri = {a, b, c};
    sort(tri.begin(), tri.end());
    auto key = make_tuple(tri[0], tri[1], tri[2]);

    if (processed.count(key)) continue;
    processed.insert(key);

    auto [center, radius] =
        Circumcircle(coords[tri[0]], coords[tri[1]], coords[tri[2]]);
    answer = max(answer, radius);
  }

  cout << fixed << setprecision(6) << answer << '\n';
}

int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
