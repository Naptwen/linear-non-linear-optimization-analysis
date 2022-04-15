#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <deque>
#include <queue>
#include <set>
#include <map>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


#define SAMPLE 0
#define POPUL 1

using namespace std;
//typcasting idea reference from https://gist.github.com/mark-d-holmberg/862733 Thank you!


//factorial
int facto(int n){
    int N = 1;
    for(int i = 1; i <= n; i++)
        N *= i;
    return N;
}
//binomial
int binom(int n, int k){
    int N = 1;
    int M = 1;
    for(int i = n; i > k; i--)
        N *= i;
    for(int i = 1; i <= n-k; i++)
        M *= i;
    return N/M;
}
//sum
float sum(float* begin, float* end){
    float sum = 0;
    while(begin <= end){
        sum += begin[0];
        begin = (begin + 1);
    }
    return sum;
}
//average
float mean(float* begin, float* end){
    float sum = 0;
    int  n = 0;
    while(begin <= end){
        sum += begin[0];
        begin = (begin + 1);
        n++;
    }
    if(n == 0) perror("[MEAN] : divided by 0");
    return sum/n;
}
// POPUL : poulation variance
// SAMPLE : sample variance
float var(float* begin, float* end, int type = POPUL){
    float m = mean(begin, end);
    float sum = 0;    
    int  n = 0;
    while(begin <= end){
        sum += (begin[0] - m)  * (begin[0] - m);
        begin = (begin + 1);
        n++;
    }
    if(n == 0) perror("[VARIANCE] : divided by 0");
    return (type == POPUL)?sum/n : sum/(n-1);
}
//sorted data required
//The 1st element : median
//The 2nd element : Q1 (median lower half)
//The 3rd element : Q3 (median upper half)
//The 4th element : IQR (Q3- Q1)
//The 5th element : Upper Fence (IQR * 1.5)
//The 6th element : Upper Whisker (IQR - Q3)
vector<float> box_plot(vector<float> data){
    int N = (data.size() + 1)>>1;
    vector<float> list;
    list.push_back(data[N]);
    list.push_back(data[N/2]);
    list.push_back(data[N + N/2 + 1]);
    list.push_back(data[N + N/2 + 1] - data[N/2]);
    list.push_back((data[N + N/2 + 1] - data[N/2]) * 1.50f);
    list.push_back((data[N + N/2 + 1] - data[N/2]) * 1.50f - data[N + N/2 + 1]);
    return list;
}
//covariance
//data x and data y should be the same size
//if not, the size is considred as the shorteset one
float cov(vector<float> datax, vector<float> datay){
    int sz =min(datax.size(), datay.size());
    float mux = mean((float*)&datax[0], (float*)&datax[sz - 1]);
    float muy = mean((float*)&datay[0], (float*)&datax[sz - 1]);
    float sum = 0;
    for(int i = 0; i < sz; i++){
        sum += datax[i] * datay[i];
    }
    return (sum / sz) - (mux * muy);
}
// type:
// POPUL : poulation variance
// SAMPLE : sample variance
float cor(vector<float> datax, vector<float> datay, int type = POPUL){
    int sz =min(datax.size(), datay.size());
    float vx = var((float*)&datax[0], (float*)&datax[sz - 1], type);
    float vy = var((float*)&datax[0], (float*)&datax[sz - 1], type);
    return cov(datax, datay)/ sqrt(vx * vy);
}
//variance for multiplication of xy
float varxy(vector<float> datax, vector<float> datay){
    return cov(datax, datax) + 2 * cov(datax, datax) + cov(datay, datay);
}



template<typename T>
void Qclear( std::queue<T> &q )
{
   queue<T> empty;
   swap( q, empty );
}

template<typename T>
T convert_to(const string &str){
    istringstream ss(str);
    T var;
    ss >> var;
    return var;
}

template<typename T>
queue<int> ref_merge(queue<int> A, queue<int> B, vector<string>* C){
    queue<int> O;
    while(A.size() > 0){
        if(B.size() > 0 && (*C)[A.front()] > (*C)[B.front()] ){
            O.push(B.front());
            B.pop();
            continue;
        }
        O.push(A.front());
        A.pop();
    }
    while(B.size() > 0){
        O.push(B.front());
        B.pop();
    }
    return O;
}

template<>
queue<int> ref_merge<float>(queue<int> A, queue<int> B, vector<string>* C){
    queue<int> O;
    while(!A.empty()){
        if(!B.empty() && stof((*C)[A.front()]) > stof((*C)[B.front()]) ){
            O.push(B.front());
            B.pop();
            continue;
        }
        O.push(A.front());
        A.pop();
    }
    while(!B.empty()){
        O.push(B.front());
        B.pop();
    }
    return O;
}

template<>
queue<int> ref_merge<string>(queue<int> A, queue<int> B, vector<string>* C){
    queue<int> O;
    while(A.size() > 0){
        if(B.size() > 0 && (*C)[A.front()].compare( (*C)[B.front()] ) > 0 ){
            O.push(B.front());
            B.pop();
            continue;
        }
        O.push(A.front());
        A.pop();
    }
    while(B.size() > 0){
        O.push(B.front());
        B.pop();
    }
    return O;
}

template<typename T>
vector<int> ref_merge_sort(vector<int> index, vector<string>* F){
    if(index.size() <= 1)
        return index;
    queue<queue<int>> D;
    queue<queue<int>> C;
    queue<int> A;
    queue<int> B;
    queue<int> E;
    int last_size = index.size();
    //dividing by 1
    while(!index.empty()){
        A.push(index.back());
        index.pop_back();
        D.push(A);
        A.pop();
    }
    while(D.front().size() < last_size){ //loop the element vector size equal to the given size
        Qclear<queue<int>>(C);
        Qclear<int>(A);
        Qclear<int>(B);
        Qclear<int>(E);
        while(D.size() > 1){
            A = D.front();
            D.pop();
            B = D.front();//memcpy(&B, &D.front(), sizeof(int) * D.front().size());
            D.pop();
            E = ref_merge<T>(A, B, F);
            C.push(E);
        }
        if(!D.empty()){
            C.push(D.front());
            D.pop();
        }
        swap(D,C);
    }
    vector<int> ANS;
    while(D.front().size() > 0){
        ANS.push_back(D.front().front());
        D.front().pop();
    }
    return ANS;
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
vector<int> merge_sort(vector<int> index){
    if(index.size() <= 1)
        return index;
    vector<vector<int>> D;
    //dividing by 1
    for(int i =0; i< index.size(); i++){
        vector<int> A(index.begin() + i, index.begin() + i + 1);
        D.push_back(A);
    }
    while(D[0].size() < index.size()){ //loop the element vector isze equal to the given size
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