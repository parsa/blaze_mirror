//=================================================================================================
/*!
//  \file src/mathtest/densevector/OperationTest.cpp
//  \brief Source file for the DenseVector functionality operation test
//
//  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <blaze/math/dense/DenseVector.h>
#include <blaze/math/DynamicVector.h>
#include <blazetest/mathtest/densevector/OperationTest.h>
#include <blazetest/mathtest/IsEqual.h>

#ifdef BLAZE_USE_HPX_THREADS
#  include <hpx/hpx_main.hpp>
#endif


namespace blazetest {

namespace mathtest {

namespace densevector {

//=================================================================================================
//
//  CONSTRUCTORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Constructor for the OperationTest class test.
//
// \exception std::runtime_error Operation error detected.
*/
OperationTest::OperationTest()
{
   testIsNan();
   testIsUniform();
   testIsZero();
   testNormalize();
   testMinimum();
   testMaximum();
   testArgmin();
   testArgmax();
   testL1Norm();
   testL2Norm();
   testL3Norm();
   testL4Norm();
   testLpNorm();
   testLength();
   testMean();
   testVar();
   testStdDev();
   testSoftmax();
   testLeftShift();
   testRightShift();
}
//*************************************************************************************************




//=================================================================================================
//
//  TEST FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Test of the \c isnan() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c isnan() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testIsNan()
{
   test_ = "isnan() function";

   // isnan with 0-dimensional vector
   {
      blaze::DynamicVector<float,blaze::rowVector> vec;

      if( blaze::isnan( vec ) != false ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isnan evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // isnan with empty 9-dimensional vector
   {
      blaze::DynamicVector<float,blaze::rowVector> vec( 9UL, 0.0F );

      if( blaze::isnan( vec ) != false ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isnan evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // isnan with filled 9-dimensional vector
   {
      blaze::DynamicVector<float,blaze::rowVector> vec( 9UL, 0.0F );
      vec[3] =  1.0F;
      vec[4] = -2.0F;
      vec[6] =  3.0F;
      vec[8] =  4.0F;

      if( blaze::isnan( vec ) != false ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isnan evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c isUniform() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c isUniform() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testIsUniform()
{
   test_ = "isUniform() function";

   // Uniform vector (0-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      if( blaze::isUniform( vec ) != true ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isUniform evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Uniform vector (1-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 5 };

      if( blaze::isUniform( vec ) != true ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isUniform evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Uniform vector (5-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 5, 5, 5, 5, 5 };

      if( blaze::isUniform( vec ) != true ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isUniform evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Non-uniform vector (5-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 5, 5, 5, 5, 3 };

      if( blaze::isUniform( vec ) != false ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isUniform evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c isZero() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c isZero() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testIsZero()
{
   test_ = "isZero() function";

   // Zero vector (0-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      if( blaze::isZero( vec ) != true ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isZero evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Zero vector (1-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0 };

      if( blaze::isZero( vec ) != true ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isZero evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Zero vector (5-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0, 0, 0, 0, 0 };

      if( blaze::isZero( vec ) != true ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isZero evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Non-zero vector (5-dimensional)
   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0, 0, 0, 0, 3 };

      if( blaze::isZero( vec ) != false ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Invalid isZero evaluation\n"
             << " Details:\n"
             << "   Vector:\n" << vec << "\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c normalize() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c normalize() function for dense vectors. In case
// an error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testNormalize()
{
   test_ = "normalize() function";

   // Initialization check
   blaze::DynamicVector<double,blaze::rowVector> vec( 4UL );
   vec[0] = 1.0;
   vec[1] = 2.0;
   vec[2] = 3.0;
   vec[3] = 4.0;

   checkSize    ( vec, 4UL );
   checkCapacity( vec, 4UL );
   checkNonZeros( vec, 4UL );

   if( vec[0] != 1.0 || vec[1] != 2.0 || vec[2] != 3.0 || vec[3] != 4.0 ) {
      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Initialization failed\n"
          << " Details:\n"
          << "   Result:\n" << vec << "\n"
          << "   Expected result:\n( 1 2 3 4 )\n";
      throw std::runtime_error( oss.str() );
   }

   // Acquiring normalized vector
   const blaze::DynamicVector<double,blaze::rowVector> normalized( normalize( vec ) );

   if( !blaze::equal( length( normalized ), 1.0 ) ) {
      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Normalization failed\n"
          << " Details:\n"
          << "   Result: " << length( normalized ) << "\n"
          << "   Expected result: 1\n";
      throw std::runtime_error( oss.str() );
   }

   // Normalizing the vector
   vec = normalize( vec );

   if( !blaze::equal( length( vec ), 1.0 ) ) {
      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Self-normalization failed\n"
          << " Details:\n"
          << "   Result: " << length( vec ) << "\n"
          << "   Expected result: 1\n";
      throw std::runtime_error( oss.str() );
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c min() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c min() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testMinimum()
{
   test_ = "min() function";

   {
      // Initialization check
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, -2, 3, -4 };

      checkSize    ( vec, 4UL );
      checkCapacity( vec, 4UL );
      checkNonZeros( vec, 4UL );

      if( vec[0] != 1 || vec[1] != -2 || vec[2] != 3 || vec[3] != -4 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Initialization failed\n"
             << " Details:\n"
             << "   Result:\n" << vec << "\n"
             << "   Expected result:\n( 1 -2 3 -4 )\n";
         throw std::runtime_error( oss.str() );
      }

      // Testing the min function
      const int minimum = min( vec );

      if( minimum != -4 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: First computation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: -4\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      // Initialization check
      blaze::DynamicVector<int,blaze::rowVector> vec{ -1, 2, 3, 4 };
      vec[0] = -1;
      vec[1] =  2;
      vec[2] =  3;
      vec[3] =  4;

      checkSize    ( vec, 4UL );
      checkCapacity( vec, 4UL );
      checkNonZeros( vec, 4UL );

      if( vec[0] != -1 || vec[1] != 2 || vec[2] != 3 || vec[3] != 4 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Initialization failed\n"
             << " Details:\n"
             << "   Result:\n" << vec << "\n"
             << "   Expected result:\n( -1 2 3 4 )\n";
         throw std::runtime_error( oss.str() );
      }

      // Testing the min function
      const int minimum = min( vec );

      if( minimum != -1 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Second computation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: -1\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c max() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c max() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testMaximum()
{
   test_ = "max() function";

   {
      // Initialization check
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, -2, -3, -4 };

      checkSize    ( vec, 4UL );
      checkCapacity( vec, 4UL );
      checkNonZeros( vec, 4UL );

      if( vec[0] != 1 || vec[1] != -2 || vec[2] != -3 || vec[3] != -4 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Initialization failed\n"
             << " Details:\n"
             << "   Result:\n" << vec << "\n"
             << "   Expected result:\n( 1 -2 -3 -4 )\n";
         throw std::runtime_error( oss.str() );
      }

      // Testing the max function
      const int maximum = max( vec );

      if( maximum != 1 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: First computation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 1\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      // Initialization check
      blaze::DynamicVector<int,blaze::rowVector> vec{ -1, 2, 3, 4 };

      checkSize    ( vec, 4UL );
      checkCapacity( vec, 4UL );
      checkNonZeros( vec, 4UL );

      if( vec[0] != -1 || vec[1] != 2 || vec[2] != 3 || vec[3] != 4 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Initialization failed\n"
             << " Details:\n"
             << "   Result:\n" << vec << "\n"
             << "   Expected result:\n( -1 2 3 4 )\n";
         throw std::runtime_error( oss.str() );
      }

      // Testing the max function
      const int maximum = max( vec );

      if( maximum != 4 ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Second computation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 4\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c argmin() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c argmin() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testArgmin()
{
   test_ = "argmin() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const size_t minimum = argmin( vec );

      checkSize    ( vec, 0UL );
      checkCapacity( vec, 0UL );
      checkNonZeros( vec, 0UL );

      if( minimum != 0UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmin evaluation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 99 };

      const size_t minimum = argmin( vec );

      checkSize    ( vec, 1UL );
      checkCapacity( vec, 1UL );
      checkNonZeros( vec, 1UL );

      if( minimum != 0UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmin evaluation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const size_t minimum = argmin( vec );

      checkSize    ( vec, 9UL );
      checkCapacity( vec, 9UL );
      checkNonZeros( vec, 9UL );

      if( minimum != 0UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmin evaluation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      const size_t minimum = argmin( vec );

      checkSize    ( vec, 9UL );
      checkCapacity( vec, 9UL );
      checkNonZeros( vec, 9UL );

      if( minimum != 8UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmin evaluation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: 8\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 2, 3, 4, 5, 1, 6, 7, 8, 9 };

      const size_t minimum = argmin( vec );

      checkSize    ( vec, 9UL );
      checkCapacity( vec, 9UL );
      checkNonZeros( vec, 9UL );

      if( minimum != 4UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmin evaluation failed\n"
             << " Details:\n"
             << "   Result: " << minimum << "\n"
             << "   Expected result: 4\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c argmax() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c argmax() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testArgmax()
{
   test_ = "argmax() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const size_t maximum = argmax( vec );

      checkSize    ( vec, 0UL );
      checkCapacity( vec, 0UL );
      checkNonZeros( vec, 0UL );

      if( maximum != 0UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmax evaluation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 99 };

      const size_t maximum = argmax( vec );

      checkSize    ( vec, 1UL );
      checkCapacity( vec, 1UL );
      checkNonZeros( vec, 1UL );

      if( maximum != 0UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmax evaluation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 9, 8, 7, 6, 5, 4, 3, 2, 1 };

      const size_t maximum = argmax( vec );

      checkSize    ( vec, 9UL );
      checkCapacity( vec, 9UL );
      checkNonZeros( vec, 9UL );

      if( maximum != 0UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmax evaluation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

      const size_t maximum = argmax( vec );

      checkSize    ( vec, 9UL );
      checkCapacity( vec, 9UL );
      checkNonZeros( vec, 9UL );

      if( maximum != 8UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmax evaluation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 8\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 8, 7, 6, 5, 9, 4, 3, 2, 1 };

      const size_t maximum = argmax( vec );

      checkSize    ( vec, 9UL );
      checkCapacity( vec, 9UL );
      checkNonZeros( vec, 9UL );

      if( maximum != 4UL ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Argmax evaluation failed\n"
             << " Details:\n"
             << "   Result: " << maximum << "\n"
             << "   Expected result: 4\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c l1Norm() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c l1Norm() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testL1Norm()
{
   test_ = "l1Norm() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const int norm = blaze::l1Norm( vec );

      if( !isEqual( norm, 0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L1 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 7UL, 0 );

      const int norm = blaze::l1Norm( vec );

      if( !isEqual( norm, 0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L1 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0, -1, 2, -2, 0, 0, -1, 0, 1, 0 };

      const int norm = blaze::l1Norm( vec );

      if( !isEqual( norm, 7 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L1 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 7\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c l2Norm() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c l2Norm() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testL2Norm()
{
   test_ = "l2Norm() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double norm = blaze::l2Norm( vec );

      if( !isEqual( norm, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L2 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 7UL, 0 );

      const double norm = blaze::l2Norm( vec );

      if( !isEqual( norm, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L2 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0, -1, 2, -2, 2, 1, -1, 0, 1, 0 };

      const double norm = blaze::l2Norm( vec );

      if( !isEqual( norm, 4.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L2 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 4\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c l3Norm() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c l3Norm() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testL3Norm()
{
   test_ = "l3Norm() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double norm = blaze::l3Norm( vec );

      if( !isEqual( norm, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L3 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 7UL, 0 );

      const double norm = l3Norm( vec );

      if( !isEqual( norm, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L3 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0, -1, 2, -2, 2, 0, -1, 0, 1, 0 };

      const double norm = blaze::l3Norm( vec );

      if( !isEqual( norm, 3.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L3 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 3\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c l4Norm() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c l4Norm() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testL4Norm()
{
   test_ = "l4Norm() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double norm = blaze::l4Norm( vec );

      if( !isEqual( norm, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L4 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 7UL, 0 );

      const double norm = blaze::l4Norm( vec );

      if( !isEqual( norm, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L4 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 0, 2, 0, -2, 2, -1, 0, -2, 0, 2 };

      const double norm = blaze::l4Norm( vec );

      if( !isEqual( norm, 3.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: L4 norm computation failed\n"
             << " Details:\n"
             << "   Result: " << norm << "\n"
             << "   Expected result: 3\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c lpNorm() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c lpNorm() function for dense vectors. In case an
// error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testLpNorm()
{
   test_ = "lpNorm() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double norm1 = blaze::lpNorm( vec, 2 );
      const double norm2 = blaze::lpNorm<2UL>( vec );

      if( !isEqual( norm1, 0.0 ) || !isEqual( norm2, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Lp norm computation failed\n"
             << " Details:\n"
             << "   lpNorm<2>(): " << norm1 << "\n"
             << "   lpNorm(2): " << norm2 << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 7UL, 0 );

      const double norm1 = blaze::lpNorm( vec, 2 );
      const double norm2 = blaze::lpNorm<2UL>( vec );

      if( !isEqual( norm1, 0.0 ) || !isEqual( norm2, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Lp norm computation failed\n"
             << " Details:\n"
             << "   lpNorm<2>(): " << norm1 << "\n"
             << "   lpNorm(2): " << norm2 << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 10UL );
      randomize( vec, -5, 5 );

      const int norm1( blaze::lpNorm( vec, 1 ) );
      const int norm2( blaze::lpNorm<1UL>( vec ) );
      const int norm3( blaze::l1Norm( vec ) );

      if( !isEqual( norm1, norm3 ) || !isEqual( norm2, norm3 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Lp norm computation failed\n"
             << " Details:\n"
             << "   lpNorm<1>(): " << norm1 << "\n"
             << "   lpNorm(1): " << norm2 << "\n"
             << "   Expected result: " << norm3 << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 10UL );
      randomize( vec, -5, 5 );

      const double norm1( blaze::lpNorm( vec, 2 ) );
      const double norm2( blaze::lpNorm<2UL>( vec ) );
      const double norm3( blaze::l2Norm( vec ) );

      if( !isEqual( norm1, norm3 ) || !isEqual( norm2, norm3 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Lp norm computation failed\n"
             << " Details:\n"
             << "   lpNorm<2>(): " << norm1 << "\n"
             << "   lpNorm(2): " << norm2 << "\n"
             << "   Expected result: " << norm3 << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 10UL );
      randomize( vec, -5, 5 );

      const double norm1( blaze::lpNorm( vec, 3 ) );
      const double norm2( blaze::lpNorm<3UL>( vec ) );
      const double norm3( blaze::l3Norm( vec ) );

      if( !isEqual( norm1, norm3 ) || !isEqual( norm2, norm3 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Lp norm computation failed\n"
             << " Details:\n"
             << "   lpNorm<3>(): " << norm1 << "\n"
             << "   lpNorm(3): " << norm2 << "\n"
             << "   Expected result: " << norm3 << "\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 10UL );
      randomize( vec, -5, 5 );

      const double norm1( blaze::lpNorm( vec, 4 ) );
      const double norm2( blaze::lpNorm<4UL>( vec ) );
      const double norm3( blaze::l4Norm( vec ) );

      if( !isEqual( norm1, norm3 ) || !isEqual( norm2, norm3 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Lp norm computation failed\n"
             << " Details:\n"
             << "   lpNorm<4>(): " << norm1 << "\n"
             << "   lpNorm(4): " << norm2 << "\n"
             << "   Expected result: " << norm3 << "\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c length() and \c sqrLength() functions for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c length() and \c sqrLength() functions for dense
// vectors. In case an error is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testLength()
{
   test_ = "length() and sqrLength() functions";

   {
      blaze::DynamicVector<double,blaze::rowVector> vec;

      // Computing the vector length
      const double len( length( vec ) );

      if( !blaze::equal( len, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Length computation failed\n"
             << " Details:\n"
             << "   Result: " << len << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }

      // Computing the vector square length
      const double sqrlen( sqrLength( vec ) );

      if( !blaze::equal( sqrlen, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Square length computation failed\n"
             << " Details:\n"
             << "   Result: " << sqrlen << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<double,blaze::rowVector> vec( 2UL );
      vec[0] = 0.0;
      vec[1] = 0.0;

      // Computing the vector length
      const double len( length( vec ) );

      if( !blaze::equal( len, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Length computation failed\n"
             << " Details:\n"
             << "   Result: " << len << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }

      // Computing the vector square length
      const double sqrlen( sqrLength( vec ) );

      if( !blaze::equal( sqrlen, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Square length computation failed\n"
             << " Details:\n"
             << "   Result: " << sqrlen << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<double,blaze::rowVector> vec( 2UL );
      vec[0] = 3.0;
      vec[1] = 4.0;

      // Computing the vector length
      const double len( length( vec ) );

      if( !blaze::equal( len, 5.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Length computation failed\n"
             << " Details:\n"
             << "   Result: " << len << "\n"
             << "   Expected result: 5\n";
         throw std::runtime_error( oss.str() );
      }

      // Computing the vector square length
      const double sqrlen( sqrLength( vec ) );

      if( !blaze::equal( sqrlen, 25.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Square length computation failed\n"
             << " Details:\n"
             << "   Result: " << sqrlen << "\n"
             << "   Expected result: 25\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c mean() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c mean() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testMean()
{
   test_ = "mean() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 5UL, 0 );

      const double mean = blaze::mean( vec );

      if( !isEqual( mean, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Mean computation failed\n"
             << " Details:\n"
             << "   Result: " << mean << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, 4, 3, 6, 7 };

      const double mean = blaze::mean( vec );

      if( !isEqual( mean, 4.2 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Mean computation failed\n"
             << " Details:\n"
             << "   Result: " << mean << "\n"
             << "   Expected result: 4.2\n";
         throw std::runtime_error( oss.str() );
      }
   }

   try {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double mean = blaze::mean( vec );

      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Mean computation of empty vector succeeded\n"
          << " Details:\n"
          << "   Result:\n" << mean << "\n";
      throw std::runtime_error( oss.str() );
   }
   catch( std::invalid_argument& ) {}
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c var() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c var() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testVar()
{
   test_ = "var() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 5UL, 0 );

      const double var = blaze::var( vec );

      if( !isEqual( var, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Variance computation failed\n"
             << " Details:\n"
             << "   Result: " << var << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, 4, 3, 6, 7 };

      const double var = blaze::var( vec );

      if( !isEqual( var, 5.7 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Variance computation failed\n"
             << " Details:\n"
             << "   Result: " << var << "\n"
             << "   Expected result: 5.7\n";
         throw std::runtime_error( oss.str() );
      }
   }

   try {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double var = blaze::var( vec );

      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Variance computation of empty vector succeeded\n"
          << " Details:\n"
          << "   Result:\n" << var << "\n";
      throw std::runtime_error( oss.str() );
   }
   catch( std::invalid_argument& ) {}

   try {
      blaze::DynamicVector<int,blaze::rowVector> vec( 1UL );

      const double var = blaze::var( vec );

      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Variance computation of 1D vector succeeded\n"
          << " Details:\n"
          << "   Result:\n" << var << "\n";
      throw std::runtime_error( oss.str() );
   }
   catch( std::invalid_argument& ) {}
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c stddev() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c stddev() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testStdDev()
{
   test_ = "stddev() function";

   {
      blaze::DynamicVector<int,blaze::rowVector> vec( 5UL, 0 );

      const double stddev = blaze::stddev( vec );

      if( !isEqual( stddev, 0.0 ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Standard deviation computation failed\n"
             << " Details:\n"
             << "   Result: " << stddev << "\n"
             << "   Expected result: 0\n";
         throw std::runtime_error( oss.str() );
      }
   }

   {
      blaze::DynamicVector<int,blaze::rowVector> vec{ 1, 4, 3, 6, 7 };

      const double stddev = blaze::stddev( vec );

      if( !isEqual( stddev, std::sqrt(5.7) ) ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Standard deviation computation failed\n"
             << " Details:\n"
             << "   Result: " << stddev << "\n"
             << "   Expected result: sqrt(5.7)\n";
         throw std::runtime_error( oss.str() );
      }
   }

   try {
      blaze::DynamicVector<int,blaze::rowVector> vec;

      const double stddev = blaze::stddev( vec );

      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Standard deviation computation of empty vector succeeded\n"
          << " Details:\n"
          << "   Result:\n" << stddev << "\n";
      throw std::runtime_error( oss.str() );
   }
   catch( std::invalid_argument& ) {}

   try {
      blaze::DynamicVector<int,blaze::rowVector> vec( 1UL );

      const double stddev = blaze::stddev( vec );

      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Standard deviation computation of 1D vector succeeded\n"
          << " Details:\n"
          << "   Result:\n" << stddev << "\n";
      throw std::runtime_error( oss.str() );
   }
   catch( std::invalid_argument& ) {}
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the \c softmax() function for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the \c softmax() function for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testSoftmax()
{
   test_ = "softmax() function";

   blaze::DynamicVector<double,blaze::rowVector> a( 4UL );
   randomize( a, -5.0, 5.0 );

   const auto b = softmax( a );

   if( b[0] <= 0.0 || b[0] > 1.0 ||
       b[1] <= 0.0 || b[1] > 1.0 ||
       b[2] <= 0.0 || b[2] > 1.0 ||
       b[3] <= 0.0 || b[3] > 1.0 ||
       !isEqual( sum( b ), 1.0 ) ) {
      std::ostringstream oss;
      oss << " Test: " << test_ << "\n"
          << " Error: Softmax computation failed\n"
          << " Details:\n"
          << "   Result: " << sum( b ) << "\n"
          << "   Expected result: 1\n";
      throw std::runtime_error( oss.str() );
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the left-shift operator for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the left-shift operator for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testLeftShift()
{
   test_ = "Left-shift operator";


   //=====================================================================================
   // Uniform left-shift tests
   //=====================================================================================

   // Uniform left-shift of an empty vector
   {
      blaze::DynamicVector<unsigned int> a;

      blaze::DynamicVector<unsigned int> b( a << 2U );

      checkSize    ( b, 0UL );
      checkCapacity( b, 0UL );
      checkNonZeros( b, 0UL );
   }

   // Uniform left-shift of a general vector
   {
      blaze::DynamicVector<unsigned int> a{ 1U, 2U, 4U, 8U, 16U, 32U, 64U, 128U, 256U };

      blaze::DynamicVector<unsigned int> b( a << 2U );

      checkSize    ( b, 9UL );
      checkCapacity( b, 9UL );
      checkNonZeros( b, 9UL );

      if( b[0] !=   4U || b[1] !=   8U || b[2] !=  16U || b[3] !=   32U || b[4] != 64U ||
          b[5] != 128U || b[6] != 256U || b[7] != 512U || b[8] != 1024U ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Uniform left-shift operation failed\n"
             << " Details:\n"
             << "   Result:\n" << b << "\n"
             << "   Expected result:\n( 4 8 16 32 64 128 256 512 1024 )\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Uniform left-shift assignment
   {
      blaze::DynamicVector<unsigned int> a{ 1U, 2U, 4U, 8U, 16U, 32U, 64U, 128U, 256U };

      a <<= 2U;

      checkSize    ( a, 9UL );
      checkCapacity( a, 9UL );
      checkNonZeros( a, 9UL );

      if( a[0] !=   4U || a[1] !=   8U || a[2] !=  16U || a[3] !=   32U || a[4] != 64U ||
          a[5] != 128U || a[6] != 256U || a[7] != 512U || a[8] != 1024U ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Uniform left-shift assignment failed\n"
             << " Details:\n"
             << "   Result:\n" << a << "\n"
             << "   Expected result:\n( 4 8 16 32 64 128 256 512 1024 )\n";
         throw std::runtime_error( oss.str() );
      }
   }


   //=====================================================================================
   // Elementwise left-shift tests
   //=====================================================================================

   // Elementwise left-shift of an empty vector
   {
      blaze::DynamicVector<unsigned int> a;
      blaze::DynamicVector<unsigned int> b;

      blaze::DynamicVector<unsigned int> c( a << b );

      checkSize    ( b, 0UL );
      checkCapacity( b, 0UL );
      checkNonZeros( b, 0UL );
   }

   // Elementwise left-shift of a general vector
   {
      blaze::DynamicVector<unsigned int> a{ 1U, 2U, 4U, 8U, 16U, 32U, 64U, 128U, 256U };
      blaze::DynamicVector<unsigned int> b{ 1U, 2U, 1U, 2U, 1U, 2U, 1U, 2U, 1U };

      blaze::DynamicVector<unsigned int> c( a << b );

      checkSize    ( c, 9UL );
      checkCapacity( c, 9UL );
      checkNonZeros( c, 9UL );

      if( c[0] !=   2U || c[1] !=   8U || c[2] !=   8U || c[3] !=  32U || c[4] != 32U ||
          c[5] != 128U || c[6] != 128U || c[7] != 512U || c[8] != 512U ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Elementwise left-shift operation failed\n"
             << " Details:\n"
             << "   Result:\n" << c << "\n"
             << "   Expected result:\n( 2 8 8 32 32 128 128 512 512 )\n";
         throw std::runtime_error( oss.str() );
      }
   }

   // Elementwise left-shift assignment
   {
      blaze::DynamicVector<unsigned int> a{ 1U, 2U, 4U, 8U, 16U, 32U, 64U, 128U, 256U };
      blaze::DynamicVector<unsigned int> b{ 1U, 2U, 1U, 2U, 1U, 2U, 1U, 2U, 1U };

      a <<= b;

      checkSize    ( a, 9UL );
      checkCapacity( a, 9UL );
      checkNonZeros( a, 9UL );

      if( a[0] !=   2U || a[1] !=   8U || a[2] !=   8U || a[3] !=  32U || a[4] != 32U ||
          a[5] != 128U || a[6] != 128U || a[7] != 512U || a[8] != 512U ) {
         std::ostringstream oss;
         oss << " Test: " << test_ << "\n"
             << " Error: Elementwise left-shift assignment failed\n"
             << " Details:\n"
             << "   Result:\n" << a << "\n"
             << "   Expected result:\n( 2 8 8 32 32 128 128 512 512 )\n";
         throw std::runtime_error( oss.str() );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Test of the right-shift operator for dense vectors.
//
// \return void
// \exception std::runtime_error Error detected.
//
// This function performs a test of the right-shift operator for dense vectors. In case an error
// is detected, a \a std::runtime_error exception is thrown.
*/
void OperationTest::testRightShift()
{
   //=====================================================================================
   // Uniform right-shift tests
   //=====================================================================================

   {
      test_ = "Uniform right-shift operator";

      // Uniform right-shift of an empty vector
      {
         blaze::DynamicVector<unsigned int> a;

         blaze::DynamicVector<unsigned int> b( a >> 2U );

         checkSize    ( b, 0UL );
         checkCapacity( b, 0UL );
         checkNonZeros( b, 0UL );
      }

      // Uniform right-shift of a general vector
      {
         blaze::DynamicVector<unsigned int> a{ 4U, 8U, 16U, 32U, 64U, 128U, 256U, 512U, 1024U };

         blaze::DynamicVector<unsigned int> b( a >> 2U );

         checkSize    ( b, 9UL );
         checkCapacity( b, 9UL );
         checkNonZeros( b, 9UL );

         if( b[0] !=  1U || b[1] !=  2U || b[2] !=   4U || b[3] !=   8U || b[4] != 16U ||
             b[5] != 32U || b[6] != 64U || b[7] != 128U || b[8] != 256U ) {
            std::ostringstream oss;
            oss << " Test: " << test_ << "\n"
                << " Error: Uniform right-shift operation failed\n"
                << " Details:\n"
                << "   Result:\n" << b << "\n"
                << "   Expected result:\n( 1 2 4 8 16 32 64 128 256 )\n";
            throw std::runtime_error( oss.str() );
         }
      }

      // Uniform right-shift assignment
      {
         blaze::DynamicVector<unsigned int> a{ 4U, 8U, 16U, 32U, 64U, 128U, 256U, 512U, 1024U };

         a >>= 2U;

         checkSize    ( a, 9UL );
         checkCapacity( a, 9UL );
         checkNonZeros( a, 9UL );

         if( a[0] !=  1U || a[1] !=  2U || a[2] !=   4U || a[3] !=   8U || a[4] != 16U ||
             a[5] != 32U || a[6] != 64U || a[7] != 128U || a[8] != 256U ) {
            std::ostringstream oss;
            oss << " Test: " << test_ << "\n"
                << " Error: Uniform right-shift assignment failed\n"
                << " Details:\n"
                << "   Result:\n" << a << "\n"
                << "   Expected result:\n( 1 2 4 8 16 32 64 128 256 )\n";
            throw std::runtime_error( oss.str() );
         }
      }
   }


   //=====================================================================================
   // Elementwise right-shift tests
   //=====================================================================================

   {
      test_ = "Elementwise right-shift operator";

      // Elementwise right-shift of an empty vector
      {
         blaze::DynamicVector<unsigned int> a;
         blaze::DynamicVector<unsigned int> b;

         blaze::DynamicVector<unsigned int> c( a >> b );

         checkSize    ( b, 0UL );
         checkCapacity( b, 0UL );
         checkNonZeros( b, 0UL );
      }

      // Elementwise right-shift of a general vector
      {
         blaze::DynamicVector<unsigned int> a{ 4U, 8U, 16U, 32U, 64U, 128U, 256U, 512U, 1024U };
         blaze::DynamicVector<unsigned int> b{ 1U, 2U,  1U, 2U, 1U, 2U, 1U, 2U, 1U };

         blaze::DynamicVector<unsigned int> c( a >> b );

         checkSize    ( c, 9UL );
         checkCapacity( c, 9UL );
         checkNonZeros( c, 9UL );

         if( c[0] !=  2U || c[1] !=   2U || c[2] !=   8U || c[3] !=   8U || c[4] != 32U ||
             c[5] != 32U || c[6] != 128U || c[7] != 128U || c[8] != 512U ) {
            std::ostringstream oss;
            oss << " Test: " << test_ << "\n"
                << " Error: Elementwise right-shift operation failed\n"
                << " Details:\n"
                << "   Result:\n" << c << "\n"
                << "   Expected result:\n( 2 2 8 8 32 32 128 128 512 )\n";
            throw std::runtime_error( oss.str() );
         }
      }

      // Elementwise right-shift assignment
      {
         blaze::DynamicVector<unsigned int> a{ 4U, 8U, 16U, 32U, 64U, 128U, 256U, 512U, 1024U };
         blaze::DynamicVector<unsigned int> b{ 1U, 2U,  1U, 2U, 1U, 2U, 1U, 2U, 1U };

         a >>= b;

         checkSize    ( a, 9UL );
         checkCapacity( a, 9UL );
         checkNonZeros( a, 9UL );

         if( a[0] !=  2U || a[1] !=   2U || a[2] !=   8U || a[3] !=   8U || a[4] != 32U ||
             a[5] != 32U || a[6] != 128U || a[7] != 128U || a[8] != 512U ) {
            std::ostringstream oss;
            oss << " Test: " << test_ << "\n"
                << " Error: Elementwise right-shift assignment failed\n"
                << " Details:\n"
                << "   Result:\n" << a << "\n"
                << "   Expected result:\n( 2 2 8 8 32 32 128 128 512 )\n";
            throw std::runtime_error( oss.str() );
         }
      }
   }
}
//*************************************************************************************************

} // namespace densevector

} // namespace mathtest

} // namespace blazetest




//=================================================================================================
//
//  MAIN FUNCTION
//
//=================================================================================================

//*************************************************************************************************
int main()
{
   std::cout << "   Running DenseVector operation test..." << std::endl;

   try
   {
      RUN_DENSEVECTOR_OPERATION_TEST;
   }
   catch( std::exception& ex ) {
      std::cerr << "\n\n ERROR DETECTED during DenseVector operation test:\n"
                << ex.what() << "\n";
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
//*************************************************************************************************
