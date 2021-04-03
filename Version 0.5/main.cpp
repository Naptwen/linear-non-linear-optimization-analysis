
#include <iostream>
#include "embPython.h"
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <vector>
#include <thread>
#include <mutex>
#include <future>

#define FRONT ">$"//"<div class=\"\YMlKec fxkbkc\"\>"
#define BACK "</div>"
#define APPLE  "https://www.google.com/finance/quote/AAPL:NASDAQ"
#define AMZN "https://www.google.com/finance/quote/AMZN:NASDAQ"
#define TSLA  "https://www.google.com/finance/quote/TSLA:NASDAQ"
#define MSFT  "https://www.google.com/finance/quote/MSFT:NASDAQ"
#define BRKA  "https://www.google.com/finance/quote/BRK.A:NYSE"
#define JPM  "https://www.google.com/finance/quote/JPM:NYSE"
#define GS  "https://www.google.com/finance/quote/GS:NYSE"

using namespace std;

mutex _m;
static bool red_button = false;

void setcolor(int txt_color, int back_color)
{
    int color = 16 * back_color + txt_color;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    return;
}

string numbering_url(string url, string front, string rear)
{
    _m.lock();
    string src = PySimpleRun("pyemb", "src_URL", url.c_str());
    _m.unlock();
    int index1, index2;
    if (index1 = src.find(front))
    {
        index1 = index1 + strlen(front.c_str());
        if (index2 = src.find(rear, index1))
        {
            if (index1 < index2)
            {
                int gap = index2 - index1;
                string str = src.substr(index1, gap);
                return str;
            }
        }
    }
    return nullptr;
}

class stock
{
public:
    string name = " ";
    string url = " ";
    float price = 0.0f;

    stock(string name, string url)
    {
        this->name = name;
        this->url = url;
    }
    void set_price(const char* front, const char* rear)
    {
        _m.lock();
        string src = PySimpleRunThread( "pyemb", "src_URL", url.c_str());
        _m.unlock();
        //string src = PySimpleRun("pyemb", "src_URL", url.c_str());
        int index1, index2;
        if (index1 = src.find(front))
        {
            index1 = index1 + strlen(front);
            if (index2 = src.find(rear, index1))
            {
                if (index1 < index2)
                {
                    int gap = index2 - index1;
                    string str = src.substr(index1, gap);
                    remove(str.begin(), str.end(), ',');
                    this->price = stof(str);
                }
            }
        }  
    }
};

void saving_stock(vector<stock> *stock_list)
{
    time_t t = time(NULL);
    struct tm tm;
    localtime_s(&tm, &t);

    FILE* fp;
    string file_name = "./" + to_string(tm.tm_mday) + "_stock.csv";
    fopen_s(&fp, file_name.c_str(), "a");
 
    if (fp)
    {
        std::fprintf(fp, "%d-%d-%d\n%d:%d:%d\t",
            tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

        for (int i = 0; i < stock_list->size(); i++)
            std::fprintf(fp, "%f\t", stock_list->at(i).price);

        std::fprintf(fp, "\n");

        std::fclose(fp);
    }
}

void market_showing()
{
    //---------------------------DATA SET--------------------------
    vector<stock> url_list;
    vector<thread> th_list;
    vector<float> cur_price;
    vector<float> max_price;
    vector<float> min_price;
    stock sAAPL("AAPL", APPLE);
    stock sAMZN("AMZN", AMZN);
    stock sTSLA("TSLA", TSLA);
    stock sMSFT("MSFT", MSFT);
    stock sBRKA("BRKA", BRKA);
    stock sJPM("JPM", JPM);
    stock sMGS("GS", GS);
    url_list.push_back(sAAPL);
    url_list.push_back(sAMZN);
    url_list.push_back(sTSLA);
    url_list.push_back(sMSFT);
    url_list.push_back(sBRKA);
    url_list.push_back(sJPM);
    url_list.push_back(sMGS);;    
    //------------------------------UI-------------------------------
    setcolor(14, 0);
    gotoxy(0, 0);
    std::printf("|------------------------------------------------------------------------|\n");
    std::printf("|                 M    A   R   K   E   T                                 |\n");
    std::printf("|------------------------------------------------------------------------|\n");
    std::printf("|                                                        [ESC] Exit      |\n");
    std::printf("|--[NAME]------[PRICE]---------------------------------------------------|\n");
    //--------------------------List---------------------------------
    setcolor(2, 0);
    for (int i = 0; i < url_list.size(); i++)
    {
        gotoxy(0, 5 + i);
        printf("L_ [%s]", url_list[i].name.c_str());
    }
    //--------------initial value-------------------------------
    for (int i = 0; i < url_list.size(); i++)
        th_list.push_back(thread(&stock::set_price, &url_list[i], FRONT, BACK));
    //--------------Waiting thread-----------------------
    for (int i = 0; i < th_list.size(); i++)
        th_list[i].join();
    //---------------SET CUR VAL-------------------------------
    for (int i = 0; i < url_list.size(); i++)
    {
        cur_price.push_back(url_list[i].price);
        max_price.push_back(url_list[i].price);
        min_price.push_back(url_list[i].price);
    }        
    //-------------------------Update--------------------------------
    while (red_button == false)
    {
        //---------------Price-------------------------------
        th_list.clear();
        for (int i = 0; i < url_list.size(); i++)
            th_list.push_back(thread(&stock::set_price, &url_list[i],FRONT, BACK));
        //--------------Waiting thread-----------------------
        for (int i = 0; i < th_list.size(); i++)
            th_list[i].join();
        //---------------DISPLAY------------------------------
        for (int i = 0; i < url_list.size(); i++)
        {
            //---------------Display price--------------------
            gotoxy(12, 5 + i);
            if (cur_price[i] > url_list[i].price)
                setcolor(4, 0);
            else
                setcolor(3, 0);
            printf("$  %.2f", url_list[i].price);
            cur_price[i] = url_list[i].price;
            //---------------MAX, MIN price-------------------
            if (max_price[i] < cur_price[i])
                max_price[i] = cur_price[i];
            if (min_price[i] > cur_price[i])
                min_price[i] = cur_price[i];
            setcolor(4, 0);
            gotoxy(30, 5 + i);
            printf("High $  %.2f", max_price[i]);
            setcolor(3, 0);
            gotoxy(50, 5 + i);
            printf("Low  $  %.2f", min_price[i]);
        }
        //---------------SAVE VAL-----------------------------
        saving_stock(&url_list);
        //----------------TIMER-------------------------------
        time_t t = time(NULL);
        struct tm tm;
        localtime_s(&tm, &t);
        setcolor(13, 0);
        gotoxy(15, 15);
        printf("%c[2K", 27);
        std::printf("TIMER [%d-%d-%d  %d:%d:%d]",
        tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
        //----------------WAIT--------------------------------
        Sleep(1000);
    }
}

void _key()
{
    while (true)
    {
        int c = _getch();
        if (c == 27)
        {
            _m.lock();
            red_button = true;
            _m.unlock();
            break;
        }
        char buf[256];
        sprintf_s(buf, "%d \n", c);
        OutputDebugStringA(buf);
        Sleep(100);
    }
}

int main()
{
 
    thread market(market_showing);
    thread key_put(_key);
    
    key_put.join();
    market.join();
}
