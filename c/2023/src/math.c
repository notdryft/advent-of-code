#include "math.h"

long long gcd(long long a, long long b) {
  long long ac = a;
  long long bc = b;
  long long tmp = b;
  while (1) {
    long long check = ac % bc;
    if (check != 0) {
      tmp = check;
      ac = bc;
      bc = tmp;
    } else {
      break;
    }
  }
  return tmp;
}

long long lcm(long long a, long long b) {
  return a * (b / gcd(a, b));
}
