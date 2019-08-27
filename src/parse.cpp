//
//  parse.cpp
//  Lok
//
//  Created by Maxwell Guppy on 13/8/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include <algorithm>
#include <iostream>

#include "clok.hpp"
#include "trees.hpp"

#include "yydef.hpp"

// A lower number means a higher precedence
std::unordered_map<std::string, int> binPrecedence = {
	{":", 0},
	{".", 1},
	{"*", 2},
	{"/", 2},
	{"%", 2},
	{"+", 3},
	{"-", 3},
	{"<<", 4},
	{">>", 4},
	{"<", 5},
	{">", 5},
	{"<=", 5},
	{">=", 5},
	{"==", 6},
	{"!=", 6},
	{"&", 7},
	{"|", 7},
	{"^", 7},
	{"&&", 8},
	{"||", 8},
	{"=", 9}
};

ParseError::ParseError(std::string what): std::logic_error(what) {}

token currentToken = {NONE, "ERROR"};
tokenType nextToken()
{
	tokenType ttp;
	while ((ttp = static_cast<tokenType>(yylex())) == WHITESPACE);
	currentToken = {ttp, yytext};
	return ttp;
}

node_t make(token type)
{
	node_t n = std::make_shared<node>();
	n->tk = type;
	return n;
}

node_t make(int i, token type)
{
	node_t n = make(type);
	n->children.reserve(i);
	return n;
}

node_t parseType()
{
	if (currentToken.type != TYPE) throw ParseError("Expected type");

	std::string type = currentToken.value;
	token t = {currentToken.type, type.substr(1, type.length() - 1)};

	return make(0, t);
}

node_t parseUse()
{
	node_t n = make(1, currentToken);
	nextToken();
	n->children.push_back(parseType());
	return n;
}

node_t parseExpression()
{
	node_t n = make(currentToken);
	int rootPrec = 0;

	nextToken();
	while (currentToken.type != SEMICOLON && currentToken.type != CLOS_PARENTHESIS && currentToken.type != CLOS_SQUARE) {
		int checkingPrec;
		try {
			checkingPrec = binPrecedence.at(currentToken.value);
		} catch (std::out_of_range e) {
			throw ParseError(std::string("Unexpected token: ") + currentToken.value);
		}
		node_t child = n;
		node_t parent = n;

		if (rootPrec < checkingPrec) {
			node_t newRoot = make(2, currentToken);

			newRoot->children.push_back(n);

			if (nextToken() == OPEN_PARENTHESIS) {
				nextToken();
				newRoot->children.push_back(parseExpression());
			} else {
				node_t lastOp = newRoot;
				while (currentToken.type == OPERATOR_PRE_UN || currentToken.value == "+" || currentToken.value == "-") {
					node_t unaryOp = make(1, currentToken);
					lastOp->children.push_back(unaryOp);
					lastOp = unaryOp;
					nextToken();
				}
				node_t item = make(0, currentToken);

				//We need to use a stack because postfixes associate left-to-right
				std::vector<node_t> postfixStack;
				while (nextToken() == OPERATOR_POST_UN || currentToken.type == TYPE) {
					postfixStack.push_back(make(1, currentToken));
				}

				for (auto rit = postfixStack.rbegin(); rit < postfixStack.rend(); rit++) {
					lastOp->children.push_back(*rit);
					lastOp = *rit;
				}
				lastOp->children.push_back(item);
			}

			n = newRoot;
		} else {
			while (rootPrec > checkingPrec) {
				parent = child;
				child = child->children[child->children.size() - 1];
			}

			node_t opNode = make(2, currentToken);

			opNode->children.push_back(child);
			std::replace(parent->children.begin(), parent->children.end(), child, opNode);

			if (nextToken() == OPEN_PARENTHESIS) {
				nextToken();
				opNode->children.push_back(parseExpression());
			} else {
				while (currentToken.type == OPERATOR_PRE_UN || currentToken.value == "+" || currentToken.value == "-") {
					node_t unaryOp = make(1, currentToken);
					opNode->children.push_back(unaryOp);
					opNode = unaryOp;
					nextToken();
				}
				node_t item = make(0, currentToken);

				//We need to use a stack because postfixes associate left-to-right
				std::vector<node_t> postfixStack;
				while (nextToken() == OPERATOR_POST_UN || currentToken.type == TYPE) {
					postfixStack.push_back(make(1, currentToken));
				}

				for (auto rit = postfixStack.rbegin(); rit < postfixStack.rend(); rit++) {
					opNode->children.push_back(*rit);
					opNode = *rit;
				}
				opNode->children.push_back(item);
			}
		}
	}

	return n;
}

node_t parseBlock()
{
	if (currentToken.type != OPEN_BRACE) {
		throw ParseError("Expected '{' at start of block statement");
	}

	node_t n = make({NONE, "block"});

	tokenType tk;
	while ((tk = nextToken()) != CLOS_BRACE) {
		//TODO: Keyword checks
		n->children.push_back(parseExpression());
	}

	return n;
}

node_t parseRun()
{
	node_t n = make(2, currentToken);
	if (nextToken() == INTEGER) {
		n->children.push_back(make(0, currentToken));
		nextToken();
	}
	n->children.push_back(parseBlock());
	return n;
}

node_t parseFunction()
{
	node_t n = make(3, currentToken);
	if (nextToken() == TYPE) {
		n->children.push_back(parseType());
		nextToken();
	}

	if (currentToken.type == OPEN_SQUARE) {
		nextToken();
		do {
			n->children.push_back(parseExpression());
		} while (currentToken.type != CLOS_SQUARE);
		nextToken();
	}

	return n;
}

node_t parseRoot()
{
	node_t root = make({NONE, "root"});

	tokenType tk;
	while ((tk = nextToken()) != END) {
		switch (tk) {
		case USE:
			root->children.push_back(parseUse());
			break;
		case RUN:
			nextToken();
			root->children.push_back(parseBlock());
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
	return *parseRoot();
}

void printNode(node_t n, int depth)
{
	if (depth > 0) {
		for (int i = 1; i < depth; i++) {
			std::cout << " ";
		}
		std::cout << "-";
	}
	std::cout << n->tk.value << std::endl;

	for (auto i = n->children.begin(); i < n->children.end(); i++) {
		printNode(*i, depth + 1);
	}
}

void clok::printAST(node root)
{
	printNode(std::make_shared<node>(root), 0);
}
