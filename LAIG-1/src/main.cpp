#include <iostream>
#include <exception>

#include "CGFapplication.h"
#include "Scene.h"
#include "Parser.h"

using std::cin;
using std::cout;
using std::exception;


int main(int argc, char* argv[]) {
	CGFapplication app = CGFapplication();

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