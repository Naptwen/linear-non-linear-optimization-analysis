#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string.h>
#include <math.h>
#include <vector>
#include "vcsv_reader.hpp"

#define LEFT 0
#define RIGHT 1
#define BOTH 2
#define SAMPLE 0
#define POPUL 1
#define is ^
#define put +
#define set = 
#define orless <=
#define more >


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
float var(float* begin, float* end, int type){
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

class X{
public:
    float r;
    float p;
    float m;
    float a, b;
    float K, N, n;
    virtual float operator set (float k)= 0;
};
class B: public X{
public :
    float operator set (float k){
        return binom(n, k) * pow(p, k) * pow((1-p), n - k) ;
    }
};
class NB: public X{
    float operator set (float k){
        return binom(k-1, r-1) * pow(p,k) * pow(1-p, k-r);
    }
};
class PO: public X{
    float operator = (float k){
        return pow(m, k) * exp(-m) / facto(k);
    }
};
class U: public X{
    float operator = (float k){
        return 1/(b-a);
    }
};
class H: public X{
    float operator set (float k){
        return binom(K, k) * binom(N - K, n - k)/binom(N, n);
    }
};
class G: public X{
    float operator set (float k){
        return pow((1-p), k-1) * p;
    }
};
class E: public X{
    float operator set (float k){
        return m * exp(-m * k);
    }
};
/*
B : Binomial
NB : Negative Binomial
PO : Possion
U : Uniform
H : Hyperbolic
G : Geometry
E : Expotnetial
*/
template<class T>
class P{
public:
    T type;
    float operator orless(float k){
        float sum = 0;
        for(int i =0; i<= (int)k; i++)
            sum += type set i;
        return sum;
    }
    float operator more(float k){
        float sum = *this orless k;
        return 1 - sum;
    }
    float operator is (float k){
        return type set k;
    }
    void operator put (float data){
        if(is_same<T, PO>::value)
            type.m = data;
        else if(is_same<T, G>::value)
            type.p = data;
        else if(is_same<T, E>::value)
            type.m = data;
    }
    void operator put (float* data){
        if(is_same<T, B>::value){
            type.n = data[0];
            type.p = data[1];
        }
        else if(is_same<T, NB>::value){
            type.r = data[0];
            type.p = data[1];
        }         
        else if(is_same<T, U>::value){
            type.a = data[0];
            type.b = data[1];
        }      
        else if(is_same<T, H>::value){
            type.N = data[0];
            type.K = data[1];
            type.n = data[2];
        }   
    }
};
//Hypothesis 
class PHANTOM{
public:
    //-------------------------------------
    float pm1 = 0; //population mean
    float pp1 = 0; //population propotion
    float ps1 = 0; //population std
    int N1 = 0;    //population size
    //-------------------------------------
    float pm2 = 0; //population mean
    float pp2 = 0; //population propotion
    float ps2 = 0; //population std
    int N2 = 0;    //population size
    //-------------------------------------
    float p1 = 0; //propotion 1
    float p2 = 0; //propotion 2
    float m1 = 0; //mean 1
    float m2 = 0; //mean 2
    int n1 = 0; //sample1 size
    int n2 = 0; //sample2 size
    float v1 = 0; //variance 1
    float v2 = 0; //variance 2
    float z = 0;    
    float t = 0;
    //-------------------------------------
    vector<float> z_table_ver;
    vector<float> z_table_hor;
    vector<float> z_table_cell;    
    //-------------------------------------
    vector<float> c_table_ver;
    vector<float> c_table_hor;
    vector<float> c_table_cell;  
    //-------------------------------------
    PHANTOM(){
        z_table();
        c_table();
    }
    void z_table(){
        z_table_ver.clear();
        z_table_hor.clear();
        string line;
        ifstream csv_file;
        csv_file.open("Z_table.csv");
        if(csv_file.is_open()){
            cout << "Z FILE OPEN" <<endl;
            getline(csv_file, line);
            z_table_ver = split<float>(line, ",");
            getline(csv_file, line);
            z_table_hor = split<float>(line, ",");
            while(getline(csv_file, line)){
                if(line.length() > 1){
                    char ch[line.length()];
                    strcpy(ch, line.c_str());
                    char * ptr = strtok(ch, ",");
                    while(ptr != NULL){
                        z_table_cell.push_back(atof(ptr));
                        ptr = strtok(NULL, ",");
                    }
                }
            }
            csv_file.close();
        }
    }
    void c_table(){
        c_table_ver.clear();
        c_table_hor.clear();
        string line;
        ifstream csv_file;
        csv_file.open("C_table.csv");
        if(csv_file.is_open()){
            cout << "C FILE OPEN" <<endl;
            getline(csv_file, line);
            c_table_ver = split<float>(line, ",");
            getline(csv_file, line);
            c_table_hor = split<float>(line, ",");
            while(getline(csv_file, line)){
                if(line.length() > 1){
                    char ch[line.length()];
                    strcpy(ch, line.c_str());
                    char * ptr = strtok(ch, ",");
                    while(ptr != NULL){
                        c_table_cell.push_back(atof(ptr));
                        ptr = strtok(NULL, ",");
                    }
                }
            }
            csv_file.close();
        }
    }
    void population_setting(float* population1, int p1size, float* population2, int p2size){
        if(population1 == NULL) perror("NULL POPULATION 1 IS INPUT!");
        pm1 = mean(population1, population1 + (p1size-1));
        ps1 = sqrt(var(population1, population1 + (p1size-1), POPUL));
        N1 = p1size;
        if(population2 != NULL){
            pm2 = mean(population2, population2 + (p2size-1));
            ps2 = sqrt(var(population2, population2 + (p2size-1), POPUL));
            N2 = p2size;
        }
    }
    void sample_setting(float* sample1, int s1size, float* sample2, int s2size){
        if(sample1 == NULL) perror("NULL SAMPLE 1 IS INPUT!");
        m1 = mean(sample1, sample1 + (s1size-1));
        v1 = var(sample1, sample1 + (s1size-1), POPUL);
        n1 = s1size;
        if(sample1 != NULL){
            m2 = mean(sample2, sample2 + (s2size-1));
            v2 = var(sample2, sample2 + (s2size-1), POPUL);
            n2 = s2size;
        }
    }
    void poportion_setting(float population1_proportion, float population2_proportion, float sample1_proportion, float sample2_proportion){
        pp1 = population1_proportion;
        pp2 = population2_proportion;
        p1 = sample1_proportion;
        p2 = sample2_proportion;
    }
    //ctable
    //n is cols, m is rows
    float c_table_var(float c, int n, int m, float alpha){
        int free_degree = (n-1) * (m-1);
        int k = 0;
        float min = 1;
        if(free_degree > 0){
            min = 1;
            k = 0;
            for(int j = 0; j < c_table_hor.size(); j++){
                float absc = (c_table_hor[free_degree - 1] <  alpha)? alpha - c_table_hor[free_degree - 1] : c_table_hor[free_degree - 1] - alpha;
                if(absc < min){
                    min = absc;
                    k = j;
                }
            }
            
        }
        return c_table_ver[free_degree - 1] + c_table_hor[k];
    }
    //p value
    //type : LEFT, RIGHT, BOTH
    float pval(float z, int type){
        int i = 0, j = 0;
        float temp = roundf(((z < 0)?-z:z) * 100) / 100;
        for(i = 0; i < z_table_ver.size(); i++)
            if(z_table_ver[i] > temp){
                break;
            } 
        for(j = 0; j < z_table_hor.size(); j++)
            if(z_table_ver[i - 1] + z_table_hor[j] == temp){
                break;
            }
        if(type == RIGHT){
           return 1 - z_table_cell[i * z_table_hor.size() + j];
        }else if(type == BOTH){
           return min(z_table_cell[i * z_table_hor.size() + j], 1 - z_table_cell[i * z_table_hor.size() + j]);
        }
        return z_table_cell[i * z_table_hor.size() + j];
    }
    //one sample z test
    float oszt(){
        z = (m1 - pm1) / (ps1 *sqrt(n1));
        return z;
    }
    //two sample z test 
    float tszt(){
        z = ((m1- m2) - (pm1 - pm2)) / sqrt(v1/n1 + v2/n2);
        return z;
    }
    //one sampe t test
    float ostt(){
        t = (m1 - pm1)/(ps1 * sqrt(n1));
        return t;
    }
    //one sample proportion z test
    float ospzt(){
        z = (p1 - pp1) / sqrt(pp1 * (1-pp1)/n1);
        return z;
    }
    //two sample proportion z test 
    float tspzt(){
        z = ((p1- p2) - (pp1 - pp2)) / sqrt((p1 * (1 - p1))/n1 + (p2 * (1 - p2))/n2);
        return z;
    }
    //two sample t test
    float tstt(){
        t = (m1 - m2 - (pm1 - pm2))/sqrt(v1/n1 + v2/n2);
        return t;
    }
    //chi_squared test
    //obs observed value, exp expected value
    float cst(float* obs, float* exp, int size){
        float sum = 0;
        for(int i = 0; i < size; i++)
            sum += (obs[i] - exp[i])*(obs[i] - exp[i])/exp[i];
        return sum;
    }
};