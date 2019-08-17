//
//  token.cpp
//  Lok
//
//  Created by Maxwell Guppy on 14/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "token.hpp"

token::token(tokenType type, std::string value): t(type), v(value) {}
token::~token() {};

tokenType token::getType()
{
	return t;
}

std::string token::getValue()
{
	return v;
}
