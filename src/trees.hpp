//
//  trees.hpp
//  Lok
//
//  Created by Maxwell Guppy on 6/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <vector>
#include <memory>

#include "token.hpp"

class node;

typedef std::shared_ptr<node> node_t;

struct node {
	node(token tk): tk(tk) {}

	bool operator==(node& other) {return children == other.children && tk == other.tk;}
	bool operator!=(node& other) {return !operator==(other);}

	std::vector<node_t> children;
	token tk;
};
