
#include <iostream>

#include <python2.7/Python.h>

#include "Python.hpp"

SINGLETON_DECLARE(Python);

Python::Python()
{
/*
    Py_Initialize();
    
    PyObject *pName, *pModule, *pDict, *pFunc, *pValue;


    // Build the name object
    pName = PyString_FromString("montest");

    // Load the module object
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    // pDict is a borrowed reference 
    pDict = PyModule_GetDict(pModule);

    // pFunc is also a borrowed reference 
    pFunc = PyDict_GetItemString(pDict, "test");

    PyObject_CallObject(pFunc, NULL);

    // Clean up
    Py_DECREF(pModule);

    */
}

Python::~Python()
{
    //Py_Finalize();
}

