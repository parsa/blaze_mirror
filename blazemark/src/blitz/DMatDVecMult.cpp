//=================================================================================================
/*!
//  \file src/blitz/DMatDVecMult.cpp
//  \brief Source file for the Blitz++ dense matrix/dense vector multiplication kernel
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
#include <blitz/array.h>
#include <boost/cast.hpp>
#include <blaze/util/Random.h>
#include <blaze/util/Timing.h>
#include <blazemark/blitz/DMatDVecMult.h>
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
/*!\brief Blitz++ dense matrix/dense vector multiplication kernel.
//
// \param N The number of rows and columns of the matrix and the size of the vector.
// \param steps The number of iteration steps to perform.
// \return Minimum runtime of the kernel function.
//
// This kernel function implements the dense matrix/dense vector multiplication by means of
// the Blitz++ functionality.
*/
double dmatdvecmult( size_t N, size_t steps )
{
   using ::blazemark::real;
   using ::boost::numeric_cast;

   ::blaze::setSeed( seed );

   ::blitz::Array<real,2> A( N, N );
   ::blitz::Array<real,1> a( N ), b( N );
   ::blitz::firstIndex i;
   ::blitz::secondIndex j;
   ::blaze::timing::WcTimer timer;

   for( int m=0; m<static_cast<int>( N ); ++m ) {
      for( int n=0; n<static_cast<int>( N ); ++n ) {
         A(m,n) = ::blaze::rand<real>();
      }
   }

   for( int m=0; m<static_cast<int>( N ); ++m ) {
      a(m) = ::blaze::rand<real>();
   }

   b = sum( A(i,j) * a(j), j );

   for( size_t rep=0UL; rep<reps; ++rep )
   {
      timer.start();
      for( size_t step=0UL; step<steps; ++step ) {
         b = sum( A(i,j) * a(j), j );
      }
      timer.end();

      if( numeric_cast<size_t>( b.size() ) != N )
         std::cerr << " Line " << __LINE__ << ": ERROR detected!!!\n";

      if( timer.last() > maxtime )
         break;
   }

   const double minTime( timer.min()     );
   const double avgTime( timer.average() );

   if( minTime * ( 1.0 + deviation*0.01 ) < avgTime )
      std::cerr << " Blitz++ kernel 'dmatdvecmult': Time deviation too large!!!\n";

   return minTime;
}
//*************************************************************************************************

} // namespace blitz

} // namespace blazemark
