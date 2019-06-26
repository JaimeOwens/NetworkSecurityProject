#include "diffie_hellman.h"

//0-（p-2）的质数
long long math::generateX(long long p) {
  if (!math::isPrime(p)){
    cout<<"p must be prime"<<endl;
    exit(1);
  }
  
  long long res;
  do {
    res = rand() % (p-2);
  } while (!math::isPrime(res));
  return res;
}

//base^exp%mod
long long math::powModN(long long base, long long exp, long long mod) {
  if (mod <= 1)
    return 0;
  long long res = 1;
  base %= mod;
  while (exp > 0) {
    if (exp & 1) 
      res = (res*base) % mod;
    base = (base*base) % mod;
    exp >>= 1;
  }
  return res;
}
/*long long math::powModN(long long base, long long exp, long long mod) {
  if (mod == 1)
    return 0;
  long long c = 1;
  for (int x = 0; x < exp; x++)
    //c = (c*base) % mod;
    c = ((c % mod) * (base % mod)) % mod;
  return c;
}*/

//质数
bool math::isPrime(long long n) {
  if (n < 2 || !(n & 1))
    return false;
  if (n == 2)
    return true;
  
  if (n % 6 != 1 && n % 6 != 5) {
        return false;
  }

  long long sq = (long long)sqrt(n);
  
  for (int i = 5; i <= sq; i+=6) {
    if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
  }
  return true;
}
