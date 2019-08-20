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
std::unordered_map<std::string, int> operators;

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

node parseExpression()
{
	node n = node(0, currentToken);

	while (nextToken() != SEMICOLON) {

	}

	return n;
}

node parseBlock()
{
	if (currentToken.getType() != OPEN_BRACE) {
		throw ParseError("Expected '{' at start of block statement");
	}

	node n = node(0, NONE_TOKEN);

	tokenType tk;
	while ((tk = nextToken()) != CLOS_BRACE) {

	}

	return n;
}

node parseRun()
{
	node n = node(2, currentToken);
	if (nextToken() == INTEGER) {
		n.addChild(node(0, currentToken));
		nextToken();
	}
	n.addChild(parseBlock());
	return n;
}

node parseFunction()
{
	node n = node(3, currentToken);
	if (nextToken() == TYPE) {
		n.addChild(parseType());
		nextToken();
	}

	if (currentToken.getType() == OPEN_SQUARE) {
		nextToken();
		do {
			n.addChild(parseExpression());
		} while (nextToken() != CLOS_SQUARE);
	}

	nextToken();
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
