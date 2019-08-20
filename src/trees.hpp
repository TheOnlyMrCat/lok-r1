//
//  trees.hpp
//  Lok
//
//  Created by Maxwell Guppy on 6/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once


#include <string>
#include <vector>

#include "token.hpp"

class node {
public:
	node();
	node(int children, token t);
	~node();

	void addChild(node node);
	void setChild(int num, node node);

	node getChild(int num);
	int childrenCount();

	std::vector<node>::iterator begin();
	std::vector<node>::iterator end();
private:
	std::vector<node> children;
	token t;
};
