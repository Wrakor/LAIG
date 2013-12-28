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

	Parser *parser;
	try
	{
		app.init(&argc, argv);
		/*if(argc<2 || argc>2)
			throw "Usage: thisProgram scene.yaf";
		Parser *parser = new Parser(argv[1]);*/
		parser = new Parser("cenas/pentago/ambiente1.yaf");
		try {
			app.setScene(&parser->scene);
			app.setInterface(&parser->interface);
//			s = parser->scene.socket;
//			app.setInterface(&parser->start_interface);
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
		if (msg != "exit")
			cout << "EXCEPTION: " << msg << endl;
	}

	delete(parser);

	return 0;
}