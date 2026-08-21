#ifndef IMPROVE_H_
#define IMPROVE_H_

struct Range;


int lcm(int a,int b);

int gcd(int a,int b);

Frac* getImproveRange(Range *range,Range *prior);

Frac* getImprovePrior(Range *range,Range *prior);

#endif // IMPROVE_H_
