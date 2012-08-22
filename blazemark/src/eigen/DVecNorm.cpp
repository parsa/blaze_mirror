//=================================================================================================
/*!
//  \file src/eigen/DVecNorm.cpp
//  \brief Source file for the Eigen dense vector norm kernel
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
#include <Eigen/Dense>
#include <blaze/util/Random.h>
#include <blaze/util/Timing.h>
#include <blazemark/eigen/DVecNorm.h>
#include <blazemark/system/Config.h>
#include <blazemark/system/Precision.h>


namespace blazemark {

namespace eigen {

//=================================================================================================
//
//  KERNEL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Eigen dense vector norm kernel.
//
// \param N The size of the vectors for the norm computation.
// \param steps The number of iteration steps to perform.
// \return Minimum runtime of the kernel function.
//
// This kernel function implements the dense vector norm by means of the Eigen functionality.
*/
double dvecnorm( size_t N, size_t steps )
{
   using ::blazemark::real;
   using ::Eigen::Dynamic;

   ::blaze::setSeed( seed );

   ::Eigen::Matrix<real,Dynamic,1> a( N );
   real scalar( 0 );
   ::blaze::timing::WcTimer timer;

   for( size_t i=0UL; i<N; ++i ) {
      a[i] = ::blaze::rand<real>();
   }

   for( size_t rep=0UL; rep<reps; ++rep )
   {
      timer.start();
      for( size_t step=0UL; step<steps; ++step ) {
         scalar += a.transpose() * a;
      }
      timer.end();

      if( scalar < real(0) )
         std::cerr << " Line " << __LINE__ << ": ERROR detected!!!\n";

      if( timer.last() > maxtime )
         break;
   }

   const double minTime( timer.min()     );
   const double avgTime( timer.average() );

   if( minTime * ( 1.0 + deviation*0.01 ) < avgTime )
      std::cerr << " Eigen kernel 'dvecnorm': Time deviation too large!!!\n";

   return minTime;
}
//*************************************************************************************************

} // namespace boost

} // namespace blazemark
