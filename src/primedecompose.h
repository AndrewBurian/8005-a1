/* ----------------------------------------------------------------------------
HEADER FILE

Name:           primedecompose.h

Program:        a1 (COMP 8005, Threads vs Processes)

Developer:      Aman Abdulla

Created On:     2015-01-09

Description:
        The includes and function prototypes required for primedecompose

Revisions:
        (none)

---------------------------------------------------------------------------- */


#ifndef PRIMEDECOMPOSE_H
#define PRIMEDECOMPOSE_H

#include <gmp.h>
int decompose(mpz_t n, mpz_t *o);

#endif
