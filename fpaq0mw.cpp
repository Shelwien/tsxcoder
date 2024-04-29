
#include "tsx.inc"
#include "tsx1.inc"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>

#define assert(x)

typedef unsigned short word;
typedef unsigned int   uint;
typedef unsigned char  byte;
typedef unsigned long long qword;

uint flen( FILE* f ) {
  fseek( f, 0, SEEK_END );
  uint len = ftell(f);
  fseek( f, 0, SEEK_SET );
  return len;
}


#include "sh_mixer.inc"

#include "model.inc"

#include "sh_v1m.inc"

#include "coder.inc"

#include "log2lut.inc"


//////////////////////////// main ////////////////////////////

Coder E;
Coder D;

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

  int unlog[256];
  for( i=0; i<256; i++ ) { unlog[i]=float(1l<<15)*pow(2.0f,-float(i)/0x10); if( unlog[i]<1 ) unlog[i]=1; }

  if( f_DEC==0 ) {
    f_len=flen(in); fwrite( &f_len,1,4,out );
    E.rc_Init(COMPRESS,out);
  } else {
    f_len=0; fread( &f_len,1,4,in );
    E.rc_Init(DECOMPRESS,in);
  }

  for( i=0; i<f_len; i++ ) { 

    for( d=0,low=0; d<256; d++ ) {
      do {
        r = _xbegin();
        if( r==-1 ) {

//////////////////////////// vvvv ////////////////////////////
          uint clen=0, c=d;
          for( int j=7; j>=0; --j ) {
            bit = (c>>j)&1; E.encode_sim( bit );
            p = E.predictor.p; clen += LOG2(SCALE)-(bit==0?LOG2(SCALE-p):LOG2(p));
          }
//////////////////////////// ^^^^ ////////////////////////////

          if( clen>0xFFFFF ) clen=0xFFFFF;
          //t_A<0,8>::dyn_xabort(byte(clen>>12));
          dyn_xabort(byte(clen>>12));
          while(1) {}
        }

        if( r&4 ) { memcpy( &D, &E, sizeof(D) ); _sleep(1); }

        if( (r&0xFFFFFF)==1 ) break;

      } while(1);

      freq[d]=unlog[r>>24]; 

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

