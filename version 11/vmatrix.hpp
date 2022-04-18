
#ifndef _VMATRIX_H__
#define _VMATRIX_H__

#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <queue>
using namespace std;

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
    if(var.size() == mat[0].size())
    mat.insert(mat.being() + i, var);
    return mat;
}
template<typename T>
vector<vector<T>> ADDCOL(vector<vector<T>> mat, vector<T> var, int i){
    if(var.size() == mat.size())
    #pragma omp for
    for(int j =0; j < mat.size(); j++)
        mat[j].insert(mat[j].begin() + i, var[j]);
    return mat;
}


template<typename T>
class MATRIX{
    public:
        vector<vector<T>> mat;
        MATRIX(size_t m, size_t n){
            mat.resize(m, vector<T>(n, 0));
        }
        void PRINTF(){
            printf("--------------MATRIX-------------\n");
            printf("%lu, %lu\n", mat.size(), mat[0].size() );
            for(int  i = 0; i < mat.size(); i++){
                for(int j = 0; j < mat[0].size(); j++){
                    printf("%2.2f\t", mat[i][j]);
                }
            printf("\n");
            }
            printf("---------------------------------\n");
        }
        void IDENTITY(){
            #pragma omp for
            for(int  i = 0; i < min(mat.size(), mat[0].size()); i++)
                mat[i][i] = 1;
        }
        void TRANS(){
            vector<vector<T>> A(mat[0].size(), vector<T>(mat.size(), 0));
            #pragma omp for
            for(int  i = 0; i < A.size(); i ++){
                for(int j = 0; j < A[0].size(); j++){
                    A[i][j] = mat[j][i];
                }
            }
            mat = A;
        }
        MATRIX operator * (MATRIX<T> t){
            MATRIX<T> A(mat.size(), t.mat[0].size());
            if(mat[0].size() == t.mat.size()){
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
            }
            return A;
        }
        void operator = (const MATRIX<T> &t){
            mat = t.mat;
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

        void ROWSWAP(int i, int j){
            swap(mat[i], mat[j]);
        }
        vector<T> ROWELETE(int i){
            vector<T> ANS = mat[i];
            mat.erase(mat.begin() + i);
            return ANS;
        }
        vector<T> COLDELETE(int i){
            vector<T> ANS(mat.size());
            #pragma omp for
            for(int j = 0; j < mat.size(); j++){
                ANS[j] = mat[j][i];
                mat[j].erase(mat[j].begin() + i);
            }
            return ANS;
        }
        

        //not a opitmal value ( not porjection of ATAx^ = ATB )
        //this is for the actual solution value
        vector<T> SOLVE(vector<T> var){
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
};

#endif