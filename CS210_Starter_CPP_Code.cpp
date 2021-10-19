#include <Python.h>
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

/*
Description:
	To call this function, simply pass the function name in Python that you wish to call.
Example:
	callProcedure("printsomething");
Output:
	Python will print on the screen: Hello from python!
Return:
	None
*/
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("CS_210_Starter_PY_Code");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("PrintMe","Test");
Output:
	Python will print on the screen:
		You sent me: Test
Return:
	100 is returned to the C++
*/
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"CS_210_Starter_PY_Code");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}

/*
Description:
	To call this function, pass the name of the Python functino you wish to call and the string parameter you want to send
Example:
	int x = callIntFunc("doublevalue",5);
Return:
	25 is returned to the C++
*/
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"CS_210_Starter_PY_Code");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference 
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference 
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean 
	delete[] procname;

	return _PyLong_AsInt(presult);
}


int main()
{
	int userInput;
	
	/*Asks for user to select an option. Each option will call a Python function*/

	cout << "Select from the following options:" << endl;
	cout << endl;
	cout << "1) Display every item and quantity." << endl;
	cout << "2) Display a single item and quantity." << endl;
	cout << "3) Display graph of option 1." << endl;
	cout << "4) Exit program" << endl << endl;

	//while (cin >> userInput) {
		while (cin >> userInput && userInput != 4) {
			if (userInput == 1) {
				/* Displays a list of items along with the quantity of each item. Python code prints the display*/
				cout << "Option 1" << endl;
				cout << endl;
				CallProcedure("DisplayGroceryQuantity");
			}
			else if (userInput == 2) {
				/*Allows for user to select an individual item, then will send that item to Python function. Python function returns the
				quantity of the item selected an C++ will print the item and quantity*/
				string groceryItem;
				cout << "Option 2" << endl << endl;
				cout << "Select one from the following to display?" << endl;
				cout << "Apples, Beets, Cantaloupe, Cauliflower, Celery, " << endl;
				cout << "Cranberries, Cucumbers, Garlic, Limes, Onions, " << endl;
				cout << "Peaches, Pears, Peas, Potatoes, Pumpkins, Radishes, " << endl;
				cout << "Spinach, Yams, Zucchini" << endl;
				cout << endl;
				cin >> groceryItem;

				cout << "Selected: " << groceryItem << endl;
				cout << "Quantity: " << callIntFunc("DisplayIndividualQuantity", groceryItem) << endl;

			}
			else if (userInput == 3) {
				/*Allows for user to see a Histogram with 'y' axis as the item and 'x' axis quantity. Python file will create a file of
				all items and associated quantity. C++ will read that file 'frequency.dat' then display the data as a histogram*/
				cout << "Option 3" << endl;
				cout << endl;
				CallProcedure("CreateHistogramFile");
				ifstream inFreq;
				ofstream outFreq;
				vector<string> groceryItems;
				string groceryItem;
				vector<int> quantities;
				int quantity;
				unsigned int i;
				char bar = '^';

				inFreq.open("frequency.dat");

				if (!inFreq.is_open()) {
					cout << "File unable to be opened." << endl;
				}
				while (!inFreq.fail()) {
					inFreq >> groceryItem;
					groceryItems.push_back(groceryItem);
					inFreq >> quantity;
					quantities.push_back(quantity);
				}
				
				for (i = 0; i < groceryItems.size() - 1; ++i) {
					cout << setw(13) << setfill(' ') << left << groceryItems.at(i) << " " << string(quantities.at(i), bar) << endl;
				}
				inFreq.close();
			}
			else {
				/*if user selects option other than 1-4 then code will ask for input again.*/
				cout << "Select from the following." << endl;
				cout << endl;
				cout << "1) Display every item and quantity." << endl;
				cout << "2) Display a single item and quantity." << endl;
				cout << "3) Display graph of option 1." << endl;
				cout << "4) Exit program" << endl;
				cout << endl;
			}	
		}
		/*Will exit the program if user input equals 4*/
		cout << endl;
		if (userInput == 4) {
			CallProcedure("ExitProgram");
		}
		else {
		cout << "Input is not an option. Please restart the program" << endl;
		}
	//}
	//cout << "Input is not an option. Please restart the program" << endl;
}