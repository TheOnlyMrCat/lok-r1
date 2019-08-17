//
//  loki.hpp
//  Lok
//
//  Created by Maxwell Guppy on 13/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <stdexcept>

class node;
class ParseError : public std::logic_error {
public:
	ParseError(std::string what);
};

namespace clok {
	bool VERBOSE;

	node parse();
}
