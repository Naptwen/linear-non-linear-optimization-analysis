
#include <iostream>
#include "embPython.h"
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define FRONT ">$"//"<div class=\"\YMlKec fxkbkc\"\>"
#define BACK "</div>"
#define APPLE "https://www.google.com/finance/quote/AAPL:NASDAQ"
#define AMAZON "https://www.google.com/finance/quote/AMZN:NASDAQ"
#define TESLA "https://www.google.com/finance/quote/TSLA:NASDAQ"
#define MICRO "https://www.google.com/finance/quote/MSFT:NASDAQ"
#define BERKS "https://www.google.com/finance/quote/BRK.A:NYSE"
#define JPMOR "https://www.google.com/finance/quote/JPM:NYSE"
#define GOLDM "https://www.google.com/finance/quote/GS:NYSE"

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
    string src = PySimpleRun("pyemb", "src_URL", url.c_str());
    int index1, index2;
    if (index1 = src.find(front))
    {
        index1 = index1 + strlen(front.c_str());
        if (index2 = src.find(rear, index1))
        {
            if (index1 < index2)
            {
                size_t gap = index2 - index1;
                string str = src.substr(index1, gap);
                return str;
            }
        }

    }
    return NULL;
}

void stock_up_dwon()
{

}

int main()
{
    bool mecro = false;
    while (true)
    {
        if (mecro == false)
        {
            system("CLS");
            setcolor(14, 0);
            std::printf("|-----------------------------------|\n");
            std::printf("|                MARKET             |\n");
            std::printf("|-----------------------------------|\n");
            std::printf("|(A) : START                        |\n");
            std::printf("|(M) : WAITING MARKET               |\n");
            std::printf("|(X) : END                          |\n");
            std::printf("|-----------------------------------|\n");
            switch (_getch())
            {
            case 'a':
                std::printf("Starting...\n");
                break;
            case 'm':
                std::printf("Waiting...\n");
                mecro = true;
                break;
            case 'e':
                std::printf("End...\n");
                return 1;
            }
        }
        time_t t = time(NULL);
        struct tm tm;
        localtime_s(&tm, &t);

        if (tm.tm_hour > 6 && tm.tm_min > 29 && tm.tm_hour < 13 && tm.tm_min < 0)
        {
            system("CLS");
            setcolor(14, 0);
            cout << tm.tm_hour << ", " << tm.tm_min << endl;
            cout << "|-------------------------------------------------------\n";
            cout << "|--[APPLE]  : \n\n";
            cout << "|--[AMAZON] : \n\n";
            cout << "|--[TESLA]  : \n\n";
            cout << "|--[MICRO]  : \n\n";
            cout << "|--[BERKS]  : \n\n";
            cout << "|--[JPM]    : \n\n";
            cout << "|--[GOLD]   : \n\n";
            float cur_appl, cur_amaz, cur_tsla, cur_mict, cur_berk, cur_jpm, cul_gpldm;
            float past_stock[7] = { 0, 0, 0, 0, 0, 0, 0 };
            float cur_max[7] = { 0, 0, 0 , 0 , 0, 0, 0 };
            float cur_min[7] = { 0, 0, 0 , 0 , 0, 0, 0 };

            while (true)
            {
                localtime_s(&tm, &t);
                setcolor(7, 0);
                string appl = numbering_url(APPLE, FRONT, BACK);
                std::printf("--");
                string amaz = numbering_url(AMAZON, FRONT, BACK);
                std::printf("--");
                string tsla = numbering_url(TESLA, FRONT, BACK);
                std::printf("--");
                string mict = numbering_url(MICRO, FRONT, BACK);
                std::printf("--");
                string berk = numbering_url(BERKS, FRONT, BACK);
                std::printf("--");
                string jpm = numbering_url(JPMOR, FRONT, BACK);
                std::printf("--");
                string gpldm = numbering_url(GOLDM, FRONT, BACK);
                std::printf("--");

                string str[7] = { appl, amaz, tsla, mict, berk, jpm, gpldm };
                float cur_stock[7] = { 0,0,0,0,0,0,0 };
                for (int i = 0; i < 7; i++)
                {
                    string temp = str[i]; // save in temp
                    remove(temp.begin(), temp.end(), ',');
                    cur_stock[i] = stof(temp);
                }

                FILE* fp;
                string file_name = "./" + to_string(tm.tm_mday) + "_stock.csv";
                fopen_s(&fp, file_name.c_str(), "a");
                fprintf(fp, "%d-%d-%d\n%d:%d:%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n"
                    , tm.tm_year, tm.tm_mon, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec
                    , cur_stock[0], cur_stock[1], cur_stock[2], cur_stock[3]
                    , cur_stock[4], cur_stock[5], cur_stock[6]);
                fclose(fp);

                for (int k = 0; k < 7; k++)
                {
                    if (cur_stock[k] > past_stock[k])
                        setcolor(15, 4);
                    else if (cur_stock[k] <= past_stock[k])
                        setcolor(15, 1);

                    if (cur_max[k] == 0 && cur_min[k] == 0)
                    {
                        cur_max[k] = cur_stock[k];
                        cur_min[k] = cur_stock[k];
                    }
                    if (cur_stock[k] > cur_max[k])
                        cur_max[k] = cur_stock[k];
                    if (cur_stock[k] < cur_min[k])
                        cur_min[k] = cur_stock[k];

                    gotoxy(12, 2 * k + 1);
                    std::printf("%.2f", cur_stock[k]);
                    past_stock[k] = cur_stock[k];

                    setcolor(3, 0);
                    gotoxy(0, 2 * k + 2);
                    std::printf("[max] %.2f", cur_max[k]);
                    gotoxy(20, 2 * k + 2);
                    std::printf("[min] %.2f", cur_min[k]);
                    gotoxy(40, 2 * k + 2);
                    std::printf("[gap] %.2f", cur_max[k] - cur_min[k]);

                    if (tm.tm_hour >= 16)
                    {
                        gotoxy(60, 2 * k + 2);
                        std::printf("[end] max %.2f", cur_max[k]);
                        gotoxy(80, 2 * k + 2);
                        std::printf("[end] min %.2f\n", cur_min[k]);
                        gotoxy(100, 2 * k + 2);
                        std::printf("[gap] %.2f\n", cur_max[k] - cur_min[k]);
                    }
                }
                //break point
                if (tm.tm_mday >= 13 && tm.tm_sec > 0)
                {
                    mecro = false;
                    break;
                }                    
                Sleep(10000);
            }
        }
        gotoxy(0, 9);
        setcolor(4, 3);
        std::printf("Current time is %d : %d : %d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
        std::printf("market is close. . . \n");
        Sleep(1000);
    }
    return 1;
}
