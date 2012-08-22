//=================================================================================================
/*!
//  \file src/mathtest/dmatsvecmult/M7x13aVCb.cpp
//  \brief Source file for the M7x13aVCb dense matrix/sparse vector multiplication math test
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

#include <cstdlib>
#include <iostream>
#include <blaze/math/CompressedVector.h>
#include <blaze/math/StaticMatrix.h>
#include <blazetest/mathtest/DMatSVecMult.h>
#include <blazetest/system/MathTest.h>
#include <blazetest/util/Creator.h>


//=================================================================================================
//
//  MAIN FUNCTION
//
//=================================================================================================

//*************************************************************************************************
int main()
{
   std::cout << "   Running 'M7x13aVCb'..." << std::endl;

   using blazetest::mathtest::TypeA;
   using blazetest::mathtest::TypeB;

   try
   {
      // Matrix type definitions
      typedef blaze::StaticMatrix<TypeA,7UL,13UL>  M7x13a;
      typedef blaze::CompressedVector<TypeB>       VCb;

      // Creator type definitions
      typedef blazetest::Creator<M7x13a>  CM7x13a;
      typedef blazetest::Creator<VCb>     CVCb;

      // Running the tests
      RUN_DMATSVECMULT_TEST( CM7x13a(), CVCb( 13UL,  0UL ) );
      RUN_DMATSVECMULT_TEST( CM7x13a(), CVCb( 13UL,  4UL ) );
      RUN_DMATSVECMULT_TEST( CM7x13a(), CVCb( 13UL,  8UL ) );
      RUN_DMATSVECMULT_TEST( CM7x13a(), CVCb( 13UL, 13UL ) );
   }
   catch( std::exception& ex ) {
      std::cerr << "\n\n ERROR DETECTED during dense matrix/sparse vector multiplication:\n"
                << ex.what() << "\n";
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
//*************************************************************************************************
