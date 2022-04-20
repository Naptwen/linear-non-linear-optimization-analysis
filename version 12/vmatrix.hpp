
#ifndef _VMATRIX_H__
#define _VMATRIX_H__

#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <queue>

using namespace std;
template<typename T>
void show_vector(vector<T> A){
    for(auto vec : A)
        printf("%2.2f  ", vec);
}
template<>
void show_vector(vector<int> A){
    for(auto vec : A)
        printf("%2d  ", vec);
}
template<typename T>
void show_2d_vector(vector<vector<T>> A){
    if(A[0].size() == 1)
        for(auto vec : A)
            printf("%2.2f  ", vec[0]);
    else
    for(auto vec : A){
        for(auto v : vec){
            printf("%2.2f  ", v);
        }
        printf("\n");
    }
}
template<typename T>
vector<vector<T>> vectorTranspose(vector<vector<T>> A){
    vector<vector<T>> B(A[0].size(), vector<T>(A.size(), 0));
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            B[j][i] = A[i][j];
    return B;
}
//deepcopy
//upper triangular reduce form
template<typename T>
vector<vector<T>> REDUCE(vector<vector<T>> A){
    int k = 0;
    while(k < A.size()){
        if(A[k][k] == 0){ //if diagonal is not a 0
            for(int i = k + 1; i < A.size(); i++)
                if(A[i][k] != 0){    //find not 0 rows in lower 
                    swap(A[k],A[i]); //swap row
                    break;
                }
            if(A[k][k] == 0) //if the diagonal is still 0
                k++;
            continue;
        }
        #pragma omp for
        for(int i = k + 1; i < A.size(); i++){
            if(A[i][k] != 0){
                float r = A[i][k]/A[k][k];
                for(int j = k; j < A[0].size(); j++)
                    A[i][j] -= A[k][j] * r;
                A[i][k] = 0; //too avoid realnumber problem
            }
        }
        k++;
    }
    return A;
}
//deepcopy
//guassian elimination form
template<typename T>
vector<vector<T>> GELIMINATION(vector<vector<T>>A){
    A = REDUCE(A);
    int i = 0;
    while(i < A.size()){
        #pragma omp for
        for(int j = 0; j < A[i].size(); j++)
            if(A[i][j] != 0){
                float p  = A[i][j];
                A[i][j] = 1; //to avoid real number problem
                for(int k = j + 1; k < A[i].size(); k++)
                    A[i][k] = A[i][k] / p;
                break;
            }
        i++;
    }
    return A;
}
//row echelon reduce form
//sz is the distinguish cols index number for argument matrix
template<typename T>
vector<vector<T>> ROWECHELONFORM(vector<vector<T>>A, int sz){
    vector<vector<T>> R = GELIMINATION(A);
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
    return R;
}
//Must reduce matrix form is required
template<typename T>
vector<int> COUNTPIVOT(vector<vector<T>> A, int sz){
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
template<typename T>
vector<vector<T>> ADDROW(vector<vector<T>> mat, vector<T> var, int i){
    if(var.size() != mat[0].size()) perror("MATRIX ADDROW size is not matching"), abort();
    mat.insert(mat.begin() + i, var);
    return mat;
}
template<typename T>
vector<vector<T>> ADDCOL(vector<vector<T>> mat, vector<T> var, int i){
    if(var.size() != mat.size()) perror("MATRIX ADDCOL size is not matching"), abort();
    #pragma omp for
    for(int j =0; j < mat.size(); j++)
        mat[j].insert(mat[j].begin() + i, var[j]);
    return mat;
}
template<typename T>
vector<vector<T>> ROWSWAP(vector<vector<T>> mat, int i, int j){
    swap(mat[i], mat[j]);
}
template<typename T>
vector<vector<T>> ROWDELETE(vector<vector<T>> mat, int i){
    vector<T> ANS = mat[i];
    mat.erase(mat.begin() + i);
    return mat;
}
template<typename T>
vector<vector<T>> COLDELETE(vector<vector<T>> mat, int i){
    vector<T> ANS(mat.size());
    #pragma omp for
    for(int j = 0; j < mat.size(); j++){
        ANS[j] = mat[j][i];
        mat[j].erase(mat[j].begin() + i);
    }
    return mat;
}
//not a opitmal value ( not porjection of ATAx^ = ATB )
//this is for the actual solution value
//if not exist return 0 size vector
template<typename T>
vector<T> MATRIX_SOLUTION(vector<vector<T>> mat, vector<T> var){
    vector<T> x(mat[0].size(), 0); //n
    if(var.size() == mat.size()){
        vector<vector<T>> tmp = ADDCOL<T>(mat, var, mat[0].size());
        vector<vector<T>> ans = ROWECHELONFORM<float>(tmp, tmp[0].size()-1);
        //find equation variables
        #pragma omp for
        for(int i = 0; i < ans.size(); i++){ 
            int j = i;
            bool pivot = false; //to detect no solution
            //find pivot in curretn rows
            while(j < ans[0].size() - 1){
                if(ans[i][j] > 0){
                    x[j] = ans[i].back(); //set pivot as the y value
                    pivot = true;
                    break;
                }
                j++;
            }
            if(pivot == false && ans[i].back() != 0){ //no any pivot but y exist no solution
                x.clear();
                perror("MATRIX MATRIX_SOLUTION NO SOLUTION ERROR");
                return x;
            }
            //if we find pivot
            for(int k = j + 1; k < ans[0].size() - 1; k++){ //since it is echlon form pivot is behind or equl to pos in diagonal
                if(ans[i][k] > 0 && x[k] == 0) //free variable
                    x[k] = 1; //set any variable
                if(x[k] != 0)
                    x[j] -= ans[i][k] * x[k];
            }
        }
        return x;
    }
    return x;
}

template<typename T>
class MATRIX{
    public:
        vector<vector<T>> mat;
        int m = 0;
        int n = 0;
        MATRIX(){
            mat.clear();
        }
        MATRIX(size_t m, size_t n){
            mat.resize(m, vector<T>(n, 0));
            SETSIZE();
        }
        MATRIX(vector<vector<T>> mat){
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
        void IDENTITY(){
            #pragma omp for
            for(int  i = 0; i < min(mat.size(), mat[0].size()); i++)
                mat[i][i] = 1;
        }
        MATRIX TRANS(){
            MATRIX B;
            vector<vector<T>> A(mat[0].size(), vector<T>(mat.size(), 0));
            #pragma omp for
            for(int  i = 0; i < A.size(); i ++){
                for(int j = 0; j < A[0].size(); j++){
                    A[i][j] = mat[j][i];
                }
            }
            B.SET(A);
            return B;
        }
        int RANK(){
            vector<vector<T>> reduce = GELIMINATION(mat);
            return COUNTPIVOT(reduce, reduce[0].size()).size();
        }
        void SETSIZE(){
            this->m = mat.size();
            this->n = mat[0].size();
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
                    mat.resize(len, vector<T>(mat[0].size(), 0));
                else if(len < mat.size())
                    mat.erase(mat.begin() + len, mat.end());
            }
            SETSIZE(); //update size
        }
        MATRIX operator * (MATRIX<T> t){
            if(mat[0].size() != t.mat.size()) perror("MATRIX A m x n and B n x k, the n is not the same size"), abort();
            MATRIX<T> A(mat.size(), t.mat[0].size());
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++){
                for(int  j = 0; j < mat[0].size(); j++){
                    T val = 0;
                    for(int  k = 0; k < t.mat.size(); k++){
                        #pragma omp atomic
                        val += mat[i][k] * t.mat[k][j];
                    }
                    A.mat[i][j] = val;
                }
            }
            return A;
        }
        // m x n * n x 1
        MATRIX operator * (vector<T> t){
            if(mat[0].size() != t.size()) perror("MATRIX A m x n and B n x k, the n is not the same size"), abort();
            MATRIX<T> A(mat.size(), 1);
            int k = 0;
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++)
                for(int  j = 0; j < mat[0].size(); j++)
                    A.mat[i][0] += mat[i][j] * t[j];
            return A;
        }
        void operator = (const MATRIX<T> &t){
            mat = t.mat;
            SETSIZE();
        }
        void operator = (const T* var){
            int k = 0;
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++)
                for(int j = 0; j < mat[0].size(); j++){
                    #pragma omp atomic
                    mat[i][j] = var[k++];
                }
        }
        void operator = (const vector<T> var){
            int k = 0;
            #pragma omp parallel for
            for(int  i = 0; i < mat.size(); i++)
                for(int j = 0; j < mat[0].size(); j++){
                    #pragma omp atomic
                    mat[i][j] = var[k++];
                }
        }
        void operator = (vector<vector<T>> var){
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
        MATRIX operator + (MATRIX<T> t){            
            if(mat.size() != t.mat.size() || mat[0].size() != t.mat[0].size())
                perror("MATRIX matrix are not the same size!"), abort();
            MATRIX<T> A(mat.size(), t.mat[0].size());
            A.mat = mat;
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] += t.mat[i][j];
            return A;
        }
        MATRIX operator - (MATRIX<T> t){
            if(mat.size() != t.mat.size() || mat[0].size() != t.mat[0].size())
                perror("MATRIX matrix are not the same size!"), abort();
            MATRIX<T> A(mat.size(), t.mat[0].size());
            A.mat = mat;
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] -= t.mat[i][j];
            return A;
        }
        MATRIX operator + (vector<T> t){            
            if(mat.size() != t.size())
                perror("MATRIX matrix'cols and vector are not the same size!"), abort();
            MATRIX<T> A(mat.size(), 1);
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] = mat[i][j] + t[i];
            return A;
        }
        MATRIX operator - (vector<T> t){            
            if(mat.size() != t.size())
                perror("MATRIX matrix'cols and vector are not the same size!"), abort();
            MATRIX<T> A(mat.size(), 1);
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
        MATRIX SUBMATRIX(int sr, int sc, int srsize, int scsize){
            MATRIX A(srsize, scsize);
            for(int i = sr; i < sr + srsize; i++)
                for(int j = sc; j < sc + scsize; j++)
                    if(j < mat[0].size() && i < mat.size())
                        A.mat[i-sr][j-sc] = mat[i][j];
                    else
                        perror("MATRIX THE SUB SETSIZE IS NOT CORRECT"), abort();
            return A;
        }
        vector<T> SUBVECTOR_COL(int j){
            
            vector<T> ANS(mat.size());
            if(j >= 0)
            #pragma omp for
            for(int i = 0; i < mat.size(); i++)
                ANS[i] = mat[i][j];
            return ANS;
        }
        vector<T> SUBVECTOR_ROW(int j){
            vector<T> ANS;
            if(j >= 0)
            ANS = mat[j];
            return ANS;
        }
        void SET(vector<vector<T>> var){
            mat.clear();
            mat = var;
            SETSIZE();
        }
        vector<int> ROWS_INDPENDENT(){
            MATRIX AT = TRANS();
            vector<vector<T>> RAT = ROWECHELONFORM(AT.mat, AT.mat[0].size());
            return COUNTPIVOT(RAT, RAT[0].size());
        }
        MATRIX INVERSE(){
            if(mat.size() == mat[0].size()){
                MATRIX<T> A__ = *this; //copy the matrix
                MATRIX<T> I(mat.size(), mat.size()); //make identity matrix
                I.IDENTITY();
                A__ = MERGECOL(A__, I); //A|I
                vector<vector<T>> AINVER = ROWECHELONFORM(A__.mat, A__.m); 
                vector<int> pivot = COUNTPIVOT(AINVER, A__.m); 
                if(pivot.size() != A__.m)
                    perror("ITs COLUMNS are NOT lineary Independent can't INVERTIBLE"), abort();
                MATRIX INVER;
                INVER.SET(AINVER);
                return INVER.SUBMATRIX(0,A__.m,A__.m, A__.m);
            }
            else
                perror("MATRIX THE M and N are DIFFERENT CAN'T INVERTIBLE"), abort();
        }
        T MAX(){
            T max = mat[0][0];
            #pragma omp for
            for(int i = 0; i < mat.size(); i++){
                T candi = *max_element(mat[i].begin(), mat[i].end());
                #pragma atomic
                if(candi > max) max = candi;
            }
            return max;
        }
        T MIN(){
            T min = mat[0][0];
            #pragma omp for
            for(int i = 0; i < mat.size(); i++){
                T candi = *max_element(mat[i].begin(), mat[i].end());
                #pragma atomic
                if(candi < min) min = candi;
            }
            return min;
        }
};

template<typename T>
MATRIX<T> MERGECOL(MATRIX<T> A, MATRIX<T> B){
    if(A.mat.size() != B.mat.size()) perror("MATRIX the #rows are not same!"), abort();
    MATRIX<T> C(A.mat.size(), A.mat[0].size() + B.mat[0].size());
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; j++)
            if(j < A.mat[0].size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B.mat[i][j - A.mat[0].size()];
    return C;
}
template<typename T>
MATRIX<T> MERGEROWS(MATRIX<T> A, MATRIX<T> B){
    if(A.mat[0].size() != B.mat[0].size()) perror("MATRIX the #cols are not same!"), abort();
    MATRIX<T> C(A.mat.size() + B.mat.size(), B.mat[0].size());
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; i++)
            if(i < A.mat.size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B.mat[i - A.mat.size()][j];
    return C;
}
template<typename T>
MATRIX<T> VECMERGECOL(MATRIX<T> A, vector<T> B){
    if(A.mat.size() != B.size()) perror("MATRIX the #cols are not same!"), abort();
    MATRIX<T> C(A.mat.size(), A.mat[0].size() + 1);
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; j++)
            if(j < A.mat[0].size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B[j - A.mat[0].size() + i];
    return C;
}
template<typename T>
MATRIX<T> VECMERGEROW(MATRIX<T> A, vector<T> B){
    if(A.mat[0].size() != B.size()) perror("MATRIX the #rows are not same!"), abort();
    MATRIX<T> C(A.mat.size() + 1, A.mat[0].size());
    for(int i = 0; i < C.m; i++)
        for(int j = 0; j < C.n; i++)
            if(i < A.mat.size())
                C.mat[i][j] = A.mat[i][j];
            else
                C.mat[i][j] = B[i - A.mat.size() + i];
    return C;
}
#endif