//=================================================================================================
/*!
//  \file blaze/math/expressions/DVecForEachExpr.h
//  \brief Header file for the dense vector for-each expression
//
//  Copyright (C) 2013 Klaus Iglberger - All Rights Reserved
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

#ifndef _BLAZE_MATH_EXPRESSIONS_DVECFOREACHEXPR_H_
#define _BLAZE_MATH_EXPRESSIONS_DVECFOREACHEXPR_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <iterator>
#include <utility>
#include <blaze/math/Aliases.h>
#include <blaze/math/constraints/DenseVector.h>
#include <blaze/math/constraints/TransposeFlag.h>
#include <blaze/math/Exception.h>
#include <blaze/math/expressions/Computation.h>
#include <blaze/math/expressions/DenseVector.h>
#include <blaze/math/expressions/Forward.h>
#include <blaze/math/expressions/VecForEachExpr.h>
#include <blaze/math/Functors.h>
#include <blaze/math/shims/Serial.h>
#include <blaze/math/SIMD.h>
#include <blaze/math/traits/ForEachExprTrait.h>
#include <blaze/math/traits/ForEachTrait.h>
#include <blaze/math/traits/SubvectorExprTrait.h>
#include <blaze/math/typetraits/IsAligned.h>
#include <blaze/math/typetraits/IsComputation.h>
#include <blaze/math/typetraits/IsExpression.h>
#include <blaze/math/typetraits/IsPadded.h>
#include <blaze/math/typetraits/RequiresEvaluation.h>
#include <blaze/math/typetraits/Size.h>
#include <blaze/math/typetraits/UnderlyingNumeric.h>
#include <blaze/system/Inline.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Reference.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/IntegralConstant.h>
#include <blaze/util/logging/FunctionTrace.h>
#include <blaze/util/mpl/And.h>
#include <blaze/util/mpl/If.h>
#include <blaze/util/mpl/Not.h>
#include <blaze/util/Template.h>
#include <blaze/util/Types.h>
#include <blaze/util/typetraits/HasMember.h>
#include <blaze/util/typetraits/IsSame.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DVECFOREACHEXPR
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Expression object for the dense vector forEach() function.
// \ingroup dense_vector_expression
//
// The DVecForEachExpr class represents the compile time expression for the evaluation of a
// custom operation on each element of a dense vector via the forEach() function.
*/
template< typename VT  // Type of the dense vector
        , typename OP  // Type of the custom operation
        , bool TF >    // Transpose flag
class DVecForEachExpr : public DenseVector< DVecForEachExpr<VT,OP,TF>, TF >
                      , private VecForEachExpr
                      , private Computation
{
 private:
   //**Type definitions****************************************************************************
   typedef ResultType_<VT>   RT;  //!< Result type of the dense vector expression.
   typedef ElementType_<VT>  ET;  //!< Element type of the dense vector expression.
   typedef ReturnType_<VT>   RN;  //!< Return type of the dense vector expression.

   //! Definition of the HasSIMDEnabled type trait.
   BLAZE_CREATE_HAS_DATA_OR_FUNCTION_MEMBER_TYPE_TRAIT( HasSIMDEnabled, simdEnabled );

   //! Definition of the HasLoad type trait.
   BLAZE_CREATE_HAS_DATA_OR_FUNCTION_MEMBER_TYPE_TRAIT( HasLoad, load );
   //**********************************************************************************************

   //**Serial evaluation strategy******************************************************************
   //! Compilation switch for the serial evaluation strategy of the for-each expression.
   /*! The \a useAssign compile time constant expression represents a compilation switch for
       the serial evaluation strategy of the for-each expression. In case the given dense
       vector expression of type \a VT requires an intermediate evaluation, \a useAssign will
       be set to 1 and the for-each expression will be evaluated via the \a assign function
       family. Otherwise \a useAssign will be set to 0 and the expression will be evaluated
       via the subscript operator. */
   enum : bool { useAssign = RequiresEvaluation<VT>::value };

   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT2 >
   struct UseAssign {
      enum : bool { value = useAssign };
   };
   /*! \endcond */
   //**********************************************************************************************

   //**Parallel evaluation strategy****************************************************************
   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the explicit application of the SFINAE principle.
   /*! The UseSMPAssign struct is a helper struct for the selection of the parallel evaluation
       strategy. In case either the target vector or the dense vector operand is not SMP
       assignable and the vector operand requires an intermediate evaluation, \a value is set
       to 1 and the expression specific evaluation strategy is selected. Otherwise \a value is
       set to 0 and the default strategy is chosen. */
   template< typename VT2 >
   struct UseSMPAssign {
      enum : bool { value = ( !VT2::smpAssignable || !VT::smpAssignable ) && useAssign };
   };
   /*! \endcond */
   //**********************************************************************************************

   //**SIMD support detection**********************************************************************
   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the detection of the SIMD capabilities of the given custom operation.
   struct UseSIMDEnabledFlag {
      enum : bool { value = OP::BLAZE_TEMPLATE simdEnabled<ET>() };
   };
   /*! \endcond */
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   typedef DVecForEachExpr<VT,OP,TF>   This;           //!< Type of this DVecForEachExpr instance.
   typedef ForEachTrait_<VT,OP>        ResultType;     //!< Result type for expression template evaluations.
   typedef TransposeType_<ResultType>  TransposeType;  //!< Transpose type for expression template evaluations.
   typedef ElementType_<ResultType>    ElementType;    //!< Resulting element type.
   typedef SIMDTrait_<ElementType>     SIMDType;       //!< Resulting SIMD element type.

   //! Return type for expression template evaluations.
   typedef decltype( std::declval<OP>()( std::declval<RN>() ) )  ReturnType;

   //! Data type for composite expression templates.
   typedef IfTrue_< useAssign, const ResultType, const DVecForEachExpr& >  CompositeType;

   //! Composite data type of the dense vector expression.
   typedef If_< IsExpression<VT>, const VT, const VT& >  Operand;

   //! Data type of the custom unary operation.
   typedef OP  Operation;
   //**********************************************************************************************

   //**ConstIterator class definition**************************************************************
   /*!\brief Iterator over the elements of the dense vector.
   */
   class ConstIterator
   {
    public:
      //**Type definitions*************************************************************************
      typedef std::random_access_iterator_tag  IteratorCategory;  //!< The iterator category.
      typedef ElementType                      ValueType;         //!< Type of the underlying elements.
      typedef ElementType*                     PointerType;       //!< Pointer return type.
      typedef ElementType&                     ReferenceType;     //!< Reference return type.
      typedef ptrdiff_t                        DifferenceType;    //!< Difference between two iterators.

      // STL iterator requirements
      typedef IteratorCategory  iterator_category;  //!< The iterator category.
      typedef ValueType         value_type;         //!< Type of the underlying elements.
      typedef PointerType       pointer;            //!< Pointer return type.
      typedef ReferenceType     reference;          //!< Reference return type.
      typedef DifferenceType    difference_type;    //!< Difference between two iterators.

      //! ConstIterator type of the left-hand side dense vector expression.
      typedef ConstIterator_<VT>  IteratorType;
      //*******************************************************************************************

      //**Constructor******************************************************************************
      /*!\brief Constructor for the ConstIterator class.
      //
      // \param it Iterator to the initial vector element.
      // \param op The custom unary operation.
      */
      explicit inline ConstIterator( IteratorType it, OP op )
         : it_( it )  // Iterator to the current vector element
         , op_( op )  // The custom unary operation
      {}
      //*******************************************************************************************

      //**Addition assignment operator*************************************************************
      /*!\brief Addition assignment operator.
      //
      // \param inc The increment of the iterator.
      // \return The incremented iterator.
      */
      inline ConstIterator& operator+=( size_t inc ) {
         it_ += inc;
         return *this;
      }
      //*******************************************************************************************

      //**Subtraction assignment operator**********************************************************
      /*!\brief Subtraction assignment operator.
      //
      // \param dec The decrement of the iterator.
      // \return The decremented iterator.
      */
      inline ConstIterator& operator-=( size_t dec ) {
         it_ -= dec;
         return *this;
      }
      //*******************************************************************************************

      //**Prefix increment operator****************************************************************
      /*!\brief Pre-increment operator.
      //
      // \return Reference to the incremented iterator.
      */
      inline ConstIterator& operator++() {
         ++it_;
         return *this;
      }
      //*******************************************************************************************

      //**Postfix increment operator***************************************************************
      /*!\brief Post-increment operator.
      //
      // \return The previous position of the iterator.
      */
      inline const ConstIterator operator++( int ) {
         return ConstIterator( it_++ );
      }
      //*******************************************************************************************

      //**Prefix decrement operator****************************************************************
      /*!\brief Pre-decrement operator.
      //
      // \return Reference to the decremented iterator.
      */
      inline ConstIterator& operator--() {
         --it_;
         return *this;
      }
      //*******************************************************************************************

      //**Postfix decrement operator***************************************************************
      /*!\brief Post-decrement operator.
      //
      // \return The previous position of the iterator.
      */
      inline const ConstIterator operator--( int ) {
         return ConstIterator( it_-- );
      }
      //*******************************************************************************************

      //**Element access operator******************************************************************
      /*!\brief Direct access to the element at the current iterator position.
      //
      // \return The resulting value.
      */
      inline ReturnType operator*() const {
         return op_( *it_ );
      }
      //*******************************************************************************************

      //**Load function****************************************************************************
      /*!\brief Access to the SIMD elements of the vector.
      //
      // \return The resulting SIMD element.
      */
      inline SIMDType load() const noexcept {
         return op_.load( it_.load() );
      }
      //*******************************************************************************************

      //**Equality operator************************************************************************
      /*!\brief Equality comparison between two ConstIterator objects.
      //
      // \param rhs The right-hand side iterator.
      // \return \a true if the iterators refer to the same element, \a false if not.
      */
      inline bool operator==( const ConstIterator& rhs ) const {
         return it_ == rhs.it_;
      }
      //*******************************************************************************************

      //**Inequality operator**********************************************************************
      /*!\brief Inequality comparison between two ConstIterator objects.
      //
      // \param rhs The right-hand side iterator.
      // \return \a true if the iterators don't refer to the same element, \a false if they do.
      */
      inline bool operator!=( const ConstIterator& rhs ) const {
         return it_ != rhs.it_;
      }
      //*******************************************************************************************

      //**Less-than operator***********************************************************************
      /*!\brief Less-than comparison between two ConstIterator objects.
      //
      // \param rhs The right-hand side iterator.
      // \return \a true if the left-hand side iterator is smaller, \a false if not.
      */
      inline bool operator<( const ConstIterator& rhs ) const {
         return it_ < rhs.it_;
      }
      //*******************************************************************************************

      //**Greater-than operator********************************************************************
      /*!\brief Greater-than comparison between two ConstIterator objects.
      //
      // \param rhs The right-hand side iterator.
      // \return \a true if the left-hand side iterator is greater, \a false if not.
      */
      inline bool operator>( const ConstIterator& rhs ) const {
         return it_ > rhs.it_;
      }
      //*******************************************************************************************

      //**Less-or-equal-than operator**************************************************************
      /*!\brief Less-than comparison between two ConstIterator objects.
      //
      // \param rhs The right-hand side iterator.
      // \return \a true if the left-hand side iterator is smaller or equal, \a false if not.
      */
      inline bool operator<=( const ConstIterator& rhs ) const {
         return it_ <= rhs.it_;
      }
      //*******************************************************************************************

      //**Greater-or-equal-than operator***********************************************************
      /*!\brief Greater-than comparison between two ConstIterator objects.
      //
      // \param rhs The right-hand side iterator.
      // \return \a true if the left-hand side iterator is greater or equal, \a false if not.
      */
      inline bool operator>=( const ConstIterator& rhs ) const {
         return it_ >= rhs.it_;
      }
      //*******************************************************************************************

      //**Subtraction operator*********************************************************************
      /*!\brief Calculating the number of elements between two iterators.
      //
      // \param rhs The right-hand side iterator.
      // \return The number of elements between the two iterators.
      */
      inline DifferenceType operator-( const ConstIterator& rhs ) const {
         return it_ - rhs.it_;
      }
      //*******************************************************************************************

      //**Addition operator************************************************************************
      /*!\brief Addition between a ConstIterator and an integral value.
      //
      // \param it The iterator to be incremented.
      // \param inc The number of elements the iterator is incremented.
      // \return The incremented iterator.
      */
      friend inline const ConstIterator operator+( const ConstIterator& it, size_t inc ) {
         return ConstIterator( it.it_ + inc );
      }
      //*******************************************************************************************

      //**Addition operator************************************************************************
      /*!\brief Addition between an integral value and a ConstIterator.
      //
      // \param inc The number of elements the iterator is incremented.
      // \param it The iterator to be incremented.
      // \return The incremented iterator.
      */
      friend inline const ConstIterator operator+( size_t inc, const ConstIterator& it ) {
         return ConstIterator( it.it_ + inc );
      }
      //*******************************************************************************************

      //**Subtraction operator*********************************************************************
      /*!\brief Subtraction between a ConstIterator and an integral value.
      //
      // \param it The iterator to be decremented.
      // \param dec The number of elements the iterator is decremented.
      // \return The decremented iterator.
      */
      friend inline const ConstIterator operator-( const ConstIterator& it, size_t dec ) {
         return ConstIterator( it.it_ - dec );
      }
      //*******************************************************************************************

    private:
      //**Member variables*************************************************************************
      IteratorType it_;  //!< Iterator to the current vector element.
      OP           op_;  //!< The custom unary operation.
      //*******************************************************************************************
   };
   //**********************************************************************************************

 public:
   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum : bool { simdEnabled = VT::simdEnabled &&
                               If_< HasSIMDEnabled<OP>, UseSIMDEnabledFlag, HasLoad<OP> >::value };

   //! Compilation switch for the expression template assignment strategy.
   enum : bool { smpAssignable = VT::smpAssignable };
   //**********************************************************************************************

   //**SIMD properties*****************************************************************************
   //! The number of elements packed within a single SIMD element.
   enum : size_t { SIMDSIZE = SIMDTrait<ElementType>::size };
   //**********************************************************************************************

   //**Constructor*********************************************************************************
   /*!\brief Constructor for the DVecForEachExpr class.
   //
   // \param dv The dense vector operand of the for-each expression.
   // \param op The custom unary operation.
   */
   explicit inline DVecForEachExpr( const VT& dv, OP op ) noexcept
      : dv_( dv )  // Dense vector of the for-each expression
      , op_( op )  // The custom unary operation
   {}
   //**********************************************************************************************

   //**Subscript operator**************************************************************************
   /*!\brief Subscript operator for the direct access to the vector elements.
   //
   // \param index Access index. The index has to be in the range \f$[0..N-1]\f$.
   // \return The resulting value.
   */
   inline ReturnType operator[]( size_t index ) const {
      BLAZE_INTERNAL_ASSERT( index < dv_.size(), "Invalid vector access index" );
      return op_( dv_[index] );
   }
   //**********************************************************************************************

   //**At function*********************************************************************************
   /*!\brief Checked access to the vector elements.
   //
   // \param index Access index. The index has to be in the range \f$[0..N-1]\f$.
   // \return The resulting value.
   // \exception std::out_of_range Invalid vector access index.
   */
   inline ReturnType at( size_t index ) const {
      if( index >= dv_.size() ) {
         BLAZE_THROW_OUT_OF_RANGE( "Invalid vector access index" );
      }
      return (*this)[index];
   }
   //**********************************************************************************************

   //**Load function*******************************************************************************
   /*!\brief Access to the SIMD elements of the vector.
   //
   // \param index Access index. The index has to be in the range \f$[0..N-1]\f$.
   // \return Reference to the accessed values.
   */
   BLAZE_ALWAYS_INLINE SIMDType load( size_t index ) const noexcept {
      BLAZE_INTERNAL_ASSERT( index < dv_.size()     , "Invalid vector access index" );
      BLAZE_INTERNAL_ASSERT( index % SIMDSIZE == 0UL, "Invalid vector access index" );
      return op_.load( dv_.load( index ) );
   }
   //**********************************************************************************************

   //**Begin function******************************************************************************
   /*!\brief Returns an iterator to the first non-zero element of the dense vector.
   //
   // \return Iterator to the first non-zero element of the dense vector.
   */
   inline ConstIterator begin() const {
      return ConstIterator( dv_.begin(), op_ );
   }
   //**********************************************************************************************

   //**End function********************************************************************************
   /*!\brief Returns an iterator just past the last non-zero element of the dense vector.
   //
   // \return Iterator just past the last non-zero element of the dense vector.
   */
   inline ConstIterator end() const {
      return ConstIterator( dv_.end(), op_ );
   }
   //**********************************************************************************************

   //**Size function*******************************************************************************
   /*!\brief Returns the current size/dimension of the vector.
   //
   // \return The size of the vector.
   */
   inline size_t size() const noexcept {
      return dv_.size();
   }
   //**********************************************************************************************

   //**Operand access******************************************************************************
   /*!\brief Returns the dense vector operand.
   //
   // \return The dense vector operand.
   */
   inline Operand operand() const noexcept {
      return dv_;
   }
   //**********************************************************************************************

   //**Operation access****************************************************************************
   /*!\brief Returns a copy of the custom operation.
   //
   // \return A copy of the custom operation.
   */
   inline Operation operation() const {
      return op_;
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression can alias with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case the expression can alias, \a false otherwise.
   */
   template< typename T >
   inline bool canAlias( const T* alias ) const noexcept {
      return IsComputation<VT>::value && dv_.canAlias( alias );
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression is aliased with the given address \a alias.
   //
   // \param alias The alias to be checked.
   // \return \a true in case an alias effect is detected, \a false otherwise.
   */
   template< typename T >
   inline bool isAliased( const T* alias ) const noexcept {
      return dv_.isAliased( alias );
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the operands of the expression are properly aligned in memory.
   //
   // \return \a true in case the operands are aligned, \a false if not.
   */
   inline bool isAligned() const noexcept {
      return dv_.isAligned();
   }
   //**********************************************************************************************

   //**********************************************************************************************
   /*!\brief Returns whether the expression can be used in SMP assignments.
   //
   // \return \a true in case the expression can be used in SMP assignments, \a false if not.
   */
   inline bool canSMPAssign() const noexcept {
      return dv_.canSMPAssign();
   }
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   Operand   dv_;  //!< Dense vector of the for-each expression.
   Operation op_;  //!< The custom unary operation.
   //**********************************************************************************************

   //**Assignment to dense vectors*****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense vector for-each
   // expression to a dense vector. Due to the explicit application of the SFINAE principle,
   // this function can only be selected by the compiler in case the operand requires an
   // intermediate evaluation and the underlying numeric data type of the operand and the
   // target vector are identical.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< And< UseAssign<VT2>
                               , IsSame< UnderlyingNumeric<VT>, UnderlyingNumeric<VT2> > > >
      assign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      assign( ~lhs, rhs.dv_ );
      assign( ~lhs, forEach( ~lhs, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to dense vectors*****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense vector for-each
   // expression to a dense vector. Due to the explicit application of the SFINAE principle,
   // this function can only be selected by the compiler in case the operand requires an
   // intermediate evaluation and the underlying numeric data type of the operand and the
   // target vector differ.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< And< UseAssign<VT2>
                               , Not< IsSame< UnderlyingNumeric<VT>, UnderlyingNumeric<VT2> > > > >
      assign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( serial( rhs.dv_ ) );
      assign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Assignment to sparse vectors****************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Assignment of a dense vector for-each expression to a sparse vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side sparse vector.
   // \param rhs The right-hand side for-each expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized assignment of a dense vector for-each
   // expression to a sparse vector. Due to the explicit application of the SFINAE principle,
   // this function can only be selected by the compiler in case the operand requires an
   // intermediate evaluation.
   */
   template< typename VT2 >  // Type of the target sparse vector
   friend inline EnableIf_< UseAssign<VT2> >
      assign( SparseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( serial( rhs.dv_ ) );
      assign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to dense vectors********************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Addition assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be added.
   // \return void
   //
   // This function implements the performance optimized addition assignment of a dense vector
   // for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the operand
   // requires an intermediate evaluation.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseAssign<VT2> >
      addAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( serial( rhs.dv_ ) );
      addAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Addition assignment to sparse vectors*******************************************************
   // No special implementation for the addition assignment to sparse vectors.
   //**********************************************************************************************

   //**Subtraction assignment to dense vectors*****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Subtraction assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized subtraction assignment of a dense
   // vector for-each expression to a dense vector. Due to the explicit application of the
   // SFINAE principle, this function can only be selected by the compiler in case the
   // operand requires an intermediate evaluation.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseAssign<VT2> >
      subAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( serial( rhs.dv_ ) );
      subAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Subtraction assignment to sparse vectors****************************************************
   // No special implementation for the subtraction assignment to sparse vectors.
   //**********************************************************************************************

   //**Multiplication assignment to dense vectors**************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Multiplication assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be multiplied.
   // \return void
   //
   // This function implements the performance optimized multiplication assignment of a dense
   // vector for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the operand requires
   // an intermediate evaluation.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseAssign<VT2> >
      multAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( serial( rhs.dv_ ) );
      multAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Multiplication assignment to sparse vectors*************************************************
   // No special implementation for the multiplication assignment to sparse vectors.
   //**********************************************************************************************

   //**Division assignment to dense vectors********************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief Division assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression divisor.
   // \return void
   //
   // This function implements the performance optimized division assignment of a dense
   // vector for-each expression to a dense vector. Due to the explicit application of the
   // SFINAE principle, this function can only be selected by the compiler in case the
   // operand requires an intermediate evaluation.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseAssign<VT2> >
      divAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( serial( rhs.dv_ ) );
      divAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**Division assignment to sparse vectors*******************************************************
   // No special implementation for the division assignment to sparse vectors.
   //**********************************************************************************************

   //**SMP assignment to dense vectors*************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized SMP assignment of a dense vector
   // for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the expression
   // specific parallel evaluation strategy is selected and the underlying numeric data
   // type of the operand and the target vector are identical
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< And< UseSMPAssign<VT2>
                               , IsSame< UnderlyingNumeric<VT>, UnderlyingNumeric<VT2> > > >
      smpAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      smpAssign( ~lhs, rhs.dv_ );
      smpAssign( ~lhs, rhs.op_( ~lhs ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP assignment to dense vectors*************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized SMP assignment of a dense vector
   // for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the expression
   // specific parallel evaluation strategy is selected and the underlying numeric data
   // type of the operand and the target vector differ.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< And< UseSMPAssign<VT2>
                               , Not< IsSame< UnderlyingNumeric<VT>, UnderlyingNumeric<VT2> > > > >
      smpAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( rhs.dv_ );
      smpAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP assignment to sparse vectors************************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP assignment of a dense vector for-each expression to a sparse vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side sparse vector.
   // \param rhs The right-hand side for-each expression to be assigned.
   // \return void
   //
   // This function implements the performance optimized SMP assignment of a dense vector
   // for-each expression to a sparse vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the expression
   // specific parallel evaluation strategy is selected.
   */
   template< typename VT2 >  // Type of the target sparse vector
   friend inline EnableIf_< UseSMPAssign<VT2> >
      smpAssign( SparseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( rhs.dv_ );
      smpAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP addition assignment to dense vectors****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP addition assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be added.
   // \return void
   //
   // This function implements the performance optimized SMP addition assignment of a dense
   // vector for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the expression
   // specific parallel evaluation strategy is selected.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseSMPAssign<VT2> >
      smpAddAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( rhs.dv_ );
      smpAddAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP addition assignment to sparse vectors***************************************************
   // No special implementation for the SMP addition assignment to sparse vectors.
   //**********************************************************************************************

   //**SMP subtraction assignment to dense vectors*************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP subtraction assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be subtracted.
   // \return void
   //
   // This function implements the performance optimized SMP subtraction assignment of a dense
   // vector for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the expression
   // specific parallel evaluation strategy is selected.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseSMPAssign<VT2> >
      smpSubAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( rhs.dv_ );
      smpSubAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP subtraction assignment to sparse vectors************************************************
   // No special implementation for the SMP subtraction assignment to sparse vectors.
   //**********************************************************************************************

   //**SMP multiplication assignment to dense vectors**********************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP multiplication assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression to be multiplied.
   // \return void
   //
   // This function implements the performance optimized SMP multiplication assignment of a
   // dense vector for-each expression to a dense vector. Due to the explicit application of
   // the SFINAE principle, this function can only be selected by the compiler in case the
   // expression specific parallel evaluation strategy is selected.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseSMPAssign<VT2> >
      smpMultAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( rhs.dv_ );
      smpMultAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP multiplication assignment to sparse vectors*********************************************
   // No special implementation for the SMP multiplication assignment to sparse vectors.
   //**********************************************************************************************

   //**SMP division assignment to dense vectors****************************************************
   /*! \cond BLAZE_INTERNAL */
   /*!\brief SMP division assignment of a dense vector for-each expression to a dense vector.
   // \ingroup dense_vector
   //
   // \param lhs The target left-hand side dense vector.
   // \param rhs The right-hand side for-each expression divisor.
   // \return void
   //
   // This function implements the performance optimized SMP division assignment of a dense
   // vector for-each expression to a dense vector. Due to the explicit application of the SFINAE
   // principle, this function can only be selected by the compiler in case the expression
   // specific parallel evaluation strategy is selected.
   */
   template< typename VT2 >  // Type of the target dense vector
   friend inline EnableIf_< UseSMPAssign<VT2> >
      smpDivAssign( DenseVector<VT2,TF>& lhs, const DVecForEachExpr& rhs )
   {
      BLAZE_FUNCTION_TRACE;

      BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( RT );
      BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( RT, TF );
      BLAZE_CONSTRAINT_MUST_BE_REFERENCE_TYPE( CompositeType_<RT> );

      BLAZE_INTERNAL_ASSERT( (~lhs).size() == rhs.size(), "Invalid vector sizes" );

      const RT tmp( rhs.dv_ );
      smpDivAssign( ~lhs, forEach( tmp, rhs.op_ ) );
   }
   /*! \endcond */
   //**********************************************************************************************

   //**SMP division assignment to sparse vectors***************************************************
   // No special implementation for the SMP division assignment to sparse vectors.
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_BE_DENSE_VECTOR_TYPE( VT );
   BLAZE_CONSTRAINT_MUST_BE_VECTOR_WITH_TRANSPOSE_FLAG( VT, TF );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Evaluates the given custom operation on each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \param op The custom operation.
// \return The custom operation applied to each single element of \a dv.
//
// The \a forEach() function evaluates the given custom operation on each element of the input
// vector \a dv. The function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a forEach() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = forEach( a, []( double a ){ return std::sqrt( a ); } );
   \endcode
*/
template< typename VT    // Type of the dense vector
        , bool TF        // Transpose flag
        , typename OP >  // Type of the custom operation
inline const DVecForEachExpr<VT,OP,TF> forEach( const DenseVector<VT,TF>& dv, OP op )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,OP,TF>( ~dv, op );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the square root of each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The square root of each single element of \a dv.
//
// The \a sqrt() function computes the square root of each element of the input vector \a dv. The
// function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a sqrt() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = sqrt( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Sqrt,TF> sqrt( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Sqrt,TF>( ~dv, Sqrt() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the inverse square root of each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The inverse square root of each single element of \a dv.
//
// The \a invsqrt() function computes the inverse square root of each element of the input vector
// \a dv. The function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a invsqrt() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = invsqrt( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,InvSqrt,TF> invsqrt( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,InvSqrt,TF>( ~dv, InvSqrt() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the cubic root of each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The cubic root of each single element of \a dv.
//
// The \a cbrt() function computes the cubic root of each element of the input vector \a dv. The
// function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a cbrt() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = cbrt( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Cbrt,TF> cbrt( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Cbrt,TF>( ~dv, Cbrt() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the inverse cubic root of each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The inverse cubic root of each single element of \a dv.
//
// The \a invcbrt() function computes the inverse cubic root of each element of the input vector
// \a dv. The function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a invcbrt() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = invcbrt( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,InvCbrt,TF> invcbrt( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,InvCbrt,TF>( ~dv, InvCbrt() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Applies the \a floor() function to each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The resulting dense vector.
//
// This function applies the \a floor() function to each element of the input vector \a dv. The
// function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a floor() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = floor( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Floor,TF> floor( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Floor,TF>( ~dv, Floor() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Applies the \a ceil() function to each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The resulting dense vector.
//
// This function applies the \a ceil() function to each element of the input vector \a dv. The
// function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a ceil() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = ceil( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Ceil,TF> ceil( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Ceil,TF>( ~dv, Ceil() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the exponential value for each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \param exp The exponent.
// \return The exponential value of each single element of \a dv.
//
// The \a pow() function computes the exponential value for each element of the input vector \a dv.
// The function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a pow() function:

   \code
   blaze::DynamicVector<double> A, B;
   // ... Resizing and initialization
   B = pow( A, 4.2 );
   \endcode
*/
template< typename VT   // Type of the dense vector
        , bool TF       // Transpose flag
        , typename T >  // Type of the exponent
inline const DVecForEachExpr<VT,Pow<T>,TF> pow( const DenseVector<VT,TF>& dv, T exp )
{
   BLAZE_FUNCTION_TRACE;

   BLAZE_CONSTRAINT_MUST_BE_NUMERIC_TYPE( T );

   return DVecForEachExpr<VT,Pow<T>,TF>( ~dv, Pow<T>( exp ) );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes \f$ e^x \f$ for each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The resulting dense vector.
//
// The \a exp() function computes \f$ e^x \f$ for each element of the input vector \a dv. The
// function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a exp() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = exp( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Exp,TF> exp( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Exp,TF>( ~dv, Exp() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the natural logarithm for each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The natural logarithm of each single element of \a dv.
//
// The \a log() function computes natural logarithm for each element of the input vector \a dv.
// The function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a log() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = log( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Log,TF> log( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Log,TF>( ~dv, Log() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the common logarithm for each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The common logarithm of each single element of \a dv.
//
// The \a log10() function computes common logarithm for each element of the input vector \a dv.
// The function returns an expression representing this operation.\n
// The following example demonstrates the use of the \a log10() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = log10( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Log10,TF> log10( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Log10,TF>( ~dv, Log10() );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Computes the sine for each single element of the dense vector \a dv.
// \ingroup dense_vector
//
// \param dv The input vector.
// \return The sine of each single element of \a dv.
//
// The \a sin() function computes sine for each element of the input vector \a dv. The function
// returns an expression representing this operation.\n
// The following example demonstrates the use of the \a sin() function:

   \code
   blaze::DynamicVector<double> a, b;
   // ... Resizing and initialization
   b = sin( a );
   \endcode
*/
template< typename VT  // Type of the dense vector
        , bool TF >    // Transpose flag
inline const DVecForEachExpr<VT,Sin,TF> sin( const DenseVector<VT,TF>& dv )
{
   BLAZE_FUNCTION_TRACE;

   return DVecForEachExpr<VT,Sin,TF>( ~dv, Sin() );
}
//*************************************************************************************************




//=================================================================================================
//
//  SIZE SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename VT, typename OP, bool TF >
struct Size< DVecForEachExpr<VT,OP,TF> > : public Size<VT>
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISALIGNED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename VT, typename OP, bool TF >
struct IsAligned< DVecForEachExpr<VT,OP,TF> >
   : public BoolConstant< IsAligned<VT>::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ISPADDED SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename VT, typename OP, bool TF >
struct IsPadded< DVecForEachExpr<VT,OP,TF> >
   : public BoolConstant< IsPadded<VT>::value >
{};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename VT, typename OP, bool TF, bool AF >
struct SubvectorExprTrait< DVecForEachExpr<VT,OP,TF>, AF >
{
 public:
   //**********************************************************************************************
   using Type = ForEachExprTrait_< SubvectorExprTrait_<const VT,AF>, OP >;
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
