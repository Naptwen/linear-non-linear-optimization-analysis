#ifndef CSV_READER_H__
#define CSV_READER_H__
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
#include "valgorithm.hpp" 

#define UNDER 7
#define OVER 11
#define ABOVE 17
#define BELOW 31
#define EQUAL 197

using namespace std;

vector<string> csv_spliting(string txt){
    vector<string> temp;
    stringstream ss(txt);
    string substr;
    string seperate;
    for(int i =0;i < txt.size(); i++){
        if(txt[i] == '"'){
            int j = i + 1;
            for(; j < txt.size(); j++){
                if(txt[j] == '"'){
                    break;
                }
            }
            temp.push_back(txt.substr(i + 1, j - i - 1));
            i = j;
        }
        else if(txt[i] == ','){
            int j = i + 1;
            for(; j < txt.size(); j++){
                if(txt[j] == ','){
                    break;
                }
            }
            if(!txt.substr(i + 1, j - i - 1).empty())
                temp.push_back(txt.substr(i + 1, j - i - 1));
            else
                temp.push_back("NULL");
            i = j- 1;
        }   
    }
    temp.pop_back();
    return temp;
}
class CSV{
public  :
    vector<vector<string>> csv;
    vector<int> print_order;
    vector<int> selected_rows;
    int cols = 0;
    int rows = 0;
    bool header = false;
    CSV(string file_name, bool HEADER){
        string line;
        ifstream csv_file;
        csv_file.open(file_name.c_str());
        if(csv_file.is_open()){
            printf("CVS %s is OPEN\n", file_name.c_str());
            int num = 0;
            if(HEADER){
                getline(csv_file, line);
                print_order.push_back(num);
                csv.push_back(split<string>(line, ","));
                header = HEADER;  
                num++;
            }
            while(getline(csv_file, line)){
                print_order.push_back(num);
                csv.push_back(csv_spliting(line));
                num++;
            }
            cols = csv[0].size();
            rows = csv.size();
            csv_file.close();
        }
        else{
            perror("NO SUCH FILE");
        }
    }
    void show(){
        cout << "________________________________" <<endl;
        for(int  i = 0; i < print_order.size(); i++){
            printf("[%5d]    ", i);
            for(auto m : csv[print_order[i]]){
                if(m.compare("NULL") != 0
                    && find(selected_rows.begin(), selected_rows.end(), print_order[i])  != selected_rows.end()){
                    printf(GREEN);
                    printf("%-10s  ", m.c_str());
                    printf(RESET);
                }
                else if(m.compare("NULL") == 0){
                    printf(RED);
                    printf("%-10s  ", m.c_str());
                    printf(RESET);
                }
                else{
                    printf("%-10s  ", m.c_str());
                }
            }
            printf("\n");
        }
    }
    // t is header name
    int find_header(string t){
        int i = 0;
        for(;i< csv[0].size(); i++)
            if(csv[0][i].compare(t) == 0)
                break;
        return i;
    }
    //cell duplicateed value finder
    map<string, vector<int>> ref_duplicate_finder(vector<int> A, int n, vector<vector<string>>* C){
        map<string, vector<int>> duplicate_counter;
        map<string, vector<int>> ANS;
        for(int j = 0; j < A.size(); j++)//key setting
            duplicate_counter[(*C)[A[j]][n]].push_back(A[j]);
        for(auto key = duplicate_counter.begin(); key != duplicate_counter.end(); key++)
            if(key->first.compare("NULL") != 0 && key->second.size() > 1)
                for(vector<int>::iterator iter = key->second.begin(); iter != key->second.end(); iter++)
                    ANS[key->first].push_back(*iter);
        return ANS;
    }
    template<typename T>
    vector<int> cell_sort(int i, vector<int> list){
        vector<string> array(csv.size());
        vector<int> null_list;
        vector<int> index;
        for(int j = 0; j < list.size(); j++){
            array[list[j]] = csv[list[j]][i];           //actual value
            if(csv[list[j]][i].compare("NULL") == 0)
                null_list.push_back(list[j]);           //null list
            else
                index.push_back(list[j]);                     //the value index
        }
        vector<int> sort = ref_merge_sort<T>(index, &array); //sorteed index
        sort.insert(sort.end(), null_list.begin(), null_list.end());
        return sort;
    }
    
    void recur_sort(queue<string> n, vector<int> index_list, vector<int> edit_index, 
                    vector<int>* print_list, vector<vector<string>>* C){
        if(!n.empty()){//if not empty heder
            string hd = n.front(); 
            int s_col = find_header(hd);//get header cols index
            n.pop();    
            if(isnumber((*C)[1][s_col][0]))
                index_list = cell_sort<float>(s_col, index_list);
            else
                index_list = cell_sort<string>(s_col, index_list);
            for(int i = 0; i < edit_index.size(); i++)
                (*print_list)[edit_index[i]] = index_list[i]; //change the print index from edit index by index list
            if(!n.empty()){ //if there were another condition
                //to group up edit index and sorted index   
                map< string, vector< pair<int, int> > > nlist; 
                for(int i = 0; i < index_list.size(); i++)
                    if((*C)[ index_list[i] ][s_col].compare("NULL") != 0)
                        //to find the duplicated values
                        nlist[ (*C)[ index_list[i] ][s_col] ].push_back( make_pair( edit_index[i], index_list[i] ) ); 
                for(auto key = nlist.begin(); key != nlist.end(); key++){
                    //if the current key from map were duplicated
                    if(key->second.size() > 1){ 
                        vector<int> next_index_list;   
                        vector<int> next_edit_index;
                        for(int j =0; j < key->second.size(); j++){
                            next_index_list.push_back(key->second[j].second); //register the index value
                            next_edit_index.push_back(key->second[j].first);  //register the pair index of print value 
                        }
                        recur_sort(n, next_index_list, next_edit_index, print_list, C); //recursion
                    }
                }
            }
        }
    }
    //The last inpu elements as NULL 
    void SORT(const char* args, ...){
        va_list ap;         //dynamic variable argument list
        va_start(ap, args); //dynamic variable argument  pointer setting
        queue<string> headers;
        while(args){ 
            string tmp = args;
            headers.push(tmp);
            args = va_arg(ap, const char*);
        }
        va_end(ap);
        int n = 0;
        vector<int> index_list(csv.size() - 1);
        iota(index_list.begin(), index_list.end(), 1);
        vector<int> edit_index(csv.size() - 1);
        iota(edit_index.begin(), edit_index.end(), 1);
        vector<int> print_index(csv.size());
        iota(print_index.begin(), print_index.end(), 0);
        recur_sort(headers, index_list, edit_index, &print_index, &csv);
        print_order = print_index;
    }
    /* 
    [t] is header name
    [stand] is the standard
     type
     [UNDER]    >
     [OVER]     <
     [BELOW]    >=
     [ABOVE]    <=
     [EQUAL]    ==
    */
    template<typename T>
    int SELECT(string t, string stand, int type = EQUAL){
        int i =0;
        for(i = 0;i< csv[0].size(); i++)
            if(csv[0][i].compare(t) == 0)
                break;
        vector<int> array;
        float _num = stof(stand);
        for(int j = 1; j < csv.size(); j++){
            if(csv[j][i].compare("NULL") != 0)
            switch(type){
                case UNDER:
                    if(_num > stof(csv[j][i])) array.push_back(j);
                    break;
                case OVER:
                    if(_num < stof(csv[j][i])) array.push_back(j);
                    break;
                case BELOW:
                    if(_num >= stof(csv[j][i])) array.push_back(j);
                    break;
                case ABOVE:
                    if(_num <= stof(csv[j][i])) 
                    array.push_back(j);
                    break;
                case EQUAL:
                    if(_num == stof(csv[j][i])) array.push_back(j);
                    break;
                default:
                    array.push_back(j);
                    break;
            }
        }
        selected_rows = array;
        return array.size();
    }
    //t is header name
    float SUM(string t){
        vector<float> values;
        int i =0;
        for(i = 0;i< csv[0].size(); i++)
            if(csv[0][i].compare(t) == 0)
                break;
        for(int j = 0; j < selected_rows.size(); j++){
            if(csv[selected_rows[j]][i].compare("NULL") != 0)
            values.push_back( stof( csv[selected_rows[j]][i] ) );
        }
        return sum((float*)&values[0], (float*)&values[values.size()-1]);
    }
    //t is header name
    //variance
    // POPUL : poulation variance
    // SAMPLE : sample variance
    float VAR(string t, int type = POPUL){
        vector<float> values;
        int i =0;
        for(i = 0;i< csv[0].size(); i++)
            if(csv[0][i].compare(t) == 0)
                break;
        for(int j = 0; j < selected_rows.size(); j++){
            if(csv[selected_rows[j]][i].compare("NULL") != 0)
            values.push_back(stof(csv[selected_rows[j]][i]));
        }
        return var((float*)&values[0], (float*)&values[values.size()-1], type);
    }
    //t is header name
    //AVERAGE
    float AVG(string t){
        vector<float> values;
        int i =0;
        for(i = 0;i< csv[0].size(); i++)
            if(csv[0][i].compare(t) == 0)
                break;
        for(int j = 0; j < selected_rows.size(); j++){
            if(csv[selected_rows[j]][i].compare("NULL") != 0)
            values.push_back(stof(csv[selected_rows[j]][i]));
        }
        return mean((float*)&values[0], (float*)&values[values.size()-1]);
    }
};

template<>
int CSV::SELECT<string>(string t, string num, int type){
    int i =0;
    for(i = 0;i< csv[0].size(); i++)
        if(csv[0][i].compare(t) == 0)
            break;
    vector<int> array;
    for(int j = 0; j < csv.size(); j++){
        switch(type){
            case UNDER:
                if(num.compare(csv[j][i]) > 0) array.push_back(j);
                break;
            case OVER:
                if(num.compare(csv[j][i]) < 0)array.push_back(j);
                break;
            case BELOW:
                if(num.compare(csv[j][i]) >= 0) array.push_back(j);
                break;
            case ABOVE:
                if(num.compare(csv[j][i]) <= 0) array.push_back(j);
                break;
            case EQUAL:
                if(num.compare(csv[j][i]) == 0)array.push_back(j);
                break;
            default:
                array.push_back(j);
                break;
        }
    }
    return array.size();
}


#endif