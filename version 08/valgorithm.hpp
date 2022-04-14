#include <vector>
#include <string>
#include <sstream>
using namespace std;
//typcasting idea reference from https://gist.github.com/mark-d-holmberg/862733 Thank you!
template<typename T>
T convert_to(const string &str){
    istringstream ss(str);
    T var;
    ss >> var;
    return var;
}

template<typename T>
vector<int> ref_merge(vector<int> A, vector<int> B, vector<string>* C){
    vector<int> O;
    int j = 0;
    int i = 0;
    while(i < A.size()){
        if(j < B.size() && (*C)[A[i]] > (*C)[B[j]] ){
            O.push_back(B[j]);
            j++;
        }
        else{
            O.push_back(A[i]);
            i++;
        }
    }
    for(;j < B.size(); j++)
        O.push_back(B[j]);
    return O;
}

template<>
vector<int> ref_merge<float>(vector<int> A, vector<int> B, vector<string>* C){
    vector<int> O;
    int j = 0;
    int i = 0;
    while(i < A.size()){
        if(j < B.size() && stof((*C)[A[i]]) > stof((*C)[B[j]]) ){
            O.push_back(B[j]);
            j++;
        }
        else{
            O.push_back(A[i]);
            i++;
        }
    }
    for(;j < B.size(); j++)
        O.push_back(B[j]);
    return O;
}

template<>
vector<int> ref_merge<string>(vector<int> A, vector<int> B, vector<string>* C){
    vector<int> O;
    int j = 0;
    int i = 0;
    while(i < A.size()){
        if(j < B.size()  && (*C)[A[i]].compare( (*C)[B[j]] ) > 0){
            O.push_back(B[j]);
            j++;
        }
        else{
            O.push_back(A[i]);
            i++;
        }
    }
    for(;j < B.size(); j++)
        O.push_back(B[j]);
    return O;
}


template<typename T>
vector<int> ref_merge_sort(vector<int> val, vector<string>* F){
    if(val.size() <= 1)
        return val;
    vector<vector<int>> D;
    //dividing by 1
    for(int i =0; i< val.size(); i++){
        vector<int> A(val.begin() + i, val.begin() + i + 1);
        D.push_back(A);
    }
    while(D[0].size() < val.size()){ //loop the element vector isze equal to the given size
        vector<vector<int>> C; //make a new vec2d
        for(int k = 0; k < (D.size()>>1); k ++){ //loop D elements
            vector<int> B = ref_merge<T>(D[k<<1], D[(k<<1)+1], F);
            C.push_back(B); //push to C
        }
        if(D.size() % 2 == 1)
            C.push_back(D[D.size() - 1]);
        D.clear();
        D = C;
    }
    return D[0];
}

vector<int> merge(vector<int> A, vector<int> B){
    vector<int> O;
    int j = 0;
    int i = 0;
    while(i < A.size()){
        if(j < B.size() && A[i] > B[j]){
            O.push_back(B[j]);
            j++;
        }
        else{
            O.push_back(A[i]);
            i++;
        }
    }
    for(;j < B.size(); j++)
        O.push_back(B[j]);
    return O;
}
vector<int> merge_sort(vector<int> val){
    if(val.size() <= 1)
        return val;
    vector<vector<int>> D;
    //dividing by 1
    for(int i =0; i< val.size(); i++){
        vector<int> A(val.begin() + i, val.begin() + i + 1);
        D.push_back(A);
    }
    while(D[0].size() < val.size()){ //loop the element vector isze equal to the given size
        vector<vector<int>> C; //make a new vec2d
        for(int k = 0; k < (D.size()>>1); k ++){ //loop D elements
            vector<int> B = merge(D[k<<1], D[(k<<1) + 1]); //merge
            C.push_back(B); //push to C
        }
        if(D.size() % 2 == 1)
            C.push_back(D[D.size() - 1]);
        D = C;
    }
    return D[0];
}

template<typename T>
vector<T> split(string txt, const char* deli){
    vector<T> temp;
    char ch[txt.length()];
    strcpy(ch, txt.c_str());
    char* ptr = strtok(ch, deli);
    while(ptr != NULL){
        temp.push_back(convert_to<T>(ptr));
        ptr = strtok(NULL, deli);
    }
    return temp;
}