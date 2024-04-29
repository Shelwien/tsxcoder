
#include <immintrin.h>

template< int x, int n >
struct t_A {
  static void dyn_xabort( int a ) {
    if( (a>>(n-1))&1 ) t_A<x*2+1,n-1>::dyn_xabort(a); else t_A<x*2+0,n-1>::dyn_xabort(a);
  }
};

template< int x >
struct t_A<x,0> {
  static void dyn_xabort( int a ) {
    _xabort(x);
    while(1) {}
  }
};
