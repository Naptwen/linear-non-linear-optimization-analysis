#ifndef VAGLORITHM_H__
#define VAGLORITHM_H__

#pragma onece
#include <vector>
#include <string>
#include <sstream>
#include <stack>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctype.h>
#include <math.h>

#define RESET "\033[0m"
#define BLACK "\033[30m"              /* Black */
#define RED "\033[31m"                /* Red */
#define GREEN "\033[32m"              /* Green */
#define YELLOW "\033[33m"             /* Yellow */
#define BLUE "\033[34m"               /* Blue */
#define MAGENTA "\033[35m"            /* Magenta */
#define CYAN "\033[36m"               /* Cyan */
#define WHITE "\033[37m"              /* White */
#define BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m"   /* Bold White */

#define SAMPLE 0
#define POPUL 1
#define SHORT 97
#define WIDE 1327

using namespace std;

struct cmp{
    bool operator()(const pair<float,float> &a, const pair<float,float> &b){
        return a.first < b.first;
    }
};

typedef struct quartile{
    float Q1, Q2, Q3, IQR, UF, UW, LF, LW;
}QUARTILE;

//-------------convinient algorithm-----------

//reference by https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
//Charles Salvia's code Thank you
//it has problem for distinguish negative sign and dot sign
//so I edit this algorithm
bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin() + 1, s.end(), 
    [](char c) { return (c == '-' || c == '.')? false : !isdigit(c); }) == s.end();
}
//Applying above code 
//detecting space string
bool is_space(const string& s)
{
    return !s.empty() && find_if(s.begin(), s.end(), 
    [](unsigned char c) { return !isspace(c); }) == s.end();
}
// q clear efficient way idea reference from https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently Thank you!
template <typename T>
void Qclear(std::queue<T> &q)
{
    queue<T> empty;
    swap(q, empty);
}
// typcasting idea reference from https://gist.github.com/mark-d-holmberg/862733 Thank you!
// by Mark Holmberg
template <typename T>
T convert_to(const string &str)
{
    istringstream ss(str);
    T var;
    ss >> var;
    return var;
}
//string split 
template <typename T>
vector<T> split(string txt, const char *deli)
{
    vector<T> temp;
    char ch[txt.length()];
    strcpy(ch, txt.c_str());
    char *ptr = strtok(ch, deli);
    while (ptr != NULL)
    {
        temp.push_back(convert_to<T>(ptr));
        ptr = strtok(NULL, deli);
    }
    return temp;
}

//-------------probability algorithm----------------

// factorial
int facto(int n)
{
    int N = 1;
    for (int i = 1; i <= n; i++)
        N *= i;
    return N;
}
// binomial
int binom(int n, int k)
{
    int N = 1;
    int M = 1;
    for (int i = n; i > k; i--)
        N *= i;
    for (int i = 1; i <= n - k; i++)
        M *= i;
    return N / M;
}
// sum
//requried continous sequence data
float sum(float *begin, float *end)
{
    float sum = 0;
    while (begin <= end)
    {
        sum += begin[0];
        begin = (begin + 1);
    }
    return sum;
}
// average
//requried continous sequence data
float mean(float *begin, float *end)
{
    float sum = 0;
    int n = 0;
    while (begin <= end)
    {
        sum += begin[0];
        begin = (begin + 1);
        n++;
    }
    if (n == 0)
        perror("[MEAN] : divided by 0");
    return sum / n;
}
// POPUL : poulation variance
// SAMPLE : sample variance
//requried continous sequence data
float var(float *begin, float *end, int type = POPUL)
{
    float m = mean(begin, end);
    float sum = 0;
    int n = 0;
    while (begin <= end)
    {
        sum += (begin[0] - m) * (begin[0] - m);
        begin = (begin + 1);
        n++;
    }
    if (n == 0)
        perror("[VARIANCE] : divided by 0");
    return (type == POPUL) ? sum / n : sum / (n - 1);
}
//max normalzation
//requried continous sequence data
vector<float> mnz(float* begin, float* end){
    vector<float> ANS;
    float max = *max_element(begin, end);
    while (begin <= end)
    {
        float sd = begin[0]/max;
        ANS.push_back(sd);
        begin = (begin + 1);
    }
    return ANS;
}
//min max normalzation
//requried continous sequence data
vector<float> mmnz(float* begin, float* end){
    vector<float> ANS;
    float max = *max_element(begin, end);
    float min = *min_element(begin, end);
    float gap = max - min;
    while (begin <= end)
    {
        float sd = (begin[0] - min)/(gap);
        ANS.push_back(sd);
        begin = (begin + 1);
    }
    return ANS;
}
//standardization
//z-score normalization
//requried continous sequence data
vector<float> stdz(float* begin, float* end){
    vector<float> ANS;
    float mu = mean(begin, end);
    float v = var(begin, end);
    if(v == 0){
        perror("STD is 0!");
        return ANS;
    }
    while (begin <= end)
    {
        float sd = (begin[0] - mu)/sqrtf(v);
        ANS.push_back(sd);
        begin = (begin + 1);
    }
    return ANS;
}
// sorted data required
// The 1st element : Q1 (median lower half)
// The 2nd element : medain 
// The 3rd element : Q3 (median upper half)
// The 4th element : IQR (Q3- Q1)
// The 5th element : Upper Fence (IQR * 1.5)
// The 6th element : Upper Whisker (IQR - Q3)
QUARTILE box_plot(vector<float> data)
{
    int N = data.size();
    if(!(data.size() % 2)){
        float median = (data[N / 2] + data[N / 2 - 1])/2;
        data.insert(data.begin() + N/2, median);
        N = data.size();
    }
    QUARTILE ANS ={
        data[N / 4],
        data[N / 2],
        data[N * 3 / 4],
        0,0,0,0,0
    };
    ANS.IQR = ANS.Q3 - ANS.Q1;
    ANS.UF = ANS.Q3 + ANS.IQR * 1.5;
    ANS.UW = ANS.UF - ANS.Q3;
    ANS.LF = ANS.Q1 - ANS.IQR * 1.5;;
    ANS.LW = ANS.Q1 - ANS.LF;
    return ANS;
}
// covariance
// data x and data y should be the same size
// if not, the size is considred as the shorteset one
float cov(vector<float> datax, vector<float> datay)
{
    int sz = min(datax.size(), datay.size());
    float mux = mean((float *)&datax[0], (float *)&datax[sz - 1]);
    float muy = mean((float *)&datay[0], (float *)&datax[sz - 1]);
    float sum = 0;
    for (int i = 0; i < sz; i++)
    {
        sum += datax[i] * datay[i];
    }
    return (sum / sz) - (mux * muy);
}
// type:
// POPUL : poulation variance
// SAMPLE : sample variance
float cor(vector<float> datax, vector<float> datay, int type = POPUL)
{
    int sz = min(datax.size(), datay.size());
    float vx = var((float *)&datax[0], (float *)&datax[sz - 1], type);
    float vy = var((float *)&datax[0], (float *)&datax[sz - 1], type);
    return cov(datax, datay) / sqrt(vx * vy);
}
// variance for multiplication of xy
float varxy(vector<float> datax, vector<float> datay)
{
    return cov(datax, datax) + 2 * cov(datax, datax) + cov(datay, datay);
}

//-----reference comaparing merge algorithm------

template <typename T>
queue<int> ref_merge(queue<int> A, queue<int> B, vector<string> *C)
{
    queue<int> O;
    while (A.size() > 0)
    {
        if (B.size() > 0 && (*C)[A.front()] > (*C)[B.front()])
        {
            O.push(B.front());
            B.pop();
            continue;
        }
        O.push(A.front());
        A.pop();
    }
    while (B.size() > 0)
    {
        O.push(B.front());
        B.pop();
    }
    return O;
}
template <>
queue<int> ref_merge<float>(queue<int> A, queue<int> B, vector<string> *C)
{
    queue<int> O;
    while (!A.empty())
    {
        if (!B.empty() && stof((*C)[A.front()]) > stof((*C)[B.front()]))
        {
            O.push(B.front());
            B.pop();
            continue;
        }
        O.push(A.front());
        A.pop();
    }
    while (!B.empty())
    {
        O.push(B.front());
        B.pop();
    }
    return O;
}
template <>
queue<int> ref_merge<string>(queue<int> A, queue<int> B, vector<string> *C)
{
    queue<int> O;
    while (A.size() > 0)
    {
        if (B.size() > 0 && (*C)[A.front()].compare((*C)[B.front()]) > 0)
        {
            O.push(B.front());
            B.pop();
            continue;
        }
        O.push(A.front());
        A.pop();
    }
    while (B.size() > 0)
    {
        O.push(B.front());
        B.pop();
    }
    return O;
}
template <typename T>
vector<int> ref_merge_sort(vector<int> index, vector<string> *F)
{
    if (index.size() <= 1)
        return index;
    queue<queue<int>> D;
    queue<queue<int>> C;
    queue<int> A;
    queue<int> B;
    queue<int> E;
    int last_size = index.size();
    // dividing by 1
    while (!index.empty())
    {
        A.push(index.back());
        index.pop_back();
        D.push(A);
        A.pop();
    }
    while (D.front().size() < last_size)
    { // loop the element vector size equal to the given size
        Qclear<queue<int>>(C);
        Qclear<int>(A);
        Qclear<int>(B);
        Qclear<int>(E);
        while (D.size() > 1)
        {
            A = D.front();
            D.pop();
            B = D.front(); // memcpy(&B, &D.front(), sizeof(int) * D.front().size());
            D.pop();
            E = ref_merge<T>(A, B, F);
            C.push(E);
        }
        if (!D.empty())
        {
            C.push(D.front());
            D.pop();
        }
        swap(D, C);
    }
    vector<int> ANS;
    while (D.front().size() > 0)
    {
        ANS.push_back(D.front().front());
        D.front().pop();
    }
    return ANS;
}

//---------merge algorithm-----------------------

vector<int> merge(vector<int> A, vector<int> B)
{
    vector<int> O;
    int j = 0;
    int i = 0;
    while (i < A.size())
    {
        if (j < B.size() && A[i] > B[j])
        {
            O.push_back(B[j]);
            j++;
        }
        else
        {
            O.push_back(A[i]);
            i++;
        }
    }
    for (; j < B.size(); j++)
        O.push_back(B[j]);
    return O;
}
vector<int> merge_sort(vector<int> index)
{
    if (index.size() <= 1)
        return index;
    vector<vector<int>> D;
    // dividing by 1
    for (int i = 0; i < index.size(); i++)
    {
        vector<int> A(index.begin() + i, index.begin() + i + 1);
        D.push_back(A);
    }
    while (D[0].size() < index.size())
    {                          // loop the element vector isze equal to the given size
        vector<vector<int>> C; // make a new vec2d
        for (int k = 0; k < (D.size() >> 1); k++)
        {                                                      // loop D elements
            vector<int> B = merge(D[k << 1], D[(k << 1) + 1]); // merge
            C.push_back(B);                                    // push to C
        }
        if (D.size() % 2 == 1)
            C.push_back(D[D.size() - 1]);
        D = C;
    }
    return D[0];
}

//------html drawing algrithm-------------

void html_canvas_create(ofstream *html_file, int w, int h, string canvs_name){
    //canvas
    *html_file << "<canvas id = \""<< canvs_name <<"\"></canvas>\n";
    *html_file << "<script>\n";
    *html_file << "var canvas = document.getElementById(\""<< canvs_name <<"\");\n"; //get canvas elements (set the drawing area)
    *html_file << "canvas.width = " << w << ";\n";
    *html_file << "canvas.height = " << h << ";\n";
    *html_file << "var img = canvas.getContext(\"2d\");\n"; //to fill background
    *html_file << "img.fillStyle = '#AAAAAA';\n";
    *html_file << "img.fillRect(0, 0, "<< w << "," << h << ");\n";
    *html_file << "</script>\n";
}
void html_box_plot(ofstream *html_file, int w, int h,  vector<vector<float>> data){
    html_canvas_create(html_file, w, h, "box_plot");
    float x_padding = w/2;
    float y_padding = h/data.size()*0.5;
    *html_file << "<script>\n";
    *html_file << "var pen = canvas.getContext(\"2d\");\n";
    for(auto vec : data){
        sort(vec.begin(), vec.end());
        vector<float> z_data = mmnz((float*)&vec[0], (float*)&vec[vec.size() - 1]);
        QUARTILE o_box = box_plot(vec);
        QUARTILE box = box_plot(z_data);
        float ratio = (box.UF - box.LF) * w * 0.1;
        //drawing point
        for(auto val : z_data){
            *html_file << "pen.beginPath();\n";
            *html_file << "pen.arc(" <<  val * ratio+ x_padding<< "," << y_padding << ", 2, 0, 2*Math.PI);\n";
            *html_file << "pen.stroke();\n";
        }
        //box plot drawing
        *html_file << "pen.beginPath();\n"; //to fill background
        *html_file << "pen.lineWidth = 1;\n";
        *html_file << "pen.rect("<< box.Q1 * ratio+ x_padding<< "," << y_padding - 10 
                    << "," << box.IQR * ratio<< "," << 20<<");\n";
        *html_file << "pen.stroke();\n";
        //lne drawing
        *html_file << "pen.beginPath();\n";
        *html_file << "pen.lineWidth = 2;\n";
        *html_file << "pen.moveTo("<< box.Q2 * ratio + x_padding << "," <<y_padding - 10 <<")\n";
        *html_file << "pen.lineTo("<< box.Q2 * ratio + x_padding << "," <<y_padding + 10 <<")\n";
        *html_file << "pen.stroke();\n";
        *html_file << "pen.beginPath();\n";
        *html_file << "pen.lineWidth = 1;\n";
        *html_file << "pen.moveTo("<< box.UF * ratio + x_padding << "," <<y_padding - 20 <<")\n";
        *html_file << "pen.lineTo("<< box.UF * ratio + x_padding << "," <<y_padding + 20 <<")\n";
        *html_file << "pen.moveTo("<< box.LF * ratio + x_padding << "," <<y_padding - 20 <<")\n";
        *html_file << "pen.lineTo("<< box.LF * ratio + x_padding << "," <<y_padding + 20 <<")\n";
        *html_file << "pen.moveTo("<< (box.Q3 + box.UW) * ratio + x_padding << "," <<y_padding<<")\n";
        *html_file << "pen.lineTo("<< box.Q3 * ratio + x_padding << "," <<y_padding<<")\n";
        *html_file << "pen.moveTo("<< (box.Q1 - box.LW) * ratio + x_padding << "," <<y_padding<<")\n";
        *html_file << "pen.lineTo("<< box.Q1 * ratio + x_padding << "," <<y_padding<<")\n";
        //text mark
        *html_file << "pen.font = \"15px Arial\";\n";
        *html_file << "pen.fillStyle = \"blue\";\n";
        *html_file << "pen.fillText(\"Q1 [\" + Math.round(" << o_box.Q1 << " * 100)/100.0 + \"]\"," << box.Q1 * ratio + x_padding << "," << y_padding - 45 << ");\n";
        *html_file << "pen.moveTo("<< box.Q1 * ratio + x_padding << "," <<y_padding - 45 <<")\n";
        *html_file << "pen.lineTo("<< box.Q1 * ratio + x_padding << "," <<y_padding <<")\n";
        *html_file << "pen.fillText(\"Q2 [\" + Math.round(" << o_box.Q2 << " * 100)/100.0 + \"]\"," << box.Q2 * ratio + x_padding << "," << y_padding - 30 << ");\n";
        *html_file << "pen.moveTo("<< box.Q2 * ratio + x_padding << "," <<y_padding - 30 <<")\n";
        *html_file << "pen.lineTo("<< box.Q2 * ratio + x_padding << "," <<y_padding <<")\n";
        *html_file << "pen.fillText(\"Q3 [\" + Math.round(" << o_box.Q3 << " * 100)/100.0 + \"]\"," << box.Q3 * ratio + x_padding << "," << y_padding - 15 << ");\n";
        *html_file << "pen.moveTo("<< box.Q3 * ratio + x_padding << "," <<y_padding - 15 <<")\n";
        *html_file << "pen.lineTo("<< box.Q3 * ratio + x_padding << "," <<y_padding <<")\n";
        *html_file << "pen.stroke();\n";
        y_padding += y_padding;
    }
    *html_file << "</script>\n";
}
//need the same size of two different data set that sorted by relationship with each other
//data_1 be the x coordinate
//data_2 ge the y coordainte
//the length should be the same if not the shortest one be
void html_2xy_plot(ofstream *html_file, int w, int h,  vector<float> data_1, vector<float> data_2){
    string name = "xyP" + to_string(time(NULL));
    if(!data_1.empty() && !data_2.empty()){
        html_canvas_create(html_file, w, h, name);
        *html_file << "<script>\n";
        *html_file << "var pen = canvas.getContext(\"2d\");\n";
        int sz = min(data_1.size(), data_2.size());
        vector<float> val_x = mnz((float*)&data_1[0], (float*)&data_1[sz-1]);
        vector<float> val_y = mnz((float*)&data_2[0], (float*)&data_2[sz-1]);
        float x_padding = 25;
        float y_padding = 25;
        int px = 15;
        *html_file << "pen.lineWidth = 1;\n";
        *html_file << "pen.moveTo("<< x_padding << "," << 0 <<")\n";
        *html_file << "pen.lineTo("<< x_padding << "," << h <<")\n";
        *html_file << "pen.moveTo("<< 0 << "," << h - y_padding <<")\n";
        *html_file << "pen.lineTo("<< w << "," << h - y_padding <<")\n";
        *html_file << "pen.stroke();\n";
        float _w_ = 0;
        float _h_ = 0;
        for(int i = 0; i < sz; i ++){
            _w_ = (w - x_padding * 2) * val_x[i];
            _h_ = (h - y_padding * 2) * val_y[i] + y_padding;
            *html_file << "pen.transform(1, 0, 0, -1, 0," << h << ");\n";
            *html_file << "pen.beginPath();\n";
            *html_file << "pen.arc("<< _w_ << "," 
                                    << _h_
                                    << ", 2, 0, 2 * Math.PI);\n";
            *html_file << "pen.stroke();\n";
            *html_file << "pen.transform(1, 0, 0, -1, 0," << h << ");\n";
            *html_file << "pen.font = \"15px Arial\";\n";
            *html_file << "pen.fillStyle = \"blue\";\n";
            //x coordinate
            *html_file << "pen.fillText(\"" << round(data_1[i] * 100)/100.0 << "\"," 
                                            << _w_ - px * 0.5  << "," 
                                            << h << ");\n";
            //y coordinate 
            *html_file << "pen.fillText(\"" << round(data_2[i] * 100)/100.0 << "\"," 
                                            << px/2 << "," 
                                            << h - _h_<< ");\n";
            *html_file << "pen.stroke();\n";
        }
        *html_file << "</script>\n";
    }
}
//x sorted line graph
void html_2xy_line(ofstream *html_file, int w, int h,  vector<float> data_1, vector<float> data_2){
    string name = "xyL" + to_string(time(NULL));
    if(!data_1.empty() && !data_2.empty()){
        html_canvas_create(html_file, w, h, name);
        *html_file << "<script>\n";
        *html_file << "var pen = canvas.getContext(\"2d\");\n";
        int sz = min(data_1.size(), data_2.size());
        vector<float> val_x = mnz((float*)&data_1[0], (float*)&data_1[sz-1]);
        vector<float> val_y = mnz((float*)&data_2[0], (float*)&data_2[sz-1]);
        float x_padding = 25;
        float y_padding = 25;
        int px = 15;
        *html_file << "pen.lineWidth = 1;\n";
        *html_file << "pen.moveTo("<< x_padding << "," << 0 <<")\n";
        *html_file << "pen.lineTo("<< x_padding << "," << h <<")\n";
        *html_file << "pen.moveTo("<< 0 << "," << h - y_padding <<")\n";
        *html_file << "pen.lineTo("<< w << "," << h - y_padding <<")\n";
        *html_file << "pen.stroke();\n";

        set<pair<float, float>> val_xy;
        set<pair<float, float>> data_xy;
        for(int i = 0; i < sz; i++){
            val_xy.insert(make_pair(val_x[i],val_y[i]));
            data_xy.insert(make_pair(data_1[i],data_2[i]));
        }

        float _w_ = 0;
        float _h_ = 0;

        *html_file << "pen.beginPath();\n";
        for(int i = 0; i < val_xy.size(); i ++){
            pair<float, float> val = *next(val_xy.begin(), i); //index i value of set access
            pair<float, float> data = *next(data_xy.begin(), i); //index i value of set access
            
            _w_ = (w - x_padding * 2) * val_x[i];
            _h_ = (h - y_padding * 2) * val_y[i] + y_padding;
            *html_file << "pen.transform(1, 0, 0, -1, 0," << h << ");\n";
            *html_file << "pen.lineTo("<< _w_ << "," << _h_ <<")\n";
            *html_file << "pen.transform(1, 0, 0, -1, 0," << h << ");\n";
            *html_file << "pen.font = \"15px Arial\";\n";
            *html_file << "pen.fillStyle = \"blue\";\n";
            //x coordinate
            *html_file << "pen.fillText(\"" << round(data.first * 100)/100.0 << "\"," 
                                            << _w_ - px * 0.5  << "," 
                                            << h << ");\n";
            //y coordinate 
            *html_file << "pen.fillText(\"" << round(data.second * 100)/100.0 << "\"," 
                                            << px/2 << "," 
                                            << h - _h_<< ");\n";
        }
        *html_file << "pen.stroke();\n";
        *html_file << "</script>\n";
    }
}
template<int type, typename T>
void html_histogram(ofstream *html_file, int w, int h, float interval, vector<T> data){};
template<>
void html_histogram<SHORT, float>(ofstream *html_file, int w, int h, float interval, vector<float> data){
    string name = "histoS" + to_string(time(NULL));
    html_canvas_create(html_file, w, h, name);
    *html_file << "<script>\n";
    *html_file << "var pen = canvas.getContext(\"2d\");\n";
    float s = 0;
    sort(data.begin(), data.end());
    map<float, int> his;
    int ymax = 0;
    int k = 0;
    for(int i = 0; i < data.size(); i++){
        if(data[i] <= k * interval)
            his[k * interval] += 1;
        else{
            while(data[i] > ++k * interval);
            his[k * interval] = 1;
        }
        if(ymax < his[k * interval])
            ymax = his[k * interval]; 
    }
    float y_padding = 15;
    float x_padding = 5;
    float font_px = 10;
    float bar_x = (w - x_padding * (his.size() + 1) )/ (his.size() + 1);
    float bar_y = h * 0.8 / ymax; 
    float rx1 = 0;
    float rx2 = 0;
    for(int j = 0; j <= ymax; j++){
        //y axis value
        *html_file << "pen.beginPath();\n"; 
        *html_file << "pen.fillStyle = \"black\";\n";   
        *html_file << "pen.fillText(\"\t"<< j << "\"," << 0 << "," 
                                        <<  h - bar_y * j - y_padding * 2 
                                        << "," << bar_x << ");\n";
        *html_file << "pen.stroke();\n";
        *html_file << "pen.stroke();\n";
    }
    int i = 0;
    for(auto v = his.begin(); v != his.end(); ++v){
        // bar graph
        *html_file  << "pen.beginPath();\n"; 
        *html_file  << "penlineWidth = 1;\n";
        *html_file  << "pen.fillStyle = \"#00FF0A\";\n";
        *html_file  << "pen.fillRect("
                    << bar_x * i + x_padding * (i + 2)
                    << "," << h - bar_y * v->second - y_padding * 2
                    << "," << bar_x << "," << bar_y * v->second << ");\n";
        i++;
    }
    i = 0;
    for(auto v = his.begin(); v != his.end(); ++v){
        // text 
        *html_file << "pen.font = \"" << font_px << "px Arial\";\n";
        *html_file << "pen.fillStyle = \"blue\";\n";   
        rx1 = round(v->first * 100)/100.0;
        rx2 = round((v->first - interval) * 100)/100.0;
        *html_file << "pen.fillText(\""<< rx1 << "\"," 
                    << bar_x * (i + 1) + x_padding * (i-2) << "," 
                    << h - y_padding - font_px << ","
                    << bar_x << ");\n";
        *html_file << "pen.fillText(\""<< rx2 << "\"," 
                    << bar_x * i + x_padding * (i + 2) << "," 
                    << h - y_padding - font_px * 2 << ","
                    << bar_x << ");\n";
        *html_file << "pen.fillStyle = \"red\";\n";  
        *html_file << "pen.fillText(\""<< v->second << "\"," 
                    << bar_x * i + x_padding * (i + 1) + bar_x * 0.5 << "," 
                    << h - bar_y * v->second - y_padding  << "," 
                    << bar_x << ");\n";
        i++;
    }
    *html_file << "</script>\n";

}
template<>
void html_histogram<WIDE, float>(ofstream *html_file, int w, int h, float interval, vector<float> data){
    string name = "histoW" + to_string(time(NULL));
    html_canvas_create(html_file, w, h, name);
    *html_file << "<script>\n";
    *html_file << "var pen = canvas.getContext(\"2d\");\n";
    float s = 0;
    sort(data.begin(), data.end());
    map<float, int> his;
    int ymax = 0;
    int k = 0;
    for(int i = 0; i < data.size(); i++){
        if(data[i] <= k * interval)
            his[k * interval] += 1;
        else{
            while(data[i] > ++k * interval);
            his[k * interval] = 1;
        }
        if(ymax < his[k * interval])
            ymax = his[k * interval]; 
    }
    float y_padding = 15;
    float x_padding = 5;
    float font_px = 10;
    float bar_x = (w - x_padding * (k + 2) )/ (k + 2);
    float bar_y = h * 0.8 / ymax; 
    float rx1 = 0;
    for(int j = 0; j <= ymax; j++){
        //y axis value
        *html_file << "pen.beginPath();\n"; 
        *html_file << "pen.fillStyle = \"black\";\n";   
        *html_file << "pen.fillText(\"\t"<< j << "\"," 
                                        << 0 << "," 
                                        <<  h - bar_y * j - y_padding * 2   << "," 
                                        << bar_x << ");\n";
        *html_file << "pen.stroke();\n";
        *html_file << "pen.stroke();\n";
    }
    for(int i = 0; i <= k; i++){
        // bar graph
        *html_file << "pen.beginPath();\n"; 
        *html_file << "penlineWidth = 1;\n";
        *html_file << "pen.fillStyle = \"#00FF0A\";\n";
        *html_file  << "pen.fillRect("
                    << bar_x * (i-1) + x_padding * (i + 2)
                    << "," << h - bar_y * his[interval * i] - y_padding * 2
                    << "," << bar_x << "," << bar_y * his[interval * i] << ");\n";
    }
    for(int i =0; i <= k; i++){
        // text 
        *html_file << "pen.font = \""<< font_px << "px Arial\";\n";
        *html_file << "pen.fillStyle = \"blue\";\n";   
        rx1 = round(interval * i * 100)/100;
        *html_file << "pen.fillText(\""<< rx1 << "\"," 
                    << bar_x * i + x_padding * (i + 2) << "," << h - y_padding - 2 << "," 
                    << bar_x << ");\n";
        if(his[interval * i] != 0){
        *html_file << "pen.fillStyle = \"red\";\n";  
        *html_file << "pen.fillText(\""<< his[interval * i] << "\"," 
                    << bar_x * (i - 0.5) + x_padding * (i + 2) << ","
                    << h - bar_y * his[interval * i] - font_px << ","
                    << bar_x << ");\n";
        }
    }
    *html_file << "</script>\n";
}
template<>
void html_histogram<SHORT, string>(ofstream *html_file, int w, int h, float interval, vector<string> data){
    string name = "histoSS" + to_string(time(NULL));
    html_canvas_create(html_file, w, h, name);
    *html_file << "<script>\n";
    *html_file << "var pen = canvas.getContext(\"2d\");\n";
    float s = 0;
    sort(data.begin(), data.end());
    map<string, int> his;
    int ymax = 0;
    int k = 0;
    for(int i = 0; i < data.size(); i++){
        his[data[i]] += 1;
        if(ymax < his[data[i]])
            ymax = his[data[i]]; 
    }
    float y_padding = 15;
    float x_padding = 10;
    float bar_x = (w - x_padding * (his.size() + 1) )/ (his.size() + 1);
    float bar_y = h * 0.8 / ymax; 
    for(int j = 0; j <= ymax; j++){
        //y axis value
        *html_file << "pen.beginPath();\n"; 
        *html_file << "pen.fillStyle = \"black\";\n";   
        *html_file << "pen.fillText(\"\t"<< j << "\"," << 0 << "," 
                                        <<  h - bar_y * j - y_padding * 2 << ","
                                        << bar_x << ");\n";
        *html_file << "pen.stroke();\n";
        *html_file << "pen.stroke();\n";
    }
    int i = 0;
    for(auto v = his.begin(); v != his.end(); ++v){
        // bar graph
        *html_file  << "pen.beginPath();\n"; 
        *html_file  << "penlineWidth = 1;\n";
        *html_file  << "pen.fillStyle = \"#00FF0A\";\n";
        *html_file  << "pen.fillRect("
                    << bar_x * i + x_padding * (i + 1)
                    << "," << h - bar_y * v->second - y_padding * 2
                    << "," << bar_x << "," << bar_y * v->second << ");\n";
        i++;
    }
    i = 0;
    for(auto v = his.begin(); v != his.end(); ++v){
        // text 
        *html_file << "pen.font = \"10px Arial\";\n";
        *html_file << "pen.fillStyle = \"blue\";\n";   
        *html_file << "pen.fillText(\""<< v->first << "\"," 
                    << bar_x * i + x_padding * (i + 1) << "," << h - y_padding - 2 << ","
                    << bar_x << ");\n";
        *html_file << "pen.fillStyle = \"red\";\n";  
        *html_file << "pen.fillText(\""<< v->second << "\"," 
                    << bar_x * i + x_padding * (i + 1) + bar_x * 0.5 << "," 
                    << h - bar_y * v->second - y_padding << ","
                    << bar_x << ");\n";
        i++;
    }
    *html_file << "</script>\n";

}
#endif