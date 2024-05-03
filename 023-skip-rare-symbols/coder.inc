
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

  uint encode_sim( int y ) {
    uint p = predictor.P();
    predictor.update(y);
    return p;
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


#ifdef SIM_FUNC

extern "C" uint encode_sim( void* _p, int y ) {
  auto& p = *(Coder*)_p;
  return p.encode_sim(y);
}

#else

enum{ N_Cells=1<<20, N_Nest=0x200 };

struct Cell {
  void* ptr;
  uint msk;
  uint val;
};

uint trkptr = 0;
uint nesting = 0;

uint nest_trkptr[N_Nest];

alignas(64) Cell trk[N_Cells];

#define trackN( _p,N ) \
{ \
  const uint mask = (1ULL<<(8*N))-1;\
  Cell& c = trk[trkptr++];\
  c.ptr = _p;\
  c.msk = mask;\
  c.val = *((uint*)_p) & mask;\
}

void NEST( void ) {
  nest_trkptr[nesting++] = trkptr;
}

void UNDO( void ) {
  uint i;
  uint k = nest_trkptr[--nesting];
  for( i=trkptr-1; int(i)>=int(k); i-- ) {
    Cell& c = trk[i];
    uint* p = (uint*)c.ptr;
    uint d = p[0] & (~c.msk);
    d |= c.val;
    p[0] = d;
  }
  trkptr = k;
}

#ifdef SKIP_ATTRS
#define attrs
#else
#define attrs __attribute__((no_caller_saved_registers)) __attribute__((target("general-regs-only")))
#endif

extern "C" {

attrs void __stdcall track4( void* _p ) { trackN(_p,4); }
attrs void __stdcall track2( void* _p ) { trackN(_p,2); }          
attrs void __stdcall track1( void* _p ) { trackN(_p,1); }

uint encode_sim( void* _p, int y );

}

#undef attrs

#endif

