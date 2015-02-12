/* ----------------------------------------------------------------------------
SOURCE FILE

Name:           primedecompose.c

Program:        a1 (COMP 8005, Threads vs Processes)

Developer:      Aman Abdulla

Created On:     2015-01-09

Functions:
        int decompose (mpz_t n, mpz_t *o);

Description:
        Has the prime decomposition function

Revisions:
        (none)

---------------------------------------------------------------------------- */
#include "primedecompose.h"


/* ----------------------------------------------------------------------------
FUNCTION

Name:           Decompose

Prototype:      int decompose (mpz_t n, mpz_t *o)

Developer:      Aman Abdulla

Created On:     2015-01-09

Parameters:
        mpz_t n     The number being decomposed
        mpz_t *o    The array for factors to be stored in

Return Values:
        int     the number of prime factors

Description:
        Factors a given number into it's prime factors and stores them in
        the provided array

Revisions:
        (none)

---------------------------------------------------------------------------- */

int decompose (mpz_t n, mpz_t *o)
{
	int i = 0;
  	mpz_t tmp, d;

	mpz_init(tmp);
  	mpz_init(d);

  	while (mpz_cmp_si (n, 1))
	{
    		mpz_set_ui(d, 1);
    		do
		{
      			mpz_add_ui(tmp, d, 1);
      			mpz_swap(tmp, d);
    		} while(!mpz_divisible_p(n, d));
    		mpz_divexact(tmp, n, d);
    		mpz_swap(tmp, n);
    		mpz_init(o[i]);
    		mpz_set(o[i], d);
    		i++;
  	}
  	return i;
}
