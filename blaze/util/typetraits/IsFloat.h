//=================================================================================================
/*!
//  \file blaze/util/typetraits/IsFloat.h
//  \brief Header file for the IsFloat type trait
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

#ifndef _BLAZE_UTIL_TYPETRAITS_ISFLOAT_H_
#define _BLAZE_UTIL_TYPETRAITS_ISFLOAT_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <blaze/util/FalseType.h>
#include <blaze/util/TrueType.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Compile time check for single precision floating point types.
// \ingroup type_traits
//
// This type trait tests whether or not the given template parameter is of float type. In
// case the type is float (ignoring the cv-qualifiers), the \a value member enumeration is
// set to 1, the nested type definition \a Type is \a TrueType, and the class derives from
// \a TrueType. Otherwise \a value is set to 0, \a Type is \a FalseType, and the class
// derives from \a FalseType.

   \code
   blaze::IsFloat<float>::value          // Evaluates to 1
   blaze::IsFloat<const float>::Type     // Results in TrueType
   blaze::IsFloat<const volatile float>  // Is derived from TrueType
   blaze::IsFloat<double>::value         // Evaluates to 0
   blaze::IsFloat<const int>::Type       // Results in FalseType
   blaze::IsFloat<volatile short>        // Is derived from FalseType
   \endcode
*/
template< typename T >
struct IsFloat : public FalseType
{
 public:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   enum { value = 0 };
   typedef FalseType  Type;
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
//! Specialization of the IsFloat type trait for the plain 'float' type.
template<>
struct IsFloat<float> : public TrueType
{
 public:
   //**********************************************************************************************
   enum { value = 1 };
   typedef TrueType  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
//! Specialization of the IsFloat type trait for 'const float'.
template<>
struct IsFloat<const float> : public TrueType
{
 public:
   //**********************************************************************************************
   enum { value = 1 };
   typedef TrueType  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
//! Specialization of the IsFloat type trait for 'volatile float'.
template<>
struct IsFloat<volatile float> : public TrueType
{
 public:
   //**********************************************************************************************
   enum { value = 1 };
   typedef TrueType  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
//! Specialization of the IsFloat type trait for 'const volatile float'.
template<>
struct IsFloat<const volatile float> : public TrueType
{
 public:
   //**********************************************************************************************
   enum { value = 1 };
   typedef TrueType  Type;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
