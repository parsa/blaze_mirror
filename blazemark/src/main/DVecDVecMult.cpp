//=================================================================================================
/*!
//  \file src/blaze/DVecDVecMult.cpp
//  \brief Source file for the dense vector/dense vector multiplication benchmark
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

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <blaze/math/DynamicVector.h>
#include <blaze/math/Functions.h>
#include <blaze/math/Infinity.h>
#include <blaze/util/Timing.h>
#include <blazemark/armadillo/DVecDVecMult.h>
#include <blazemark/blaze/DVecDVecMult.h>
#include <blazemark/blitz/DVecDVecMult.h>
#include <blazemark/boost/DVecDVecMult.h>
#include <blazemark/classic/DVecDVecMult.h>
#include <blazemark/clike/DVecDVecMult.h>
#include <blazemark/eigen/DVecDVecMult.h>
#include <blazemark/system/Armadillo.h>
#include <blazemark/system/Blitz.h>
#include <blazemark/system/Config.h>
#include <blazemark/system/Eigen.h>
#include <blazemark/system/Precision.h>
#include <blazemark/system/Types.h>
#include <blazemark/util/Benchmarks.h>
#include <blazemark/util/DenseRun.h>
#include <blazemark/util/Parser.h>


//*************************************************************************************************
// Using declarations
//*************************************************************************************************

using blazemark::Benchmarks;
using blazemark::DenseRun;
using blazemark::Parser;




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Estimating the necessary number of steps for each benchmark.
//
// \param run The parameters for the benchmark run.
// \return void
//
// This function estimates the necessary number of steps for the given benchmark based on the
// performance of the Blaze library.
*/
void estimateSteps( DenseRun& run )
{
   using blazemark::real;
   using blaze::columnVector;

   const size_t N( run.getSize() );

   blaze::DynamicVector<real,columnVector> a( N, 0.1 ), b( N, 0.1 ), c( N );
   blaze::timing::WcTimer timer;
   double wct( 0.0 );
   size_t steps( 1UL );

   while( true ) {
      timer.start();
      for( size_t i=0UL; i<steps; ++i ) {
         c = a * b;
      }
      timer.end();
      wct = timer.last();
      if( wct >= 0.2 ) break;
      steps *= 2UL;
   }

   if( c.size() != N )
      std::cerr << " Line " << __LINE__ << ": ERROR detected!!!\n";

   run.setSteps( blaze::max( 1UL, ( blazemark::runtime * steps ) / timer.last() ) );
}
//*************************************************************************************************




//=================================================================================================
//
//  BENCHMARK FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Dense vector/dense vector multiplication benchmark function.
//
// \param runs The specified benchmark runs.
// \param benchmarks The selection of benchmarks.
// \return void
*/
void dvecdvecmult( std::vector<DenseRun>& runs, Benchmarks benchmarks )
{
   std::cout << std::left;

   std::sort( runs.begin(), runs.end() );

   size_t slowSize( blaze::inf );
   for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
      if( run->getSteps() == 0UL ) {
         if( run->getSize() < slowSize ) {
            estimateSteps( *run );
            if( run->getSteps() == 1UL )
               slowSize = run->getSize();
         }
         else run->setSteps( 1UL );
      }
   }

   if( benchmarks.runClike ) {
      std::cout << "   C-like implementation [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setClikeResult( blazemark::clike::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getClikeResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }

   if( benchmarks.runClassic ) {
      std::cout << "   Classic operator overloading [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setClassicResult( blazemark::classic::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getClassicResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }

   if( benchmarks.runBlaze ) {
      std::cout << "   Blaze [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setBlazeResult( blazemark::blaze::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getBlazeResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }

   if( benchmarks.runBoost ) {
      std::cout << "   Boost uBLAS [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setBoostResult( blazemark::boost::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getBoostResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }

#if BLAZEMARK_BLITZ_MODE
   if( benchmarks.runBlitz ) {
      std::cout << "   Blitz++ [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setBlitzResult( blazemark::blitz::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getBlitzResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }
#endif

#if BLAZEMARK_ARMADILLO_MODE
   if( benchmarks.runArmadillo ) {
      std::cout << "   Armadillo [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setArmadilloResult( blazemark::armadillo::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getArmadilloResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }
#endif

#if BLAZEMARK_EIGEN_MODE
   if( benchmarks.runEigen ) {
      std::cout << "   Eigen [MFlop/s]:\n";
      for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
         const size_t N    ( run->getSize()  );
         const size_t steps( run->getSteps() );
         run->setEigenResult( blazemark::eigen::dvecdvecmult( N, steps ) );
         const double mflops( ( N ) * steps / run->getEigenResult() / 1E6 );
         std::cout << "     " << std::setw(12) << N << mflops << std::endl;
      }
   }
#endif

   for( std::vector<DenseRun>::iterator run=runs.begin(); run!=runs.end(); ++run ) {
      std::cout << *run;
   }
}
//*************************************************************************************************




//=================================================================================================
//
//  MAIN FUNCTION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief The main function for the dense vector/dense vector multiplication benchmark.
//
// \param argc The total number of command line arguments.
// \param argv The array of command line arguments.
// \return void
*/
int main( int argc, char** argv )
{
   std::cout << "\n Dense Vector/Dense Vector Multiplication:\n";

   Benchmarks benchmarks;
   parseCommandLineArguments( argc, argv, benchmarks );

   const std::string installPath( INSTALL_PATH );
   const std::string parameterFile( installPath + "/params/dvecdvecmult.prm" );
   Parser<DenseRun> parser;
   std::vector<DenseRun> runs;

   try {
      parser.parse( parameterFile.c_str(), runs );
   }
   catch( std::exception& ex ) {
      std::cerr << "   Error during parameter extraction: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }

   try {
      dvecdvecmult( runs, benchmarks );
   }
   catch( std::exception& ex ) {
      std::cerr << "   Error during benchmark execution: " << ex.what() << "\n";
      return EXIT_FAILURE;
   }
}
//*************************************************************************************************
