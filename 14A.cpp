// Задача 14.A (Заповедники)
#include <bits/stdc++.h>

using namespace std;

struct Rect {
  int id, x1, y1, x2, y2;
};

struct Node {
  int x1, y1, x2, y2;
  vector<int> children;
  vector<int> rects;
  bool is_leaf;

  Node() : x1(1e9), y1(1e9), x2(-1e9), y2(-1e9), is_leaf(false) {}
};

void Solve() {
  int n;
  cin >> n;

  vector<Rect> rects(n);
  for (int i = 0; i < n; ++i) {
    rects[i].id = i + 1;
    cin >> rects[i].x1 >> rects[i].y1 >> rects[i].x2 >> rects[i].y2;
  }

  vector<Node> tree;

  auto build = [&](auto& self, vector<int>& indices, int depth) -> int {
    int node_idx = tree.size();
    tree.emplace_back();

    if (indices.size() <= 4) {
      tree[node_idx].is_leaf = true;
      tree[node_idx].rects = indices;
      tree[node_idx].x1 = tree[node_idx].y1 = 1e9;
      tree[node_idx].x2 = tree[node_idx].y2 = -1e9;

      for (int idx : indices) {
        const Rect& r = rects[idx];
        tree[node_idx].x1 = min(tree[node_idx].x1, r.x1);
        tree[node_idx].y1 = min(tree[node_idx].y1, r.y1);
        tree[node_idx].x2 = max(tree[node_idx].x2, r.x2);
        tree[node_idx].y2 = max(tree[node_idx].y2, r.y2);
      }
    } else {
      tree[node_idx].is_leaf = false;
      bool use_x = (depth % 2 == 0);

      sort(indices.begin(), indices.end(), [use_x, &rects](int i, int j) {
        double ci = use_x ? (rects[i].x1 + rects[i].x2) / 2.0
                          : (rects[i].y1 + rects[i].y2) / 2.0;
        double cj = use_x ? (rects[j].x1 + rects[j].x2) / 2.0
                          : (rects[j].y1 + rects[j].y2) / 2.0;
        return ci < cj;
      });

      int total = indices.size();
      int part_size = (total + 3) / 4;
      for (int part = 0; part < 4; ++part) {
        int l = part * part_size;
        int r = min(l + part_size, total);
        if (l >= r) {
          break;
        }
        vector<int> sub(indices.begin() + l, indices.begin() + r);
        int child = self(self, sub, depth + 1);
        tree[node_idx].children.push_back(child);
      }

      tree[node_idx].x1 = tree[node_idx].y1 = 1e9;
      tree[node_idx].x2 = tree[node_idx].y2 = -1e9;
      for (int ch : tree[node_idx].children) {
        const Node& child = tree[ch];
        tree[node_idx].x1 = min(tree[node_idx].x1, child.x1);
        tree[node_idx].y1 = min(tree[node_idx].y1, child.y1);
        tree[node_idx].x2 = max(tree[node_idx].x2, child.x2);
        tree[node_idx].y2 = max(tree[node_idx].y2, child.y2);
      }
    }
    return node_idx;
  };

  auto query = [&](auto& self, int node_idx, int x, int y,
                   vector<int>& result) -> void {
    const Node& node = tree[node_idx];
    if (x < node.x1 || x > node.x2 || y < node.y1 || y > node.y2) {
      return;
    }
    if (node.is_leaf) {
      for (int idx : node.rects) {
        const Rect& r = rects[idx];
        if (x >= r.x1 && x <= r.x2 && y >= r.y1 && y <= r.y2) {
          result.push_back(r.id);
        }
      }
    } else {
      for (int ch : node.children) {
        self(self, ch, x, y, result);
      }
    }
  };

  vector<int> all_indices(n);
  for (int i = 0; i < n; ++i) {
    all_indices[i] = i;
  }
  int root = build(build, all_indices, 0);

  int q;
  cin >> q;
  vector<int> result;
  while (q--) {
    int x, y;
    cin >> x >> y;
    result.clear();
    query(query, root, x, y, result);
    sort(result.begin(), result.end());
    cout << result.size();
    for (int id : result) {
      cout << ' ' << id;
    }
    cout << '\n';
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
