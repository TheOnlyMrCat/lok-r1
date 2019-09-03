//
//  main.cpp
//  Lok
//
//  Created by Maxwell Guppy on 5/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "cxxopts.hpp"
#include "clok.hpp"
#include "yydef.hpp"

#include <iostream>
#include <fstream>
#include <cstdio>

extern "C"
int yywrap() {
	return 1; //TODO?
}

namespace clok {
	bool VERBOSE;
}

extern FILE *yyin;

int main(int argc, char *argv[])
{
	auto optParser = cxxopts::Options("clok", "Compiler for the Lok programming language");

	optParser.add_options()
	("h,help", "Print this help message, then exit")
	(  "version", "Print version information, then exit")
	("v,verbose", "Enable verbose logging")
	(  "ast", "Generate Abstract Syntax Tree only", cxxopts::value<std::string>()->implicit_value(""))
	;

	auto options = optParser.parse(argc, argv);

	if (options.count("version")) {
		std::cout << "Lok compiler (clok) version 0.1 alpha" << std::endl;
		return EXIT_SUCCESS;
	}

	if (options.count("help")) {
		std::cout << optParser.help() << std::endl;
		return EXIT_SUCCESS;
	}

	clok::VERBOSE = options.count("verbose") > 0;

	std::vector<node_t> programs;
	programs.reserve(argc - 1);

	for (int i = 1; i < argc; i++) {
		if (clok::VERBOSE) std::cout << "Parsing file: " << argv[i] << std::endl;
		yyin = fopen(argv[i], "r");
		if (yyin == NULL) {
			std::cerr << "clok: Could not open file: " << argv[i] << std::endl;
			return EXIT_FAILURE;
		}

		try {
			programs.push_back(clok::parse());
		} catch (ParseError e) {
			std::cerr << "clok: Parsing error (" << argv[i] << ':' << yylineno << ":" << yycol << ")" << std::endl
			          << "\t" << e.what() << std::endl;
			fclose(yyin);
		}
		fclose(yyin);
	}

	if (options.count("ast")) {
		std::ostream *output = &std::cout;

		std::string filename = options["ast"].as<std::string>();
		if (filename != "") output = new std::fstream(filename);
		
		for (node_t n : programs) {
			clok::printAST(n, output);
		}

		if (filename != "") delete output;
	}

	return EXIT_SUCCESS;
}
