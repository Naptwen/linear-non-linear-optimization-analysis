#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <conio.h>
#include "embPython.h"
using namespace std;
int main()
{

	string rec = PySimpleRun("pyemb", "hello", "USG");

	printf("Bow Wow");
	_getch();
	return 0;
}