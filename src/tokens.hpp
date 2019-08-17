//
//  tokens.h
//  Lok
//
//  Created by Maxwell Guppy on 5/7/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#pragma once

enum tokenType {
	NONE = 0, // For use in the AST and action tree
	END,
	UNKNOWN,
	FUNCTION,
	USE,
	RETURN,
	IDENTIFIER,
	TYPE,
	NAMESPACE,
	INTEGER,
	FLOAT,
	STRING,
	OPERATOR_UN,
	OPERATOR_BIN,
	BRACE,
	PARENTHESIS,
	SQUARE,
	PUBLIC
};

