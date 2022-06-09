#include<bits/stdc++.h>
using namespace std;

typedef struct{
    int label;
    float prob;
}pred_t;

extern pred_t* libaudioAPI(const char* audiofeatures, pred_t* pred);


void writetofile(string outfile, string inp_file, pred_t* pred) {
    string name[] = { "silence", "unknown",	 "yes",	 "no",	 "up",	 "down",	 "left",	 "right",	 "on",	 "off",	 "stop",	 "go"};
    fstream file;
    file.open(outfile, std::ios_base::app);
    if (file.is_open()) {
        file<<inp_file<<" "<<name[pred[0].label]<<" "<<name[pred[1].label]<<" "<<name[pred[2].label]<<" "<<pred[0].prob<<" "<<pred[1].prob<<" "<<pred[2].prob<<endl; 
        file.close();
    }
    else {
        cout << "Unable to write to file.\n";
        exit(0);
    }
}


int main(int n,char* params[])
{
    int counter;
    string s1(params[1]);
    if(n==1)
        cout<<"Not a proper command. Use ./yourcode.out help to get help.\n";
    if(n>=2)
    {
        if(n!=3) {
            cout<<"Not correct set of parameters passed. Use ./yourcode.out help to get help.\n";
            exit(0);
        }
        pred_t* pred = new pred_t[3];
        pred_t* outputstruc = libaudioAPI(params[1], pred);
        writetofile(string(params[2]), string(params[1]), outputstruc);
    }
        
    else {
        if(s1!="help") {
            cout<<"The command entered is not correct. Refer to the help to know more...\n";
        }
        cout<<"HELP :- Call the functions using given format:\n";
        cout<<"\t\t./yourcode.out inputfile.txt outputfile.txt\n";
        return 0;
    }
}