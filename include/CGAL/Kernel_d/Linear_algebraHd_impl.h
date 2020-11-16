// Copyright (c) 1997-2000
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v5.1.1/Kernel_d/include/CGAL/Kernel_d/Linear_algebraHd_impl.h $
// $Id: Linear_algebraHd_impl.h 0779373 2020-03-26T13:31:46+01:00 Sébastien Loriot
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Michael Seel <seel@mpi-sb.mpg.de>
//---------------------------------------------------------------------
// file generated by notangle from Linear_algebra.lw
// please debug or modify noweb file
// based on LEDA architecture by S. Naeher, C. Uhrig
// coding: K. Mehlhorn, M. Seel
// debugging and templatization: M. Seel
//---------------------------------------------------------------------
#ifndef CGAL_LINEAR_ALGEBRAHD_C
#define CGAL_LINEAR_ALGEBRAHD_C
namespace CGAL {

template <class RT_, class AL_>
bool Linear_algebraHd<RT_,AL_>::
linear_solver(const Matrix& A, const Vector& b,
              Vector& x, RT& D,
              Matrix& spanning_vectors, Vector& c)
{
  bool solvable = true;
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }


  for(i = rank; i < rows && C(i,cols) == 0; ++i) {}
  if (i < rows)
  { solvable = false; c = L.row(i); }

  if (solvable) {
    x = Vector(cols);
    D = denom;
    for(i = rank - 1; i >= 0; i--) {
      RT_ h = C(i,cols) * D;
      for (j = i + 1; j < rank; j++) {
        h -= C(i,j)*x[var[j]];
      }
      x[var[i]]= h / C(i,i);
    }
    #ifdef CGAL_LA_SELFTEST
      CGAL_assertion( (M*x).is_zero() );
    #endif

    int defect = cols - rank; // dimension of kernel
    spanning_vectors = Matrix(cols,defect);

    if (defect > 0) {
      for(int l=0; l < defect; l++) {
        spanning_vectors(var[rank + l],l)=D;
        for(i = rank - 1; i >= 0 ; i--) {
          RT_ h = - C(i,rank + l)*D;
          for ( j= i + 1; j<rank; j++)
            h -= C(i,j)*spanning_vectors(var[j],l);
          spanning_vectors(var[i],l)= h / C(i,i);
        }
    #ifdef CGAL_LA_SELFTEST
          /* we check whether the $l$ - th spanning vector is a solution
             of the homogeneous system */
          CGAL_assertion( (M*spanning_vectors.column(l)).is_zero() );
    #endif
      }
    }

  }
  return solvable;
}

template <class RT_, class AL_>
RT_ Linear_algebraHd<RT_,AL_>::
determinant(const Matrix& A)
{
  if (A.row_dimension() != A.column_dimension())
    CGAL_error_msg(      "determinant(): only square matrices are legal inputs.");
  Vector b(A.row_dimension()); // zero - vector
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }



  if (rank < rows)
    return 0;
  else
    return RT(sign) * denom;
}


template <class RT_, class AL_>
RT_ Linear_algebraHd<RT_,AL_>::
determinant(const Matrix& A,
            Matrix& Ld, Matrix& Ud,
            std::vector<int>& q, Vector& c)
{
  if (A.row_dimension() != A.column_dimension())
    CGAL_error_msg(      "determinant(): only square matrices are legal inputs.");
  Vector b(A.row_dimension()); // zero - vector
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }



  if (rank < rows) {
    c = L.row(rows - 1);
    return 0;
  } else {
    Ld = L;
    Ud = Matrix(rows); // square
    for (i = 0; i < rows; i++)
      for(j = 0; j <rows; j++)
        Ud(i,j) = C(i,j);
    q = var;
    return RT(sign) * denom;
  }
}

template <class RT_, class AL_>
int  Linear_algebraHd<RT_,AL_>::
sign_of_determinant(const Matrix& M)
{ return CGAL_NTS sign(determinant(M)); }

template <class RT_, class AL_>
bool Linear_algebraHd<RT_,AL_>::
verify_determinant(const Matrix& A, RT_ D,
                   Matrix& L, Matrix& U,
                   const std::vector<int>& q, Vector& c)
{
  if ((int)q.size() != A.column_dimension())
    CGAL_error_msg(    "verify_determinant: q should be a permutation array \
    with index range [0,A.column_dimension() - 1].");
  int n = A.row_dimension();
  int i,j;
  if (D == 0) { /* we have $c^T \cdot A = 0$  */
    Vector zero(n);
    return  (transpose(A) * c == zero);
  } else {
    /* we check the conditions on |L| and |U| */
    if (L(0,0) != 1) return false;
    for (i = 0; i<n; i++) {
      for (j = 0; j < i; j++)
        if (U(i,j) != 0)
          return false;

      if (i > 0 && L(i,i) != U(i - 1,i - 1))
        return false;

      for (j = i + 1; j < n; j++)
        if (L(i,j) != 0)
          return false;
    }

    /* check whether $L \cdot A \cdot Q = U$ */
   Matrix LA = L * A;
   for (j = 0; j < n; j++)
     if (LA.column(q[j]) != U.column(j))
       return false;

    /* compute sign |s| of |Q| */
    int sign = 1;

    /* we chase the cycles of |q|. An even length cycle contributes - 1
       and vice versa */

    std::vector<bool> already_considered(n);

    for (i = 0; i < n; i++)
      already_considered[i] = false;

    for (i = 0; i < n; i++)
      already_considered[q[i]] = true;

    for (i = 0; i < n; i++)
      if (! already_considered[i])
        CGAL_error_msg("verify_determinant:q is not a permutation.");
      else
        already_considered[i] = false;

    for (i = 0; i < n; i++) {
      if (already_considered[i]) continue;

      /* we have found a new cycle with minimal element $i$. */
      int k = q[i];
      already_considered[i] =true;

      while (k != i) {
        sign = - sign;
        already_considered[k]= true;
        k = q[k];
      }
    }
    return (D == RT(sign) * U(n - 1,n - 1));
  }
}

template <class RT_, class AL_>
int Linear_algebraHd<RT_,AL_>::
independent_columns(const Matrix& A,
                    std::vector<int>& columns)
{
  Vector b(A.row_dimension()); // zero - vector
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }


  /* at this point we have:
     |C| has an $rank \times rank$ upper triangular matrix
     in its left upper corner;
     |var| tells us the columns of |A| corresponding to the
     dependent variables; */

  columns = std::vector<int>(rank);
  for(i = 0; i < rank; i++)
    columns[i] = var[i];
  return rank;
}


template <class RT_, class AL_>
int  Linear_algebraHd<RT_,AL_>::
rank(const Matrix& A)
{
  Vector b(A.row_dimension()); // zero - vector
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }


  return rank;
}

template <class RT_, class AL_>
bool  Linear_algebraHd<RT_,AL_>::
inverse(const Matrix& A, Matrix& inverse,
        RT_& D, Vector& c)
{
  if (A.row_dimension() != A.column_dimension())
    CGAL_error_msg("inverse: only square matrices are legal inputs.");
  Vector b(A.row_dimension()); // zero - vector
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }



  if (rank < rows) {
    // matrix is singular; return a vector $c$ with $c^T \cdot A = 0$.*/
    c = L.row(rows-1);
    return false;
  }
  D = denom;
  inverse = Matrix(rows); //square
  RT_ h;
  for(i = 0; i <rows; i++)
  {  // $i$-th column of inverse
    for (j = rows - 1; j >= 0; j--) {
      h = L (j,i) * D;
      for (int l = j + 1; l<rows; l++)
        h -= C(j,l)*inverse(var[l],i);
      inverse(var[j],i) = h / C(j,j);
    }
  }

  #ifdef CGAL_LA_SELFTEST
  if (A*inverse != Matrix(rows,Matrix::RT_val(1))*D)
    CGAL_error_msg("inverse(): matrix inverse computed incorrectly.");
  #endif

  return true;
}


template <class RT_, class AL_>
int  Linear_algebraHd<RT_,AL_>::
homogeneous_linear_solver(const Matrix &A,
                          Matrix& spanning_vectors)
/* returns the dimension of the solution space of the homogeneous system
   $Ax = 0$. The columns of spanning\_vectors span the solution space. */
{
  Vector b(A.row_dimension()); // zero - vector
  RT_ D;
  int i,j,k; // indices to step through the matrix
  int rows = A.row_dimension();
  int cols = A.column_dimension();

  /* at this point one might want to check whether the computation can
     be carried out with doubles, see section \ref{ optimization }. */

  CGAL_assertion_msg(b.dimension() == rows,
    "linear_solver: b has wrong dimension");

  Matrix C(rows,cols + 1);
  // the matrix in which we will calculate ($C = (A \vert b)$)

  /* copy |A| and |b| into |C| and L becomes the identity matrix */
  Matrix L(rows); // zero initialized
  for(i=0; i<rows; i++) {
    for(j=0; j<cols; j++)
      C(i,j)=A(i,j);
    C(i,cols)=b[i];
    L(i,i) = 1; // diagonal elements are 1
  }


  std::vector<int> var(cols);
  // column $j$ of |C| represents the |var[j]| - th variable
  // the array is indexed between |0| and |cols - 1|

  for(j=0; j<cols; j++)
    var[j]= j; // at the beginning, variable $x_j$ stands in column $j$

  RT_ denom = 1; // the determinant of an empty matrix is 1
  int sign = 1; // no interchanges yet
  int rank = 0; // we have not seen any non-zero row yet

  /* here comes the main loop */
  for(k=0; k<rows; k++) {
    bool non_zero_found = false;
    for(i = k; i < rows; i++) { // step through rows $k$ to |rows - 1|
      for (j = k ; j < cols && C(i,j) == 0; j++) {}
      // step through columns |k| to |cols - 1|
      if (j < cols) {
        non_zero_found = true;
        break;
      }
    }


    if ( non_zero_found ) {
      rank++; //increase the rank
      if (i != k) {
        sign = -sign;
        /* we interchange rows |k| and |i| of |L| and |C| */
        L.swap_rows(i,k); C.swap_rows(i,k);
      }
      if (j != k) {
        /* We interchange columns |k| and |j|, and
           store the exchange of variables in |var| */
        sign = - sign;
        C.swap_columns(j,k);
        std::swap(var[k],var[j]);
      }

      for(i = k + 1; i < rows; i++)
        for (j = 0; j <  rows; j++)  //and all columns of |L|
          L(i,j) = (L(i,j)*C(k,k) - C(i,k)*L(k,j))/denom;

      for(i = k + 1; i < rows; i++) {
        /* the following iteration uses and changes |C(i,k)| */
        RT temp = C(i,k);
        for (j = k; j <= cols; j++)
          C(i,j) = (C(i,j)*C(k,k) - temp*C(k,j))/denom;
      }
      denom = C(k,k);
      #ifdef CGAL_LA_SELFTEST
      for(i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
          RT Sum = 0;
          for (int l = 0; l < rows; l++)
            Sum += L(i,l)*A(l, var[j]);
          CGAL_assertion_msg( Sum  == C(i,j),
            "linear_solver: L*A*P different from C");
        }
        RT Sum = 0;
        for (int l = 0; l < rows; l++)
          Sum += L(i,l)*b[l];
          CGAL_assertion_msg( Sum  == C(i,cols),
            "linear_solver: L*A*P different from C");
      }
      #endif


    }
    else
      break;
  }


  Vector x;
  x = Vector(cols);
  D = denom;
  for(i = rank - 1; i >= 0; i--) {
    RT_ h = C(i,cols) * D;
    for (j = i + 1; j < rank; j++) {
      h -= C(i,j)*x[var[j]];
    }
    x[var[i]]= h / C(i,i);
  }
  #ifdef CGAL_LA_SELFTEST
    CGAL_assertion( (M*x).is_zero() );
  #endif

  int defect = cols - rank; // dimension of kernel
  spanning_vectors = Matrix(cols,defect);

  if (defect > 0) {
    for(int l=0; l < defect; l++) {
      spanning_vectors(var[rank + l],l)=D;
      for(i = rank - 1; i >= 0 ; i--) {
        RT_ h = - C(i,rank + l)*D;
        for ( j= i + 1; j<rank; j++)
          h -= C(i,j)*spanning_vectors(var[j],l);
        spanning_vectors(var[i],l)= h / C(i,i);
      }
  #ifdef CGAL_LA_SELFTEST
        /* we check whether the $l$ - th spanning vector is a solution
           of the homogeneous system */
        CGAL_assertion( (M*spanning_vectors.column(l)).is_zero() );
  #endif
    }
  }
;
  return defect;
}

template <class RT_, class AL_>
bool  Linear_algebraHd<RT_,AL_>::
homogeneous_linear_solver(const Matrix& A, Vector& x)
/* returns true if the homogeneous system $Ax = 0$ has a non - trivial
   solution and false otherwise. */
{
  x = Vector(A.row_dimension());
  Matrix spanning_vectors;
  int dimension = homogeneous_linear_solver(A,spanning_vectors);
  if (dimension == 0) return false;

  /* return first column of |spanning_vectors| */
  for (int i = 0; i < spanning_vectors.row_dimension(); i++)
    x[i] = spanning_vectors(i,0);
  return true;
}


template <class RT_, class AL_>
typename Linear_algebraHd<RT_,AL_>::Matrix
Linear_algebraHd<RT_,AL_>::transpose(const Matrix& M)
{
  int d1 = M.row_dimension();
  int d2 = M.column_dimension();
  Matrix result(d2,d1);
  for(int i = 0; i < d2; i++)
    for(int j = 0; j < d1; j++)
      result(i,j) = M(j,i);
  return result;
}


} //namespace CGAL
#endif //CGAL_LINEAR_ALGEBRAHD_C
