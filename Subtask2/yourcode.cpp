#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "pthrd.cpp"
#include "openblas.cpp"
using namespace std;

// Reads matrix from a .txt file
vector<vector<float>> matrixfileread(string filename) {
    string line;
    ifstream filehandle (filename);
    vector<vector<float>> matrix;
    int c, r;
    if (filehandle.is_open()) {
        if(getline (filehandle,line)) {
            c = stoi(line);
        }
        else {
            cout<<"Matrix data insufficient.\n";
            exit(0);
        }
        if(getline (filehandle,line)) {
            r = stoi(line);
        }
        else {
            cout<<"Matrix data insufficient.\n";
            exit(0);
        }
        matrix.resize(r);
        for(int i = 0; i<r; i++) {
            matrix[i].resize(c);
        }
        for(int i = 0; i<c; i++) {
            for(int j = 0; j<r; j++) {
                if(getline (filehandle,line)) {
                    float xji = stof(line);
                    matrix[j][i] = xji;
                }
                else {
                    cout<<"Matrix data insufficient.\n";
                    exit(0);
                }
            }
        }
        filehandle.close();
    }
    else {
        cout<<"Unable to read from file. Make sure the matrix files are present in the directory.\n";
        exit(0);
    }
    return matrix;
}

// Reads a vector form a .txt file
vector<float> vectorfileread(string filename) {
    string line;
    ifstream filehandle (filename);
    vector<float> vec;
    int r;
    if (filehandle.is_open()) {
        if(getline (filehandle,line)) {
            r = stoi(line);
        }
        else {
            cout<<"Vector data insufficient.\n";
            exit(0);
        }
        vec.resize(r);
        for(int i = 0; i<r; i++) {
            if(getline (filehandle,line)) {
                float xji = stof(line);
                vec[i] = xji;
            }
            else {
                cout<<"Vector data insufficient.\n";
                exit(0);
            }
        }
        filehandle.close();
    }
    else {
        cout<<"Unable to read from file. Make sure the vector input file is present in the directory.\n";
        exit(0);
    }
    return vec;
}


// Applies relu function to x
float relutype(float x) {
    return max(0.0f, x);
}

// Applies tanh function to x
float tanhtype(float x) {
    return ((expf(x)-expf(-x))/(expf(x)+expf(-x)));
}

// Writes a matrix to a file
void mattofile(string filename, vector<vector<float>> outmatrix) {
    int r1 = outmatrix.size();
    int c1 = outmatrix[0].size();
    ofstream myfile (filename);
    if (myfile.is_open())
    {
        myfile<<c1<<"\n";
        myfile<<r1<<"\n";
        for(int i = 0; i<c1; i++) {
            for(int j = 0; j<r1; j++) {
                myfile<<outmatrix[j][i]<<"\n";
            }
        }
        myfile.close();
    }
    else {
        cout << "Unable to write to file.\n";
        exit(0);
    }
    return;
}

// Writes a vector to a file
void vectofile(string filename, vector<float> outvec) {
    int n = outvec.size();
    ofstream myfile (filename);
    if (myfile.is_open())
    {
        myfile<<n<<"\n";
        for(int i = 0; i<n; i++) {
            myfile<<outvec[i]<<"\n";
        }
        myfile.close();
    }
    else {
        cout << "Unable to write to file.\n";
        exit(0);
    }
    return;
}

// Fully connected layer generated
void fullyconnected(string inputfile, string weightfile, string biasfile, string outputfile, string option){
    // Reads input matrix from a given file
    vector<vector<float>> X = matrixfileread(inputfile);
    int r1 = X.size();
    int c1 = X[0].size();
    
    // Reading Weight Matrix from a given file
    vector<vector<float>> W = matrixfileread(weightfile);
    int r2 = W.size();
    int c2 = W[0].size();

    if(c1!=r2) {
        cout<<"Input and Weight Matrix Incompatible.\n";
        exit(0);
    }

    // Reading Bias Matrix file
    vector<vector<float>> biasm = matrixfileread(biasfile);
    int r3 = biasm.size();
    int c3 = biasm[0].size();
    
    if(r1!=r3 || c2!=c3) {
        cout<<"Product and Bias Matrix Incompatible.\n";
        exit(0);
    }

    // Generates the required output matrix
    vector<vector<float>> outmatrix(r1, vector<float>(c2, 0));

    if(option == "openblas")
        opn(X, W, outmatrix, r1, c1, c2);
    else if(option == "pthread") 
        pt(X, W, outmatrix, r1, c1, c2);

    for(int i = 0; i<r1; i++) {
        for(int j = 0; j<c2; j++) {
            outmatrix[i][j] += biasm[i][j];
        }
    }
    // Writing the resultant matrix to output file
    mattofile(outputfile, outmatrix);
    return;
}


void activation (string inputfile, string type, string outfile) {
    // Reading the input matrix from the given file
    vector<vector<float>> X = matrixfileread(inputfile);
    int r1 = X.size();
    int c1 = X[0].size();

    // Generates the resultant matrix
    vector<vector<float>> outmatrix(r1, vector<float>(c1, 0));
    for(int i = 0; i<c1; i++) {
        for(int j = 0; j<r1; j++) {
            if(type == "tanh") {
                outmatrix[j][i] = tanhtype(X[j][i]);
            }
            else if(type == "relu") {
                outmatrix[j][i] = relutype(X[j][i]);
            }
            else {
                cout<<"Not tanh or relu.\n";
                exit(0);
            }
        }
    }

    // Writing the resultant matrix to output file
    mattofile(outfile, outmatrix);
    return;
}

void pooling (string inputfile, string type, int stridesize, string outfile) {
    // Reading the input matrix from the given file
    vector<vector<float>> X = matrixfileread(inputfile);
    int r1 = X.size();
    int c1 = X[0].size();

    if(!(r1>=stridesize && r1%stridesize==0 && c1>=stridesize && c1%stridesize==0)) {
        cout<<"Cannot do pooling with given stride.\n";
        exit(0);
    }

    // Generating the output matrix
    int a = r1/stridesize, b = c1/stridesize;
    vector<vector<float>> outmatrix(a, vector<float>(b, 0));
    // Processing pooling
    for(int i = 0, a = 0; i<r1; i+=stridesize, a++) {
        for(int j = 0, b = 0; j<c1; j+=stridesize, b++) {
            float res;
            if(type == "max") {
                res = X[i][j];
                for(int k = i; k<i+stridesize; k++) {
                    for(int l = j; l<j+stridesize; l++) {
                        res = max(res, X[k][l]);
                    }
                }
            }
            else if(type == "average") {
                res = 0;
                for(int k = i; k<i+stridesize; k++) {
                    for(int l = j; l<j+stridesize; l++) {
                        res += X[k][l];
                    }
                }
                res /= (stridesize*stridesize);
            }
            else {
                cout<<"Invalide type.\n";
                exit(0);
            }
            outmatrix[a][b] = res;
        }
    }
    // Writing the resultant matrix to given file
    mattofile(outfile, outmatrix);
    return;
}


void probability(string inputfile, string type, string outfile) {
    // Reading the vector from given file
    vector<float> inputvec = vectorfileread(inputfile);
    int n = inputvec.size();

    // Generating the output vector
    vector<float> outvec (n);
    if(type == "softmax") {
        float sum = 0.0f;
        for(int i = 0; i<n; i++) {
            sum += expf(inputvec[i]);
        }
        for(int i = 0; i<n; i++) {
            outvec[i] = expf(inputvec[i])/sum;
        }
    }
    else if (type == "sigmoid") {
        for(int i = 0; i<n; i++) {
            outvec[i] = 1.0f/(1.0f + expf(-inputvec[i]));
        }
    }
    // Writing vector to the given file
    vectofile(outfile, outvec);
    return;
}

int main(int n,char* params[])
{
    int counter;
    string s1(params[1]);
    if(n==1)
        cout<<"Not a proper command. Use ./yourcode.out help to get help.\n";
    if(n>=2)
    {
        if(s1=="fullyconnected") {
            if(n!=7) {
                cout<<"Not correct set of parameters passed. Use ./yourcode.out help to get help.\n";
                exit(0);
            }
            fullyconnected(string(params[2]), string(params[3]), string(params[4]), string(params[5]), string(params[6]));
        }
        else if(s1=="activation") {
            if(n!=5) {
                cout<<"Not correct set of parameters passed. Use ./yourcode.out help to get help.\n";
                exit(0);
            }
            activation(string(params[3]), string(params[2]), string(params[4]));
        }
        else if(s1=="pooling") {
            if(n!=6) {
                cout<<"Not correct set of parameters passed. Use ./yourcode.out help to get help.\n";
                exit(0);
            }
            pooling(string(params[3]), string(params[2]), stoi(string(params[4])), string(params[5]));
        }
        else if(s1=="probability") {
            if(n!=5) {
                cout<<"Not correct set of parameters passed. Use ./yourcode.out help to get help.\n";
                exit(0);
            }
            probability(string(params[3]), string(params[2]), string(params[4]));
        }
        else {
            if(s1!="help") {
                cout<<"The command entered is not correct. Refer to the help to know more...\n";
            }
            cout<<"HELP :- Call the functions using given format:\n";
            cout<<"\t\t./yourcode.out fullyconnected inputmatrix.txt weightmatrix.txt biasmatrix.txt outputmatrix.txt\n";
            cout<<"\t\t./yourcode.out activation relu inputmatrix.txt outputmatrix.txt\n";
            cout<<"\t\t./yourcode.out activation tanh inputmatrix.txt outputmatrix.txt\n";
            cout<<"\t\t./yourcode.out pooling max inputmatrix.txt stride outputmatrix.txt\n";
            cout<<"\t\t./yourcode.out pooling average inputmatrix.txt stride outputmatrix.txt\n";
            cout<<"\t\t./yourcode.out probability softmax inputvector.txt outputvector.txt\n";
            cout<<"\t\t./yourcode.out probability sigmoid inputvector.txt outputvector.txt\n";
        }
    }
    return 0;
}
