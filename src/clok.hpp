//
//  loki.hpp
//  Lok
//
//  Created by Maxwell Guppy on 13/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <stdexcept>
#include <unordered_map>

class node;
class ParseError : public std::logic_error {
public:
	ParseError(std::string what);
};

namespace clok {
	extern bool VERBOSE;
	extern std::unordered_map<std::string, int> operators;

	node parse();
	void printAST(node root);
}
