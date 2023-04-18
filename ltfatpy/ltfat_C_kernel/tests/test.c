#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <complex.h>
#include <fftw3.h>
#include "ltfat.h"

double frand_a_b(double a, double b){
	return ( rand()/(double)RAND_MAX ) * (b-a) + a;
}

void print_double_array(const double * ar, int size, char* name){
	printf("size = %d\n",size);
	printf("%s =[", name);
		for(long unsigned int i=0; i<size; i++){
			printf("%f,",ar[i]);
		}
		printf("]\n");
}

void print_complex_array(const double complex * ar, int size, const char* name){
	printf("size = %d\n",size);
	printf("%s =[", name);
		for(int i=0; i<size; i++){
			printf("(%f,%f),",creal(ar[i]),cimag(ar[i]));
		}
		printf("]\n");
}

double complex * comp_dgtreal_long_d(const double * f,
		const double * g, const int L,
		const int W, const int a, const int M, const int M2,
		const dgt_phasetype ptype)
{
	int size = M2 * W * L/a;
	double complex * res = (double complex *) malloc(sizeof(double complex) * size);
	dgtreal_long_d(f,g,L,W,a,M,ptype,res);
	return res;
}

double complex * comp_dgtreal_fb_d(const double * f,
		const double * g, const int L, const int gl,
		const int W, const int a, const int M, const int M2,
		const dgt_phasetype ptype){
	int size = M2 * W * L/a;
	double complex * res = (double complex *) malloc(sizeof(double complex) * size);
	dgtreal_fb_d(f,g,L,gl,W,a,M,ptype,res);
	return res;
}

double complex * comp_sepdgtreal(const double * f, int size_f, const double * g, int size_g, const int a,
		const int M, dgt_phasetype ptype){
	int L, W;
	printf("\n### comp_sepdgtreal\n");
	print_double_array(f,size_f,"f");
	L = size_f;
	W = 1;
	int gl = size_g;
	int N = L/a;
	int M2 = M / 2 + 1;
	printf("N * M2 = %d * %d\n", N, M2);
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
	srand(time(NULL));
	int M = 40;
	int a = 100;
	int L = a * M;
	double * fr = (double *) malloc(sizeof(double) * L);
	for(int i=0; i<L; i++){
		fr[i] = frand_a_b(-50.0,50.0);
	}
	double * gr = (double *) malloc(sizeof(double) * a);
	pgauss_d(a,1.0,0,gr);
	dgt_phasetype ptype = FREQINV;
	double complex * spdgtr = comp_sepdgtreal(fr,L,gr,a,a,M,ptype);
	int N = L / a;
	int M2 = M / 2 + 1;
	print_complex_array(spdgtr,N*M2,"spdgtr");
}
