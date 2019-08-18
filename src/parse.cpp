//
//  parse.cpp
//  Lok
//
//  Created by Maxwell Guppy on 13/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "clok.hpp"
#include "trees.hpp"

#include "yydef.hpp"

token NONE_TOKEN = token(NONE, "");

ParseError::ParseError(std::string what): std::logic_error(what) {}

token currentToken = NONE_TOKEN;
tokenType nextToken()
{
	currentToken = token(static_cast<tokenType>(yylex()), yytext);
	return currentToken.getType();
}

node parseType()
{
	if (currentToken.getType() != TYPE) throw ParseError("Expected type");

	std::string type = currentToken.getValue();
	token t = token(currentToken.getType(), type.substr(1, type.length() - 1));

	return node(0, t);
}

node parseUse()
{
	node n = node(1, currentToken);
	nextToken();
	n.addChild(parseType());
	return n;
}

node parseBody()
{
	node n = node(0, NONE_TOKEN);

	tokenType tk;
	while ((tk = nextToken()) != BRACE);

	return n;
}

node parseRun()
{
	node n = node(2, currentToken);
	if (nextToken() == INTEGER) {
		n.addChild(node(0, currentToken));
		nextToken();
	}
	n.addChild(parseBody());
	return n;
}

node parseRoot()
{
	node root = node(0, NONE_TOKEN);

	tokenType tk;
	while ((tk = nextToken()) != END) {
		switch (tk) {
		case USE:
			root.addChild(parseUse());
			break;
		case UNKNOWN:
			throw ParseError(std::string("Unknown token: ") + yytext);
		default:
			throw ParseError(std::string("Unexpected token: ") + yytext);
		}
	}

	return root;
}

node clok::parse()
{
	return parseRoot();
}
