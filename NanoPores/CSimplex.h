#pragma once

#include <CNoise.h>
#include <simplexnoise.h>

/*
 * Simplex noise class. From Ken Perlin.
 *
 *
*/
class CSimplex : public CNoise {
 public:

 CSimplex(int o,double f,double p,int s) : CNoise( o,f,p,s)  {
  }


  double raw_2d(const double& x, const double& y) {
    return raw_noise_2d(x,y);
  }

  double raw_3d(const double& x, const double& y, const double& z) {
    return raw_noise_3d(x,y,z);
  }


};
