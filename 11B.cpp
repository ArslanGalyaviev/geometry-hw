// Задача 11.B (Ближайшие самокаты)
#include <bits/stdc++.h>

using namespace std;

using ll = long long;

struct Point {
  int x, y;
  Point() {}
  Point(int new_x, int new_y) : x(new_x), y(new_y) {}
};

struct Node {
  Point p;
  int left, right;
  Node() : left(-1), right(-1) {}
  explicit Node(const Point& new_p) : p(new_p), left(-1), right(-1) {}
};

void Solve() {
  int n;
  cin >> n;
  vector<Point> pts(n);
  for (int i = 0; i < n; ++i) {
    cin >> pts[i].x >> pts[i].y;
  }

  vector<Node> tree;
  tree.reserve(n);
  ll best_dist1 = LLONG_MAX;
  ll best_dist2 = LLONG_MAX;
  Point best1;
  Point best2;

  auto build = [&](auto& self, int l, int r, int axis) -> int {
    if (l >= r) {
      return -1;
    }
    int mid = (l + r) / 2;
    nth_element(pts.begin() + l, pts.begin() + mid, pts.begin() + r,
                [axis](const Point& a, const Point& b) {
                  if (axis == 0) {
                    return a.x < b.x;
                  } else {
                    return a.y < b.y;
                  }
                });
    int id = tree.size();
    tree.emplace_back(pts[mid]);
    tree[id].left = self(self, l, mid, 1 - axis);
    tree[id].right = self(self, mid + 1, r, 1 - axis);
    return id;
  };

  auto query = [&](auto& self, int u, int qx, int qy, int axis) -> void {
    if (u == -1) {
      return;
    }

    ll dx = qx - tree[u].p.x;
    ll dy = qy - tree[u].p.y;
    ll dist = dx * dx + dy * dy;

    if (dist < best_dist1) {
      best_dist2 = best_dist1;
      best2 = best1;
      best_dist1 = dist;
      best1 = tree[u].p;
    } else if (dist < best_dist2) {
      best_dist2 = dist;
      best2 = tree[u].p;
    }

    int first, second;
    if (axis == 0) {
      if (qx < tree[u].p.x) {
        first = tree[u].left;
        second = tree[u].right;
      } else {
        first = tree[u].right;
        second = tree[u].left;
      }
    } else {
      if (qy < tree[u].p.y) {
        first = tree[u].left;
        second = tree[u].right;
      } else {
        first = tree[u].right;
        second = tree[u].left;
      }
    }

    self(self, first, qx, qy, 1 - axis);

    ll diff = (axis == 0) ? (qx - tree[u].p.x) : (qy - tree[u].p.y);
    if (diff * diff < best_dist2) {
      self(self, second, qx, qy, 1 - axis);
    }
  };

  int root = build(build, 0, n, 0);

  int q;
  cin >> q;
  while (q--) {
    int qx, qy;
    cin >> qx >> qy;

    best_dist1 = best_dist2 = LLONG_MAX;
    query(query, root, qx, qy, 0);

    cout << best1.x << ' ' << best1.y << ' ' << best2.x << ' ' << best2.y
         << '\n';
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
