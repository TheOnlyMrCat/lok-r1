//
//  main.cpp
//  Lok
//
//  Created by TheOnlyMrCat on 5/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "cxxopts.hpp"
#include "clok.hpp"
#include "yydef.hpp"

#include <iostream>
#include <fstream>
#include <cstdio>

#include <boost/filesystem.hpp>

namespace bfs = boost::filesystem;

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
	(  "help", "Print this help message, then exit")
	(  "version", "Print version information, then exit")
	("v,verbose", "Enable verbose logging")
	(  "ast", "Generate Abstract Syntax Tree only")
	("o,output", "File name to output to", cxxopts::value<std::string>())
	("h,header", "Generate a header for each input file")
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
		bfs::path file = bfs::absolute(argv[i]);
		bfs::path relative = bfs::relative(file);

		if (clok::VERBOSE) std::cout << "Parsing file: " << relative << std::endl;

		yyin = fopen(argv[i], "r");
		if (yyin == NULL) {
			std::cerr << "clok: Could not open file: " << relative << std::endl;
			return EXIT_FAILURE;
		}

		try {
			programs.push_back(clok::parse());
		} catch (ParseError e) {
			std::cerr << "clok: Parsing error (" << relative << ':' << yylineno << ":" << yycol << ")" << std::endl
			          << "\t" << e.what() << std::endl;
			fclose(yyin);
		}
		fclose(yyin);
	}

	if (options.count("ast")) {
		std::ostream *output;
		bool outputByName = false;

		if (options.count("output")) {
			output = new std::ofstream(options["output"].as<std::string>());
		} else {
			outputByName = true;
		}

		for (int i = 0; i < programs.size(); i++) {
			if (outputByName) {
				delete output;
				output = new std::ofstream(bfs::absolute(argv[i + 1]).filename().string() + ".ast");
			}
			clok::printAST(programs[i], output);
		}

		delete output;
		return EXIT_SUCCESS;
	}

	if (options.count("header")) {
		std::ostream *output;

		if (options.count("output")) {
			output = new std::ofstream(options["output"].as<std::string>());
		} else {
			output = new std::ofstream("");
		}
	}

	return EXIT_SUCCESS;
}
