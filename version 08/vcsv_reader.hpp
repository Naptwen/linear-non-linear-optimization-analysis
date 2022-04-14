#ifndef CSV_READER_H__
#define CSV_READER_H__
#include <stdlib.h>
#include <iostream>
#include <ostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "valgorithm.hpp" 

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
    vector<int> order;
    int cols = 0;
    int rows = 0;
    bool header = false;
    CSV(string file_name, bool HEADER){
        string line;
        ifstream csv_file;
        csv_file.open(file_name.c_str());
        if(csv_file.is_open()){
            cout << file_name <<endl;
            int num = 0;
            if(HEADER){
                getline(csv_file, line);
                order.push_back(num);
                csv.push_back(split<string>(line, ","));
                header = HEADER;  
                num++;
            }
            while(getline(csv_file, line)){
                order.push_back(num);
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
        int g = 0;
        for(auto n : order){
            printf("[%d]    ", g++);
            for(auto m : csv[n]){
                printf("%-10s  ", m.c_str());
            }
            printf("\n");
        }
    }
    template<typename T>
    void SORT(string t){
        int s = 0;
        if(header) s++;
        int i = 0;
        for(;i< csv[0].size(); i++)
            if(csv[0][i].compare(t) == 0)
                break;
        vector<string> array;
        vector<int> null_list;
        vector<int> index;
        for(int j = 0; j < csv.size(); j++)
            array.push_back(csv[j][i]);
        for(int j = s; j < csv.size(); j++){
            if(csv[j][i].compare("NULL") == 0){
                null_list.push_back(j);
            }else{
                index.push_back(j);
            }
        }
        vector<int> sort = ref_merge_sort<T>(index, &array);
        sort.insert(sort.begin(),  0);
        sort.insert(sort.end(), null_list.begin(), null_list.end());
        order.clear();
        order = sort;
           
    }
};
#endif