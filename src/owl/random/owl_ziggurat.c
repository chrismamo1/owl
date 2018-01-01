
#include <math.h>
#include "owl_random.h"

static uint32_t kn[128], ke[256];
static float wn[128], fn[128], we[256], fe[256];


// init the internal state for exponential prng
inline float rng_std_exp ( ) {
  float value, x;
  uint32_t jz = sfmt_rand32;
  uint32_t iz = ( jz & 255 );

  if ( jz < ke[iz] )
    value = ( float ) ( jz ) * we[iz];
  else {
    for ( ; ; ) {
      if ( iz == 0 ) {
        value = 7.69711 - log ( sfmt_f64_1 );
        break;
      }

      x = ( float ) ( jz ) * we[iz];

      if ( fe[iz] + sfmt_f64_1 * ( fe[iz-1] - fe[iz] ) < exp ( - x ) ) {
        value = x;
        break;
      }

      jz = sfmt_rand32;
      iz = ( jz & 255 );

      if ( jz < ke[iz] ) {
        value = ( float ) ( jz ) * we[iz];
        break;
      }
    }
  }
  return value;
}


inline float rng_exp (float lambda) {
  return (lambda * rng_std_exp());
}


// init the internal state for exponential prng
void rng_std_exp_init ( ) {
  double de = 7.697117470131487;
  const double m2 = 2147483648.0;
  double te = 7.697117470131487;
  const double ve = 3.949659822581572E-03;

  double q = ve / exp ( - de );

  ke[0] = ( uint32_t ) ( ( de / q ) * m2 );
  ke[1] = 0;

  we[0] = ( float ) ( q / m2 );
  we[255] = ( float ) ( de / m2 );

  fe[0] = 1.0;
  fe[255] = ( float ) ( exp ( - de ) );

  for ( int i = 254; 1 <= i; i-- ) {
    de = - log ( ve / de + exp ( - de ) );
    ke[i+1] = ( uint32_t ) ( ( de / te ) * m2 );
    te = de;
    fe[i] = ( float ) ( exp ( - de ) );
    we[i] = ( float ) ( de / m2 );
  }
}

// generate a prng of gaussian distribution
inline float rng_std_gaussian ( ) {
  const float r = 3.442620;
  float value, x, y;

  int hz = ( int ) sfmt_rand32;
  uint32_t iz = ( hz & 127 );

  if ( fabs ( hz ) < kn[iz] )
    value = ( float ) ( hz ) * wn[iz];
  else {
    for ( ; ; ) {
      if ( iz == 0 ) {
        for ( ; ; ) {
          x = - 0.2904764 * log ( sfmt_f64_1 );
          y = - log ( sfmt_f64_1 );
          if ( x * x <= y + y )
            break;
        }
        value = ( hz > 0 ) ? (+ r + x) : (- r - x);
        break;
      }

      x = ( float ) ( hz ) * wn[iz];

      if ( fn[iz] + ( sfmt_f64_1 ) * ( fn[iz-1] - fn[iz] ) < exp ( - 0.5 * x * x ) ) {
        value = x;
        break;
      }

      hz = ( int ) sfmt_rand32;
      iz = ( hz & 127 );

      if ( fabs ( hz ) < kn[iz] ) {
        value = ( float ) ( hz ) * wn[iz];
        break;
      }
    }
  }

  return value;
}


inline float rng_gaussian (float mu, float sigma) {
  return (mu + sigma * rng_std_gaussian());
}


// init the internal state for gaussian prng
void rng_std_gaussian_init ( ) {
  double dn = 3.442619855899;
  const double m1 = 2147483648.0;
  double tn = 3.442619855899;
  const double vn = 9.91256303526217E-03;

  double q = vn / exp ( - 0.5 * dn * dn );

  kn[0] = ( uint32_t ) ( ( dn / q ) * m1 );
  kn[1] = 0;

  wn[0] = ( float ) ( q / m1 );
  wn[127] = ( float ) ( dn / m1 );

  fn[0] = 1.0;
  fn[127] = ( float ) ( exp ( - 0.5 * dn * dn ) );

  for ( int i = 126; 1 <= i; i-- ) {
    dn = sqrt ( - 2.0 * log ( vn / dn + exp ( - 0.5 * dn * dn ) ) );
    kn[i+1] = ( uint32_t ) ( ( dn / tn ) * m1 );
    tn = dn;
    fn[i] = ( float ) ( exp ( - 0.5 * dn * dn ) );
    wn[i] = ( float ) ( dn / m1 );
  }
}


// init the internal table of ziggurat module
void ziggurat_init ( ) {
  rng_std_exp_init();
  rng_std_gaussian_init();
}
