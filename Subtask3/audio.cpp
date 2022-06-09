#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "openblas.cpp"
#include "dnn_weights.h"
using namespace std;

typedef struct{
    int label;
    float prob;
}pred_t;


vector<vector<float>> wt1, wt2, wt3, wt4, bi1, bi2, bi3, bi4, feature_vector;

void read_feature_vector(string filename){
    feature_vector.resize(1);
    ifstream file (filename);
    string line;
    float value;
    int c=0;

    if (file.is_open()) {
        getline(file, line);
        stringstream s(line);
        while(s >> value){
            c++;
            feature_vector[0].push_back(value);
        }
        if (c!=250)
            cout<<"Input file does not have 250 entries.";
        file.close();
    }
    else {
        cout<<"Unable to read from file. Make sure the matrix files are present in the directory.\n";
        exit(0);
    }
}

void convert(){
    int n, m;

    vector<float> t_wt1, t_wt2, t_wt3, t_wt4, t_bi1, t_bi2, t_bi3, t_bi4;
    t_wt1 =  IP1_WT; t_wt2 =  IP2_WT; t_wt3 =  IP3_WT; t_wt4 =  IP4_WT;
    t_bi1 = IP1_BIAS; t_bi2 = IP2_BIAS; t_bi3 = IP3_BIAS; t_bi4 = IP4_BIAS;

    n=250; m=144;
    wt1.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        wt1[i].push_back(t_wt1[i*m+j]);

    n=144; m=144;
    wt2.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        wt2[i].push_back(t_wt2[i*m+j]);

    n=144; m=144;
    wt3.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        wt3[i].push_back(t_wt3[i*m+j]);

    n=144; m=12;
    wt4.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        wt4[i].push_back(t_wt4[i*m+j]);
    
    n=1; m=144;
    bi1.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        bi1[i].push_back(t_bi1[i*m+j]);
    
    n=1; m=144;
    bi2.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        bi2[i].push_back(t_bi2[i*m+j]);
    
    n=1; m=144;
    bi3.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        bi3[i].push_back(t_bi3[i*m+j]);
    
    n=1; m=12;
    bi4.resize(n);
    for(int i=0; i<n; i++)
    for(int j=0; j<m; j++)
        bi4[i].push_back(t_bi4[i*m+j]);
}

vector<vector<float>> activation_relu(vector<vector<float>> inm){
    int n = inm[0].size();
    vector<vector<float>> oum(1);
    for(int i=0; i<n; i++)
        oum[0].push_back(max(0.0f, inm[0][i]));
    return oum;
}

// Fully connected layer generated
vector<vector<float>> fullyconnected(vector<vector<float>> inp, vector<vector<float>> weight, vector<vector<float>> bias){
    int r=inp.size(), c1 = inp[0].size(), c2 = weight[0].size();
    vector<vector<float>> out(1, vector<float>(c2, 0));
    opn(inp, weight, out, r, c1, c2);
    for(int i = 0; i<r; i++)
        for(int j = 0; j<c2; j++)
            out[i][j] += bias[i][j];
    return out;
}

vector<float> softmax(vector<vector<float>> inp){
    float sum = 0.0;
    int n = inp[0].size();
    vector<float> outputvector(n);
    //calculate softmax
    for(int i=0; i<n; i++){
        inp[0][i] = exp(inp[0][i]);
        sum += inp[0][i];
    }
    for(int i=0; i<n; i++)
        outputvector[i] = inp[0][i]/sum;
    
    return outputvector;
}

pred_t* libaudioAPI(const char* infile, pred_t* l){
    string inp_file = string(infile);

    read_feature_vector(inp_file);
    convert();

    vector<vector<float>> temp_f1, temp_r1;
    temp_f1 = fullyconnected(feature_vector, wt1, bi1);
    temp_r1 = activation_relu(temp_f1);

    vector<vector<float>> temp_f2, temp_r2;
    temp_f2 = fullyconnected(temp_r1, wt2, bi2);
    temp_r2 = activation_relu(temp_f2);

    vector<vector<float>> temp_f3, temp_r3;
    temp_f3 = fullyconnected(temp_r2, wt3, bi3);
    temp_r3 = activation_relu(temp_f3);

    vector<vector<float>> temp_f4;
    temp_f4 = fullyconnected(temp_r3, wt4, bi4);

    vector<float> final = softmax(temp_f4);

    vector<pair<float, int>> ans(12);
    for(int i=0; i<12; i++)
        ans[i] = make_pair(final[i], i);

    sort(ans.rbegin(), ans.rend());

    l = new pred_t[3];
    l[0].label = ans[0].second;
    l[0].prob = ans[0].first;
    l[1].label = ans[1].second;
    l[1].prob = ans[1].first;
    l[2].label = ans[2].second;
    l[2].prob = ans[2].first;
    return l; 
}