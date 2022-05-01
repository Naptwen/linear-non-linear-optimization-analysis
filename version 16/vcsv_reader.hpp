#ifndef VCSV_READER_H__
#define VCSV_READER_H__

#pragma once
#include "valgorithm.hpp"
#include "vmath_solver.hpp"

#define UNDER 7
#define OVER 11
#define ABOVE 17
#define BELOW 31
#define EQUAL 197
#define ALL 311
#define DEDUPLE 727
#define EMPTY ""
using namespace std;


vector<string> csv_spliting(string txt)
{
    vector<string> temp;
    int p = 0;
    int buffer = 0;
    string str;
    for (int i = 0; i < txt.size(); i++)
    {
        if (txt[i] != '\"')
        {
            if (buffer != 0) // if buffer is not empty
                str.append(1, txt[i]);
            else if (buffer == 0 && txt[i] == ',')
            { // if buffer is empty and comma
                if (str.empty() || is_space(str))
                    str = "NULL";
                temp.push_back(str);
                str.clear();
            }
            else
            {
                str.append(1, txt[i]);
            }
        }
        else
        {
            if (buffer == 0) // if buffer is empty, increase
                buffer++;
            else
                buffer--;
        }
    }
    if (is_space(str) || str.empty())
        str = "NULL";
    temp.push_back(str);
    return temp;
}
class CSV
{
public:
    vector<vector<string>> csv;
    vector<int> print_order;
    vector<int> selected_rows;
    set<int> selected_headers;
    int cols = 0;
    int rows = 0;
    bool header = false;
    // Now the sorting is based on HEADER
    CSV(string file_name);
    int find_header(string t);
    map<string, vector<int>> ref_duplicate_finder(vector<int> A, int n, vector<vector<string>> *C);
    template <typename T>
    vector<int> cell_sort(int i, vector<int> list);
    void recur_sort(queue<string> n, vector<int> index_list, vector<int> edit_index, vector<int> *print_list, vector<vector<string>> *C);
    template <typename T>
    vector<int> select(string stand, int cols, int type, const vector<int> selected);
    vector<vector<float>> selected_data();

    void SHOW();
    void SELECTSHOW();
    void SORT(const char *args, ...);
    void UNSELECT();
    void SELECT(string t, string stand, int type = ALL);
    void RANDOMPICK(string t, unsigned long count);
    float SUM(string t);
    float VAR(string t, int type = POPUL);
    float AVG(string t);
    QUARTILE BOX(string col_name);
    void HTML_LINE_GRAPH(string file_name, int w, int h, string x_cols, string y_cols);
    void HTML_PLOT_GRPAH(string file_name, int w, int h, string x_cols, string y_cols, bool spline = false);
    void HTML_HISTO_GRAPH(string file_name, string col, int w, int h, float interval, int type = WIDE);
    void HTML_BOX_GRAPH(string file_name, int w, int h);
    void HTML_TABLE_DRAW(string file_name);
    void HTML_CLEAR(string file_name)
    {
        ofstream html_file(file_name.data());
        html_file.close();
    }
    void HTML_3D_SCATTER_PLOT(string file_name, int w, int h);
};

//-------------------------------------------------------------------------------------------------------

CSV::CSV(string file_name)
{
    string line;
    ifstream csv_file;
    csv_file.open(file_name.c_str());
    if (csv_file.is_open())
    {
        printf("CVS %s is OPEN\n", file_name.c_str());
        int num = 0;
        while (getline(csv_file, line))
        {
            print_order.push_back(num);
            csv.push_back(csv_spliting(line));
            num++;
        }
        cols = csv[0].size();
        rows = csv.size();
        csv_file.close();
    }
    else
    {
        perror("NO SUCH FILE");
    }
}
void CSV::SHOW()
{
    cout << "________________________________" << endl;
    for (int i = 0; i < print_order.size(); i++)
    {
        printf("[%5d]    ", i);
        for (auto m : csv[print_order[i]])
        {
            if (m.compare("NULL") != 0 && find(selected_rows.begin(), selected_rows.end(), print_order[i]) != selected_rows.end())
            {
                printf(GREEN);
                printf("%4.4s  ", m.c_str());
                printf(RESET);
            }
            else if (m.compare("NULL") == 0)
            {
                printf(RED);
                printf("%4.4s  ", m.c_str());
                printf(RESET);
            }
            else
            {
                printf("%4.4s  ", m.c_str());
            }
        }
        printf("\n");
    }
}
void CSV::SELECTSHOW()
{
    cout << "____________________________________________________________________________" << endl;
    for (auto i : selected_headers)
        printf("%4.4s  ", csv[0][i].c_str());
    printf("\n");
    for(auto i : selected_rows)
    {
        for (auto j : selected_headers)
        {
            if (csv[i][j].compare("NULL") != 0)
            {
                printf(GREEN);
                printf("%4.4s  ", csv[i][j].c_str());
                printf(RESET);
            }
            else if (csv[i][j].compare("NULL") == 0)
            {
                printf(RED);
                printf("%4.4s  ", csv[i][j].c_str());
                printf(RESET);
            }
            else
            {
                printf("%4.4s  ", csv[i][j].c_str());
            }
        }
        printf("\n");
    }
}
// t is header name
int CSV::find_header(string t)
{
    auto i = find(csv[0].begin(), csv[0].end(), t.c_str());
    if (i == csv[0].end())
        perror("HEADER NAME IS WRONG!");
    return i - csv[0].begin();
}
// cell duplicateed value finder
map<string, vector<int>> CSV::ref_duplicate_finder(vector<int> A, int n, vector<vector<string>> *C)
{
    map<string, vector<int>> duplicate_counter;
    map<string, vector<int>> ANS;
    for (int j = 0; j < A.size(); j++) // key setting
        duplicate_counter[(*C)[A[j]][n]].push_back(A[j]);
    for (auto key = duplicate_counter.begin(); key != duplicate_counter.end(); key++)
        if (key->first.compare("NULL") != 0 && key->second.size() > 1)
            for (vector<int>::iterator iter = key->second.begin(); iter != key->second.end(); iter++)
                ANS[key->first].push_back(*iter);
    return ANS;
}
template <typename T>
vector<int> CSV::cell_sort(int i, vector<int> list)
{
    vector<string> array(csv.size());
    vector<int> null_list;
    vector<int> index;
    for (int j = 0; j < list.size(); j++)
    {
        array[list[j]] = csv[list[j]][i]; // actual value
        if (csv[list[j]][i].compare("NULL") == 0)
            null_list.push_back(list[j]); // null list
        else
            index.push_back(list[j]); // the value index
    }
    vector<int> sort = ref_merge_sort<T>(index, &array); // sorteed index
    sort.insert(sort.end(), null_list.begin(), null_list.end());
    return sort;
}
void CSV::recur_sort(queue<string> n, vector<int> index_list, vector<int> edit_index,
                     vector<int> *print_list, vector<vector<string>> *C)
{
    if (!n.empty())
    { // if not empty heder
        string hd = n.front();
        int s_col = find_header(hd); // get header cols index
        n.pop();
        // it might be problem since only check middle and the first value are number or not
        if (is_number((*C)[1][s_col]) && is_number((*C)[csv.size() / 2][s_col]))
            index_list = cell_sort<float>(s_col, index_list);
        else
            index_list = cell_sort<string>(s_col, index_list);
        for (int i = 0; i < edit_index.size(); i++)
            (*print_list)[edit_index[i]] = index_list[i]; // change the print index from edit index by index list
        if (!n.empty())
        { // if there were another condition
            // to group up edit index and sorted index
            map<string, vector<pair<int, int>>> nlist;
            for (int i = 0; i < index_list.size(); i++)
                if ((*C)[index_list[i]][s_col].compare("NULL") != 0)
                    // to find the duplicated values
                    nlist[(*C)[index_list[i]][s_col]].push_back(make_pair(edit_index[i], index_list[i]));
            for (auto key = nlist.begin(); key != nlist.end(); key++)
            {
                // if the current key from map were duplicated
                if (key->second.size() > 1)
                {
                    vector<int> next_index_list;
                    vector<int> next_edit_index;
                    for (int j = 0; j < key->second.size(); j++)
                    {
                        next_index_list.push_back(key->second[j].second); // register the index value
                        next_edit_index.push_back(key->second[j].first);  // register the pair index of print value
                    }
                    recur_sort(n, next_index_list, next_edit_index, print_list, C); // recursion
                }
            }
        }
    }
}
template <>
vector<int> CSV::select<float>(string stand, int cols, int type, const vector<int> selected)
{
    vector<int> array;
    float _num = 0;
    if(type != DEDUPLE && type != ALL && !stand.empty())
        _num = stof(stand);
    for (int k = 0; k < selected.size(); k++)
    {
        int j = selected[k];
        if (csv[j][cols].compare("NULL") != 0)
            switch (type)
            {
            case UNDER:
                if (_num > stof(csv[j][cols]))
                    array.push_back(j);
                break;
            case OVER:
                if (_num < stof(csv[j][cols]))
                    array.push_back(j);
                break;
            case BELOW:
                if (_num >= stof(csv[j][cols]))
                    array.push_back(j);
                break;
            case ABOVE:
                if (_num <= stof(csv[j][cols]))
                    array.push_back(j);
                break;
            case EQUAL:
                if (_num == stof(csv[j][cols]))
                    array.push_back(j);
                break;
            case DEDUPLE:
            {
                bool b = true;
                float v = stof(csv[j][cols]);
                for(auto vec : array)
                {
                    if(stof(csv[vec][cols]) == v)
                    {
                        b = false;
                        break;
                    }
                }
                if(b)
                    array.push_back(j);
                break;
            }
            default:
                array.push_back(j);
                break;
            }
    }
    return array;
}
template <>
vector<int> CSV::select<string>(string stand, int cols, int type, const vector<int> selected)
{
    vector<int> array;
    string num = "";
    if(!stand.empty())
        num = stand;
    for (int k = 0; k < selected.size(); k++)
    {
        int j = selected[k];
        switch (type)
        {
        case UNDER:
            if (num.compare(csv[j][cols]) > 0)
                array.push_back(j);
            break;
        case OVER:
            if (num.compare(csv[j][cols]) < 0)
                array.push_back(j);
            break;
        case BELOW:
            if (num.compare(csv[j][cols]) >= 0)
                array.push_back(j);
            break;
        case ABOVE:
            if (num.compare(csv[j][cols]) <= 0)
                array.push_back(j);
            break;
        case EQUAL:
            if (num.compare(csv[j][cols]) == 0)
                array.push_back(j);
            break;
        case DEDUPLE:
            {
                bool b = true;
                for(auto vec : array)
                {
                    if(csv[vec][cols].compare(csv[j][cols]) == 0)
                    {
                        b = false;
                        break;
                    }
                }
                if(b)
                    array.push_back(j);
                break;
            }
        default:
            array.push_back(j);
            break;
        }
    }
    return array;
}
vector<vector<float>> CSV::selected_data()
{
    vector<vector<float>> ANS;
    if (!selected_rows.empty() && !selected_headers.empty())
    {
        for (set<int>::iterator k = selected_headers.begin(); k != selected_headers.end(); ++k)
        {
            vector<float> tmp;
            for (auto i : selected_rows)
                tmp.push_back(stof(csv[i][*k]));
            ANS.push_back(tmp);
        }
    }
    return ANS;
}
// [t] is header name
// [stand] is the standard
//  type
//  [UNDER]    >
//  [OVER]     <
//  [BELOW]    >=
//  [ABOVE]    <=
//  [EQUAL]    ==
//  [DEDUPLE]  duplicate remove
void CSV::SELECT(string t, string stand, int type)
{
    int i = find_header(t);
    selected_headers.insert(i);
    if (selected_rows.empty())
    {
        selected_rows.clear();
        selected_rows.resize(csv.size() - 1);
        iota(selected_rows.begin(), selected_rows.end(), 1);
    }
    if (is_number(csv[selected_rows[0]][i])) // might be problem if only the first rows is a number
        selected_rows = select<float>(stand, i, type, selected_rows);
    else
        selected_rows = select<string>(stand, i, type, selected_rows);
}
void CSV::RANDOMPICK(string t, unsigned long count)
{
    int i = find_header(t);
    set<int> randindex;
    selected_headers.insert(i);
    int j = 0;
    if (selected_rows.empty())
    {
        selected_rows.resize(csv.size() - 1);
        iota(selected_rows.begin(), selected_rows.end(), 1);
    }
    if(count > selected_rows.size())
        count = selected_rows.size();
    mt19937 mtRand(time(0));
    uniform_int_distribution<__int32_t> udist(0, selected_rows.size() - 1);
    do
    {
        int t = udist(mtRand);
        j = selected_rows[t];
        if(csv[i][j].compare("NULL") != 0)
            randindex.insert(j);
    }
    while(randindex.size() < count);
    selected_rows.clear();
    selected_rows.resize(count);
    copy(randindex.begin(), randindex.end(), selected_rows.begin());
}
// The last input elements as NULL
void CSV::SORT(const char *args, ...)
{
    va_list ap;         // dynamic variable argument list
    va_start(ap, args); // dynamic variable argument  pointer setting
    queue<string> headers;
    while (args)
    {
        string tmp = args;
        headers.push(tmp);
        args = va_arg(ap, const char *);
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
void CSV::UNSELECT()
{
    selected_headers.clear();
    selected_rows.clear();
}
// t is header name
// SELECT IS REQUIRED
float CSV::SUM(string t)
{
    vector<float> values;
    int i = find_header(t);
    for (int j = 0; j < selected_rows.size(); j++)
    {
        if (csv[selected_rows[j]][i].compare("NULL") != 0)
            values.push_back(stof(csv[selected_rows[j]][i]));
    }
    return Σ((float *)&values[0], (float *)&values[values.size() - 1]);
}
// t is header name
// variance
//  POPUL : poulation variance
//  SAMPLE : sample variance
// SELECT IS REQUIRED
float CSV::VAR(string t, int type)
{
    vector<float> values;
    int i = find_header(t);
    for (int j = 0; j < selected_rows.size(); j++)
    {
        if (csv[selected_rows[j]][i].compare("NULL") != 0)
            values.push_back(stof(csv[selected_rows[j]][i]));
    }
    return σ2((float *)&values[0], (float *)&values[values.size() - 1], type);
}
// t is header name
// AVERAGE
// SELECT IS REQUIRED
float CSV::AVG(string t)
{
    vector<float> values;
    int i = find_header(t);
    for (int j = 0; j < selected_rows.size(); j++)
    {
        if (csv[selected_rows[j]][i].compare("NULL") != 0)
            values.push_back(stof(csv[selected_rows[j]][i]));
    }
    return μ((float *)&values[0], (float *)&values[values.size() - 1]);
}
// BOX PLOT
// cols is required
// SELECT IS REQUIRED
QUARTILE CSV::BOX(string col_name)
{
    int cols = find_header(col_name);
    vector<float> data;
    for (int i = 0; i < selected_rows.size(); i++)
        // could be problem when the string start from number but current let is
        if (csv[selected_rows[i]][cols].compare("NULL") != 0 && isnumber(csv[selected_rows[i]][cols][0]))
            data.push_back(stof(csv[selected_rows[i]][cols]));
    sort(data.begin(), data.end());
    return box_plot(data);
    ;
}
// If selected data is exsisted, it search based on the row of selected data values
// If selected data is not exsisted, it search based on the all row of all data values
// The NULL data is always skipped
// ONLY NUMERIC VALUE
void CSV::HTML_LINE_GRAPH(string file_name, int w, int h, string x_cols, string y_cols)
{
     int x = find_header(x_cols);
    int y = find_header(y_cols);
    //sorting
    priority_queue < pair<float, float>, vector<pair<float, float>>, rcmp<float> > xydata;
    vector<float> xdata;
    vector<float> ydata;
    if (!selected_rows.empty())
    {
        for (auto vec : selected_rows)
        {
            if (csv[vec][x].compare("NULL") != 0 && csv[vec][y].compare("NULL") != 0)
            {
                xydata.push({stof(csv[vec][x]), stof(csv[vec][y])});
            }
        }
    }
    else
    {
        for (int i = 1; i < csv.size(); i++)
        {
            if (csv[i][x].compare("NULL") != 0 && csv[i][y].compare("NULL") != 0)
            {
                xydata.push({stof(csv[i][x]), stof(csv[i][y])});
            }
        }
    }
    while(!xydata.empty())
    {
        xdata.push_back(xydata.top().first);
        ydata.push_back(xydata.top().second);
        xydata.pop();
    }
    if (!xdata.empty() && !ydata.empty())
    {
        ofstream html_file(file_name.data(), ios::app);
        if (html_file.is_open()){
            string name = "xyL" + to_string(time(NULL));
            html_canvas_create(&html_file, w, h, name);
            v2_html_2xy_line(&html_file, w, h, "blue", 1, xdata, ydata);
        }
    }
}
// If selected data is exsisted, it search based on the row of selected data values
// If selected data is not exsisted, it search based on the all row of all data values
// The NULL data is always skipped
// ONLY NUMERIC VALUE
void CSV::HTML_PLOT_GRPAH(string file_name, int w, int h, string x_cols, string y_cols, bool spline)
{
    int x = find_header(x_cols);
    int y = find_header(y_cols);
    //sorting
    priority_queue < pair<float, float>, vector<pair<float, float>>, rcmp<float> > xydata;
    vector<float> xdata;
    vector<float> ydata;
    if (!selected_rows.empty())
    {
        for (auto vec : selected_rows)
        {
            if (csv[vec][x].compare("NULL") != 0 && csv[vec][y].compare("NULL") != 0)
            {
                xydata.push({stof(csv[vec][x]), stof(csv[vec][y])});
            }
        }
    }
    else
    {
        for (int i = 1; i < csv.size(); i++)
        {
            if (csv[i][x].compare("NULL") != 0 && csv[i][y].compare("NULL") != 0)
            {
                xydata.push({stof(csv[i][x]), stof(csv[i][y])});
            }
        }
    }
    while(!xydata.empty())
    {
        xdata.push_back(xydata.top().first);
        ydata.push_back(xydata.top().second);
        xydata.pop();
    }
    if (!xdata.empty() && !ydata.empty())
    {
        ofstream html_file(file_name.data(), ios::app);
        if (html_file.is_open()){
            string name = "xyP" + to_string(time(NULL));
            html_canvas_create(&html_file, w, h, name);
            v2_html_2xy_plot(&html_file, w, h, "red", 4, xdata, ydata);
            if(spline)
            {
                vector<vector<float>> c = cubi_spline(xdata, ydata, xdata.size() * 4);
                v2_html_2xy_line(&html_file, w, h, "blue", 1, c[0], c[1]);
            }
            // v2_html_2xy_plot(&html_file, w, h, "blue", 2, c[0], c[1]);
        }
    }
}
// SELECT IS REQUIRED
// type
// WIDE - show all interval
// SHORT - show shrink interval
// ONLY NUMERIC VALUE
void CSV::HTML_HISTO_GRAPH(string file_name, string col, int w, int h, float interval, int type)
{
    ofstream html_file(file_name.data(), ios::app);
    if (html_file.is_open())
    {
        int n = find_header(col);
        vector<string> data_s;
        vector<float> data_f;
        if (!selected_rows.empty())
        {
            // it might be the error for distinguish the vlaue is number or not
            if (is_number(csv[selected_rows[0]][n]) && is_number(csv[selected_rows[0]][n]))
            {
                for (auto vec : selected_rows)
                {
                    if (csv[vec][n].compare("NULL") != 0)
                    {
                        data_f.push_back(stof(csv[vec][n]));
                    }
                }
                if (type == SHORT)
                    html_histogram<SHORT, float>(&html_file, w, h, interval, data_f);
                else
                    html_histogram<WIDE, float>(&html_file, w, h, interval, data_f);
            }
            else
            {
                for (auto vec : selected_rows)
                {
                    if (csv[vec][n].compare("NULL") != 0)
                    {
                        data_s.push_back(csv[vec][n]);
                    }
                }
                html_histogram<SHORT, string>(&html_file, w, h, interval, data_s);
            }
        }
        else
        {
            // it might be the error for distinguish the vlaue is number or not
            if (is_number(csv[1][n]) && is_number(csv[csv.size() / 2][n]))
            {
                for (int i = 1; i < csv.size(); i++)
                {
                    if (csv[i][n].compare("NULL") != 0)
                    {
                        data_f.push_back(stof(csv[i][n]));
                    }
                }
                if (type == SHORT)
                    html_histogram<SHORT, float>(&html_file, w, h, interval, data_f);
                else
                    html_histogram<WIDE, float>(&html_file, w, h, interval, data_f);
            }
            else
            {
                for (int i = 1; i < csv.size(); i++)
                {
                    if (csv[i][n].compare("NULL") != 0)
                    {
                        data_s.push_back(csv[i][n]);
                    }
                }
                html_histogram<SHORT, string>(&html_file, w, h, interval, data_s);
            }
        }
        html_file.close();
    }
}
// SELECT IS REQUIRED
// ONLY NUMERIC VALUE
void CSV::HTML_BOX_GRAPH(string file_name, int w, int h)
{
    ofstream html_file(file_name.data(), ios::app);
    if (html_file.is_open())
    {
        vector<vector<float>> data = selected_data();
        if (!data.empty())
            html_box_plot(&html_file, w, h, data);
    }
    html_file.close();
}
// SELECT IS REQUIRED
void CSV::HTML_TABLE_DRAW(string file_name)
{
    ofstream html_file(file_name.data(), ios::app);
    if (html_file.is_open())
    {
        html_file << "<title> DATA CSV FILE </title>";
        // creating table
        html_file << "<div style = \"float: left; padding:10px;\">\n";
        html_file << "<b1>DATA TABLE</b1>\n";
        html_file << "<table border = \"1\">\n";
        // table header
        for (int i = 0; i < csv[0].size(); i++)
            html_file << "<th>" << csv[0][i] << "</th>";
        // table variable
        for (int i = 1; i < csv.size(); i++)
        {
            html_file << "<tr>\n";
            for (int j = 0; j < csv[i].size(); j++)
                // if NULL red text color
                if (csv[print_order[i]][j].compare("NULL") == 0)
                    html_file << "<td style=\"color:red\">" << csv[print_order[i]][j] << "</td>";
                // selecteed row check
                else if (find(selected_rows.begin(), selected_rows.end(), i) != selected_rows.end() && find(selected_headers.begin(), selected_headers.end(), j) != selected_headers.end())
                    html_file << "<td bgcolor=\"#D5F5E3\">" << csv[print_order[i]][j] << "</td>";
                else
                    html_file << "<td>" << csv[print_order[i]][j] << "</td>";
            html_file << "</tr>\n";
        }
        html_file << "</table>\n </div>\n";
        // selected data table
        html_file << "<div style = \"float: left; padding:10px;\">\n";
        html_file << "<b1>SELECTED DATA TABLE</b1>\n";
        if (!selected_rows.empty())
        {
            html_file << "<table border = \"1\">\n";
            for (set<int>::iterator key = selected_headers.begin(); key != selected_headers.end(); ++key)
                html_file << "<th>" << csv[0][*key] << "</th>";
            for (int i = 0; i < selected_rows.size(); i++)
            {
                html_file << "<tr>\n";
                for (set<int>::iterator key = selected_headers.begin(); key != selected_headers.end(); ++key)
                    if (csv[selected_rows[i]][*key].compare("NULL") == 0)
                        html_file << "<td  style=\"color:red\">" << csv[i][*key] << "</td>";
                    else
                        html_file << "<td>" << csv[selected_rows[i]][*key] << "</td>";
                html_file << "</tr>\n";
            }
        }
        html_file << "</table>\n </div>\n";
    }
    html_file.close();
}
// SELECT IS REQUIRED
void CSV::HTML_3D_SCATTER_PLOT(string file_name, int w, int h)
{
    ofstream html_file(file_name.data(), ios::app);
    if (html_file.is_open())
    {
        string name = "xy3D" + to_string(time(NULL));
        vector<vector<float>> data = selected_data();
        if (!data.empty())
        {
        
        }
    }
    html_file.close();
}

#endif