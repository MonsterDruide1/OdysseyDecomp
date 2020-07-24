#include <MathUtil.h>

float normalize(signed int var, signed int min, signed int max)
{
  signed int v3;
  float result;

  if ( var <= min )
    return 0.0;
  if ( max - min <= 0 )
  {
    result = 1.0;
    if ( var < min )
      result = 0.0;
  }
  else
  {
    if ( var <= max )
      v3 = var;
    else
      v3 = max;
    if ( var < min )
      v3 = min;
    result = (v3 - min) / (max - min);
  }
  return result;
}