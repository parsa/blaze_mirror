//=================================================================================================
/*!
//  \file blaze/math/traits/DMatScalarMultTrait.h
//  \brief Header file for the DMatScalarMultTrait class template
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

#ifndef _BLAZE_MATH_TRAITS_DMATSCALARMULTTRAIT_H_
#define _BLAZE_MATH_TRAITS_DMATSCALARMULTTRAIT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/expressions/Forward.h>
#include <blaze/math/MathTrait.h>
#include <blaze/math/typetraits/BaseElementType.h>
#include <blaze/math/typetraits/IsDenseMatrix.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/util/InvalidType.h>
#include <blaze/util/typetraits/IsNumeric.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Auxiliary helper struct for the DMatScalarMultTrait trait.
// \ingroup math_traits
*/
template< typename MT
        , typename ST
        , bool Condition >
struct DMatScalarMultTraitHelper
{
 private:
   //**********************************************************************************************
   typedef typename MathTrait<typename BaseElementType<MT>::Type,ST>::MultType  ElementType;
   //**********************************************************************************************

 public:
   //**********************************************************************************************
   typedef DMatScalarMultExpr<MT,ElementType,false>  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the DMatScalarMultTraitHelper class template.
// \ingroup math_traits
*/
template< typename MT
        , typename ST >
struct DMatScalarMultTraitHelper<MT,ST,false>
{
 public:
   //**********************************************************************************************
   typedef INVALID_TYPE  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Evaluation of the expression type of a dense matrix/scalar multiplication.
// \ingroup math_traits
//
// Via this type trait it is possible to evaluate the resulting expression type of a dense
// matrix/scalar multiplication. Given the row-major dense matrix type \a MT and the scalar
// type \a ST, the nested type \a Type corresponds to the resulting expression type. In case
// either \a MT is not a row-major dense matrix type or \a ST is not a scalar type, the
// resulting \a Type is set to \a INVALID_TYPE.
*/
template< typename MT    // Type of the left-hand side dense matrix
        , typename ST >  // Type of the right-hand side scalar
struct DMatScalarMultTrait
{
 private:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   enum { condition = IsDenseMatrix<MT>::value && IsRowMajorMatrix<MT>::value &&
                      IsNumeric<ST>::value };
   /*! \endcond */
   //**********************************************************************************************

 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename DMatScalarMultTraitHelper<MT,ST,condition>::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************

} // namespace blaze

#endif
