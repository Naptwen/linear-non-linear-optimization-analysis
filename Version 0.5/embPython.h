#ifdef _DEBUG
#define _DEBUG_WAS_DEFINED 1
#undef _DEBUG
#endif
#include <python.h>
#ifdef _DEBUG_WAS_DEFINED
#define _DEBUG 1
#endif
#pragma once
#include <stdio.h>
#include <string>

///////////////////////////////////////////////////
//Reference from //////////////////////////////////
//https://docs.python.org/3/extending/embedding.htm
///////////////////////////////////////////////////

using namespace std;

PyObject* PyRun(string python_name, string def_name, const char* types, ...);
string PyObj2str(PyObject* obj);
int PyObjint(PyObject* obj);
//Send Str and get Str
string PySimpleRun(const char* file_name, const char* file_module, const char* input_str);
string PySimpleRunThread(const char* file_name, const char* file_module, const char* input_str);
