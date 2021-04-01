

#include <iostream>
#include "embPython.h"
#include <conio.h>
#include <vector>
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

string numbering_url (string url, string front, string rear)
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

int main()
{
    cout << "Hello World!\n";
    string appl = numbering_url(APPLE, FRONT, BACK);
    string amaz = numbering_url(AMAZON, FRONT, BACK);
    string tsla = numbering_url(TESLA, FRONT, BACK);
    string mict = numbering_url(MICRO, FRONT, BACK);
    string berk = numbering_url(BERKS, FRONT, BACK);
    string jpm = numbering_url(JPMOR, FRONT, BACK);
    string gpldm = numbering_url(GOLDM, FRONT, BACK);


    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
    cout << "[APPLE]  : " << appl << endl;
    cout << "[AMAZON] : " << amaz << endl;
    cout << "[TESLA]  : " << tsla << endl;
    cout << "[MICRO]  : " << mict << endl;
    cout << "[BERKS]  : " << berk << endl;
    cout << "[JPM]    : " << jpm << endl;
    cout << "[GOLD]   : " << gpldm << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);

    vector<string> str = { appl, amaz, tsla, mict, berk, jpm, gpldm };
    vector<float> c;

    for (int i = 0; i < str.size(); i++)
    {
        string temp = str.at(i); // save in temp
        for (int j = 0; j < temp.length(); j++) //remove ,
        {
            if (temp[j] == ',')
            {
                temp.replace(j, 1, "");
            }
        }        
        c.push_back(stof(temp));
    }


    if (c.size() >= 14)
    {
        for (int k = 7; k > 0; k--)
        {
            if (c.at(c.size() - k) > c.at(c.size() - k * 2))
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
            else
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
            switch (k)
            {
            case 7:
                 cout << "[APPLE]  : " << appl << endl;
                 break;
             //case 6:
             //    cout << "[AMAZON] : " << amaz << endl;
             //    break;
             //case 5:
             //    cout << "[TESLA]  : " << tsla << endl;
             //    break;
             //case 4:
             //    cout << "[MICRO]  : " << mict << endl;
             //    break;;
             //case 3:
             //    cout << "[BERKS]  : " << berk << endl;
             //    break;
             //case 2:
             //    cout << "[JPM]    : " << jpm << endl;
             //    break;
             //case 1:
             //    cout << "[GOLD]   : " << gpldm << endl;
             //    break;
            }            
        }      
    }









    _getch();
    return 1;
}
