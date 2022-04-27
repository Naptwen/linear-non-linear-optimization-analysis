#include "vmatrix.hpp"
#include "valgorithm.hpp"
//Copyright (c) 2022. Useop Gim
//GNU License
using namespace std;
#define arithmetic operation

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
    void PRINTF(bool answer = false)
    {
        for (auto vec : equ)
            printf("%s ", vec.c_str());
        if(answer)printf("answer = %f\n", y);
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
            cout << formula_str.size() << "x" << formula_str[0].size() <<endl;
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
    vector<vector<float>>  Solve_formula(vector<float> x, bool show = false)
    {
        vector<vector<float>> solution( funmat.size(), vector<float>(funmat[0].size(), 0) );
        for(int i = 0; i < funmat.size(); i++)
            for(int j = 0; j < funmat[0].size(); j++)
            {
                funmat[i][j].set_variable(x);
                solution[i][j] = funmat[i][j].solve(show);
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
        show_2d_vector(mat);
    }
};
