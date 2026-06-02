// Задача 13.A (GeoHash кодирование)
#include <bits/stdc++.h>

using namespace std;

using ld = long double;

const string kBase32 = "0123456789bcdefghjkmnpqrstuvwxyz";

void Solve() {
  int t;
  cin >> t;
  while (t--) {
    int d;
    ld lon, lat;
    cin >> d >> lon >> lat;

    const ld val[2] = {lon, lat};
    ld left[2] = {-180.0, -90.0};
    ld right[2] = {180.0, 90.0};

    int bits = 0;
    int count = 0;
    string hash;

    for (int i = 0; i < d * 5; ++i) {
      int type = i % 2;
      ld mid = (left[type] + right[type]) / 2.0;

      int bit;
      if (val[type] >= mid) {
        bit = 1;
        left[type] = mid;
      } else {
        bit = 0;
        right[type] = mid;
      }

      bits = (bits << 1) | bit;
      count++;

      if (count == 5) {
        hash += kBase32[bits];
        bits = 0;
        count = 0;
      }
    }

    cout << hash << '\n';
  }
}

int main() {
  ios::sync_with_stdio(0);
  cin.tie(0);
  cout.tie(0);

  Solve();

  return 0;
}
