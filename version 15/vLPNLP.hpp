#ifndef _VLPNLP_H__
#define _VLPNLP_H__
#include "vmathe_solver.hpp"
//Copyright (c) 2022. Useop Gim
//GNU License
#define MSE 1 // Mean square error
#define ETRO 2 //Entorpy error 
//vector -> array
struct pscmp{
    bool operator()(pair<int, int> a, pair<int, int> b){return a.first > b.first;}
};

class NLP{
public:
    FUNMATRIX min;  //
    FUNMATRIX f;    //function or subject
    FUNMATRIX df;   //Jaccobian or derivative
    FUNMATRIX d2f;  //Hessian
    FUNMATRIX R;    //constrains fomula matrix
    vector<vector<float>> reports;
    void PRINTF(){
        cout << "-------------------NP-------------------------\n";
        printf("[min]\n"); min.PRINTF_formula();
        cout << "-------------------R--------------------------\n";
        R.PRINTF_formula();
        cout << "-------------------f--------------------------\n";
        if(!f.funmat.empty())
        printf("\t%lu x %lu\n", f.funmat.size(), f.funmat[0].size());
        f.PRINTF_formula();
        cout << "-------------------df-------------------------\n";
        if(!df.funmat.empty())
        printf("\t%lu x %lu\n", df.funmat.size(), df.funmat[0].size());
        df.PRINTF_formula();
        cout << "-------------------d2f------------------------\n";
        if(!d2f.funmat.empty())
        printf("\t%lu x %lu\n", d2f.funmat.size(), d2f.funmat[0].size());
        d2f.PRINTF_formula();
        cout << "----------------------------------------------\n";
    }
    void READING(string file_name){
        string fx = "";
        string equation = "";
        vector<string> r_x;
        vector<vector<string>> ss;
        int i = 0;
        CSV data(file_name);
        //setting minimizer level of curve
        if(data.csv[i][0].compare("min")==0) fx = data.csv[0][1];
        else if(data.csv[i][0].compare("max")==0) fx = "-1 * (" + data.csv[0][1] + ")";
        ss = {{fx}};
        min.Set_formula(ss);
        //setting subject contraints
        ss.clear();
        for(i = 1; i < data.csv.size(); i++)
        {
            r_x.clear();
            if(data.csv[i][0].compare("df")==0) break;
            else if(data.csv[i][1].compare("<=")==0)
                equation = "-1 * (" + data.csv[i][0] + ") + " + data.csv[i][2];
            else
            {
                if(data.csv[i][2].compare("0") !=0 )
                    equation = data.csv[i][0] + " - 1 * (" + data.csv[i][2] + ")";
                else
                    equation = data.csv[i][0];
            }
            r_x.push_back(equation);
            ss.push_back(r_x);
        }
        R.Set_formula(ss);
        f = R;
        //setting derivative constraints
        ss.clear();
        for(i++; i < data.csv.size(); i++)
        {
            if(data.csv[i][0].compare("d2f")==0) break;
            ss.push_back(data.csv[i]);
        }
        df.Set_formula(ss);
        //setting second derivative constraints
        ss.clear();
        for(i++; i < data.csv.size(); i++)
        {
            ss.push_back(data.csv[i]);
        }
        d2f.Set_formula(ss);
    }
    void RECORDS(string file_name){
        ofstream write(file_name.data());
        if(write.is_open())
        for(int i = 0; i < reports.size(); i++)
        {
            for(int j = 0; j < reports[0].size(); j++)
                write << reports[i][j] << ",";
            write << "\n";
        }
        write.close();
    }
    vector<vector<float>> MODEL(vector<vector<float>> f, vector<vector<float>> df, vector<float> x_xk)
    {   
        return f + df * x_xk;
    }
    //unconstrain
    //vector<float> NEWTON_METHOD(vector<float> x );
    //vector<float> MODIFIED_NEWTON_AMIJO_LINE_SEARCH(vector<float> x);
    //vector<float> GAUSS_NEWTON_AMIJO_LINE_SEARCH(vector<float> x);
    // vector<float> NEWTON_LAGRANGE_KKTsystem();
    // vector<float> GAUSS_NEWTON_AMIJO_LINE_SEARCH();
    // vector<float> SR1_AMIJO_LINE_SEARCH();
    // vector<float> Penalty_NEP();
    // vector<float> Barrier_Method__NIP();
    // vector<float> BRGS_AMIJO_LINE_SEARCH();
   
    vector<float> THE_LEVENBERG_MARQUARDT_GAUSS_NEWTON_AMIJO_LINE_SEARCH(vector<float> x, bool approx, bool record = false)
    {
        // 0 < ηa < ηe < 1
        float ηe = 0.7f; 
        float ηa = 0.2f; 
        // 0 < rc < 1 < re
        float re = 2.0f;
        float rc = 0.5f; 
        // 0 <= δ
        float δk = 0.5f;
        vector<float> pδ;
        vector<float> xk = x;
        vector<vector<float>> F; 
        vector<vector<float>> J; 
        vector<vector<float>> H; 
        vector<vector<float>> JTJ;
        vector<vector<float>> JT;
        vector<vector<float>> dI;
        vector<vector<float>> F_2;
        vector<vector<float>> M_2;
        float F_FK = 0;
        float M_MK = 0;
        while(true)
        {
            F = f.Solve_formula(x); 
            if(!approx)
                J = df.Solve_formula(x);
            else
                J = f.Solve_diff_formula(x);
            JT = J^T;
            JTJ = JT * J;
            dI = identity_matrix<float>(JTJ.size()) * δk;
            pδ = ((  ( (JTJ + dI)^-1 ) * -1.0f * JT * F)^T)[0];
            F_2 = f.Solve_formula(x + pδ); //F(x+pk)
            M_2 = MODEL(F,J, pδ); //M(x+pk)
            // M = f(xk) + f'(xk)(x-xk)
            // |F(x)| - |F(x+p)| <= eta * |M(x)| - |M(x+p)|
            // pk = |F(x)| - |F(x+p)|/|M(x)| - |M(x+p)| ratio
            if(record)
                reports.push_back(extending(x, SHRINK(F)));
            F_FK = Least_sqaures(F) - Least_sqaures(F_2);
            M_MK = Least_sqaures(F) - Least_sqaures(M_2);
            if(M_MK == 0) break;
            if( F_FK >= ηa *  M_MK)
            {
                x = x + pδ;
                if( F_FK >= ηe * M_MK )
                {
                    δk = rc * δk;
                }
            }
            else
            {
                δk = re * δk;
            }
        }
        printf("\tconv\n"); show_vector(F);
        printf("\tx\n"); show_vector(x); printf("\n");
        return x;
    }
    vector<float> QUASI_BFGS_NEWTON_AMIJO_LINE_SEARCH(vector<float> x, bool approx, bool record = false)
    {
        float rc = 0.5f;
        float ηa = 0.4f;
        float δ = 0.5f;
        float a = 1;
        vector<vector<float>> dk(x.size(),vector<float>(1,0));
        vector<vector<float>> pk(x.size(),vector<float>(1,0));
        vector<vector<float>> fk;
        vector<vector<float>> g;
        vector<vector<float>> gk;
        vector<vector<float>> yk;
        vector<vector<float>> B = identity_matrix<float>(x.size());
        int t = 0;
        while(true){
            fk = f.Solve_formula(x);
            if(!approx)
                g = df.Solve_formula(x)^T; 
            else
                g = f.Solve_diff_formula(x)^T;
            pk CLUMPLE ( MATRIX_SOLUTION(B, -1.0f * (g^T)[0]) );
            if(g == 0.0f) break;
            if(record)
                reports.push_back(extending(x, SHRINK(fk)));
            a = 1;
            while( THE_FIRST( f.Solve_formula(x + SHRINK( (a * pk) ) ) ) 
                >  THE_FIRST(fk)  + ηa * a * THE_FIRST( ( (g^T) * pk) ) )
            {
                a = rc * a;
            }
            dk = a * pk; 
            x = x + SHRINK( (a * pk) );
            if(!approx)
                gk = df.Solve_formula(x)^T; 
            else
                gk = f.Solve_diff_formula(x)^T;
            yk = gk - g;
            if(yk == 0.0f) break;
            else if(eigen_value(B) < 0.0f) break;
            if( THE_FIRST( (yk^T) * dk ) >= δ * THE_FIRST( (g^T) * pk) )
            {
                //BFGS
                B = B  
                    - 1.0f/THE_FIRST( ((dk^T) * (B * dk)) ) 
                    * (B * (dk * ((dk^T) * B)))
                    + 1.0f/THE_FIRST( ((yk^T) * dk) )
                    * (yk * (yk^T));
            }
        }
        printf("\tconv\n"); show_vector(fk);
        printf("\tx\n"); show_vector(x); printf("\n");
        return x;
    }
};

class LP{
public:
    bool min = true;
    MATRIX<float> A_m;
    vector<float> c;
    vector<float> b;
    vector<char> oper_val; //to save operator
    vector<bool> x_val; //mark used x variable
    float answer = INFINITY;
    vector<float> answer_x;
    void READING(string file_name){
        CSV data(file_name);
        queue<float> c_v;
        vector<int> c_x_v;
        vector<vector<float>> A(data.csv.size()-1);
        vector<char> cst;
        //reading the header as the level of curve equation
        for(auto txt : data.csv[0]){
            if(txt.compare("min")==0) min = true;
            else if(txt.compare("max")==0) min = false;
            else if(is_number(txt)) c_v.push(stof(txt));
            else if(txt[0] == 'x')  c_x_v.push_back(stoi(txt.substr(1,txt.length() - 1)));
        };
        for(int i = 1; i < data.csv.size(); i++){
            //other reading as A and x and b
            float dir = 1.0f; //check the constraint for at
            float b_v;
            queue<float> a;
            vector<int> x_v;
            for(auto txt : data.csv[i]){
                if(is_number(txt)) a.push(stof(txt));
                else if(txt[0] == 'x') x_v.push_back(stoi(txt.substr(1,txt.length() - 1)));
                else if(txt.compare(">=")==0)break;
                else if(txt.compare("=")==0){dir = 0.0f; break;}
                else if(txt.compare("<=")==0){dir = -1.0f; break;}
                else fprintf(stderr, "READING CSV FILE WRONG! %s", txt.c_str()), abort();
            }
            b_v = stof(data.csv[i].back());
            int max_index = *max_element(x_v.begin(), x_v.end());
            if(max_index > A[0].size())
                for(auto at = A.begin(); at != A.end(); ++at)
                    at->resize(max_index);
            if(max_index > x_val.size())
                x_val.resize(max_index);
            for(auto index : x_v){
                if(x_val[index - 1] == false) x_val[index - 1] = true;
                A[i-1][index - 1] = a.front();
                a.pop();
            }
            cst.push_back(dir);
            b.push_back(b_v);
        }
        c.resize(A[0].size());
        for(auto v : c_x_v){
            if(x_val[v-1] == false) x_val[v-1] = true;
            c[v-1] = (min)?c_v.front():-c_v.front();
            c_v.pop();
        }
        A_m = A;
        oper_val = cst;
        ZERO_REMOVER();
    }
    void PRINTF(){
        printf("------------------------------------------\n");
        printf("min\n");
        for(int i =0; i < c.size(); i++)
            printf("x%d[%-.1f]  ",i + 1, c[i]);
        printf("\n");
        printf("subject to \n");
        for(int i =0; i < x_val.size(); i++)
            if(x_val[i])
                printf("x%-2d\t", i + 1);
            else
                printf("NULL\t");
        printf("\n");
        for(int i =0; i < A_m.mat.size(); i++){
            for(int j =0; j < A_m.mat[i].size(); j++){
                printf("%2.1f\t", A_m.mat[i][j]);
            }
            if(oper_val[i] == 1) printf(" >= ");
            else if(oper_val[i] == 0) printf(" == ");
            else if(oper_val[i] == -1) printf(" <= ");
            printf("%2.1f\t\n", b[i]);

        }
        printf("------------------------------------------\n");
    }
    void PRINTFORM(vector<vector<float>> A, vector<float> b, vector<float> c, vector<bool> x_val, vector<char> oper_val){
        printf("_____________________________________\n");
        if(min)
            printf("min\n");
        else
            printf("max\n");
        for(int i =0; i < c.size(); i++)
            printf("x%d[%-2.2f]  ",i + 1, c[i]);
        printf("\n");
        printf("subject to \n");
        for(int i =0; i < x_val.size(); i++)
            if(x_val[i])
                printf("x%-2d\t", i + 1);
            else
                printf("NULL\t");
        printf("\n");
        for(int i =0; i < A.size(); i++){
            for(int j =0; j < A[i].size(); j++){
                printf("%-2.2f\t", A[i][j]);
            }
            if(oper_val[i] == 1) printf(" >= ");
            else if(oper_val[i] == 0) printf(" == ");
            else if(oper_val[i] == -1) printf(" <= ");
            printf("%2.2f\t\n", b[i]);

        }
        printf("_____________________________________\n");
    }
    void ZERO_REMOVER(){
        for(int i = 0; i < A_m.mat.size(); i++){
            if((A_m.mat[i].size() - count(A_m.mat[i].begin(), A_m.mat[i].end(),0)) == 0){
                if(oper_val[i] == 0 && b[i]  != 0)
                    perror("THIS EQUATION CAN'T BE MATRIX_SOLUTION 0 == !0"), abort();
                else if(oper_val[i] > 0 && b[i] > 0)
                    perror("THIS EQUATION CAN'T BE MATRIX_SOLUTION 0 > +"), abort();
                else if(oper_val[i] < 0 && b[i] < 0)
                    perror("THIS EQUATION CAN'T BE MATRIX_SOLUTION 0 < -"), abort();
                else{
                    A_m.mat = ROWDELETE(A_m.mat, i);
                    oper_val.erase(oper_val.begin() + i);
                    b.erase(b.begin() + i);
                    A_m.SETSIZE();
                    i--;
                } 
            }
        }
        for(int j = 0; j < A_m.mat[0].size(); j++){
            int count = 0;
            for(int i = 0; i < A_m.mat.size(); i++)
            {
                if(A_m.mat[i][j] != 0) count++;
            }
            if(count == 0){
                A_m.mat = COLDELETE( A_m.mat, j);
                x_val.erase(x_val.begin() + j);
                c.erase(c.begin() + j);
                A_m.SETSIZE();
                j--;
            }
        }
    }
    void MATRIXSORTING(){
        priority_queue< pair<int, int>, vector< pair<int, int>>, rcmp<int>> id;
        vector<int> at;

        vector<vector<float>> sort_A(A_m.mat.size(), vector<float>(A_m.mat[0].size()));
        vector<float> sort_b(b.size());
        vector<char> sort_oper_val(oper_val.size());
        #pragma omp for
        for(int i = 0; i < A_m.mat.size(); i++)
            if((A_m.mat[i].size() - count(A_m.mat[i].begin(), A_m.mat[i].end(),0)) == 1)
                id.push( make_pair( find(A_m.mat[i].begin(), A_m.mat[i].end(), 1) - A_m.mat[i].begin(), i) );
            else
                at.push_back(i);
        #pragma omp for
        for(int i = 0; i < at.size(); i++){
            sort_A[i] = A_m.mat[at[i]];
            sort_b[i] = b[at[i]];
            oper_val[i] = 0;
        }
        for(int i = at.size(); i < A_m.mat.size(); i++){
            sort_A[i] = A_m.mat[id.top().second];
            sort_b[i] = b[id.top().second];
            oper_val[i] = 1;
            id.pop();
        }
        A_m = sort_A;
        b = sort_b;
    }
    void LP_IEQ_SET(){
        for(int i =0; i < oper_val.size(); i++){
            if(oper_val[i] == -1){
                transform(A_m.mat[i].begin(), A_m.mat[i].end(),A_m.mat[i].begin(),[](float x){return -x;});
                b[i] *= -1.0f;
            }
            else if(oper_val[i] == 0){
                vector<float> tmp(A_m.mat[i].begin(), A_m.mat[i].end());
                transform(tmp.begin(), tmp.end(), tmp.begin(),[](float x){return -x;});
                A_m.mat = ADDROW(A_m.mat, tmp, i+1);
                b.insert(b.begin() + i + 1, -b[i]);
                A_m.SETSIZE();
            }
        }
        oper_val.resize(A_m.m);
        transform(oper_val.begin(), oper_val.end(), oper_val.begin(), [](float x){return 1.0f;});
    }
    void LP_EQ_SET(){
        for(int i =0; i < oper_val.size(); i++){
            if(oper_val[i] == -1 && (A_m.n - count(A_m.mat[i].begin(), A_m.mat[i].end(),0)) > 1){ //if <= && not iosolated variable
                int id = find(x_val.begin(), x_val.end(), false) - x_val.begin(); //find empty x variable space
                if(id != x_val.size()){ //if x variable space is existed in current x list
                    x_val[id] = true;   //set it as used
                    A_m.mat[i][id] = 1; //since the size of x_val is as same as A_m's row'length just change the value
                    vector<float> new_row(A_m.n); //to add the new at
                    new_row[id] = 1;  //set the variable in new at
                    A_m.mat = ADDROW(A_m.mat, new_row, A_m.m); //adding row to matrix
                    A_m.SETSIZE(); //update size
                    b.push_back(0); //Also add new b
                    continue;   // skip to for loop
                }
                x_val.push_back(true); //if no more x variable to use add a new x_val
                A_m.COLRESIZE(x_val.size());//since x_val len was as same as matrix's row length
                A_m.mat[i][x_val.size()-1] = 1; //set the x value as 1 since x1 + x2 <= b -> x1 + x2 + (x3) = b
                vector<float> new_row(A_m.n); //to add the new at
                new_row[x_val.size()-1] = 1;  //set the variable in new at
                A_m.mat = ADDROW(A_m.mat, new_row, A_m.m); //adding row to matrix
                A_m.SETSIZE(); //update size
                b.push_back(0); //Also add new b
            }
            //as same as above but difference is that the operator 
            else if(oper_val[i] == 1 && (A_m.n - count(A_m.mat[i].begin(), A_m.mat[i].end(),0)) > 1){
                int id = find(x_val.begin(), x_val.end(), false) - x_val.begin();
                if(id != x_val.size()){
                    x_val[id] = true;
                    A_m.mat[i][id] = 1;
                    vector<float> new_row(A_m.n); //to add the new at
                    new_row[id] = 1;  //set the variable in new at
                    A_m.mat = ADDROW(A_m.mat, new_row, A_m.m); //adding row to matrix
                    A_m.SETSIZE(); //update size
                    b.push_back(0); //Also add new b
                    continue;
                }
                x_val.push_back(true);
                A_m.COLRESIZE(x_val.size());
                A_m.mat[i][x_val.size()-1] = -1;
                vector<float> new_row(A_m.n); //to add the new at
                new_row[id] = 1;  //set the variable in new at
                A_m.mat = ADDROW(A_m.mat, new_row, A_m.m); //adding row to matrix
                A_m.SETSIZE(); //update size
                b.push_back(0); //Also add new b
            }
        }
        //convert the symbol of constraints
        oper_val.resize(A_m.m);
        c.resize(A_m.n);
        MATRIXSORTING();
    }
    bool ELP(){
        MATRIX AB = VEC_MERGE_COL(A_m, b);
        int rankA = A_m.RANK();
        int rankB = AB.RANK();
        if(rankA < rankB){ // b is indepedent fomr A -> b it not linear comibnation of A-> b is not in range(A) -> incomputable
            printf("NO feasible point exist %d < %d\n", rankA, rankB);
            return false;
        }
        vector<float> x = MATRIX_SOLUTION(A_m.mat, b); //feasible is exist
        MATRIX AT = A_m.TRANS();
        MATRIX ATC = VEC_MERGE_COL(AT, c);
        int rankC = AT.RANK();
        int rankD = ATC.RANK();
        if(rankC < rankD){
            printf("NO lagrange multiplier exist unbounded %d < %d\n", rankC, rankD);
            return false;
        }
        vector<float> lamda0 = MATRIX_SOLUTION(AT.mat, c); //since feasible is exist and lamda is lagrangian multiplication is exist
        //since Lagrange multiplier is exist and ELP
        //Any feasible point are feasible point opitmal then level of curve are the same
        MATRIX<float> CT(1,c.size());
        CT = c;
        MATRIX<float> X(x.size(), 1);
        X = x;
        float ANS = (CT * X).mat[0][0];
        printf("ELP : ANSWER = minimum is %2.2f", ANS);
        return true;
    }
    
    bool PHASE1(){
        cout <<"[PHASE1 SETTING]"<<endl;
        //-------------Initializing seta finding
        vector<float> x0(A_m.mat[0].size(), 0); //set the initial value x as 0 vector
        MATRIX r = A_m * x0 - b; //find residual (diff between cur and target : sign tell us the direction)
        //seta -> atx + seta = b _> seta = b -atx -> seta = -r(x) 
        //seta will be shifted all constraints for satisfying x0
        float seta = -(r.MIN()); //to find seta that none violate for other constraintsut (choose max(-r(x)))
        //except the positive Ax-b (since x0 is feasible)shift all other constraints by seta
        //---------------Shifiting constraints
        vector<float> shift_list(A_m.mat.size(), 0);
        #pragma omp for
        for(int i=0; i < r.mat.size(); i++)
            if(r.mat[i][0] < 0) shift_list[i] = 1; //since m x 1 matrix
        vector<vector<float>> p1a = ADDCOL(A_m.mat, shift_list, A_m.mat[0].size());
        vector<float> last_seta(p1a[0].size() ,0);
        last_seta.back() = 1;
        p1a = ADDROW(p1a, last_seta, p1a.size());
        x0.push_back(seta);
        //----------------recalculate residual
        MATRIX p1A(p1a);
        printf("\tPHASE 1 Matrix");
        p1A.PRINTF();
        vector<float> p1b(b.begin(), b.end()); p1b.push_back(0);
        vector<float> p1c(c.size(),0); p1c.push_back(1);
        vector<float> answer_x = SIMPLEX(p1A, p1b, p1c, x0);
        if(!answer_x.empty()){
            answer_x.resize(A_m.n);
            answer_x = SIMPLEX(A_m,b,c,answer_x);
            if(!answer_x.empty()){
                float mini_c = 0;
                for(int i = 0; i < c.size(); i++)
                    mini_c += c[i] * answer_x[i];
                answer = mini_c;
                return true;
            }
        }
        return false;
    }
    bool PHASE1_STANDARD(){
        if(A_m.m > A_m.n){
            MATRIX p1A = A_m;
            vector<int> Bid(A_m.m - A_m.n,0);
            vector<int> Nid(A_m.n,0);
            iota(Nid.begin(), Nid.end(), 0);
            iota(Bid.begin(), Bid.end(), A_m.n);
            p1A.ROWRESIZE(A_m.m + (A_m.m-A_m.n));
            int k = A_m.m;
            for(int i = 0; i < A_m.m - A_m.n; i++){
                vector<float> e(p1A.m,0);
                e[i] = 1;
                e[k++] = 1;
                p1A.mat = ADDCOL(p1A.mat, e, p1A.mat[0].size());
            }
            vector<float> pC(p1A.mat[0].size(), 0);
            for(int i = A_m.n; i < p1A.mat[0].size(); i++)
                pC[i] = 1;
            answer_x = SIMPLEX_STANDARD(p1A, b, Bid, pC);
            if(!answer_x.empty()){
                vector<int> bid;
                for(int i = 0; i < A_m.n; i++)
                    if(answer_x[i] != 0)
                        bid.push_back(i);
                answer_x = SIMPLEX_STANDARD(A_m, b, bid, c);
                if(!answer_x.empty()){
                    float mini_c = 0;
                    for(int i = 0; i < c.size(); i++)
                        mini_c += c[i] * answer_x[i];
                    answer = mini_c;
                    return true;
                }
                    
            }
        }
        return false;
    }
    vector<float> SIMPLEX(MATRIX<float> sA, vector<float> sb, vector<float> sc, vector<float> x0, bool show = false){
        if(show){
            printf("\tx0\n"); show_vector(x0); printf("\n");
        }
        MATRIX r0 = sA * x0 - sb;
        if(show){
            printf("\tr0\n");
            r0.PRINTF();
        }
        //working set
        vector<int> W;
        for(int i = 0; i < r0.mat.size(); i++)
            if(r0.mat[i][0] == 0 && W.size() < sA.mat[0].size()) W.push_back(i);
        if(W.size() < sA.mat[0].size()){
            printf("[vertex is non-degenerate andso infeasible]\n");
            x0.clear();
            return x0;
        }
        MATRIX<float> A0(W.size(),0);
        vector<float> lamda0;
        vector<float> e0(A0.mat.size()); 
        vector<float> p0;
        MATRIX<float> Ap0;
        vector<float> delta0(sb.size());
        float min = 0;
        int s = -1;
        int t = 0;
        int max_iter = binom(sA.mat.size(), sA.mat[0].size());
        for(int k = 0; k < max_iter; k++){
            //residual 
            r0 = sA * x0 - sb;
            //printing
            if(show){
                printf("\n-----------------------------------------\n");
                printf("\t iterator %d\n", k);
                printf("\tc\n"); show_vector(sc); printf("\n");
                printf("\tx\n"); show_vector(x0); printf("\n");
                printf("\tr\n"); show_vector(r0.mat); printf("\n");
                printf("\tW\n"); show_vector(W); printf("\n");
            }
            //Active set
            for(int i = 0; i < W.size(); i++)
                A0.mat[i] = sA.mat[W[i]];
            if(show){
                printf("\tA0\n");
                A0.PRINTF();
            }
            //check optimality
            lamda0 = MATRIX_SOLUTION(A0.TRANS().mat, sc); //since c always have the same number of the # cols in A0
            //Bland's least index Rule
            s = -1;
            for(int i = 0; i < W.size(); i++)
                if(lamda0[i] < 0){
                    if(s < 0) s = i;
                    else if(W[s] > W[i]) s = i;
                }
            if(show){
                printf("\tlamda0\n"); show_vector(lamda0); printf("\n");
            }
            if(s == -1){ //it is optimal value
                printf("FIND OPTIMAL VALUE!\n");
                break;
            }
            if(show){
                printf("\ts = %d", s); printf("\n");
            }
            //by Farkas's lemma 
            //   c = AtL for some L >= 0 -> x is optimal
            //Or ∃p s.t ctp < 0, Ap = e and Ap >= 0
            //making e
            memset(&e0[0], 0, sizeof(float) * e0.size());
            e0[s] = 1;
            if(show){
                printf("\te0\n"); show_vector(e0); printf("\n");
            }
            //Compute the search direction
            p0 = MATRIX_SOLUTION(A0.mat, e0);
            if(show){
                printf("\tp0\n"); show_vector(p0); printf("\n");
            }
            //Take a step
            Ap0 = sA * p0; //by Farkas'lemma we need to consider the delta from A0 to be >= 0
            if(show){
                printf("\tr\n"); show_vector(r0.mat); printf("\n");
                printf("\tAp0\n"); show_vector(Ap0.mat); printf("\n");
            }
            memset(&delta0[0], 0, sizeof(float) * delta0.size()); //that variable should be delta wich indicate the step alpha
            for(int i = 0; i < r0.mat.size(); i++)
                if(Ap0.mat[i][0] < 0 && r0.mat[i][0] != 0) //finding decaresing constraints since >= 
                    delta0[i] = r0.mat[i][0]/(-Ap0.mat[i][0]);
                else
                    delta0[i] = INFINITY;
            if(show){
                printf("\tdelta0\n"); show_vector(delta0);printf("\n");
            }
            //Finding min delta
            min = delta0[0];
            t = 0;
            for(int i = 1; i < delta0.size(); i++)
                if(delta0[i] < min) min = delta0[i], t = i;
            if(show){
                printf("\tt = %d\n",t);
            }
            if(delta0[t] == INFINITY){ //there are no more, it is unbounded
                printf("[it is unbounded]\n");
                x0.clear();
                return x0;
            }
            //Take the step
            #pragma omp for
            for(int i = 0; i < x0.size(); i++)
                x0[i] = x0[i] + delta0[t] * p0[i];
            //swap Working and active
            if(show){
                printf("SWAP : s = [W]%d = %d<-> t = %d\n", s, W[s], t);
            }
            swap(W[s], t);
        }
        if(show){
            printf("\tx\n");show_vector(x0); printf("\n");
        }
        float mini_c = 0;
        for(int i = 0; i < c.size(); i++)
            mini_c += c[i] * x0[i];
        if(show){
            printf("\tmin C\n%2.2f\n", mini_c);
        }
        return x0;
    }
    vector<float> SIMPLEX_STANDARD(MATRIX<float> pA, vector<float> bA, vector<int> Bid, vector<float> pC, bool show = false){
        MATRIX A = pA;
        A.ROWRESIZE(Bid.size());
        bA.resize(Bid.size());
        //excract BT and NT and cb
        vector<int> Nid;
        for(int i = 0;i < A.n; i++)
            if(find(Bid.begin(), Bid.end(), i) == Bid.end())
                Nid.push_back(i);
        MATRIX<float> B;
        MATRIX<float> N;
        MATRIX<float> BT(Bid.size(),0);
        MATRIX<float> NT(Nid.size(),0);
        MATRIX<float> Cb(Bid.size(),1);
        MATRIX<float> Cn(Nid.size(),1);
        vector<float> xb;
        vector<float> pib;
        MATRIX<float> Zn;
        vector<float> as;
        vector<float> pb;
        vector<float> delta(bA.size());
        vector<float> x(A.mat[0].size(), 0);//return value
        int t = 0;
        int s = -1;
        float min = INFINITY;
        int count = 0;
        int max_iter = binom(A.mat[0].size(), Bid.size());
        if(show){
            pA.PRINTF();
            printf("\t A\n"); A.PRINTF();
            printf("\t bA\n"); show_vector(bA); printf("\n");
            printf("\t pC\n"); show_vector(pC);
        }
        for(int k = 0; k < max_iter; k++){
            if(show){
                printf("\n-----------------------------------------\n");
                printf("\t iterator %d\n", k);
                printf("\t B set\n"); show_vector(Bid); printf("\n");
                printf("\t N set\n"); show_vector(Nid); printf("\n");
            }
            for(int i = 0; i < Bid.size(); i++){
                BT.mat[i] = A.SUBVECTOR_COL(Bid[i]);
                Cb.mat[i][0] = pC[Bid[i]];
            }
            for(int i = 0; i < Nid.size(); i++){
                NT.mat[i] = A.SUBVECTOR_COL(Nid[i]);   
                Cn.mat[i][0]= pC[Nid[i]];
            }
            B = BT.TRANS();
            N = NT.TRANS();
            if(show){
                printf("\tB"); B.PRINTF();
                printf("\tN"); N.PRINTF();
                printf("\t cB set\n"); Cb.PRINTF(); printf("\n");
                printf("\t cN set\n"); Cn.PRINTF(); printf("\n");
            }
            //#1 Check the optimality
            pib = MATRIX_SOLUTION(BT.mat, Cb.TRANS().mat[0]); //lagrange multipliers for NT BT * pi = Cb
            if(show){
                printf("\t pib\n"); show_vector(pib); printf("\n");
            }
            //reduce cost Zn = Cn - NT(pib)
            Zn = Cn - NT * pib;
            if(show){
                printf("\tZn");
                Zn.PRINTF();
            }
            s = -1;
            min = INFINITY;
            count = 0;
            for(int i = 0; i < Zn.mat.size(); i++){
                if(Zn.mat[i][0] < min) s = i, min = Zn.mat[i][0];
                else if(Zn.mat[i][0] == min && Nid[s] < Nid[i]) s = i;
                if(Zn.mat[i][0] >= 0) count++;
            }
            if(show){
                printf("\tS = %d\n", s);
            }
            if(count >= Zn.mat.size()){
                printf("THE ANSWER FOUND\n");
                break;
            }
            //compute search direction
            as = A.SUBVECTOR_COL(Nid[s]);
            transform(as.begin(), as.end(), as.begin(), [](float x){return -x;});
            pb = MATRIX_SOLUTION(B.mat, as);            
            xb = MATRIX_SOLUTION(B.mat, bA); 
            if(show){
                printf("\t bA\n"); show_vector(bA); printf("\n");
                printf("\t xb\n"); show_vector(xb); printf("\n");
                printf("\t as\n"); show_vector(as); printf("\n");
                printf("\t pb\n"); show_vector(pb); printf("\n");
            }
            //setp to adjacent vertex
            for(int i = 0; i < pb.size(); i++){
                if(xb[i] != 0 && pb[i] < 0) delta[i] = xb[i]/(-pb[i]);
                else delta[i] = INFINITY;
            }
            t = min_element(delta.begin(), delta.end()) -  delta.begin();
            if(show){
                printf("\tdelta\n"); show_vector(delta); printf("\n");
                printf("\t T = %d\n", t); 
            }
            if(delta[t] == INFINITY){
                printf("SIMPLEX : it is unbounded\n"); 
                x.clear();
                return x;
                }
            for(int i = 0; i < xb.size(); i++)
                xb[i] += pb[i] * delta[t];
            //Xn = xn + alpha e 
            //xn is alwyas initializing with 0 since Null space so we just put alpah
            xb[t] += delta[t]; 
            //swap BT and NT index
            if(show){
                printf("swap %d <-> %d\n", t, s);
            }
            swap(Bid[t], Nid[s]);
        }
        for(int i = 0; i < Bid.size(); i++)
            x[Bid[i]] = xb[i];
        float mini_c = 0;
        for(int i = 0; i < c.size(); i++)
            mini_c += c[i] * x[i];
        if(show){
            printf("\t x\n"); show_vector(x);printf("\n");
            printf("\tc\n"); show_vector(c);printf("\n");
            printf("\t min C\n%2.2f\n", mini_c);
        }
        return x;
    }
    
    bool LP_BEST(){
        cout << "[ORIGINAL]\n";
        PRINTF();
        if(A_m.mat.size() > A_m.mat[0].size())
        {
            LP_IEQ_SET();  //LP primary will be inequality form
            cout << "LP IEQ SETTING IS FINISH" <<endl;
            PRINTF();
            cout << "LP IEQ PHASE 1 START" <<endl;
            return PHASE1();
        }
        else
        {
            LP_EQ_SET();  
            cout << "LP EQ STANDARD SETTING IS FINISH" <<endl;
            PRINTF();
            cout << "LP EQ STANDARD PHASE 1 START" <<endl;
            return PHASE1_STANDARD();
        }
    }
};

#endif