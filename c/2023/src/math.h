#ifndef MATH
#define MATH

#include "commons.h"

#define max(a, b) (a>b?a:b)
#define min(a, b) (a<b?a:b)

long long gcd(long long a, long long b);

#define lcm(a, b) (a*(b/gcd(a, b)))

#endif
