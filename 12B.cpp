// Задача 12.B (Крипы в Gota2)
#include <bits/stdc++.h>

using namespace std;

using ll = long long;

const double kFieldSize = 1024.0;
const double kRadius = 2.0;
const double kHpMax = 10.0;

struct Creep {
  double x, y, hp;
};

struct QuadTree {
  double x1, y1, x2, y2;
  vector<int> indices;
  int ch[4] = {-1, -1, -1, -1};

  QuadTree(double new_x1, double new_y1, double new_x2, double new_y2)
      : x1(new_x1), y1(new_y1), x2(new_x2), y2(new_y2) {}
};

void Build(int id, const vector<Creep>& pts, const vector<int>& idxs,
           vector<QuadTree>& nodes, int depth = 0) {
  if (idxs.empty()) {
    return;
  }
  if (idxs.size() <= 8 || depth > 10) {
    nodes[id].indices = idxs;
    return;
  }
  double mid_x = (nodes[id].x1 + nodes[id].x2) / 2.0;
  double mid_y = (nodes[id].y1 + nodes[id].y2) / 2.0;
  vector<int> idx[4];
  for (int cur_id : idxs) {
    const Creep& c = pts[cur_id];
    int quad = (c.x <= mid_x ? 0 : 1) + (c.y <= mid_y ? 0 : 2);
    idx[quad].push_back(cur_id);
  }

  if (!idx[0].empty()) {
    nodes[id].ch[0] = nodes.size();
    nodes.emplace_back(nodes[id].x1, nodes[id].y1, mid_x, mid_y);
    Build(nodes[id].ch[0], pts, idx[0], nodes, depth + 1);
  }
  if (!idx[1].empty()) {
    nodes[id].ch[1] = nodes.size();
    nodes.emplace_back(mid_x, nodes[id].y1, nodes[id].x2, mid_y);
    Build(nodes[id].ch[1], pts, idx[1], nodes, depth + 1);
  }
  if (!idx[2].empty()) {
    nodes[id].ch[2] = nodes.size();
    nodes.emplace_back(nodes[id].x1, mid_y, mid_x, nodes[id].y2);
    Build(nodes[id].ch[2], pts, idx[2], nodes, depth + 1);
  }
  if (!idx[3].empty()) {
    nodes[id].ch[3] = nodes.size();
    nodes.emplace_back(mid_x, mid_y, nodes[id].x2, nodes[id].y2);
    Build(nodes[id].ch[3], pts, idx[3], nodes, depth + 1);
  }
}

void QueryRadius(int u, double qx, double qy, double radius,
                 const vector<Creep>& pts, vector<int>& result,
                 const vector<QuadTree>& nodes) {
  if (u == -1) {
    return;
  }
  double rx1 = qx - radius, ry1 = qy - radius;
  double rx2 = qx + radius, ry2 = qy + radius;
  if (nodes[u].x2 < rx1 || nodes[u].x1 > rx2 || nodes[u].y2 < ry1 ||
      nodes[u].y1 > ry2) {
    return;
  }

  if (!nodes[u].indices.empty()) {
    for (int id : nodes[u].indices) {
      const Creep& c = pts[id];
      double dx = c.x - qx, dy = c.y - qy;
      if (dx * dx + dy * dy < radius * radius) {
        result.push_back(id);
      }
    }
    return;
  }
  for (int i = 0; i < 4; ++i) {
    QueryRadius(nodes[u].ch[i], qx, qy, radius, pts, result, nodes);
  }
}

double SimulateFraction(vector<Creep>& attackers, vector<Creep>& defenders,
                        int n) {
  double total_damage = 0.0;
  vector<int> all_indices(n);
  iota(all_indices.begin(), all_indices.end(), 0);

  vector<QuadTree> nodes;
  nodes.reserve(n * 2);
  nodes.emplace_back(0.0, 0.0, kFieldSize, kFieldSize);
  Build(0, defenders, all_indices, nodes);

  for (int i = 0; i < n; ++i) {
    char type;
    cin >> type;
    if (type == 'm') {
      double vx, vy;
      cin >> vx >> vy;
      attackers[i].x += vx;
      attackers[i].y += vy;
      attackers[i].x = max(0.0, min(kFieldSize, attackers[i].x));
      attackers[i].y = max(0.0, min(kFieldSize, attackers[i].y));
    } else if (type == 'f') {
      if (attackers[i].hp <= 0.0) {
        continue;
      }
      vector<int> victims;
      QueryRadius(0, attackers[i].x, attackers[i].y, kRadius, defenders,
                  victims, nodes);
      for (int j : victims) {
        if (defenders[j].hp <= 0.0) {
          continue;
        }
        double dx = attackers[i].x - defenders[j].x;
        double dy = attackers[i].y - defenders[j].y;
        double dist = sqrt(dx * dx + dy * dy);
        if (dist >= kRadius) {
          continue;
        }
        double factor = (dist < 1.0) ? 1.0 : (2.0 - dist);
        double damage = min(attackers[i].hp * factor * 0.1, defenders[j].hp);
        defenders[j].hp -= damage;
        total_damage += damage;
      }
    }
  }
  return total_damage;
}

void Solve() {
  int n;
  cin >> n;
  vector<Creep> light(n), dark(n);
  for (int i = 0; i < n; ++i) {
    cin >> light[i].x >> light[i].y;
    light[i].hp = kHpMax;
  }
  for (int i = 0; i < n; ++i) {
    cin >> dark[i].x >> dark[i].y;
    dark[i].hp = kHpMax;
  }

  int m;
  cin >> m;
  for (int sec = 0; sec < m; ++sec) {
    double dmg_light = SimulateFraction(light, dark, n);
    double dmg_dark = SimulateFraction(dark, light, n);
    cout << fixed << setprecision(6) << dmg_light << "\n" << dmg_dark << "\n";
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
