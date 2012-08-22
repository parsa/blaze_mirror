//=================================================================================================
/*!
//  \file blaze/math/traits/SVecScalarDivTrait.h
//  \brief Header file for the SVecScalarDivTrait class template
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

#ifndef _BLAZE_MATH_TRAITS_SVECSCALARDIVTRAIT_H_
#define _BLAZE_MATH_TRAITS_SVECSCALARDIVTRAIT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/math/expressions/Forward.h>
#include <blaze/math/MathTrait.h>
#include <blaze/math/typetraits/BaseElementType.h>
#include <blaze/math/typetraits/IsSparseVector.h>
#include <blaze/math/typetraits/IsTransposeVector.h>
#include <blaze/util/InvalidType.h>
#include <blaze/util/SelectType.h>
#include <blaze/util/typetraits/IsFloatingPoint.h>
#include <blaze/util/typetraits/IsNumeric.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Auxiliary helper struct for the SVecScalarDivTrait trait.
// \ingroup math_traits
*/
template< typename VT
        , typename ST
        , bool Condition >
struct SVecScalarDivTraitHelper
{
 private:
   //**********************************************************************************************
   typedef typename MathTrait<typename BaseElementType<VT>::Type,ST>::DivType  ElementType;
   //**********************************************************************************************

 public:
   //**********************************************************************************************
   typedef typename SelectType< IsFloatingPoint<ElementType>::value
                              , SVecScalarMultExpr<VT,ElementType,false>
                              , SVecScalarDivExpr<VT,ElementType,false> >::Type  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the SVecScalarDivTraitHelper class template.
// \ingroup math_traits
*/
template< typename VT
        , typename ST >
struct SVecScalarDivTraitHelper<VT,ST,false>
{
 public:
   //**********************************************************************************************
   typedef INVALID_TYPE  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Evaluation of the expression type of a sparse vector/scalar division.
// \ingroup math_traits
//
// Via this type trait it is possible to evaluate the resulting expression type of a sparse
// vector/scalar division. Given the non-transpose sparse vector type \a VT and the scalar
// type \a ST, the nested type \a Type corresponds to the resulting expression type. In case
// either \a VT is not a non-transpose sparse vector type or \a ST is not a scalar type, the
// resulting \a Type is set to \a INVALID_TYPE.
*/
template< typename VT    // Type of the left-hand side sparse vector
        , typename ST >  // Type of the right-hand side scalar
struct SVecScalarDivTrait
{
 private:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   enum { condition = IsSparseVector<VT>::value && !IsTransposeVector<VT>::value &&
                      IsNumeric<ST>::value };
   /*! \endcond */
   //**********************************************************************************************

 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   typedef typename SVecScalarDivTraitHelper<VT,ST,condition>::Type  Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************

} // namespace blaze

#endif
