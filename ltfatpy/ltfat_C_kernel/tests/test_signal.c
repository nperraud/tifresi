#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <fftw3.h>
#include "ltfat.h"





double frand_a_b(double a, double b){
	return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

void print_double_array(const double * ar, ltfatInt size, char* name){
	printf("size = %u\n",size);
	printf("%s =[", name);
		for(ltfatInt i=0; i<size; i++){
			printf("%f,",ar[i]);
		}
		printf("]\n");
}

void print_complex_array(const double complex * ar, ltfatInt size, const char* name){
	printf("size = %u\n",size);
	printf("%s =[", name);
		for(ltfatInt i=0; i<size; i++){
			printf("(%f,%f),",creal(ar[i]),cimag(ar[i]));
		}
		printf("]\n");
}

double complex * comp_dgtreal_long_d(const double * f,
		const double * g, const ltfatInt L,
		const ltfatInt W, const ltfatInt a, const ltfatInt M, const ltfatInt M2,
		const dgt_phasetype ptype)
{
	ltfatInt size = M2 * W * L/a;
	double complex * res = (double complex *) malloc(sizeof(double complex) * size);
	dgtreal_long_d(f,g,L,W,a,M,ptype,res);
	return res;
}

double complex * comp_dgtreal_fb_d(const double * f,
		const double * g, const ltfatInt L, const ltfatInt gl,
		const ltfatInt W, const ltfatInt a, const ltfatInt M, const ltfatInt M2,
		const dgt_phasetype ptype){
	ltfatInt size = M2 * W * L/a;
	double complex * res = (double complex *) malloc(sizeof(double complex) * size);
	dgtreal_fb_d(f,g,L,gl,W,a,M,ptype,res);
	return res;
}

double complex * comp_sepdgtreal(const double * f, ltfatInt size_f, const double * g, int size_g, const int a,
		const ltfatInt M, dgt_phasetype ptype){
	ltfatInt L, W;
	printf("\n### comp_sepdgtreal\n");
	print_double_array(f,size_f,"f");
	L = size_f;
	W = 1;
	ltfatInt gl = size_g;
	ltfatInt N = L/a;
	ltfatInt M2 = M / 2 + 1;
	printf("N * M2 = %u * %u\n", N, M2);
	double complex * res;
	if (gl<L){
		res = comp_dgtreal_fb_d(f,g,L,gl,W,a,M,M2,ptype);
	}else{
		res = comp_dgtreal_long_d(f,g,L,W,a,M,M2,ptype);
	}
	print_complex_array(res,M2*N,"res");
	printf("### End of comp_sepdgtreal\n");
	return res;
}

void main(){
	printf("Start\n");
	dgt_phasetype PHASETYPE=FREQINV;
	ltfatInt L=24;
	ltfatInt W=1;
	ltfatInt a=6;
	ltfatInt M=8;
	ltfatInt gl=16;
	double SIGNAL[24]={6.3380146847e-01, 8.6483043758e-01, 2.0468446456e-01,
			9.6233752901e-01, 3.8778783919e-01,	1.4812246846e-01, 3.4830711563e-01,
			1.0844226591e-01, 8.9018637399e-01, 7.6586243203e-01, 3.8583987069e-01,
			9.6810335001e-01, 7.3717322244e-01, 4.4391373929e-01, 8.8498500894e-02,
			6.2769751739e-01, 9.1673514039e-01, 2.3115585646e-01, 7.2660679553e-01,
			2.4040222953e-01, 2.2176659487e-01, 4.6770858311e-01, 7.6130401807e-01,
			5.3029817869e-01};
	double WINDOW[16]={4.0392415581e-02, 4.0884444702e-01, 4.5303908051e-01,
			5.9607836127e-01, 3.3359746146e-01, 7.6271054775e-01, 8.9109428282e-02,
			5.1598863475e-01, 8.4070564683e-01,	8.3106147463e-01, 3.0061798065e-01,
			9.3981478510e-01, 8.0045753487e-01,	3.3751113385e-02, 2.5411151179e-01,
			9.5102061330e-01};

	double complex * spdgtr = comp_sepdgtreal(SIGNAL,L,WINDOW,gl,a,M,PHASETYPE);
	ltfatInt N = L / a;
	ltfatInt M2 = M / 2 + 1;
	print_complex_array(spdgtr,N*M2,"spdgtr");
}
