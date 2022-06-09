Name: Roshan Prashant Bara
Entry No.: 2020CS10377

All the functions have been implemented in yourcode.cpp

1. RUN Instructions:
    1.1 In commandline navigate to the folder containing the yourcode.cpp as well as the test files.
    1.2 Type make and hit ENTER. This will compile my code and generate yourcode.out file.
    1.3 Now, the yourcode.out can be used as was given on Piazza using the following commands:
            ./yourcode.out fullyconnected inputmatrix.txt weightmatrix.txt biasmatrix.txt outputmatrix.txt
            ./yourcode.out activation relu inputmatrix.txt outputmatrix.txt
            ./yourcode.out activation tanh inputmatrix.txt outputmatrix.txt
            ./yourcode.out pooling max inputmatrix.txt stride outputmatrix.txt
            ./yourcode.out pooling average inputmatrix.txt stride outputmatrix.txt
            ./yourcode.out probability softmax inputvector.txt outputvector.txt
            ./yourcode.out probability sigmoid inputvector.txt outputvector.txt



Function Descriptions:
1. matrixfileread(string filename) :- Reads the file whose name is passed as parameter and returns a Matrix of float.
2. vectorfileread(string filename) :- Reads the file whose name is passed as parameter and returns a vector of floats.
3. relutype(float x) :- Returns the resultant after applying ReLU function to the float passed to it.
4. tanhtype(float x) :- Returns the resultant after applying tanh function to the float passed to it.
5. mattofile(string filename, matrix) :- Writes matrix to the file whose name is passed as parameter.
6. vectofile(string filename, vector) :- Writes vector to the file whose name is passed as parameter.
7. fullyconnected(string inputmatrix, string weightmatrix, string biasmatrix, string outputmatrix) :- Does Matrix multiplication and adds bias to it and prints it to the output file.
8. activation(string inputmatrix, string type, string outputmatrix) :- Applies activation function to the entire matrix and prints it to output file.
9. pooling(string inputmatrix, string type, int stride, string outputmatrix) :- Does pooling over an entire matrix and returns the resultant matrix to the output file.
10. probability(string inputmatrix, string type, string outputmatrix) :- Applies the given type of funv=ction over entire vector and prints the resultant vector to file.
11. main(int, array of char array) :- Takes in the function to be called along with all other necessary parameters on commandline.


Thank You!