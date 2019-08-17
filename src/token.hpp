//
//  token.hpp
//  Lok
//
//  Created by Maxwell Guppy on 14/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <string>

#include "tokens.hpp"

class token {
public:
	token(tokenType type, std::string value);
	~token();
	
	tokenType getType();
	std::string getValue();
private:
	tokenType t;
	std::string v;
};

extern token NONE_TOKEN;
