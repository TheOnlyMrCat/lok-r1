//
//  trees.cpp
//  Lok
//
//  Created by Maxwell Guppy on 6/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "trees.hpp"

node::node(): node(0, NONE_TOKEN) {}
node::node(int children, token t) : children(children), t(t) {}
node::~node() {}

void node::addChild(node child)
{
	children.push_back(child);
}

void node::setChild(int num, node node)
{
	children[num] = node;
}

std::vector<node>::iterator node::begin()
{
	return children.begin();
}

std::vector<node>::iterator node::end()
{
	return children.end();
}
