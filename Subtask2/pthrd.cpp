#include <bits/stdc++.h>
using namespace std;

int n, m, o;
int step_i = 0;

vector<vector<float>> matA, matB, matC;

void* multi(void* arg){
    int i = step_i++; //i denotes row number of resultant matC
	for (int j = 0; j < o; j++)
	for (int k = 0; k < m; k++)
		matC[i][j] += matA[i][k] * matB[k][j];

    pthread_exit(0);
}

void pt(vector<vector<float>>& a, vector<vector<float>>& b, vector<vector<float>>& c, int n1, int m1, int o1){
      n=n1; m=m1; o=o1;
      matA.resize(n);
      matB.resize(m);
      matC.resize(n);

	    for (int i = 0; i < n; i++)
	    	for (int j = 0; j < m; j++)
	    		matA[i].push_back(a[i][j]);
      for (int i = 0; i < m; i++)
	    	for (int j = 0; j < o; j++) 
	    		matB[i].push_back(b[i][j]);
      for (int i = 0; i < n; i++)
	    	for (int j = 0; j < o; j++)
	    		matC[i].push_back(c[i][j]);

    vector<pthread_t> threads(n);

	for (int i = 0; i < n; i++) {
		int* p;
		pthread_create(&threads[i], NULL, multi, (void*)(p));
	}

	// joining and waiting for all threads to complete
	for (int i = 0; i < n; i++)
		pthread_join(threads[i], NULL);
	
  for (int i = 0; i < n; i++)
	    	for (int j = 0; j < o; j++)
	    		c[i][j] = matC[i][j];
}
