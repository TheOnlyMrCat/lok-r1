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
#include "trees.hpp"

#include <iostream>
#include <cstdio>

extern "C"
int yywrap() {
	return 1; //TODO?
}

extern FILE *yyin;

int main(int argc, char *argv[]) {
	auto options = cxxopts::Options("clok", "Compiler for the Lok programming language");

	options.add_options()
	("version", "Print version information, then exit")
	("v,verbose", "Enable verbose logging")
	("h,help", "Print this help message, then exit")
	;

	auto result = options.parse(argc, argv);

	if (result.count("version")) {
		std::cout << "Lok compiler (clok) version 0.1 alpha" << std::endl;
	}

	if (result.count("help")) {
		std::cout << options.help() << std::endl;
	}

	VERBOSE = result.count("verbose") > 0;

	std::vector<node> programs = std::vector<node>(argc - 1);

	for (int i = 1; i < argc; i++) {
		if (VERBOSE) std::cout << "Reading file: " << argv[i] << std::endl;
		yyin = fopen(argv[i], "r");
		try {
			programs.push_back(clok::parse());
		} catch (ParseError e) {
			std::cout << "clok: Error on line " << yylineno << ":" << std::endl
			          << "\t" << e.what() << std::endl;
		}
		fclose(yyin);
	}

	return EXIT_SUCCESS;
}
