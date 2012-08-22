//=================================================================================================
/*!
//  \file blaze/math/traits/SMatSMatAddTrait.h
//  \brief Header file for the SMatSMatAddTrait class template
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

#ifndef _BLAZE_MATH_TRAITS_SMATSMATADDTRAIT_H_
#define _BLAZE_MATH_TRAITS_SMATSMATADDTRAIT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/expressions/Forward.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/math/typetraits/IsSparseMatrix.h>
#include <blaze/util/InvalidType.h>
#include <blaze/util/SelectType.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Evaluation of the expression type of a sparse matrix/sparse matrix addition.
// \ingroup math_traits
//
// Via this type trait it is possible to evaluate the resulting expression type of a sparse
// matrix/sparse matrix addition. Given the two row-major sparse matrix types \a MT1 and
// \a MT2, the nested type \a Type corresponds to the resulting expression type. In case
// either \a MT1 or \a MT2 is not a row-major sparse matrix, the resulting data type
// \a Type is set to \a INVALID_TYPE.
*/
template< typename MT1    // Type of the left-hand side row-major sparse matrix
        , typename MT2 >  // Type of the right-hand side row-major sparse matrix
struct SMatSMatAddTrait
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SelectType< IsSparseMatrix<MT1>::value && IsRowMajorMatrix<MT1>::value &&
                                IsSparseMatrix<MT2>::value && IsRowMajorMatrix<MT2>::value
                              , SMatSMatAddExpr<MT1,MT2>, INVALID_TYPE >::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************

} // namespace blaze

#endif
