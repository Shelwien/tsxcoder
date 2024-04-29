
typedef enum {COMPRESS, DECOMPRESS} Mode;

struct Coder {

  Mode mode;

  Predictor predictor;

  Rangecoder rc;


  void rc_Init( Mode _mode, FILE* f ) {
    mode = _mode;
    if( mode==DECOMPRESS ) {
      rc.StartDecode(f);
    } else {
      rc.StartEncode(f);
    }
  }

  void encode_p( int y, int p ) {
    if( y==1 ) {
      rc.rc_Process( 0, p,       SCALE );
    } else {
      rc.rc_Process( p, SCALE-p, SCALE );
    }
  }

  int decode_p( int p ) {
#if 0
    uint r = rc.rc_GetFreq(SCALE);
    uint y = r<p;
#else 
    uint y = rc.code < rc.rc_Bound(p,SCALE);
#endif
    encode_p( y, p );
    return y;
  }

  void encode( int y ) {
    encode_p( y, predictor.P() );
    predictor.update(y);
  }

  void encode_sim( int y ) {
    predictor.P();
    predictor.update(y);
  }


  int decode() {
    int y = decode_p( predictor.P() );
    predictor.update(y);
    return y;
  }

  void flush() {
    rc.rc_Quit();
  }

};
