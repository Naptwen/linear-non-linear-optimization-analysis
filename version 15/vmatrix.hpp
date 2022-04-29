#ifndef _VMATRIX_H__
#define _VMATRIX_H__
//Copyright (c) 2022. Useop Gim
//GNU License
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <queue>
#include <math.h>
#include <set>
#include <stack>
#include <numeric>
using namespace std;
#define dot *
#define THE_FIRST_VEC(X) X[0]
#define THE_FIRST(X) X[0][0]
#define SIGN(X) (X<0)?-1:1
#define SHRINK(X) VMATRIX::vec2d2vec1d(X)
#define CLUMPLE &
struct matrixT {};
const matrixT T{}; //transpose
struct matrixP {};
const matrixP p{}; //pesudo inverse

namespace VMATRIX
{
    template<typename K>
    void show_vector(vector<K> A);
    template<>
    void show_vector(vector<int> A);
    template<>
    void show_vector(vector<string> A);
    template<typename K>
    void show_vector(vector<vector<K>> A);
    template<typename K>
    void show_size(vector<vector<K>> A);
    template<typename K>
    void show_size(vector<K> A);
    
    //---------1d vector operator-----------

    template<typename K>
    K operator dot (vector<K> AT, vector<K> B);
    template<typename K>
    vector<K> operator + (vector<K> A, vector<K> B);
    template<typename K>
    vector<K> operator + (K A, vector<K> B);
    template<typename K>
    vector<K> operator + (vector<K> B, K A);
    template<typename K>
    vector<K> operator - (vector<K> A, vector<K> B);
    template<typename K>
    vector<K> operator - (vector<K> A, K B);
    template<typename K>
    vector<K>& operator - (vector<K>& A);
    template<typename K>
    vector<K> operator * (K c, vector<K> A);
    template<typename K>
    vector<K> operator * (vector<K> A, K c);
    template<typename K>
    vector<K> operator / (vector<K> A, K c);

    //--------1d vector user operator---------

    template<typename K>
    vector<vector<K>> operator ^ (vector<K> A, matrixT);
    template<typename K>
    vector<vector<K>> operator ^ (vector<K> A, matrixP);
    //--------1d vector algorithm---------

    // proj u [v] v on u
    template<typename K>
    vector<K> poject(vector<K> u, vector<K> v);
    template<typename K>
    K norm2(vector<K> u);
    template<typename K>
    vector<K> extending(vector<K> u, vector<K> v);
    template<typename K>
    vector<vector<K>> vectorTranspose(vector<K> A);
    template<typename K>
    vector<vector<K>> vectorTranspose(vector<K>* A);

    //---------2d vector operator----------

    //not a opitmal value ( not porjection of ATAx^ = ATB )
    //this is for the actual solution value
    //if not exist return 0 size vector
    template<typename K>
    vector<vector<K>> operator + (vector<vector<K>> A, vector<vector<K>> B);
    template<typename K>
    vector<vector<K>> operator - (vector<vector<K>> A, vector<vector<K>> B);
    template<typename K>
    vector<vector<K>> operator * (vector<vector<K>> A, vector<vector<K>> B);
    template<typename K>
    vector<vector<K>> operator * (vector<vector<K>> A, vector<K> B);
    template<typename K>
    vector<vector<K>> operator * (vector<K> B, vector<vector<K>> A);
    template<typename K>
    vector<vector<K>> operator * (vector<vector<K>> A, K c);
    template<typename K>
    vector<vector<K>> operator * (K c, vector<vector<K>> A);
    template<typename K>
    bool operator > (vector<K> A, K c);
    template<typename K>
    bool operator < (vector<K> A, K c);
    template<typename K>
    bool operator == (vector<vector<K>> A, K c);

    //--------2d vector user operator---------

    template<typename K>
    vector<vector<K>> operator ^ (vector<vector<K>> A, matrixT);
    template<typename K>
    vector<vector<K>> operator ^ (vector<vector<K>> A, matrixP);
    template<typename K>
    vector<vector<K>> operator ^ (vector<vector<K>> A, int pow);
    template<typename K>
    vector<vector<K>>& operator CLUMPLE (vector<vector<K>>& A, const vector<K>& B);

    //--------2d vector algorithm---------

    template<typename K>
    vector<vector<K>> vectorTranspose(vector<vector<K>> A);
    template<typename K>
    vector<vector<K>> vectorTranspose(vector<vector<K>>* A);


    //-------2d vector algorithm 2----------

    template<typename K>
    vector<K> vec2d2vec1d (vector<vector<K>> A);
    template<typename K>
    K norm1(vector<vector<K>> A);
    template<typename K>
    K Least_sqaures(vector<vector<K>> A);
    template<typename K>
    K iter_error(vector<K> A, vector<K> B, unsigned int r);
    template<typename K>
    K iter_error(vector<vector<K>> A, vector<vector<K>> B, unsigned int r);
    template<typename K>
    vector<vector<K>> identity_matrix(int m);
    template<typename K>
    vector<int> COUNTPIVOT(vector<vector<K>> A, int sz);
    //upper triangular reduce form
    template<typename K> //fix swap algorithm
    vector<vector<K>> REDUCE(vector<vector<K>> A, bool show = false);
    //[0] is P
    //[1] is L
    //[2] is U
    template<typename K>
    vector<vector<vector<K>>> PLU_decomposition(vector<vector<K>> U, bool show = false);
    template<typename K>
    vector<K> REDUCE_SOLVE_METHOD(vector<vector<K>> A, vector<K> b, bool show = false);
    template<typename K>
    vector<K> JACOBI_SOLVE_METHOD(vector<vector<K>> A, vector<K> b , bool show = false);
    //guassian elimination form
    template<typename K>
    vector<vector<K>> GRF(vector<vector<K>> A, bool show = false);
    //row echelon reduce form
    //sz is the distinguish cols index number for argument matrix
    template<typename K>
    vector<vector<K>> RREF(vector<vector<K>> A, int sz, bool show = false);
    template<typename K>
    vector<vector<K>> Grim_Schmidt(vector<vector<K>> A);
    template<typename K>
    vector<vector<K>> Power_method(vector<vector<K>> A);
    template<typename K>
    bool isTri(vector<vector<K>> A, float decimal = 10000.0f);
    template<typename K>
    vector<K> eigen_value(vector<vector<K>> A, bool show = false);
    template<typename K>
    vector<vector<K>> MATRIX_SUB(vector<vector<K>> A, int sr, int sc, int srsize, int scsize);
    template<typename K>
    vector<vector<K>> ADDROW(vector<vector<K>> mat, vector<K> var, int i);
    template<typename K>
    vector<vector<K>> ADDCOL(vector<vector<K>> mat, vector<K> var, int i);
    template<typename K>
    vector<vector<K>> MATRIXADDCOL(vector<vector<K>> A, vector<vector<K>> B);
    template<typename K>
    vector<vector<K>> ROWSWAP(vector<vector<K>> mat, int i, int j);
    template<typename K>
    vector<vector<K>> ROWDELETE(vector<vector<K>> mat, int i);
    template<typename K>
    vector<vector<K>> COLDELETE(vector<vector<K>> mat, int i);
    template<typename K>
    bool MATRIX_INF_NAN_CHECK(vector<vector<K>> A);
    template<typename K>
    vector<K> MATRIX_SOLUTION(vector<vector<K>> mat, vector<K> var, bool show = false);
    template<typename K>
    vector<vector<K>> MATRIX_INVERSE(vector<vector<K>> A, bool show = false);
    template<typename K>
    vector<vector<K>> MATRIX_PESUDOINVERSE(vector<vector<K>> A, bool show = false);
    template<typename K>
    vector<vector<vector<K>>> EIGEN_DECOMPOSITION(vector<vector<K>> A, bool show = false);
    template<typename K>
    bool diagonal_positive_check(vector<vector<K>> A);


//---------code-----------------------------

template<typename K>
void show_vector(vector<K> A){
    for(auto vec : A)
        printf("%.7f,  ", vec);
}
template<>
void show_vector(vector<int> A){
    for(auto vec : A)
        printf("%2d  ", vec);
}
template<>
void show_vector(vector<string> A){
    for(auto vec : A)
        printf("%s  ", vec.c_str());
}
template<typename K>
void show_vector(vector<vector<K>> A){
    for(auto vec : A){
        for(auto v : vec){
            printf("%.8f,  ", v);
        }
        printf("\n");
    }
}
template<typename K>
void show_size(vector<vector<K>> A){
    printf("%lu x %lu\n", A.size(), A[0].size());
}
template<typename K>
void show_size(vector<K> A){
   
    printf("%lu \n", A.size());
}

template<typename K>
vector<vector<K>> operator ^ (vector<vector<K>> A, matrixT);
template<typename K>
vector<vector<K>> operator ^ (vector<vector<K>> A, int pow);
template<typename K>
vector<vector<K>>& operator CLUMPLE (vector<vector<K>>& A, const vector<K>& B);


//---------1d vector operator-----------

template<typename K>
K operator dot (vector<K> AT, vector<K> B){
    if(AT.size() != B.size()){
        show_size(AT);
        show_size(B);
        throw length_error("[vector dot] the size of vectors is not match");
    } 
    K ans = 0;
    for(int i = 0; i < AT.size(); i++)
        ans += AT[i] * B[i];
    return ans;
}
template<typename K>
vector<K> operator + (vector<K> A, vector<K> B){
    if(A.size() != B.size())
    { 
        show_size(A);
        show_size(B);
        throw length_error("[vector operator +] the size of vectors is not match");
    }
    vector<K> C(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = A[i] + B[i];
    return C;
}
template<typename K>
vector<K> operator + (K A, vector<K> B){
    vector<K> C(B.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = A + B[i];
    return C;
}
template<typename K>
vector<K> operator + (vector<K> B, K A){
    vector<K> C(B.size());
    #pragma omp for
    for(int i = 0; i < B.size(); i++)
        C[i] = A + B[i];
    return C;
}
template<typename K>
vector<K> operator - (vector<K> A, vector<K> B){
    if(A.size() != B.size()){
        show_size(A);
        show_size(B);
        throw length_error("[vector opertator] - :  the size of vectors is not match");
    } 
    vector<K> C(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = A[i] - B[i];
    return C;
}
template<typename K>
vector<K> operator - (vector<K> A, K B){
    vector<K> C = A;
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = A[i] - B;
    return C;
}
template<typename K>
vector<K>& operator - (vector<K>& A){
    vector<K> C(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = -A[i];
    return C;
}
template<typename K>
vector<K> operator * (K c, vector<K> A){
    vector<K> ans(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        ans[i] = A[i] * c;
    return ans;
}
template<typename K>
vector<K> operator * (vector<K> A, K c){
    vector<K> ans(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        ans[i] = c * A[i];
    return ans;
}
template<typename K>
vector<K> operator / (vector<K> A, K c){
    vector<K> ans(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        if(c != 0)
            ans[i] = A[i] / c;
        else{
           
            throw runtime_error("[vector] / : divided by 0 error");
        }
    return ans;
}

//---------2d vector operator----------

//not a opitmal value ( not porjection of ATAx^ = ATB )
//this is for the actual solution value
//if not exist return 0 size vector
template<typename K>
vector<vector<K>> operator + (vector<vector<K>> A, vector<vector<K>> B){
    if(A.size() != B.size() && A[0].size() != B[0].size()){
        show_size(A);
        show_size(B);
        throw length_error("[vector2d] + : the size is not matching!");
    } 
    vector<vector<K>> C(A.size(), vector<K>(A[0].size(), 0) );
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}   
template<typename K>
vector<vector<K>> operator - (vector<vector<K>> A, vector<vector<K>> B){
    if(A.size() != B.size() && A[0].size() != B[0].size())
    {
        show_size(A);
        show_size(B);
        throw length_error("[vector2d] - : the size is not matching!");
    }
    vector<vector<K>> C(A.size(), vector<K>(A[0].size(), 0) );
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}   
template<typename K>
vector<vector<K>> operator * (vector<vector<K>> A, vector<vector<K>> B){
    if(A[0].size() != B.size())
    {
        show_size(A);
        show_size(B);
        throw length_error("[vector2d] * : the size is not matching!");
    }     
    vector<vector<K>> C(A.size(), vector<K>(B[0].size(),0));
    #pragma omp for
    for(int  i = 0; i < A.size(); i++)
        for(int  j = 0; j < B[0].size(); j++){
            K val = 0;
            for(int  k = 0; k < A[0].size(); k++){
                #pragma omp atomic
                val += A[i][k] * B[k][j];
            }
            C[i][j] = val;
        }
    return C;
}   
template<typename K>
vector<vector<K>> operator * (vector<vector<K>> A, vector<K> B){
    if(A[0].size() != B.size()){
        show_size(A);
        show_size(B);
        throw length_error("[vector2d] * : the size is not matching!");
    } 
    vector<vector<K>> C(A.size(), vector<K>(1,0));
    #pragma omp for
    for(int  i = 0; i < A.size(); i++){
        K val = 0;
        for(int  k = 0; k < B.size(); k++){
            #pragma omp atomic
            val += A[i][k] * B[k];
        }
        C[i][0] = val;
    }
    return C;
}
template<typename K>
vector<vector<K>> operator * (vector<K> B, vector<vector<K>> A){
    if(A.size() != B.size()){
        show_size(B);
        show_size(A);
        throw length_error("[vector2d] * : the size is not matching!");
    } 
    vector<vector<K>> C(1, vector<K>(A[0].size(),0));
    #pragma omp for
    for(int  i = 0; i < B.size(); i++){
        K val = 0;
        for(int  k = 0; k < B.size(); k++){
            #pragma omp atomic
            val += B[k] * A[k][i];
        }
        C[0][i] = val;
    }
    return C;
}
template<typename K>
vector<vector<K>> operator * (vector<vector<K>> A, K c){
    vector<vector<K>> C(A.size(), vector<K>(A[0].size(),0));
    #pragma omp parallel for
    for(int  i = 0; i < A.size(); i++)
        for(int  j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] * c;
    return C;
}   
template<typename K>
vector<vector<K>> operator * (K c, vector<vector<K>> A){
    vector<vector<K>> C(A.size(), vector<K>(A[0].size(),0));
    #pragma omp parallel for
    for(int  i = 0; i < A.size(); i++)
        for(int  j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] * c;
    return C;
}   
template<typename K>
bool operator > (vector<K> A, K c){
    if(find_if_not(A.begin(), A.end(), [c](K x){return x > c;}) != A.end())
        return false;
    return true;
}
template<typename K>
bool operator < (vector<K> A, K c){
    if(find_if_not(A.begin(), A.end(), [c](K x){return x < c;}) != A.end())
        return false;
    return true;
}
template<typename K>
bool operator == (vector<vector<K>> A, K c){
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        if(find_if_not(A[i].begin(), A[i].end(), [c](K x){return x == c;}) != A[i].end())
            return false;
    return true;
}

//--------1d vector algorithm---------

// proj u [v] v on u
template<typename K>
vector<K> poject(vector<K> u, vector<K> v){
    vector<K> proj(u.size());
    K c = 0;
    c += v dot u;
    K norm = u dot u;
    c /= (norm != 0)?norm : 1;
    proj = c * u;
    return proj;
}
template<typename K>
K norm2(vector<K> u){
    K ans = 0;
    for(int i = 0; i < u.size(); i++)
        ans += u[i] * u[i];
    return sqrt(ans);
}
template<typename K>
vector<K> extending(vector<K> u, vector<K> v){
    vector<K> tmp(u.size() + v.size());
    memcpy(&tmp[0], &u[0], sizeof(K) * u.size());
    memcpy(&tmp[u.size()], &v[0], sizeof(K) * v.size());
    return tmp;
}
template<typename K>
vector<vector<K>> vectorTranspose(vector<K> A)
{
    vector<vector<K>> B(A.size(),vector<K>(1,0));
    for(int i = 0; i < A.size(); i++)
        B[i][0] = A[i];
    return B;
}
template<typename K>
vector<vector<K>> vectorTranspose(vector<K>* A)
{
    vector<vector<K>> B(A->size(),vector<K>(1,0));
    for(int i = 0; i < A->size(); i++)
        B[i][0] = (*A)[i];
    return B;
}


//--------1d vector user operator---------

template<typename K>
vector<vector<K>> operator ^ (vector<K> A, matrixT)
{
    return vectorTranspose(A);
}

//--------2d vector algorithm---------

template<typename K>
vector<vector<K>> vectorTranspose(vector<vector<K>> A){
    vector<vector<K>> B(A[0].size(), vector<K>(A.size(), 0));
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            B[j][i] = A[i][j];
    return B;
}
template<typename K>
vector<vector<K>> vectorTranspose(vector<vector<K>>* A){
    vector<vector<K>> B(A[0]->size(), vector<K>(A->size(), 0));
    #pragma omp for
    for(int i = 0; i < A->size(); i++)
        for(int j = 0; j < A[0]->size(); j++)
            B[j][i] = (*A)[i][j];
    return B;
}
template<typename K>
vector<vector<K>> operator ^ (vector<vector<K>> A, matrixT)
{
    return vectorTranspose(A);
}
template<typename K>
vector<vector<K>> operator ^ (vector<vector<K>> A, matrixP)
{
    return MATRIX_PESUDOINVERSE(A);
}

//-------2d vector algorithm 2----------

template<typename K>
vector<K> vec2d2vec1d (vector<vector<K>> A){
    if(A[0].size() == 1)
        return (A^T)[0];
    else if(A.size() == 1)
        return A[0];
    vector<K> B(A.size() * A[0].size());
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            B[A[0].size() * i + j] = A[i][j];
    return B;
}
template<typename K>
K norm1(vector<vector<K>> A){
    vector<K> L(A[0].size(),0);
    K sum = 0;
    for(int j =0; j < A[0].size(); j++){
        sum = 0;
        for(int i =0; i < A.size(); i++)
            sum += abs(A[i][j]);
        L[j] = sum;
    }
    return *max_element(L.begin(), L.end());
}
template<typename K>
K Least_sqaures(vector<vector<K>> A){
    return (A * vectorTranspose(A))[0][0];
}
template<typename K>
K iter_error(vector<K> A, vector<K> B, unsigned int r){
    K xa = accumulate(A.begin(), A.end(),K(0));
    K xb = accumulate(B.begin(), B.end(),K(0));
    return xa/pow(xb,r);
}
template<typename K>
K iter_error(vector<vector<K>> A, vector<vector<K>> B, unsigned int r){
    K xa = 0;
    K xb = 0;
    for(int i = 0; i < A.size(); i++){
        xa += accumulate(A[i].begin(), A[i].end(), K(0));
        xb += accumulate(B[i].begin(), B[i].end(), K(0));
    }
    return abs(xa)/pow(abs(xb),r);
}
template<typename K>
vector<vector<K>> identity_matrix(int m){
    vector<vector<K>> A(m, vector<K>(m,0));
    for(int i = 0; i < m; i++)
        A[i][i] = 1;
    return A;
}
template<typename K>
vector<int> COUNTPIVOT(vector<vector<K>> A, int sz){
    vector<int> pivot_pos;
    int k = 0;
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < sz; j++)
            if(A[i][j] != 0){
                pivot_pos.push_back(j);
                break;
            }
    return pivot_pos;
} 
//upper triangular reduce form
template<typename K> //fix swap algorithm
vector<vector<K>> REDUCE(vector<vector<K>> A, bool show){
    int k = 0;
    if(show){
        printf("\tORIGIN\n");
        show_vector(A);
    }
    while(k < min(A.size(), A[0].size())){
        if(A[k][k] == 0) //if diagonal is a 0
        {   
            int j = k;
            for(; j < A[0].size(); j++)
            {
                for(int i = k + 1; i < A.size(); i++)
                {
                    if(A[i][j] != 0)
                    {    //find not 0 rows in lower 
                        swap(A[k],A[i]); //swap row
                        break;
                    }
                }
                if(A[k][j] != 0) //if the diagonal is not 0
                    break;
            }
        }
        #pragma omp for
        for(int i = k + 1; i < A.size(); i++)
        {
            if(A[i][k] != 0)
            {
                float r = log( abs(A[i][k]) ) - log( abs(A[k][k]) );
                const char sign1 = SIGN(A[i][k]);
                const char sign2 = SIGN(A[k][k]);
                for(int j = 0; j < A[0].size(); j++)
                {
                    const char sign3 = SIGN(A[k][j]);
                    if(sign1 * sign2 * sign3 > 0)
                        A[i][j] = A[i][j] - exp( log( abs(A[k][j]) ) + r);
                    else
                        A[i][j] = A[i][j] + exp( log( abs(A[k][j]) ) + r);
                    if(abs(A[i][j]) < 0.00000001) //it should be the error problem
                        A[i][j] = 0;
                }
                A[i][k] = 0;//too avoid realnumber problem
            }
        }
        k++;
    }
    if(show){
        printf("\tREDUCE FORM\n");
        show_vector(A);
    }
    return A;
}
//[0] is P
//[1] is L
//[2] is U
template<typename K>
vector<vector<vector<K>>> PLU_decomposition(vector<vector<K>> U, bool show)
{
    int k = 0;
    vector<vector<K>> L(U.size(), vector<K>(U.size(), 0));
    vector<vector<K>> P = identity_matrix<K>(U.size());
    if(show){
        printf("\tORIGIN\n");
        show_vector(U);
    }
    while(k < min(U.size(), U[0].size())){
        if(U[k][k] == 0) //if diagonal is a 0
        {   
            for(int j = k; j < U[0].size(); j++)
            {
                for(int i = k + 1; i < U.size(); i++)
                {
                    if(U[i][j] != 0)
                    {    //find not 0 rows in lower 
                        swap(U[k],U[i]); //swap row
                        swap(P[k],P[i]);
                        swap(L[k],L[i]);
                        cout << k << "<->" << i <<endl;
                        break;
                    }
                }
                if(U[k][j] != 0) //if the diagonal is not 0
                    break;
            }
        }
        #pragma omp for
        for(int i = k + 1; i < U.size(); i++)
        {
            if(U[i][k] != 0)
            {
                float r = log( abs(U[i][k]) ) - log( abs(U[k][k]) );
                const char sign1 = SIGN(U[i][k]);
                const char sign2 = SIGN(U[k][k]);
                if(k < L[0].size()) //update lower triangular matrix
                    L[i][k] = exp(r) * sign1 * sign2;
                for(int j = 0; j < U[0].size(); j++)
                {
                    const char sign3 = SIGN(U[k][j]);
                    if(sign1 * sign2 * sign3 > 0)
                        U[i][j] = U[i][j] - exp( log( abs(U[k][j]) ) + r);
                    else
                        U[i][j] = U[i][j] + exp( log( abs(U[k][j]) ) + r);
                    if(abs(U[i][j]) < 0.00000001) //it should be the error problem
                        U[i][j] = 0;
                }
                U[i][k] = 0;//too avoid realnumber problem
            }
        }
        k++;
    }
    for(int i = 0; i < L.size(); i++)
        L[i][i] = 1;
    if(show){
        printf("\n PLU decomposition form\n");
        printf("\tP\n");show_vector(P);
        printf("\tL\n");show_vector(L);
        printf("\tU\n");show_vector(U);
        printf("\tA\n");show_vector(P * L * U);
    }
    return {P,L,U};
}
template<typename K>
vector<K> JACOBI_SOLVE_METHOD(vector<vector<K>> A, vector<K> b , bool show)
{
    //if diagonally dominant matrix
    vector<vector<K>> invD(A.size(), vector<K>(A[0].size(), 0));    //inverse strict Diagonal triangular matrix
    vector<vector<K>> SLU(A.size(), vector<K>(A[0].size(), 0));      //negative strict Upper triangular matrix
    vector<vector<K>> B = b^T;
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
    {
        for(int j = 0; j < A.size(); j++)
        {
            if(i == j)
            {
                if(A[i][j] == 0) throw logic_error("[JACOBI_SOLVE_METHOD] : Diagonal is zero!");
                else invD[i][j] = 1/A[i][j];
            }
            else if(i < j)
            {
                SLU[i][j] = -A[i][j];
            }
            else
            {
                SLU[i][j] = -A[i][j];
            }
        }
    }
    vector<vector<K>> x(1, vector<K>(b.size(),1));
    vector<vector<K>> xk(1, vector<K>(b.size(),1));
    x = x^T;
    unsigned int r = 1;
    while(true)
    {
        xk = invD * (SLU * x + B);
        if(iter_error(x,xk,r) == 0)
            break;
        x = xk;
        r++;
    }
    return SHRINK(x);
}
template<typename K>
vector<K> REDUCE_SOLVE_METHOD(vector<vector<K>> A, vector<K> b, bool show)
{
    if(A.size() != b.size())
        throw length_error("[REDUCE_SOLVE_METHOD] A rows and B rows size are not the same!");
    vector<vector<K>> Ab = ADDCOL<K>(A, b, A[0].size());
    vector<vector<K>> ans = RREF<K>(Ab, Ab[0].size()-1, show);
    //find equation variables
    vector<K> temp;
    vector<K> x(A[0].size(), 0); 
    for(int i = 0; i < ans[0].size() - 1; i++)
    {
        temp.clear();
        for(int j = i; j < ans[0].size() - 1; j++)
            if(ans[i][j] != 0)
                temp.push_back(ans[i][j]);
        if(temp.empty() && ans[i].back() != 0)
        { 
            printf("[MATRIX_SOLUTION] : no solution\n");
            perror("MATRIX_SOLUTION()");  
            return x;
        }
        else if(temp.empty() && ans[i].back() == 0)
            x[i] = 1;
    }
    //solve equation by fixed varibles
    for(int i = 0; i < ans[0].size() - 1; i++)
    {
        if(x[i] == 0){
            x[i] = ans[i].back();
            for(int j = i + 1; j < ans[0].size() - 1; j++)
                if(x[j] != 0) x[i] -= ans[i][j] * x[j];
        }
    }
    return x;
}
template<typename K>
vector<K> MATRIX_SOLUTION(vector<vector<K>> mat, vector<K> var, bool show){
    vector<K> x(mat.size(),0);
    if(var.size() == mat.size())
    {
        if(MATRIX_INF_NAN_CHECK(mat))
        { 
            printf("[MATRIX_SOLUTION] : no solution\n");
            perror("MATRIX_SOLUTION()");
            return x;
        }
        if(mat[0].size() == var.size())
        {
            K max = mat[0][0];
            for(int i = 0; i < mat.size(); i++)
            {
                max = abs(mat[i][i]);
                if(mat[i][i] == 0)
                    return REDUCE_SOLVE_METHOD(mat, var, show);
                for(int j = 0; j < mat[0].size(); j++) //check it is diagonally dominant matrix
                    if(i != j && max < abs(mat[i][j]) )
                        return REDUCE_SOLVE_METHOD(mat, var, show);
            }
            return JACOBI_SOLVE_METHOD(mat, var, show);
        }
        else if(mat[0].size() >= var.size())
        {
            return REDUCE_SOLVE_METHOD(mat, var, show);
        }
    }
    return x;
}

//guassian elimination form
template<typename K>
vector<vector<K>> GRF(vector<vector<K>> A, bool show){
    if(show)
        printf("\tOriginal\n"), show_vector(A), printf("\n");
    int k = 0;
    while(k < min(A.size(), A[0].size())){
        if(A[k][k] == 0) //if diagonal is a 0
        {   
            int j = k;
            for(; j < A[0].size(); j++)
            {
                for(int i = k + 1; i < A.size(); i++)
                {
                    if(A[i][j] != 0)
                    {    //find not 0 rows in lower 
                        swap(A[k],A[i]); //swap row
                        break;
                    }
                }
                if(A[k][j] != 0) //if the diagonal is not 0
                    break;
            }
        }
        #pragma omp for
        if(A[k][k] != 1 && A[k][k] != 0)
        {
            float r = log( abs(A[k][k]) );
            const char sign1 = SIGN(A[k][k]);
            A[k][k] = 1; //to avoid real number problem
            for(int i = k + 1; i < A[0].size(); i++)
            {
                if(A[k][i] != 0)
                {
                    const char sign2 = SIGN(A[k][i]);
                    A[k][i] = sign1 * sign2 * exp ( log( abs(A[k][i]) ) - r);
                }
            }
            
        }
        for(int i = k + 1; i < A.size(); i++)
        {
            if(A[i][k] != 0)
            {
                float r = log( abs(A[i][k]) ) - log( abs(A[k][k]) );
                const char sign1 = SIGN(A[i][k]);
                const char sign2 = SIGN(A[k][k]);
                for(int j = 0; j < A[0].size(); j++)
                {
                    const char sign3 = SIGN(A[k][j]);
                    if(sign1 * sign2 * sign3 > 0)
                        A[i][j] = A[i][j] - exp( log( abs(A[k][j]) ) + r);
                    else
                        A[i][j] = A[i][j] + exp( log( abs(A[k][j]) ) + r);
                    if(abs(A[i][j]) < 0.00000001) //it should be the error problem
                        A[i][j] = 0;
                }
                A[i][k] = 0;//too avoid realnumber problem
            }
        }
        k++;
    }
    if(show){
        printf("\tGAUSSIAN ELIMINAION\n");
        show_vector(A);
    }
    return A;
}
//row echelon reduce form
//sz is the distinguish cols index number for argument matrix
template<typename K>
vector<vector<K>> RREF(vector<vector<K>> A, int sz, bool show){
    vector<vector<K>> R = GRF(A, show);
    #pragma omp for
    for(int i = R.size() - 1; i >= 0; i--) //from the bottom
        for(int j = 0; j < sz; j++)//find the pivot untill sz 
            if(R[i][j] != 0){  //if find pivot
                for(int k = i - 1; k >= 0; k--){ //find other rows
                    if(R[k][j] != 0){ //if the same col is not a 0
                        float p = R[k][j]; //multiplier
                        R[k][j] = 0; //avoid real number problem
                        for(int t = j + 1; t < R[0].size(); t++)
                            R[k][t] -= p * R[i][t];
                    }
                }
            break;
            }
    if(show)
    {
        printf("\tRREF\n");
        show_vector(R);
    }
    return R;
}
template<typename K>
vector<vector<K>> Grim_Schmidt(vector<vector<K>> A){
    vector<vector<K>> B = A^T;
    vector<vector<K>> U(B.size()); 
    U[0] = B[0];
    if(norm2(B[0]) != 0)
        U[0] = U[0] / norm2(B[0]);
    for(int i = 1; i < B.size(); i++){
        U[i] = B[i];
        for(int j = 0; j < i; j++)
            U[i] = U[i] - poject(U[j], B[i]);
        if(norm2(U[i]) != 0)
            U[i] = U[i]/ norm2(U[i]);
    }
    return U^T;
}
template<typename K>
vector<vector<K>> Power_method(vector<vector<K>> A)
{
    if(A.size() != A[0].size()){
        show_size(A);
        throw invalid_argument("[Power_method] : A is not a square matrix");
    } 
    vector<K> x(A.size());
    vector<K> p;
    K n = 0;
    transform(x.begin(), x.end(), x.begin(),[](K v){return 1;});
    int k = 0;
    while(k++ < 1000)
    {
        p = A * x;
        n = *max_element(p.begin(), p.end());
        transform(p.begin(), p.end(), p.begin(),[n](K v){return (n != 0)?v/n:0;});        
        if(p == x) break;
        else if(iter_error(p,x,k) == 0) break;
        x = p;
    }
    printf("λ = %f\n",n);
    printf("\tv\n");show_vector(x);printf("\n");
}
template<typename K>
bool isTri(vector<vector<K>> A, float decimal){
    if(A.size() != A[0].size())
    {
        show_size(A);
        throw invalid_argument("[isTri] : it is not a sqaure matrix");
    }
    for(int i = 0; i < A.size(); i ++)
        for(int j = 0; j < i; j++)
            if(round(A[i][j] * decimal)/decimal != 0) return false;
            else A[i][j] = 0; //to avoid real number problem.
    return true;
}
template<typename K>
vector<K> eigen_value(vector<vector<K>> A, bool show){
    vector<K> eigenvalue(A.size());
    unsigned int k = 1;
    vector<vector<K>> Q;
    vector<vector<K>> NQ;
    while(true)
    {
        Q = Grim_Schmidt(A);
        if(isnan(Q[0][0])) throw logic_error("[eigen_value] : nan value is detected!");
        else if(Q == NQ) break;
        else if(iter_error(NQ,Q,k++) == 0.0f) break;
        NQ = Q;
        A = (Q^T) * A * Q;
    }
    for(int i = 0; i < A.size(); i++)
        eigenvalue[i] = A[i][i];
    if(show){
        printf("\tλ\n");
        show_vector(eigenvalue); printf("\n");
    }
    return eigenvalue;
}
template<typename K>
vector<vector<K>> MATRIX_SUB(vector<vector<K>> A, int sr, int sc, int srsize, int scsize){
    vector<vector<K>> B(srsize, vector<K>(scsize, 0));
    for(int i = sr; i < sr + srsize; i++)
        for(int j = sc; j < sc + scsize; j++)
            if(j < A[0].size() && i < A.size())
                B[i-sr][j-sc] = A[i][j];
            else{
                show_size(A);
                printf("SUB [%d,%d]->[%d,%d]", sr,sc,sr + srsize, sc + scsize);
                throw out_of_range("[MATRIX_SUB] : The sub size is out of range");
            }
    return B;
}
template<typename K>
vector<vector<K>> ADDROW(vector<vector<K>> mat, vector<K> var, int i){
    if(var.size() != mat[0].size())
    {
        show_size(mat);
        show_size(var);
        throw length_error("[ADDROW] row size is not matching");
    } 
    mat.insert(mat.begin() + i, var);
    return mat;
}
template<typename K>
vector<vector<K>> ADDCOL(vector<vector<K>> mat, vector<K> var, int i){
    if(var.size() != mat.size())
    { 
        show_size(mat);
        show_size(var);
        throw out_of_range("[ADDCOL] col size is not matching");
    }
    #pragma omp for
    for(int j =0; j < mat.size(); j++)
        mat[j].insert(mat[j].begin() + i, var[j]);
    return mat;
}
template<typename K>
vector<vector<K>> MATRIXADDCOL(vector<vector<K>> A, vector<vector<K>> B){
    if(A.size() != B.size())
    {
        show_size(A);
        show_size(B);
        throw out_of_range("[MATRIXADDCOL] col size is not matching");
    }
    vector<vector<K>> C(A.size(), vector<K>(A[0].size() + B[0].size(),0));
    #pragma omp for
    for(int i =0; i < C.size(); i++){
        memcpy(&C[i][0], &A[i][0], sizeof(K) * A[0].size());
        memcpy(&C[i][A[0].size()], &B[i][0], sizeof(K) * B[0].size());
    }
    return C;
}
template<typename K>
vector<vector<K>> ROWSWAP(vector<vector<K>> mat, int i, int j){
    swap(mat[i], mat[j]);
}
template<typename K>
vector<vector<K>> ROWDELETE(vector<vector<K>> mat, int i){
    vector<K> ANS = mat[i];
    mat.erase(mat.begin() + i);
    return mat;
}
template<typename K>
vector<vector<K>> COLDELETE(vector<vector<K>> mat, int i){
    vector<K> ANS(mat.size());
    #pragma omp for
    for(int j = 0; j < mat.size(); j++){
        ANS[j] = mat[j][i];
        mat[j].erase(mat[j].begin() + i);
    }
    return mat;
}
template<typename K>
bool MATRIX_INF_NAN_CHECK(vector<vector<K>> A){
   for(int i = 0;i < A.size(); i++)
        for(int j = 0; j < A[i].size(); j++)
            if(isnan(A[i][j])) return true;
            else if(isinf(A[i][j])) return true;
    return false;
}
template<typename K>
vector<vector<K>> MATRIX_INVERSE(vector<vector<K>> A, bool show){
    if(A.size() != A[0].size())
    { 
        show_size(A);
        throw out_of_range("[MATRIX_INVERE] : A size is not square");
    }
    vector<vector<K>> I = identity_matrix<K>(A.size());
    vector<vector<K>> B = MATRIXADDCOL(A,I);
    vector<vector<K>> INVA = RREF(B, A.size(), show);
    if(show){
        printf("\tECHELON\n");
        show_vector(INVA);
    }
    INVA = MATRIX_SUB(INVA, 0, INVA[0].size()/2, INVA[0].size()/2, INVA[0].size()/2);
    if(show){
        printf("\tINVERSE\n");
        show_vector(INVA);
    }
    return INVA;
}
template<typename K>
vector<vector<K>> MATRIX_PESUDOINVERSE(vector<vector<K>> A, bool show){
    return MATRIX_INVERSE(vectorTranspose(A) * A, show) * vectorTranspose(A);
}
template<typename K>
vector<vector<vector<K>>> EIGEN_DECOMPOSITION(vector<vector<K>> A, bool show){
    if(A[0].size() != A.size())
    {
        show_size(A);
        throw out_of_range("[DECOMPOSITION] : it is not a square matrix!");
    }
    vector<vector<K>> Λ;
    vector<vector<K>> V;
    vector<K> zero(A.size(), 0);  
    vector<K> λ = eigen_value(A, show);
    bool zero_v = true;
    if(show)
    {
        printf("\tA\n"), show_vector(A);
    }
    sort(λ.begin(), λ.end(), greater<K>());
    for(int i = 0; i < λ.size(); i++)
    {
        Λ = identity_matrix<K>(A.size());
        Λ =  Λ * λ[i]; //to avoid real number problem
        vector<K> v = MATRIX_SOLUTION(A - Λ, zero);
        for(int i = 0; i < v.size(); i++)
            if(v[i] != 0){
                zero_v = false;
                break;
            }
        V.push_back(v);
    }
    Λ =  identity_matrix<K>(Λ.size());
    for(int i = 0; i < A.size(); i++)
        Λ[i][i] = λ[i];
    V = vectorTranspose(V);

    if(show){
        printf("\tΛ\n"), show_vector(Λ);
        printf("\tV\n"), show_vector(V);
    }
    vector< vector< vector<K> > > VΛ;
    VΛ.push_back(V);
    VΛ.push_back(Λ);
    return VΛ;
}
template<typename K>
bool diagonal_positive_check(vector<vector<K>> A)
{
    for(int i = 0; i < min(A.size(), A[0].siz()); i++)
        if(A[i][i] < 0) return false;
    return true;
}

//--------2d vector user operator---------


template<typename K>
vector<vector<K>> operator ^ (vector<vector<K>> A, int pow)
{
    if(pow == -1) return MATRIX_INVERSE(A);
    else if(pow < 0){
       
        printf("A^%d\n",pow);
        throw overflow_error("[vector2d operator] ^ : unknown exponent is input!");
    } 
    for(int i = 0; i < pow; i++)
        A = A * A;
    return A;
}
template<typename K>
vector<vector<K>>& operator CLUMPLE (vector<vector<K>>& A, const vector<K>& B)
{
    if(A.size() * A[0].size() != B.size()) {
        show_size(A);
        show_size(B);
        throw length_error("[vector2d] CLUMPLE : total A size and B size are not the same!");
    }
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            A[i][j] = B[A[0].size() * i + j];
    return A;
}

//------------MATRIX------------------------

template<typename K>
class MATRIX{
    public:
        vector<vector<K>> mat;
        int m = 0;
        int n = 0;
        //Matrix setting
        MATRIX(){
            mat.clear();
        }
        MATRIX(size_t m, size_t n){
            mat.resize(m, vector<K>(n, 0));
            SETSIZE();
        }
        MATRIX(vector<vector<K>> mat){
            this->mat = mat;
            SETSIZE();
        }
        void PRINTF(){
            printf("\t matrix %d x %d\n", m, n);
            printf("%lu, %lu\n", mat.size(), mat[0].size() );
            for(int  i = 0; i < mat.size(); i++){
                for(int j = 0; j < mat[0].size(); j++)
                    printf("%2.1f\t", mat[i][j]);
                printf("\n");
            }
        }
        void SETSIZE(){
            this->m = mat.size();
            this->n = mat[0].size();
        }
        void SETMAT(vector<vector<K>> var){
            mat.clear();
            mat = var;
            SETSIZE();
        }
        void IDENTITY(int m){
            mat.clear();
            mat.resize(m, vector<K>(m,0));
            for(int i = 0; i < mat.size(); i++)
                mat[i][i] = 1;
        }
        void COLRESIZE(int len){
            if(len >= 0){
                if(len >= mat[0].size()){
                #pragma omp for
                for(int i = 0; i < mat.size(); i++)
                    mat[i].resize(len);
                }
                else{
                #pragma omp for
                    for(int i = 0; i < mat.size(); i++)
                        mat[i].erase(mat[i].begin() + len, mat[i].end());
                }
            }
            SETSIZE(); //update size
        }
        void ROWRESIZE(int len){
            if(len >= 0){
                if(len > mat.size())
                    mat.resize(len, vector<K>(mat[0].size(), 0));
                else if(len < mat.size())
                    mat.erase(mat.begin() + len, mat.end());
            }
            SETSIZE(); //update size
        }
        K MAX(){
            K max = mat[0][0];
            #pragma omp for
            for(int i = 0; i < mat.size(); i++){
                K candi = *max_element(mat[i].begin(), mat[i].end());
                #pragma atomic
                if(candi > max) max = candi;
            }
            return max;
        }
        K MIN(){
            K min = mat[0][0];
            #pragma omp for
            for(int i = 0; i < mat.size(); i++){
                K candi = *max_element(mat[i].begin(), mat[i].end());
                #pragma atomic
                if(candi < min) min = candi;
            }
            return min;
        }
        //Matrix tool

        MATRIX SUBMATRIX(int sr, int sc, int srsize, int scsize){
            MATRIX A(srsize, scsize);
            for(int i = sr; i < sr + srsize; i++)
                for(int j = sc; j < sc + scsize; j++)
                    if(j < mat[0].size() && i < mat.size())
                        A.mat[i-sr][j-sc] = mat[i][j];
                    else{
                    throw out_of_range("[MATRIX] SUB MATRIX: The sub size is out of range");
                    }
                     
            return A;
        }
        vector<K> SUBVECTOR_COL(int j){
            
            vector<K> ANS(mat.size());
            if(j >= 0)
            #pragma omp for
            for(int i = 0; i < mat.size(); i++)
                ANS[i] = mat[i][j];
            return ANS;
        }
        vector<K> SUBVECTOR_ROW(int j){
            vector<K> ANS;
            if(j >= 0)
            ANS = mat[j];
            return ANS;
        }
        vector<int> ROWS_INDPENDENT(){
            MATRIX AT = TRANS();
            vector<vector<K>> RAT = RREF(AT.mat, AT.mat[0].size());
            return COUNTPIVOT(RAT, RAT[0].size());
        }
        MATRIX TRANS(){
            MATRIX B;
            vector<vector<K>> A(mat[0].size(), vector<K>(mat.size(), 0));
            #pragma omp for
            for(int  i = 0; i < A.size(); i ++){
                for(int j = 0; j < A[0].size(); j++){
                    A[i][j] = mat[j][i];
                }
            }
            B.SETMAT(A);
            return B;
        }
        MATRIX INVERSE(){
            if(mat.size() == mat[0].size()){
                MATRIX<K> A__ = *this; //copy the matrix
                MATRIX<K> I(mat.size(), mat.size()); //make identity matrix
                I.IDENTITY();
                A__ = MERGECOL(A__, I); //A|I
                vector<vector<K>> AINVER = RREF(A__.mat, A__.m); 
                vector<int> pivot = COUNTPIVOT(AINVER, A__.m); 
                if(pivot.size() != A__.m)
                    throw out_of_range("[MATRIX] INVERSE :It's COLUMNS are NOT lineary Independent can't INVERTIBLE"), abort();
                MATRIX INVER;
                INVER.SETMAT(AINVER);
                return INVER.SUBMATRIX(0,A__.m,A__.m, A__.m);
            }
            else
                throw out_of_range("[MATRIX] INVERSE: The m and n size different can't inverse");
        }
        
        // MATRIX EIGENVALUE(){
        //     if(mat[0].size() != mat.size()) throw out_of_range("[MATRIX] EIGENVALUE : it is not a square matrix\n");
        //     vector<vector<K>> I = identity_matrix(mat.size());

        // }
        int RANK(){
            vector<vector<K>> reduce = GRF(mat);
            return COUNTPIVOT(reduce, reduce[0].size()).size();
        }
        //Matrix operator

        MATRIX operator * (MATRIX<K> t){
            if(mat[0].size() != t.mat.size()) throw out_of_range("[MATRIX] * Matrix :  A m x n and B n x k, the n is not the same size");
            MATRIX<K> A(mat.size(), t.mat[0].size());
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++){
                for(int  j = 0; j < t.mat[0].size(); j++){
                    K val = 0;
                    for(int  k = 0; k < t.mat.size(); k++){
                        #pragma omp atomic
                        val += mat[i][k] * t.mat[k][j];
                    }
                    A.mat[i][j] = val;
                }
            }
            return A;
        }
        MATRIX operator * (vector<K> t){
            if(mat[0].size() != t.size()) throw out_of_range("[MATRIX] A m x n and B n x k, the n is not the same size");
            MATRIX<K> A(mat.size(), 1);
            int k = 0;
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++)
                for(int  j = 0; j < mat[0].size(); j++)
                    A.mat[i][0] += mat[i][j] * t[j];
            return A;
        }
        void operator = (const MATRIX<K> &t){
            mat = t.mat;
            SETSIZE();
        }
        void operator = (const K* var){
            int k = 0;
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++)
                for(int j = 0; j < mat[0].size(); j++){
                    #pragma omp atomic
                    mat[i][j] = var[k++];
                }
        }
        void operator = (const vector<K> var){
            int k = 0;
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++)
                for(int j = 0; j < mat[0].size(); j++){
                    #pragma omp atomic
                    mat[i][j] = var[k++];
                }
        }
        void operator = (vector<vector<K>> var){
            int k;
            if(var.size() == mat.size() && var[0].size() == mat[0].size()){
                #pragma omp parallel for
                for(int  i = 0; i < mat.size(); i++)
                    for(int j = 0; j < mat[0].size(); j++){
                        #pragma omp atomic
                        mat[i][j] = var[i][j];
                    }
            }
        }
        MATRIX operator + (MATRIX<K> t){            
            if(mat.size() != t.mat.size() || mat[0].size() != t.mat[0].size())
                throw out_of_range("[MATRIX] + MATRIX: matrix are not the same size!"), abort();
            MATRIX<K> A(mat.size(), t.mat[0].size());
            A.mat = mat;
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] += t.mat[i][j];
            return A;
        }
        MATRIX operator + (vector<K> t){            
            if(mat.size() != t.size())
                throw out_of_range("[MATRIX] + vector: matrix'cols and vector are not the same size!"), abort();
            MATRIX<K> A(mat.size(), 1);
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] = mat[i][j] + t[i];
            return A;
        }
        MATRIX operator - (MATRIX<K> t){
            if(mat.size() != t.mat.size() || mat[0].size() != t.mat[0].size())
                throw out_of_range("[MATRIX] - MATRIX: matrix are not the same size!"), abort();
            MATRIX<K> A(mat.size(), t.mat[0].size());
            A.mat = mat;
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] -= t.mat[i][j];
            return A;
        }
        MATRIX operator - (vector<K> t){            
            if(mat.size() != t.size())
                throw out_of_range("[MATRIX] - vector: matrix'cols and vector are not the same size!"), abort();
            MATRIX<K> A(mat.size(), 1);
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] = mat[i][j] - t[i];
            return A;
        }
        MATRIX& operator - (){
            #pragma omp for
            for(int i = 0; i < mat.size(); i++)
               for(int j = 0; j < mat[i].size(); j++){
                    mat[i][j] = -mat[i][j];
               }
            return *this;
        }
};
template<typename K>
MATRIX<K> MERGE_COL(MATRIX<K> A, MATRIX<K> B){
    if(A.mat.size() != B.mat.size()) throw out_of_range("[MATRIX] MERGECOL: the #rows are not same!"), abort();
    MATRIX<K> C(A.mat.size(), A.mat[0].size() + B.mat[0].size());
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; j++)
            if(j < A.mat[0].size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B.mat[i][j - A.mat[0].size()];
    return C;
}
template<typename K>
MATRIX<K> MERGE_ROW(MATRIX<K> A, MATRIX<K> B){
    if(A.mat[0].size() != B.mat[0].size()) throw out_of_range("[MATRIX] MERGEROWS: the #cols are not same!"), abort();
    MATRIX<K> C(A.mat.size() + B.mat.size(), B.mat[0].size());
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; i++)
            if(i < A.mat.size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B.mat[i - A.mat.size()][j];
    return C;
}
template<typename K>
MATRIX<K> VEC_MERGE_COL(MATRIX<K> A, vector<K> B){
    if(A.mat.size() != B.size()) throw out_of_range("[MATRIX] VECMERGECOL: the #cols are not same!"), abort();
    MATRIX<K> C(A.mat.size(), A.mat[0].size() + 1);
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; j++)
            if(j < A.mat[0].size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B[j - A.mat[0].size() + i];
    return C;
}
template<typename K>
MATRIX<K> VEC_MERGE_ROW(MATRIX<K> A, vector<K> B){
    if(A.mat[0].size() != B.size()) throw out_of_range("[MATRIX] VECMERGEROW: the #rows are not same!"), abort();
    MATRIX<K> C(A.mat.size() + 1, A.mat[0].size());
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; i++)
            if(i < A.mat.size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B[i - A.mat.size() + i];
    return C;
}

}
#endif