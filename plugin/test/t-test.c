#include <math.h>

unsigned int
syracuse ( int N )
{
  int n = N;
  unsigned int a, b, c = 0;
  (void)a;
  (void)b;

  while ( n != 1 )
    {
      {
        int c;

        if ( n & 1 )
          c = 3*n + 1;
        else
          c = n / 2;

        n = c;
      }

      ++c;
    }

  return c;
}

int
main ( )
{
  unsigned int i;
  unsigned int syr;
  double v;
  double vp;
  float a;

  syr = 0;

  for ( i=1; i<=8; ++i )
    syr += syracuse ( i );

  v = pow ( syr, 2 );
  vp = v * M_PI;
  a = (float) sqrt ( vp );
  return (int) a;
}
