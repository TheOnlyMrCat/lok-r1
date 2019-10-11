//
//  loki.hpp
//  Lok
//
//  Created by TheOnlyMrCat on 13/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <ostream>
#include <stdexcept>

typedef class std::shared_ptr<class node> node_t;
typedef class std::unique_ptr<struct header_node> header_node_t;

class ParseError : public std::logic_error {
public:
	explicit ParseError(std::string what);
};

namespace clok {
	extern bool VERBOSE;

	node_t parse();
	void printAST(node_t root, std::ostream *stream);

	header_node_t header_gen(node_t program);
}
