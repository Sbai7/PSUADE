// ************************************************************************
// Copyright (c) 2007   Lawrence Livermore National Security, LLC.
// Produced at the Lawrence Livermore National Laboratory.
// Written by the PSUADE team.
// All rights reserved.
//
// Please see the COPYRIGHT_and_LICENSE file for the copyright notice,
// disclaimer, contact information and the GNU Lesser General Public License.
//
// PSUADE is free software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License (as published by the Free Software
// Foundation) version 2.1 dated February 1999.
//
// PSUADE is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the terms and conditions of the GNU General
// Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
// ************************************************************************
// psMatrix functions (modified from Pelikan's (University of Cincinati) code
// AUTHOR : CHARLES TONG
// DATE   : 2008
// ************************************************************************
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Matrix.h"

//#define PS_DEBUG 1

// ************************************************************************
// Constructor
// ------------------------------------------------------------------------
psMatrix::psMatrix()
{
#ifdef PS_DEBUG
   printf("psMatrix constructor\n");
#endif
   nRows_ = 0;
   nCols_ = 0;
   Mat_ = NULL;
   status_ = 0;
   determinant_ = 0.0;
#ifdef PS_DEBUG
   printf("psMatrix constructor ends\n");
#endif
}

// ************************************************************************
// Copy Constructor by Bill Oliver
// ------------------------------------------------------------------------
psMatrix::psMatrix(const psMatrix & ma)
{
   int ii, jj;
   nRows_ = ma.nRows_;
   nCols_ = ma.nCols_;
   status_ = ma.status_;
   if (nRows_ > 0 && nCols_ > 0)
   {
      Mat_ = new double*[nRows_];
      for (ii = 0; ii < nRows_; ii++)
      {
         Mat_[ii] = new double[nCols_];
         for(jj = 0; jj < nCols_; jj++)
            Mat_[ii][jj] = ma.Mat_[ii][jj];
      }
   }
   determinant_ = ma.determinant_;
}

// ************************************************************************
// operator=  by Bill Oliver
// ------------------------------------------------------------------------
psMatrix & psMatrix::operator=(const psMatrix & ma)
{
   int ii, jj;
   if (this == &ma) return *this;

   if (Mat_ != NULL)
   {
      for(ii = 0; ii < nRows_; ii++) delete [] Mat_[ii];
      delete [] Mat_;
   }
   Mat_ = NULL;

   nRows_ = ma.nRows_;
   nCols_ = ma.nCols_;
   status_ = ma.status_;
   determinant_ = ma.determinant_;
   if (nRows_ > 0 && nCols_ > 0)
   {
      Mat_ = new double*[nRows_];
      for(ii = 0; ii < nRows_; ii++)
      {
         Mat_[ii] = new double[nCols_];
         for(jj = 0; jj < nCols_; jj++) Mat_[ii][jj] = ma.Mat_[ii][jj];
      }
   }
   return *this;
}


// ************************************************************************
// destructor
// ------------------------------------------------------------------------
psMatrix::~psMatrix()
{
#ifdef PS_DEBUG
   printf("psMatrix destructor\n");
#endif
   if (Mat_ != NULL)
   {
      for (int ii = 0; ii < nRows_; ii++)
         if (Mat_[ii] != NULL) delete [] Mat_[ii];
      delete [] Mat_;
   }
   nRows_ = nCols_ = 0;
   Mat_ = NULL;
#ifdef PS_DEBUG
   printf("psMatrix destructor ends\n");
#endif
}

// ************************************************************************
// get number of rows 
// ------------------------------------------------------------------------
int psMatrix::nrows()
{
   return nRows_;
}

// ************************************************************************
// get number of columns 
// ------------------------------------------------------------------------
int psMatrix::ncols()
{
   return nCols_;
}

// ************************************************************************
// load matrix from another matrix
// ------------------------------------------------------------------------
int psMatrix::load(psMatrix &inMat)
{
   int ii, jj;

#ifdef PS_DEBUG
   printf("psMatrix load\n");
#endif
   if (Mat_ != NULL)
   {
      for (ii = 0; ii < nRows_; ii++)
         if (Mat_[ii] != NULL) delete [] Mat_[ii];
      delete [] Mat_;
   }
   Mat_ = NULL;

   assert(this != &inMat);
   nRows_ = inMat.nrows();
   nCols_ = inMat.ncols();
   if (nRows_ > 0 && nCols_ > 0)
   {
      Mat_ = new double*[nRows_];
      assert(Mat_ != NULL);
      for (ii = 0; ii < nRows_; ii++)
      {
         Mat_[ii] = new double[nCols_];
         assert(Mat_[ii] != NULL);
         for (jj = 0; jj < nCols_; jj++) 
            Mat_[ii][jj] = inMat.getEntry(ii,jj);
      }
   }
   status_ = 0;
   determinant_ = inMat.determinant_;
#ifdef PS_DEBUG
   printf("psMatrix load ends\n");
#endif
   return 0;
}

// ************************************************************************
// set matrix dimension
// ------------------------------------------------------------------------
int psMatrix::setDim(int nrows, int ncols)
{
   int ii, jj;

   if (Mat_ != NULL)
   {
      for (ii = 0; ii < nRows_; ii++)
         if (Mat_[ii] != NULL) delete [] Mat_[ii];
      delete [] Mat_;
   }
   Mat_ = NULL;

   nRows_ = nrows;
   nCols_ = ncols;
   if (nRows_ <= 0 || nCols_ <= 0) return -1;
   Mat_ = new double*[nRows_];
   assert(Mat_ != NULL);
   for (ii = 0; ii < nRows_; ii++)
   {
      Mat_[ii] = new double[nCols_];
      assert(Mat_[ii] != NULL);
      for (jj = 0; jj < nCols_; jj++) Mat_[ii][jj] = 0.0;
   }
   status_ = 0;
   return 0;
}

// ************************************************************************
// set entry
// ------------------------------------------------------------------------
void psMatrix::setEntry(const int row, const int col, const double ddata)
{
   if (row < 0 || row >= nRows_ || col < 0 || col >= nCols_)
   {
      printf("psMatrix setEntry ERROR: index (%d,%d) out of range (%d,%d)\n",
             row, col, nRows_, nCols_);
      exit(1);
   }
   Mat_[row][col] = ddata;
   status_ = 0;
}

// ************************************************************************
// get entry
// ------------------------------------------------------------------------
double psMatrix::getEntry(const int row, const int col)
{
   assert(row >= 0 && row < nRows_);
   assert(col >= 0 && col < nCols_);
#ifdef PS_DEBUG
   printf("psMatrix getEntry (%d,%d) : Mat(%d,%d) = %e\n",
          nRow_, nCols_, row, col, Mat_[row][col]);
#endif
   return Mat_[row][col];
}

// ************************************************************************
// get determinant 
// ------------------------------------------------------------------------
double psMatrix::getDeterminant()
{
   return determinant_;
}

// ************************************************************************
// extract submatrix
// ------------------------------------------------------------------------
int psMatrix::submatrix(psMatrix &inMat, const int num, const int *indices)
{
   int nrows, ncols, ii, jj, row, col;

#ifdef PS_DEBUG
   printf("psMatrix submatrix\n");
#endif
   nrows = inMat.nrows();
   ncols = inMat.ncols();
   if (nrows != ncols)
   {
      printf("psMatrix::submatrix ERROR : incoming matrix is rectangular.\n");
      exit(1);
   }
   if (num <= 0 || indices == NULL)
   {
      printf("psMatrix::submatrix ERROR : no incoming indices.\n");
      exit(1);
   }
   for (ii = 0; ii < num; ii++)
   {
      row = indices[ii];
      if (row < 0 || row >= nrows)
      {
         printf("psMatrix::submatrix ERROR : index out of bound (%d)\n",row); 
         exit(1);
      }
   }
   setDim(num, num);
   for (ii = 0; ii < num; ii++)
   {
      row = indices[ii];
      for (jj = 0; jj < num; jj++)
      {
         col = indices[jj];
         Mat_[ii][jj] = inMat.getEntry(row, col);
      }
   }
#ifdef PS_DEBUG
   printf("psMatrix::submatrix: incoming matrix\n");
   for (ii = 0; ii < nrows; ii++)
   {
      for (jj = 0; jj < nrows; jj++) printf("%e ",inMat.getEntry(ii,jj));;
      printf("\n");
   }
   printf("psMatrix::submatrix: outgoing matrix\n");
   for (ii = 0; ii < num; ii++)
   {
      for (jj = 0; jj < num; jj++) printf("%e ",Mat_[ii][jj]);
      printf("\n");
   }
#endif
#ifdef PS_DEBUG
   printf("psMatrix submatrix ends\n");
#endif
   return 0;
}

// ************************************************************************
// Cholesky decomposition (A = L L^T)
// ------------------------------------------------------------------------
int psMatrix::CholDecompose()
{
   int     ii, jj, kk;
   double  ddata;

#ifdef PS_DEBUG
   printf("psMatrix CholDecompose\n");
#endif
   if (status_ != 0) 
   {
      printf("psMatrix ERROR : matrix has been decomposed.\n");
      exit(1);
   }
   assert(nRows_ == nCols_);

#ifdef PS_DEBUG
   determinant_ = computeDeterminant(nRows_, Mat_); 
   printf("psMatrix determinant = %e\n",<< determinant_);
   for (ii = 0; ii < nRows_; ii++)
      for (jj = ii; jj < nCols_; jj++)
         printf("psMatrix (%d,%d) = %e\n", ii+1, jj+1, Mat_[ii][jj]);
#endif
   for (ii = 0; ii < nRows_; ii++)
   {
      for (jj = 0; jj <= ii; jj++)
      {
         ddata = Mat_[ii][jj];
         for (kk = 0; kk < jj; kk++) ddata -= Mat_[ii][kk] * Mat_[jj][kk];
         if (ii == jj)
         {
            if (ddata <= 0.0)
            {
               printf("CholDecompose : matrix not positive definite.\n");
               printf("dim = (%d,%d) : %e\n", ii+1, jj+1, ddata);
               return -1;
            }
            Mat_[ii][ii] = sqrt(ddata);
         }
         else Mat_[ii][jj] = ddata / Mat_[jj][jj];
#ifdef PS_DEBUG
         printf("psMatrix Chol (%d,%d) = %e\n", ii+1, jj+1, Mat_[jj][ii]);
#endif
      }
   } 
   for (ii = 0; ii < nRows_; ii++)
      for (jj = ii+1; jj < nCols_; jj++) Mat_[ii][jj] = Mat_[jj][ii];
   status_ = 1;
#ifdef PS_DEBUG
   printf("psMatrix CholDecompose ends\n");
#endif
   return 0;
}

// ************************************************************************
// matrix vector multiply  
// ------------------------------------------------------------------------
void psMatrix::CholMatvec(psVector &vec)
{
   int    ii, jj;
   double ddata;

#ifdef PS_DEBUG
   printf("psMatrix CholMatvec\n");
#endif
   assert(vec.length() == nCols_);
   if (status_ == 0) CholDecompose();
   for (ii = nRows_-1; ii >= 0; ii--)
   {
      ddata = 0.0;
      for (jj = 0; jj <= ii; jj++) ddata += Mat_[ii][jj] * vec[jj];
      vec[ii] = ddata;
   }
#ifdef PS_DEBUG
   printf("psMatrix CholMatvec ends\n");
#endif
}

// ************************************************************************
// Cholesky L-solve 
// ------------------------------------------------------------------------
void psMatrix::CholSolve(psVector &vec)
{
   int    ii, jj;
   double ddata;

#ifdef PS_DEBUG
   printf("psMatrix CholSolve\n");
#endif
   assert(vec.length() == nCols_);
   if (status_ == 0) CholDecompose();
   for (ii = 0; ii < nRows_; ii++)
   {
      ddata = vec[ii];
      for (jj = 0; jj < ii; jj++) ddata -= Mat_[ii][jj] * vec[jj];
      vec[ii] = ddata / Mat_[ii][ii];
   }
#ifdef PS_DEBUG
   printf("psMatrix CholSolve ends\n");
#endif
}

// ************************************************************************
// Cholesky LT-solve 
// ------------------------------------------------------------------------
void psMatrix::CholTSolve(psVector &vec)
{
   int    ii, jj;
   double ddata;

#ifdef PS_DEBUG
   printf("psMatrix CholTSolve (transpose)\n");
#endif
   assert(vec.length() == nCols_);
   if (status_ == 0) CholDecompose();
   for (ii = nRows_-1; ii > 0; ii--)
   {
      ddata = vec[ii];
      for (jj = ii+1; jj < nRows_; jj++) ddata -= Mat_[jj][ii] * vec[jj];
      vec[ii] = ddata / Mat_[ii][ii];
   }
#ifdef PS_DEBUG
   printf("psMatrix CholTSolve ends\n");
#endif
}

// ************************************************************************
// print matrix
// ------------------------------------------------------------------------
void psMatrix::print()
{
   int ii, jj;
   printf("psMatrix print: \n");
   for (ii = 0; ii < nRows_; ii++)
   {
      for (jj = 0; jj < nCols_; jj++) printf("%e ", Mat_[ii][jj]);
      printf("\n");
   }
}

// ************************************************************************
// Compute determinant (by Bourke)
// ------------------------------------------------------------------------
double psMatrix::computeDeterminant(int ndim, double **mat)
{
   int    ii, jj, kk, ind;
   double result = 0.0;
   double **localMat = NULL;

   if (ndim == 1)
   {
      result = mat[0][0];
   }
   else if (ndim == 2)
   {
      result = mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1];
   }
   else
   {
      result = 0.0;
      for (ii = 0; ii < ndim; ii++)
      {
         localMat = new double*[ndim-1];
         for (kk = 0; kk < ndim-1; kk++)
            localMat[kk] = new double[ndim-1];
         for (kk = 1; kk < ndim; kk++)
         {
            ind = 0;
            for (jj = 0; jj < ndim; jj++)
            {
               if (jj == ii) continue;
               localMat[kk-1][ind] = mat[kk][jj];
               ind++;
            }
         }
         result += pow(-1.0,1.0+ii+1.0) * mat[0][ii] * 
                   computeDeterminant(ndim-1, localMat);
         for (kk = 0; kk < ndim-1; kk++) delete [] localMat[kk];
         delete [] localMat;
      }
   }
   return(result);
}

