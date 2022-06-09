#include <bits/stdc++.h>
#include "cblas.h"

using namespace std;

void opn(vector<vector<float>>& matA, vector<vector<float>>& matB, vector<vector<float>>& matC, int n, int m, int o){
    float A[n*m], B[m*o], C[n*o];

    for (int i = 0; i < n; i++)
		for (int j = 0; j < m; j++)
			A[i*m+j] = matA[i][j];
    for (int i = 0; i < m; i++)
		for (int j = 0; j < o; j++) 
			B[i*o+j] = matB[i][j];
    for (int i = 0; i < n; i++)
		for (int j = 0; j < o; j++)
			C[i*o+j] = matC[i][j];

    cblas_sgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                n, o, m, 1.0, A, m, B, o, 0.0, C, o); 
    
    for (int i = 0; i < n; i++)
		for (int j = 0; j < o; j++)
			matC[i][j] = C[i*o+j];
}