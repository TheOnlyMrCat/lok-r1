//
//  token.hpp
//  Lok
//
//  Created by TheOnlyMrCat on 14/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <string>

#include "tokens.h"

struct token {
	token(const token &tk): type(tk.type), value(tk.value) {}
	token(tokenType tp, std::string val): type(tp), value(val) {}

	bool operator==(token& other) {return type == other.type && value == other.value;}

	tokenType type;
	std::string value;
};
