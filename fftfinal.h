
#ifndef FFTFINAL_H
#define FFTFINAL_H


#include <math.h>
#include "fixed_math.h"
#include <string.h>
#include "fft_weight.h"
#include <stdio.h>
#include "int_dec_math.h"
static void fix_fft(short *fr , short *fi , short m)
{

	long int mr = 0, nn, i, j, l, k, istep, n, shift;
	short qr, qi, tr, ti, wr, wi;

	n = 1 << m;
	nn = n - 1;

	/* max FFT size = N_WAVE */
	//if (n > N_WAVE) return -1;

	/* decimation in time - re-order data */
	for (m=1; m<=nn; ++m)
	{
		l = n;
		do
		{
			l >>= 1;
		} while (mr+l > nn);

		mr = (mr & (l-1)) + l;
		if (mr <= m) continue;

		tr = fr[m];
		fr[m] = fr[mr];
		fr[mr] = tr;
		ti = fi[m];
		fi[m] = fi[mr];
		fi[mr] = ti;
	}

	l = 1;
	k = LOG2_N_WAVE-1;

	while (l < n)
	{


		// Variables for multiplication code
		long int c;
		short b;

		istep = l << 1;
		for (m=0; m<l; ++m)
		{
			j = m << k;
			/* 0 <= j < N_WAVE/2 */
			wr =  Sinewave[j+N_WAVE/4];
			wi = -Sinewave[j];

			wr >>= 1;
			wi >>= 1;

			for (i=m; i<n; i+=istep)
			{
				j = i + l;



				 tr = fract16_multr(wr,fr[j]) - fract16_multr(wi,fi[j]);


				ti = fract16_multr(wr,fi[j]) + fract16_multr(wi,fr[j]);


				qr = fr[i];
				qi = fi[i];
				qr >>= 1;
				qi >>= 1;

				fr[j] = qr - tr;
				fi[j] = qi - ti;
				fr[i] = qr + tr;
				fi[i] = qi + ti;

			}
		}

		--k;
		l = istep;
	}



}
#endif


