//
//  trees.hpp
//  Lok
//
//  Created by TheOnlyMrCat on 6/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <vector>
#include <memory>

#include "token.hpp"

struct node;

typedef std::shared_ptr<node> node_t;

struct node {
	explicit node(token t): tk(t) {}

	bool operator==(node& other) {return children == other.children && tk == other.tk;}
	bool operator!=(node& other) {return !operator==(other);}

	std::vector<std::shared_ptr<node>> children;
	token tk;
};

class action_node {
	virtual ~action_node();
};
