
//#include "tsx.inc"
//#include "tsx1.inc"
enum{ PSCALE=1<<16 };

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define assert(x)

typedef unsigned short word;
typedef unsigned int   uint;
typedef unsigned char  byte;
typedef unsigned long long qword;

#ifdef __GNUC__
 #define INLINE   __attribute__((always_inline)) 
 #define NOINLINE __attribute__((noinline))
 #define ALIGN(n) __attribute__((aligned(n)))
#else
 #define INLINE   __forceinline
 #define NOINLINE __declspec(noinline)
 #define ALIGN(n) __declspec(align(n))
#endif


#include "sh_mixer.inc"

#include "model.inc"

#include "sh_v1m.inc"

#include "coder.inc"

#ifndef SIM_FUNC

#include "log2lut.inc"

uint flen( FILE* f ) {
  fseek( f, 0, SEEK_END );
  uint len = ftell(f);
  fseek( f, 0, SEEK_SET );
  return len;
}


//////////////////////////// main ////////////////////////////

Coder E;

template< uint bit, uint ctx, int K >
struct TEST_ENCODE {
  INLINE
  inline static void test_encode( uint (&clen)[0x200] ) {
    constexpr uint cty = ctx*2 + bit;
    NEST();

    uint p = encode_sim( &E, bit );

    if( bit==0 ) {
      clen[ctx*2+0] = clen[ctx] + LOG2(SCALE-p);
      clen[ctx*2+1] = clen[ctx] + LOG2(p);
    }

    if( clen[cty]>0x90000*(8-K) ) {
      TEST_ENCODE<0,cty,K-1>::test_encode( clen ); 
      TEST_ENCODE<1,cty,K-1>::test_encode( clen ); 
    }

    UNDO();
  }
};

template< uint bit, uint ctx >
struct TEST_ENCODE<bit,ctx,0> {
  INLINE
  inline static void test_encode( uint (&clen)[0x200] ) { if( bit==0 ) {
    uint p = E.predictor.P(); //E.predictor.p; 
    clen[ctx*2+0] = clen[ctx] + LOG2(SCALE-p);
    clen[ctx*2+1] = clen[ctx] + LOG2(p);
  }}
};

void test_encode( uint (&clen)[0x200] ) {
  TEST_ENCODE<0,1,7>::test_encode( clen );
  TEST_ENCODE<1,1,7>::test_encode( clen );
}

int unlog[0x100000];

int main(int argc, char** argv) {

  // Chech arguments: fpaq0 c/d input output
  if (argc!=4 || (argv[1][0]!='c' && argv[1][0]!='d')) {
    printf("To compress:   fpaq0m c input output\n"
           "To decompress: fpaq0m d input output\n");
    exit(1);
  }

  uint f_DEC = (argv[1][0]!='c');

  // Open files
  FILE *in=fopen(argv[2], "rb");  if( in==0 ) perror(argv[2]), exit(1);
  FILE *out=fopen(argv[3], "wb"); if( out==0 ) perror(argv[3]), exit(1);
  int c,d;
  uint i,j,f_len,bit,p,r;
  int code,low,total=0,freq[1+256];

  LOG2.Init();
  for( i=0; i<0x100000; i++ ) { unlog[i]=double(PSCALE)*pow(2.0,-double(i)/0x10000); if( unlog[i]<1 ) unlog[i]=1; }

  if( f_DEC==0 ) {
    f_len=flen(in); fwrite( &f_len,1,4,out );
    E.rc_Init(COMPRESS,out);
  } else {
    f_len=0; fread( &f_len,1,4,in );
    E.rc_Init(DECOMPRESS,in);
  }

  for( i=0; i<f_len; i++ ) { 

    uint clen[0x200]; 
    for( j=0; j<0x200; j++ ) clen[j]=0xFFFFF; clen[1]=0;

    test_encode( clen );

    for( d=0,low=0; d<256; d++ ) {
      r = LOG2(SCALE)*8 - clen[0x100+d];

      if( r>0xFFFFF ) r=0xFFFFF;
      freq[d]=unlog[r]; 
      low += freq[d];
    } freq[256] = total = low;

    if( f_DEC==0 ) {

      c = byte(getc(in));
      for( j=0,low=0; j<c; j++ ) low+=freq[j];

    } else {

      code = E.rc.rc_GetFreq(total);
      for( c=0,low=0; low+freq[c]<=code; c++ ) low+=freq[c];
      putc( byte(c), out );

    }

    E.rc.rc_Process(low,freq[c],total);

    // actually update model with current symbol
    for( int j=7; j>=0; --j ) E.encode_sim( (c>>j)&1 );

  } // for i

  if( f_DEC==0 ) E.flush();

  return 0;
}

#endif
