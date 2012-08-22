//=================================================================================================
/*!
//  \file src/mtl/CG.cpp
//  \brief Source file for the MTL conjugate gradient kernel
//
//  Copyright (C) 2011 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. This library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as published by the
//  Free Software Foundation; either version 3, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with a special
//  exception for linking and compiling against the Blaze library, the so-called "runtime
//  exception"; see the file COPYING. If not, see http://www.gnu.org/licenses/.
*/
//=================================================================================================


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <blaze/util/Random.h>
#include <blaze/util/Timing.h>
#include <blazemark/blaze/CG.h>
#include <blazemark/system/Config.h>
#include <blazemark/system/Precision.h>


namespace blazemark {

namespace mtl {

//=================================================================================================
//
//  KERNEL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Blaze conjugate gradient kernel.
//
// \param N The number of rows and columns of the 2D discretized grid.
// \param steps The number of solving steps to perform.
// \param iterations The number of iterations to perform.
// \return Minimum runtime of the kernel function.
//
// This kernel function implements the conjugate gradient method by means of the Blaze
// functionality.
*/
double cg( size_t N, size_t steps, size_t iterations )
{
   using ::blazemark::real;

   typedef ::mtl::dense_vector<real>  dense_vector;
   typedef ::mtl::tag::row_major  row_major;
   typedef ::mtl::matrix::parameters<row_major>  parameters;
   typedef ::mtl::compressed2D<real,parameters>  compressed2D;
   typedef ::mtl::matrix::inserter<compressed2D>  inserter;

   ::blaze::setSeed( seed );

   const size_t NN( N*N );

   compressed2D A( NN, NN );
   dense_vector x( NN ), b( NN ), r( NN ), d( NN ), h( NN ), init( NN );
   real alpha, beta, delta;
   ::blaze::timing::WcTimer timer;

   {
      inserter ins( A );

      for( size_t i=0UL; i<N; ++i ) {
         for( size_t j=0UL; j<N; ++j ) {
            if( i > 0UL   ) ins[i*N+j][(i-1UL)*N+j] = -1.0;  // Top neighbor
            if( j > 0UL   ) ins[i*N+j][i*N+j-1UL  ] = -1.0;  // Left neighbor
            ins[i*N+j][i*N+j] = 4.0;
            if( j < N-1UL ) ins[i*N+j][i*N+j+1UL  ] = -1.0;  // Right neighbor
            if( i < N-1UL ) ins[i*N+j][(i+1UL)*N+j] = -1.0;  // Bottom neighbor
         }
      }
   }

   for( size_t i=0UL; i<NN; ++i ) {
      b[i]    = real(0);
      init[i] = ::blaze::rand<real>();
   }

   for( size_t rep=0UL; rep<reps; ++rep )
   {
      timer.start();
      for( size_t step=0UL; step<steps; ++step )
      {
         x = init;
         r = A * x - b;
         delta = trans(r) * r;
         d = -r;

         for( size_t iteration=0UL; iteration<iterations; ++iteration )
         {
            h = A * d;
            alpha = delta / ( trans(d) * h );
            x += alpha * d;
            r += alpha * h;
            beta = trans(r) * r;
            d = ( beta / delta ) * d - r;
            delta = beta;
         }
      }
      timer.end();

      if( size(x) != NN )
         std::cerr << " Line " << __LINE__ << ": ERROR detected!!!\n";

      if( timer.last() > maxtime )
         break;
   }

   const double minTime( timer.min()     );
   const double avgTime( timer.average() );

   if( minTime * ( 1.0 + deviation*0.01 ) < avgTime )
      std::cerr << " MTL kernel 'cg': Time deviation too large!!!\n";

   return minTime;
}
//*************************************************************************************************

} // namespace mtl

} // namespace blazemark
