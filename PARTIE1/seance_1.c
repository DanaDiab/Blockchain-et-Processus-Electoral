#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"seance_1.h"

int naive_is_prime(long n)
{
	for(long i=2; i<n; i++)
	{
		if(n%i==0)return 0;
	}
	return 1;
}

long modpow_naive(long a,long m,long n){
	long res=1;
	for (long i=0; i<m; i++){
		res=(res*a)%n;
	}
	return res;
}

int modpow(long a, long m,long n){
	long res=a;
	while(m>1){
		if (m%2==0){
			res=(res*res)%n;
		}
		else{
			res=(res*res*a)%n;
		}
	m=m/2;
	}
	return res;
}
