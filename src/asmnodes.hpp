//
//  astnode.hpp
//  Lok
//
//  Created by TheOnlyMrCat on 25/9/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <string>
#include <vector>
#include <memory>

struct action_node {
	virtual ~action_node();
};

struct header_node {
	virtual ~header_node();

	virtual std::string c_gen();
};

typedef std::unique_ptr<header_node> header_node_t;

struct namespace_node : header_node {
    std::string type;
	std::vector<header_node_t> sub_elements;

    std::string c_gen() override;
};

struct class_node : header_node {
	std::string type;
	std::vector<header_node_t> public_sub;
	std::vector<header_node_t> protect_sub;
	std::vector<header_node_t> internal_sub;
	std::vector<header_node_t> private_sub;

	std::string c_gen() override;
};

struct fn_node : header_node {
	std::string name;
	std::string return_type;
	
	// First item: type, second item: name
	std::vector<std::pair<std::string, std::string>> parameters;

	std::string c_gen() override;
};