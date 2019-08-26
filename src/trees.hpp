//
//  trees.hpp
//  Lok
//
//  Created by Maxwell Guppy on 6/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <vector>

#include "token.hpp"

class node;

typedef std::shared_ptr<node> node_t;

struct node {
	std::vector<node_t> children;
	token token;
};
