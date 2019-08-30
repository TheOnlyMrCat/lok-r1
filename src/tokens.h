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
	RUN,
	IDENTIFIER,
	TYPE,
	NAMESPACE,
	INTEGER,
	FLOAT,
	STRING,
	OPEN_BRACE,
	CLOS_BRACE,
	OPEN_PARENTHESIS,
	CLOS_PARENTHESIS,
	OPEN_SQUARE,
	CLOS_SQUARE,
	SEMICOLON,
	WHITESPACE,
	PUBLIC,

	// Operators can be
	OPERATOR = 0b01000000,
	OPERATOR_PRE_UN = 0b01000001,
	OPERATOR_POST_UN = 0b01000010,
	OPERATOR_BIN = 0b01000100,
};
