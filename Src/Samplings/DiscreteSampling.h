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
// Definition for the Discrete sampling class
// AUTHOR : CHARLES TONG
// DATE   : 2010
// ************************************************************************
#ifndef __DISCRETESAMPLINGH__
#define __DISCRETESAMPLINGH__

#include <string.h>
#include "Sampling.h"

/**
 * @name Discrete samping method
 *
 **/

// ************************************************************************
// class definition
// ************************************************************************
class DiscreteSampling : public Sampling 
{
   int    **inputValues_;
   int    *inputValCnts_;
   double **inputProbs_;

public:

   /** constructor */
   DiscreteSampling();

   // Copy Constructor by Bill Oliver
   DiscreteSampling(const DiscreteSampling &);

   /** destructor */
   ~DiscreteSampling();

   /** initialization 
       @param flag: flag to signal how far to initialize
    */
   int initialize(int flag);

   /** This function refines an incoming sample
       @param ratio: refinement ratio
       @param randomize: generate randomized sample
       @param thresh: threshold
       @param nSamples: sample size
       @param sampleErrs: errors for each sample point
    */
   int refine(int ratio,int randomize,double thresh,int nSamples,
              double *sampleErrs);

   /** This function overloads the assignment operator
       @param obj : Sampling object
    */
   DiscreteSampling& operator=(const DiscreteSampling &);
};

#endif // __DISCRETESAMPLINGH__

