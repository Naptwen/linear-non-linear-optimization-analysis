#pragma once
#include "vmatrix.hpp"
#include "valgorithm.hpp"
//Copyright (c) 2022. Useop Gim
//GNU License
using namespace std;
#define arithmetic operation
#define RICHARDSON 122777
#define TAYLOR 192877

int priority_operation(string *symbol)
{
    if (symbol->compare("+") == 0)
        return 3;
    else if (symbol->compare("-") == 0)
        return 3;
    else if (symbol->compare("/") == 0)
        return 5;
    else if (symbol->compare("*") == 0)
        return 5;
    else if (symbol->compare("^") == 0)
        return 7;
    else if (symbol->compare("exp") == 0)
        return 11;
    else if (symbol->compare("cos") == 0)
        return 11;
    else if (symbol->compare("sin") == 0)
        return 11;
    else if (symbol->compare("tan") == 0)
        return 11;
    else if (symbol->compare("ln") == 0)
        return 11;
    else if (symbol->compare("sqrt") == 0)
        return 11;
    else
        return 0;
}

void check_hirechy_operator(string *eq, stack<string> *algebra, queue<string> *equation)
{
    // check hirechy
    while (!algebra->empty())
    {
        if (priority_operation(eq) > priority_operation(&algebra->top()) || algebra->top().compare("(") == 0)
        {
            algebra->push(*eq);
            break;
        }
        equation->push(algebra->top());
        algebra->pop();
    }
    if (algebra->empty())
        algebra->push(*eq);
}

int math_eq_variable(vector<string> eq)
{
    int ans = 0;
    for (int i = 0; i < eq.size(); i++)
        if (is_variable(eq[i]) >= 0)
            ans++;
    return ans;
}

vector<string> math_eq_converter(string txt, bool show = false)
{
    bool number = true;
    string c = "";
    string val = "";
    vector<string> equation(0);
    // remove space
    size_t pos = txt.find(" ", 0);
    while (pos != string::npos)
    {
        txt.replace(pos, 1, "");
        pos = txt.find(" ", 0);
    }
    //
    for (int i = 0; i < txt.length(); i++)
    {
        if (isdigit(txt[i]))
        {
            val.clear();
            while (isdigit(txt[i]) || txt[i] == '.')
                val += txt[i++];
            equation.push_back(val);
            i--;
        }
        else if (isalpha(txt[i]) && txt[i + 1] == '_')
        {
            val.clear();
            val += txt[i++];
            val += txt[i++];
            while (isdigit(txt[i]))
                val += txt[i++];
            equation.push_back("1");
            equation.push_back("*");
            equation.push_back(val);
            i--;
        }
        //specific operation
        else if (txt.substr(i,4).compare("sqrt") == 0)
        {
            equation.push_back( "sqrt" ) ;
            i = i + 3;
        } 
        else if (txt.substr(i,3).compare("exp") == 0)
        {
            equation.push_back("exp");
            i = i + 2;
        }
        else if (txt.substr(i,3).compare("cos") == 0)
        {
            equation.push_back("cos");
            i = i + 2;
        }
        else if (txt.substr(i,3).compare("sin") == 0)
        {
            equation.push_back("sin");
            i = i + 2;
        }
        else if (txt.substr(i,3).compare("sin") == 0)
        {
            equation.push_back("sin");
            i = i + 2;
        }       
        else if (txt.substr(i,3).compare("tan") == 0)
        {
            equation.push_back("tan");
            i = i + 2;
        }     
        else if (txt.substr(i,2).compare("ln") == 0)
        {
            equation.push_back( "ln" ) ;
            i = i + 1;
        }
        //specific value
        else if (txt.substr(i,2).compare("pi") == 0)
        {
            equation.push_back( "pi" ) ;
            i = i + 1;
        }
        else if (txt[i] == 'e')
        {
            equation.push_back("e");
        }
        // check it is subtract or sign
        else if (txt[i] == '-')
        {
            if ((isdigit(txt[i + 1]) || isalpha(txt[i + 1]) || txt[i + 1] == '(') && (isdigit(txt[i - 1]) || isalpha(txt[i - 1]) || txt[i - 1] == ')'))
            {
                equation.push_back("-");
            }
            else
            {
                equation.push_back("-1");
                equation.push_back("*");
            }
        }
        else
            equation.push_back(string(1, txt[i]));
    }
    // change all / to ^-1 for derivative
    int bb = 0;
    for (int i = 0; i + 1 < equation.size(); i++)
    {
        if (equation[i].compare("/") == 0)
        {
            equation[i] = "*";
            if(is_variable(equation[i + 3]) >= 0)
            {
                equation.insert(equation.begin() + i + 4, "^");
                equation.insert(equation.begin() + i + 5, "-1");
                i += 5;
                continue;
            }
            bb = 0;
            for (int j = i + 1; j < equation.size(); j++)
            {
                if (equation[j].compare("(") == 0)
                    bb++;
                else if (equation[j].compare(")") == 0)
                    bb--;
                if (bb == 0)
                {
                    equation.insert(equation.begin() + j + 1, "^");
                    equation.insert(equation.begin() + j + 2, "-1");
                    i = j + 2;
                    break;
                }
            }
        }
    }
    if(show){
        for(auto str: equation) 
            wcout << str.c_str();
        wcout << endl;
    } 
    return equation;
}

float math_eq_solver(vector<string> eq, vector<float> x, bool show = false)
{
    queue<string> equation;
    stack<string> algebra;
    stack<float> number;
    string oper = "";
    string check = "";
    for (int i = 0; i < eq.size(); i++)
    {
        // if positive number
        if (is_number(eq[i]))
        {
            equation.push(eq[i]);
        }
        // if it is algebra operator
        else if (priority_operation(&eq[i]) > 0)
        {
            check_hirechy_operator(&eq[i], &algebra, &equation);
        }
        // if bracket
        else if (eq[i].compare("(") == 0)
            algebra.push(eq[i]);
        else if (eq[i].compare(")") == 0)
        {
            while (!algebra.empty())
            {
                if (algebra.top().compare("(") == 0)
                {
                    algebra.pop();
                    break;
                }
                equation.push(algebra.top());
                algebra.pop();
            }
        }
        // if variable
        else if (is_variable(eq[i]) >= 0)
        {
            equation.push(to_string(x[is_variable(eq[i])]));
        }
        else if(eq[i].compare("e") == 0)
        {
            equation.push(to_string(M_E));
        }
        else if(eq[i].compare("pi") == 0)
        {
            equation.push(to_string(M_PI));
        }
    }
    while (!algebra.empty())
    {
        equation.push(algebra.top());
        algebra.pop();
    }
    if(show) Queue_print(equation);
    float num = 0.0f, a = 0.0f, b = 0.0f;
    while (!equation.empty())
    {
        if(show){
            printf("number : ");
            Stack_print(number);
            printf("equation : ");
            Queue_print(equation);
        }
        if (is_number(equation.front())){
            number.push( atof( equation.front().c_str() ) ); //has real number problem
        }
        else if (equation.front().compare("+") == 0)
        {
            a = number.top();
            number.pop();
            b = number.top();
            number.pop();
            num = b + a;
            number.push(num);
        }
        else if (equation.front().compare("-") == 0)
        {
            a = number.top();
            number.pop();
            b = number.top();
            number.pop();
            num = b - a;
            number.push(num);
        }
        else if (equation.front().compare("*") == 0)
        {
            a = number.top();
            number.pop();
            b = number.top();
            number.pop();
            num = b * a;
            number.push(num);
        }
        else if (equation.front().compare("/") == 0)
        {
            a = number.top();
            number.pop();
            b = number.top();
            number.pop();
            num = b / a;
            number.push(num);
        }
        else if (equation.front().compare("^") == 0)
        {
            a = number.top();
            number.pop();
            b = number.top();
            number.pop();
            if(a == 2)
                num = b * b;
            else if(a == -1)
                if(b != 0)
                    num = 1/b;
                else
                    throw out_of_range("[math_eq_solver] : 1/0 error undefined");
            else
                num = pow(b, a);
            number.push(num);
        }
        else if (equation.front().compare("sqrt") == 0)
        {
            a = number.top();
            number.pop();
            num = sqrtf(a);
            number.push(num);
        }
        else if (equation.front().compare("exp") == 0)
        {
            a = number.top();
            number.pop();
            num = expf(a);
            number.push(num);
        }
        else if (equation.front().compare("cos") == 0)
        {
            a = number.top();
            number.pop();
            num = cosf(a);
            number.push(num);
        }
        else if (equation.front().compare("sin") == 0)
        {
            a = number.top();
            number.pop();
            num = sinf(a);
            number.push(num);
        }
        else if (equation.front().compare("tan") == 0)
        {
            a = number.top();
            number.pop();
            num = tanf(a);
            number.push(num);
        }
        else if (equation.front().compare("ln") == 0)
        {
            a = number.top();
            number.pop();
            num = logf(a);
            number.push(num);
        }
        equation.pop();
    }
    if(show){
        printf("number : ");
        Stack_print(number);
    }
    return number.top();
}

float equ_solver(string function, vector<float> x)
{
   vector<string> equ = math_eq_converter(function);
   return math_eq_solver(equ, x);
}

float Richardson_extrpolation(vector<string> fn, int dx, vector<float> x_list, bool show  = false)
{
    vector<float> N_list;  
    vector<float> xh_1 = x_list;
    vector<float> xh_2 = x_list;
    int k = 1;
    float h = abs(x_list[dx]) * 0.1f;
    float fx_p_h;
    float fx_s_h;
    float N1;
    float N2;
    float Nn;
    vector<float> N;
    vector<float> new_N;

    while(true)
    {
        xh_1[dx] = x_list[dx] + h;
        xh_2[dx] = x_list[dx] - h;
        fx_p_h = math_eq_solver(fn, xh_1);
        fx_s_h = math_eq_solver(fn, xh_2);
        if(N.size() > 1 && abs(N[N.size() - 1] - N[N.size() - 2]) >= h)
        { 
            N.pop_back();
            break;
        }
        N.push_back((fx_p_h - fx_s_h)/(2 * h));
        h = h / 2;
    }
    while(N.size() > 1)
    {
        new_N.clear();
        for(int i = 0; i + 1 < N.size(); i++)
        {
            Nn = N[i + 1] + (N[i + 1] - N[i])/(pow(4,k) - 1);
            new_N.push_back(Nn);
        }
        N = new_N;
        k++;
    }
    return N.front();
}

float Taylor_approx_diff(vector<string> fn, int dx, vector<float> x_list, bool show  = false)
{
    float h = abs(x_list[dx]) * 0.01f; //1%
    vector<float> xh_1 = x_list; xh_1[dx] = x_list[dx] + h;
    vector<float> xh_2 = x_list; xh_2[dx] = x_list[dx] - h;
    vector<float> xh_3 = x_list; xh_3[dx] = x_list[dx] + 2 * h;
    vector<float> xh_4 = x_list; xh_4[dx] = x_list[dx] - 2 * h;
    float fx_p_h = math_eq_solver(fn, xh_1);
    float fx_s_h = math_eq_solver(fn, xh_2);
    float fx_p_2h = math_eq_solver(fn, xh_3);
    float fx_s_2h = math_eq_solver(fn, xh_4);
    float dfdx = (8 * fx_p_h - 8 * fx_s_h + fx_s_2h - fx_p_2h)/(12 * h);
    return dfdx;
}

//Referenced from 
//Richard L. Burden J Douglas, "Numerical Anlysis". 2010. 9th edition, pp147-148.
vector<vector<float>> cubi_spline(const vector<float> &x, const vector<float> &y, unsigned long interval, bool show = false)
{
    if(x.size() != y.size()) throw length_error("[cubi_spline] : x y size are not the same!");
    //S(x) = a(x-xi)^3+b(x-xi)^2+c(x-xi)+di
    //condition S(x) = y | S(xi) = yi | Si-1'(x) = Si'(x) | Si-1"(x) = Si"(x) |
    vector<float> a(x.size());
    vector<float> B(x.size());
    vector<float> b(x.size() - 1);
    vector<float> d(x.size() - 1);
    vector<vector<float>> A(x.size(), vector<float>(x.size(),0));
    vector<vector<float>> CS; // cubis spline
    vector<float> x_n;
    vector<float> y_n;
    float _x_ = x.front();
    float _x_gap = ( x.back() - x.front() )/interval;
    int j = 0;
    int k = 0;
    vector<float> h(x.size() - 1);
    for(int i =0; i + 1 < x.size(); i++)
    {
        h[i] = x[i + 1] - x[i];
        if(h[i] == 0)
            throw logic_error("[cubic spline] : h is 0 since x[i] is overlap!");
        a[i] = y[i];
    }
    a.back() = y.back();
    for(int i = 1; i + 1 < y.size(); i ++)
    {
        A[i][i] = 2 * (h[i-1] + h[i]);
        A[i][i-1] = h[i-1];
        A[i][i+1] = h[i];
        B[i] = 3 * (a[i+1] - a[i]) / h[i] - 3 * (a[i] - a[i-1]) / h[i-1];
    }
    A[0][0] = 1; //positive definite tridiagonal matrix - > invertible - > linearly independent -> solution is exist
    A[A.size() - 1][A.size()-1] = 1;
    B[0] = 0;
    B.back() = 0;
    a[0] = y[0];
    a.back() = y.back();
    vector<float> c = SHRINK((A^-1) * B);
    for(int i = 0; i < b.size(); i++)
    {
        b[i] = (a[i + 1] - a[i])/h[i] - h[i]/3 * (c[i + 1] + 2 * c[i]);
        d[i] = (c[i + 1] - c[i])/(3 * h[i]);
    }
    while(k++ < interval)
    {
        while( !( x[j] <= _x_ && _x_ <= x[j+1] ) )j++;
        x_n.push_back(_x_);
        y_n.push_back(a[j] + b[j] * (_x_ - x[j]) + c[j] * powf((_x_ - x[j]),2) + d[j] * powf((_x_ - x[j]), 3));
        _x_ += _x_gap;
    }
    x_n.push_back(x.back());
    y_n.push_back(y.back());
    CS.push_back(x_n);
    CS.push_back(y_n);
    return CS;
}

class math_eq
{
public:
    vector<string> equ;
    vector<float> x;
    int x_size = 0;
    float y = 0;
    void set_formula(string txt, bool show = false)
    {
        equ = math_eq_converter(txt, show);
        x_size = math_eq_variable(equ);
    }
    void set_variable(vector<float> variable)
    {
        x = variable;
    }
    float solve(bool show = false)
    {
        return y = math_eq_solver(equ, x, show);
    }
    float solve_aprox_diff(int dx, int type, bool show = false){
        if(type == RICHARDSON)
            return y = Richardson_extrpolation(equ, dx, x, show);
        return y = Taylor_approx_diff(equ, dx, x, show);
    }
    void PRINTF(bool answer = false)
    {
        for (auto vec : equ)
            printf("%s ", vec.c_str());
        if(answer) printf("answer = %f\n", y);
    }
    void operator = (math_eq A)
    {
        equ = A.equ;
        x = A.x;
        x_size = A.x_size;
        y = A.y;
    }

};

class FUNMATRIX:public MATRIX<float>
{
public:
    vector<vector<math_eq>> funmat;
    void Set_formula(vector<vector<string>> formula_str)
    {
        if(!formula_str.empty())
        {
            vector<vector<math_eq>> tmp(formula_str.size(), vector<math_eq>(formula_str[0].size()));
            for(int i = 0; i < tmp.size(); i++)
                for(int j = 0; j < tmp[0].size(); j++)
                    tmp[i][j].set_formula(formula_str[i][j]);
            funmat = tmp;
        }
    }
    void PRINTF_formula(){
        for(int i = 0; i < funmat.size(); i++){
            for(int j = 0; j < funmat[0].size(); j++)
                funmat[i][j].PRINTF(), printf(",\t");
            printf("\n");
        }
    }
    vector<vector<float>> Solve_formula(vector<float> x, bool show = false)
    {
        vector<vector<float>> solution( funmat.size(), vector<float>(funmat[0].size(), 0) );
        for(int i = 0; i < funmat.size(); i++)
            for(int j = 0; j < funmat[0].size(); j++)
            {
                funmat[i][j].x = x;
                solution[i][j] = funmat[i][j].solve(show);
            }
        return solution;
    }
    vector<vector<float>> Solve_diff_formula(vector<float> x, bool show = false)
    {
        vector<vector<float>> solution( funmat.size(), vector<float>(x.size(), 0) );
        for(int i = 0; i < funmat.size(); i++)
        {
            funmat[i][0].x = x;
            for(int j = 0; j < x.size(); j++)
                solution[i][j] = funmat[i][0].solve_aprox_diff(j, TAYLOR, show);
        }
        return solution;
    }
    void operator = (FUNMATRIX A)
    {
        funmat = A.funmat;
        mat = A.mat;
        if(!mat.empty())
            SETSIZE();
    }
    void Show_Solution()
    {
        printf("------------------solution------------\n");
        show_vector(mat);
    }
};
