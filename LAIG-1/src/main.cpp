#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "Scene.h"
#include "Parser.h"

using std::cin;
using std::cout;
using std::exception;

#include <typeinfo> //apagar


int main(int argc, char* argv[]) {
	CGFapplication app = CGFapplication();

	OrthoCamera *a = new OrthoCamera("a", 2, 3, 4, 5, 6, 7);
	string s = typeid(a).name();
	cout << "a;"<< s<< "b" << endl;
	if (s == "class OrthoCamera *")
		cout << "fuck yeah";

	try
	{
		Parser *parser = new Parser("teste.yaf");
		try {
			app.init(&argc, argv);
			app.setScene(&parser->scene);
			app.setInterface(&parser->interface);
		
			app.run();
		}
		catch(GLexception& ex) {
			cout << "Error: " << ex.what();
			return -1;
		}
		catch(exception& ex) {
			cout << "Unexpected error: " << ex.what();
			return -1;
		}
	}
	catch (const char* msg) { //parse exception
		cout << "EXCEPTION: " << msg;
		cin.get();
		exit(1);
	}

	return 0;
}