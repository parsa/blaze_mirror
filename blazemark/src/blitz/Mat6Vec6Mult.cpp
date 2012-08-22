//=================================================================================================
/*!
//  \file src/blitz/Mat6Vec6Mult.cpp
//  \brief Source file for the Blitz++ 6D matrix/vector multiplication kernel
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
#include <vector>
#include <blitz/array.h>
#include <blaze/util/Random.h>
#include <blaze/util/Timing.h>
#include <blazemark/blitz/Mat6Vec6Mult.h>
#include <blazemark/system/Config.h>
#include <blazemark/system/Precision.h>


namespace blazemark {

namespace blitz {

//=================================================================================================
//
//  KERNEL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Blitz++ 6-dimensional matrix/vector multiplication kernel.
//
// \param N The number of 6D vectors to be computed.
// \param steps The number of iteration steps to perform.
// \return Minimum runtime of the kernel function.
//
// This kernel function implements the 6-dimensional matrix/vector multiplication by means of
// the Blitz++ functionality.
*/
double mat6vec6mult( size_t N, size_t steps )
{
   using ::blazemark::real;

   ::blaze::setSeed( seed );

   ::std::vector< ::blitz::Array<real,2> > A( N );
   ::std::vector< ::blitz::Array<real,1> > a( N ), b( N );
   ::blitz::firstIndex i;
   ::blitz::secondIndex j;
   ::blaze::timing::WcTimer timer;

   for( size_t l=0UL; l<N; ++l ) {
      A[l].resize( 6, 6 );
      for( int m=0; m<6; ++m ) {
         for( int n=0; n<6; ++n ) {
            A[l](m,n) = ::blaze::rand<real>();
         }
      }
   }

   for( size_t l=0UL; l<N; ++l ) {
      a[l].resize( 6 );
      b[l].resize( 6 );
      for( int m=0; m<6; ++m ) {
         a[l](m) = ::blaze::rand<real>();
      }
   }

   for( size_t l=0UL; l<N; ++l ) {
      b[l] = sum( A[l](i,j) * a[l](j), j );
   }

   for( size_t rep=0UL; rep<reps; ++rep )
   {
      timer.start();
      for( size_t step=0UL, l=0UL; step<steps; ++step, ++l ) {
         if( l == N ) l = 0UL;
         b[l] = sum( A[l](i,j) * a[l](j), j );
      }
      timer.end();

      for( size_t l=0UL; l<N; ++l )
         if( b[l](0) < real(0) )
            std::cerr << " Line " << __LINE__ << ": ERROR detected!!!\n";

      if( timer.last() > maxtime )
         break;
   }

   const double minTime( timer.min()     );
   const double avgTime( timer.average() );

   if( minTime * ( 1.0 + deviation*0.01 ) < avgTime )
      std::cerr << " Blitz++ kernel 'mat6vec6mult': Time deviation too large!!!\n";

   return minTime;
}
//*************************************************************************************************

} // namespace blitz

} // namespace blazemark
