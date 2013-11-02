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
		app.init(&argc, argv);
		/*if(argc<2 || argc>2)
			throw "Usage: thisProgram scene.yaf";
		Parser *parser = new Parser(argv[1]);*/
		Parser *parser = new Parser("cenas/nosso/teste.yaf"); //for developing
		try {
			app.setScene(&parser->scene);
			app.setInterface(&parser->interface);
			app.run();
		}
		catch(GLexception& ex) {
			throw "Error: " + *ex.what();
		}
		catch(exception& ex) {
			throw "Unexpected error: " + *ex.what();
		}
	}
	catch (const char* msg) { //parse exception
		cout << "EXCEPTION: " << msg;
		exit(1);
	}

	return 0;
}