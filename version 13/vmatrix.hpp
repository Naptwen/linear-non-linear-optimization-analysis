
#ifndef _VMATRIX_H__
#define _VMATRIX_H__

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

using namespace std;
#define dot *


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
            printf("%-8.8f  ", vec[0]);
    else
    for(auto vec : A){
        for(auto v : vec){
            printf("%-8.8f  ", v);
        }
        printf("\n");
    }
}

//---------1d vector-----------

template<typename T>
T operator dot (vector<T> AT, vector<T> B){
    if(AT.size() != B.size()) throw overflow_error("[dot_product] the size of vectors is not match");
    T ans = 0;
    for(int i = 0; i < AT.size(); i++)
        ans += AT[i] * B[i];
    return ans;
}
template<typename T>
vector<T> operator + (vector<T> A, vector<T> B){
    if(A.size() != B.size()) throw overflow_error("[dot_product] the size of vectors is not match");
    vector<T> C(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = A[i] + B[i];
    return C;
}
template<typename T>
vector<T> operator - (vector<T> A, vector<T> B){
    if(A.size() != B.size()) throw overflow_error("[dot_product] the size of vectors is not match");
    vector<T> C(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = A[i] - B[i];
    return C;
}
template<typename T>
vector<T>& operator - (vector<T>& A){
    vector<T> C(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        C[i] = -A[i];
    return C;
}

template<typename T>
vector<T> operator * (T c, vector<T> A){
    vector<T> ans(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        ans[i] = A[i] * c;
    return ans;
}
template<typename T>
vector<T> operator * (vector<T> A, T c){
    vector<T> ans(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        ans[i] = c * A[i];
    return ans;
}

template<typename T>
vector<T> operator / (vector<T> A, T c){
    vector<T> ans(A.size());
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        if(c != 0)
            ans[i] = A[i] / c;
        else
            throw overflow_error("[vector] / : divided by 0 error");
    return ans;
}
template<typename T>
vector<vector<T>> identity_matrix (int m){
    vector<vector<T>> A(m, vector<T>(m,0));
    for(int i = 0; i < m; i++)
        A[i][i] = 1;
    return A;
}

//---------2d vector----------

//not a opitmal value ( not porjection of ATAx^ = ATB )
//this is for the actual solution value
//if not exist return 0 size vector
template<typename T>
vector<vector<T>> operator + (vector<vector<T>> A, vector<vector<T>> B){
    if(A.size() != B.size() && A[0].size() != B[0].size()) throw out_of_range("[vector2d] - : the size is not matching!");
    vector<vector<T>> C(A.size(), vector<T>(A[0].size(), 0) );
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}   
template<typename T>
vector<vector<T>> operator - (vector<vector<T>> A, vector<vector<T>> B){
    if(A.size() != B.size() && A[0].size() != B[0].size()) throw out_of_range("[vector2d] - : the size is not matching!");
    vector<vector<T>> C(A.size(), vector<T>(A[0].size(), 0) );
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        for(int j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}   
template<typename T>
vector<vector<T>> operator * (vector<vector<T>> A, vector<vector<T>> B){
    if(A[0].size() != B.size()) throw out_of_range("[vector2d] * : the size is not matching!");
    vector<vector<T>> C(A.size(), vector<T>(B[0].size(),0));
    #pragma omp for
    for(int  i = 0; i < A.size(); i++)
        for(int  j = 0; j < A[0].size(); j++){
            T val = 0;
            for(int  k = 0; k < B.size(); k++){
                #pragma omp atomic
                val += A[i][k] * B[k][j];
            }
            C[i][j] = val;
        }
    return C;
}   
template<typename T>
vector<T> operator * (vector<vector<T>> A, vector<T> B){
    if(A[0].size() != B.size()) throw out_of_range("[vector2d] * : the size is not matching!");
    vector<T> C(A.size());
    #pragma omp for
    for(int  i = 0; i < A.size(); i++){
        T val = 0;
        for(int  k = 0; k < B.size(); k++){
            #pragma omp atomic
            val += A[i][k] * B[k];
        }
        C[i] = val;
    }
    return C;
}

template<typename T>
vector<vector<T>> operator * (vector<vector<T>> A, T c){
    vector<vector<T>> C(A.size(), vector<T>(A[0].size(),0));
    #pragma omp parallel for
    for(int  i = 0; i < A.size(); i++)
        for(int  j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] * c;
    return C;
}   
template<typename T>
vector<vector<T>> operator * (T c, vector<vector<T>> A){
    vector<vector<T>> C(A.size(), vector<T>(A[0].size(),0));
    #pragma omp parallel for
    for(int  i = 0; i < A.size(); i++)
        for(int  j = 0; j < A[0].size(); j++)
            C[i][j] = A[i][j] * c;
    return C;
}   
//--------1d vector algorithm---------
// proj u [v] v on u

template<typename T>
vector<T> poject(vector<T> u, vector<T> v){
    vector<T> proj(u.size());
    T c = 0;
    c += v dot u;
    T norm = u dot u;
    c /= (norm != 0)?norm : 1;
    proj = c * u;
    return proj;
}
template<typename T>
T norm2(vector<T> u){
    T ans = 0;
    for(int i = 0; i < u.size(); i++)
        ans += u[i] * u[i];
    return sqrt(ans);
}

//--------2d vector algorithm---------

template<typename T>
vector<vector<T>> zero_vector(vector<vector<T>> A){
    #pragma omp for
    for(int i = 0; i < A.size(); i++)
        transform(A[i].begin(), A[i].end(), A[i].begin(), 0);
    return A;
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
//deepcopy
//upper triangular reduce form
template<typename T>
vector<vector<T>> REDUCE(vector<vector<T>> A, bool show = true){
    int k = 0;
    if(show){
        printf("\tORIGIN\n");
        show_2d_vector(A);
    }
    while(k < A.size()){
        if(A[k][k] == 0 && k < min(A.size(), A[0].size()){ //if diagonal is a 0
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
                float r = log( abs(A[i][k]) ) - log( abs(A[k][k]) );
                const char sign1 = dir_sign(A[i][k]);
                const char sign2 = dir_sign(A[k][k]);
                for(int j = 0; j < A[0].size(); j++){
                    const char sign3 = dir_sign(A[k][j]);
                    if(sign1 * sign2 * sign3 > 0)
                        A[i][j] = A[i][j] - exp( log( abs(A[k][j]) ) + r);
                    else
                        A[i][j] = A[i][j] + exp( log( abs(A[k][j]) ) + r);
                    if(A[i][j] < 0.00000001) //it should be the error problem
                        A[i][j] = 0;
                }
                // A[i][k] = 0; //too avoid realnumber problem
            }
        }
        k++;
    }
    if(show){
        printf("\tREDUCE FORM\n");
        show_2d_vector(A);
    }
    return A;
}
//deepcopy
//guassian elimination form
template<typename T>
vector<vector<T>> GELIMINATION(vector<vector<T>>A, bool show = false){
    A = REDUCE(A, show);
    int i = 0;
    while(i < A.size()){
        #pragma omp for
        for(int j = 0; j < A[i].size(); j++)
            if(A[i][j] != 0){
                float p  = A[i][j];
                A[i][j] = 1; //to avoid real number problem
                for(int k = j + 1; k < A[i].size(); k++){
                    const char sign = dir_sign(A[i][k]) * dir_sign(p);
                    A[i][k] = exp(log(abs(A[i][k])) - log(abs(p))) * sign;
                }
                break;
            }
        i++;
    }
    if(show){
        printf("\tGAUSSIAN ELIMINAION\n");
        show_2d_vector(A);
    }
    return A;
}
//row echelon reduce form
//sz is the distinguish cols index number for argument matrix
template<typename T>
vector<vector<T>> ROWECHELONFORM(vector<vector<T>>A, int sz, bool show = false){
    vector<vector<T>> R = GELIMINATION(A, show);
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
        show_2d_vector(R);
    }
    return R;
}
//assume A is linearly independent matrix with column basis
//Required full column square rank (non sigualr square matrix = linearly independet full column matrix)
template<typename T>
vector<vector<T>> Grim_Schmidt(vector<vector<T>> A){
    vector<vector<T>> B = vectorTranspose(A);
    vector<vector<T>> U(B.size()); 
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
    return vectorTranspose(U);
}
template<typename T>
bool isTri(vector<vector<T>> A, float decimal = 10000.0f){
    if(A.size() != A[0].size()) throw invalid_argument("[isTri] : it is not a sqaure matrix");
    for(int i = 0; i < A.size(); i ++)
        for(int j = 0; j < i; j++)
            if(round(A[i][j] * decimal)/decimal != 0) return false;
            else A[i][j] = 0; //to avoid real number problem.
    return true;
}
template<typename T>
vector<T> eigen_value(vector<vector<T>> A, bool show = false){
    vector<vector<T>> Q = Grim_Schmidt(A);
    vector<T> eigenvalue(A.size());
    unsigned int k = 0;
    if(show){
        printf("\tQ\n");
        show_2d_vector(Q);
        printf("\tR\n");
        show_2d_vector(vectorTranspose(Q) * A);
    }
    while(!isTri(A,1000.0f) && k++ < 100000){
        Q = Grim_Schmidt(A);
        A = vectorTranspose(Q) * A * Q;
    }
    if(k > 100000) throw out_of_range("[eigen_value] it is imaginary # or out of iteration"), abort();
    for(int i = 0; i < A.size(); i++)
        eigenvalue[i] = A[i][i];
    return eigenvalue;
}
template<typename T>
vector<vector<T>> MATRIX_SUB(vector<vector<T>> A, int sr, int sc, int srsize, int scsize){
    vector<vector<T>> B(srsize, vector<T>(scsize, 0));
    for(int i = sr; i < sr + srsize; i++)
        for(int j = sc; j < sc + scsize; j++)
            if(j < A[0].size() && i < A.size())
                B[i-sr][j-sc] = A[i][j];
            else
                printf("A[%d, %d]\n", i, j),throw out_of_range("[MATRIX_SUB] : The sub size is out of range");
    return B;
}
template<typename T>
vector<vector<T>> ADDROW(vector<vector<T>> mat, vector<T> var, int i){
    if(var.size() != mat[0].size()) throw out_of_range("[ADDROW] row size is not matching"), abort();
    mat.insert(mat.begin() + i, var);
    return mat;
}
template<typename T>
vector<vector<T>> ADDCOL(vector<vector<T>> mat, vector<T> var, int i){
    if(var.size() != mat.size()) throw out_of_range("[ADDCOL] col size is not matching"), abort();
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
template<typename T>
vector<T> MATRIX_SOLUTION(vector<vector<T>> mat, vector<T> var, bool show = false){
    if(var.size() == mat.size()){
        vector<vector<T>> tmp = ADDCOL<T>(mat, var, mat[0].size());
        vector<vector<T>> ans = ROWECHELONFORM<float>(tmp, tmp[0].size()-1, show);
        //find equation variables
        vector<T> temp;
        vector<T> x(mat[0].size(), 0); 
        for(int i = 0; i < ans[0].size() - 1; i++){
            temp.clear();
            for(int j = i; j < ans[0].size() - 1; j++)
                if(ans[i][j] != 0)
                    temp.push_back(ans[i][j]);
            if(temp.empty() && ans[i].back() != 0)
                perror("[MATRIX_SOLUTION] : no solution");
            else if(temp.empty() && ans[i].back() == 0)
                x[i] = 1;
        }
        //
        for(int i = 0; i < ans[0].size() - 1; i++){
            if(x[i] == 0){
                x[i] = ans[i].back();
                for(int j = i + 1; j < ans[0].size() - 1; j++)
                    if(x[j] != 0) x[i] -= ans[i][j] * x[j];
            }
        }
        return x;
    }
    vector<T> x;
    return x;
}
//Don't detect lineary independent 
template<typename T>
vector<vector<T>> MATRIX_INVERSE(vector<vector<T>> A, bool show = false){
    if(A.size() != A[0].size()) throw out_of_range("[MATRIX_INVERE] : A size is not square");
    vector<vector<T>> B = A;
    for(int i = 0; i < A[0].size(); i++){
        vector<T> e(B.size(), 0);
        e[i] = 1;
        B = ADDCOL(B, e, B[0].size());
    }
    vector<vector<T>> INVA = ROWECHELONFORM(B, A.size());
    if(show){
        printf("\tECHELON\n");
        show_2d_vector(INVA);
    }
    INVA = MATRIX_SUB(INVA, 0, INVA[0].size()/2, INVA[0].size()/2, INVA[0].size()/2);
    if(show){
        printf("\tINVERSE\n");
        show_2d_vector(INVA);
    }
    return INVA;
}

template<typename T>
vector<vector<vector<T>>> EIGEN_DECOMPOSITION(vector<vector<T>> A, bool show = false){
    if(A[0].size() != A.size()) throw out_of_range("[DECOMPOSITION] : it is not a square matrix!");
    vector<vector<T>> Λ;
    vector<vector<T>> V;
    vector<T> zero(A.size(), 0);  
    vector<T> λ = eigen_value(A, show);
    if(show){
        printf("\tA\n"), show_2d_vector(A);
    }
    sort(λ.begin(), λ.end(), greater<T>());
    for(int i = 0; i < λ.size(); i++){
        Λ = identity_matrix<T>(A.size());
        Λ =  Λ * λ[i]; //to avoid real number problem
        if(show){
            printf("\tλ * I\n"), show_2d_vector(Λ);
        }
        vector<T> v = MATRIX_SOLUTION(A - Λ, zero, show);
        V.push_back(v);
    }
    Λ =  identity_matrix<T>(Λ.size());
    for(int i = 0; i < A.size(); i++)
        Λ[i][i] = λ[i];
    V = vectorTranspose(V);
    if(show){
        printf("\tΛ\n"), show_2d_vector(Λ);
        printf("\tV\n"), show_2d_vector(V);
    }
    vector< vector< vector<T> > > VΛ;
    VΛ.push_back(V);
    VΛ.push_back(Λ);
    return VΛ;
}

template<typename T>
bool diagonal_positive_check(vector<vector<T>> A)
{
    for(int i = 0; i < min(A.size(), A[0].siz()); i++)
        if(A[i][i] < 0) return false;
    return true;
}
//      MATRIX

template<typename T>
class MATRIX{
    public:
        vector<vector<T>> mat;
        int m = 0;
        int n = 0;
        //Matrix setting

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
        void SETSIZE(){
            this->m = mat.size();
            this->n = mat[0].size();
        }
        void SET(vector<vector<T>> var){
            mat.clear();
            mat = var;
            SETSIZE();
        }
        void IDENTITY(int m){
            mat.clear();
            mat.resize(m, vector<T>(m,0));
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
                    mat.resize(len, vector<T>(mat[0].size(), 0));
                else if(len < mat.size())
                    mat.erase(mat.begin() + len, mat.end());
            }
            SETSIZE(); //update size
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
        //Matrix tool

        MATRIX SUBMATRIX(int sr, int sc, int srsize, int scsize){
            MATRIX A(srsize, scsize);
            for(int i = sr; i < sr + srsize; i++)
                for(int j = sc; j < sc + scsize; j++)
                    if(j < mat[0].size() && i < mat.size())
                        A.mat[i-sr][j-sc] = mat[i][j];
                    else
                        throw out_of_range("[MATRIX] SUB MATRIX: The sub size is out of range");
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
        vector<int> ROWS_INDPENDENT(){
            MATRIX AT = TRANS();
            vector<vector<T>> RAT = ROWECHELONFORM(AT.mat, AT.mat[0].size());
            return COUNTPIVOT(RAT, RAT[0].size());
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
        MATRIX INVERSE(){
            if(mat.size() == mat[0].size()){
                MATRIX<T> A__ = *this; //copy the matrix
                MATRIX<T> I(mat.size(), mat.size()); //make identity matrix
                I.IDENTITY();
                A__ = MERGECOL(A__, I); //A|I
                vector<vector<T>> AINVER = ROWECHELONFORM(A__.mat, A__.m); 
                vector<int> pivot = COUNTPIVOT(AINVER, A__.m); 
                if(pivot.size() != A__.m)
                    throw out_of_range("[MATRIX] INVERSE :It's COLUMNS are NOT lineary Independent can't INVERTIBLE"), abort();
                MATRIX INVER;
                INVER.SET(AINVER);
                return INVER.SUBMATRIX(0,A__.m,A__.m, A__.m);
            }
            else
                throw out_of_range("[MATRIX] INVERSE: The m and n size different can't inverse");
        }
        
        // MATRIX EIGENVALUE(){
        //     if(mat[0].size() != mat.size()) throw out_of_range("[MATRIX] EIGENVALUE : it is not a square matrix\n");
        //     vector<vector<T>> I = identity_matrix(mat.size());

        // }
        int RANK(){
            vector<vector<T>> reduce = GELIMINATION(mat);
            return COUNTPIVOT(reduce, reduce[0].size()).size();
        }
        //Matrix operator

        MATRIX operator * (MATRIX<T> t){
            if(mat[0].size() != t.mat.size()) throw out_of_range("[MATRIX] * Matrix :  A m x n and B n x k, the n is not the same size");
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
        MATRIX operator * (vector<T> t){
            if(mat[0].size() != t.size()) throw out_of_range("[MATRIX] A m x n and B n x k, the n is not the same size");
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
                throw out_of_range("[MATRIX] + MATRIX: matrix are not the same size!"), abort();
            MATRIX<T> A(mat.size(), t.mat[0].size());
            A.mat = mat;
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] += t.mat[i][j];
            return A;
        }
        MATRIX operator + (vector<T> t){            
            if(mat.size() != t.size())
                throw out_of_range("[MATRIX] + vector: matrix'cols and vector are not the same size!"), abort();
            MATRIX<T> A(mat.size(), 1);
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] = mat[i][j] + t[i];
            return A;
        }
        MATRIX operator - (MATRIX<T> t){
            if(mat.size() != t.mat.size() || mat[0].size() != t.mat[0].size())
                throw out_of_range("[MATRIX] - MATRIX: matrix are not the same size!"), abort();
            MATRIX<T> A(mat.size(), t.mat[0].size());
            A.mat = mat;
            #pragma omp for
            for(int i =0; i< A.mat.size(); i++)
                for(int j = 0; j< A.mat[i].size(); j++)
                    A.mat[i][j] -= t.mat[i][j];
            return A;
        }
        MATRIX operator - (vector<T> t){            
            if(mat.size() != t.size())
                throw out_of_range("[MATRIX] - vector: matrix'cols and vector are not the same size!"), abort();
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
};
template<typename T>
MATRIX<T> MERGE_COL(MATRIX<T> A, MATRIX<T> B){
    if(A.mat.size() != B.mat.size()) throw out_of_range("[MATRIX] MERGECOL: the #rows are not same!"), abort();
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
MATRIX<T> MERGE_ROW(MATRIX<T> A, MATRIX<T> B){
    if(A.mat[0].size() != B.mat[0].size()) throw out_of_range("[MATRIX] MERGEROWS: the #cols are not same!"), abort();
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
MATRIX<T> VEC_MERGE_COL(MATRIX<T> A, vector<T> B){
    if(A.mat.size() != B.size()) throw out_of_range("[MATRIX] VECMERGECOL: the #cols are not same!"), abort();
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
MATRIX<T> VEC_MERGE_ROW(MATRIX<T> A, vector<T> B){
    if(A.mat[0].size() != B.size()) throw out_of_range("[MATRIX] VECMERGEROW: the #rows are not same!"), abort();
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
