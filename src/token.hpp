//
//  token.hpp
//  Lok
//
//  Created by Maxwell Guppy on 14/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

#include <string>

#include "tokens.h"

struct token {
	tokenType type;
	std::string value;
};
