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

#include "yydef.hpp"

// A lower number means a higher precedence
// A higher precedence means it is applied earlier
std::unordered_map<std::string, int> binPrecedence = {
	{"(", 0},
	{":", 1},
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

node_t make(token t)
{
	return std::make_shared<node>(t);
}

node_t make(int i, token t)
{
	node_t n = make(t);
	n->children.reserve(i);
	return n;
}

node_t parseType()
{
	if (clok::VERBOSE) std::cout << "Parsing type" << std::endl;
	if (currentToken.type != TYPE) throw ParseError(std::string("Unexpected token: '") + currentToken.value + "'. Expected type");

	std::string type = currentToken.value;
	token t = {TYPE, type.substr(1, type.length() - 2)};

	return make(0, t);
}

node_t parseNamespace()
{
	if (clok::VERBOSE) std::cout << "Parsing namespace" << std::endl;
	node_t n = make(1, currentToken);
	nextToken();
	n->children.push_back(parseType());
	return n;
}

node_t parseUse()
{
	if (clok::VERBOSE) std::cout << "Parsing use" << std::endl;
	node_t n = make(1, currentToken);
	nextToken();
	n->children.push_back(parseType());
	return n;
}

node_t parseExpression()
{
	if (clok::VERBOSE) std::cout << "Parsing expression" << std::endl;

	node_t n = make(currentToken);
	node_t lastOp = n;
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
		if (currentToken.type == TYPE) {
			postfixStack.push_back(parseType());
		} else {
			postfixStack.push_back(make(1, currentToken));
		}
	}

	for (auto rit = postfixStack.rbegin(); rit < postfixStack.rend(); rit++) {
		if (*n == *item) {
			n = *rit;
		} else {
			lastOp->children.push_back(*rit);
		}
		lastOp = *rit;
	}
	if (*lastOp != *item) lastOp->children.push_back(item);
	int rootPrec = 0;

	while (currentToken.type != SEMICOLON && currentToken.type != CLOS_PARENTHESIS && currentToken.type != CLOS_SQUARE) {
		int checkingPrec;
		try {
			checkingPrec = binPrecedence.at(currentToken.value);
		} catch (std::out_of_range e) {
			throw ParseError(std::string("Unexpected token: '") + currentToken.value + "'. Expected operator");
		}
		node_t child = n;
		node_t parent = n;

		if (rootPrec < checkingPrec) {
			node_t newRoot = make(2, currentToken);

			newRoot->children.push_back(n);

			if (nextToken() == OPEN_PARENTHESIS) {
				nextToken();
				node_t expr = make(1, {OPEN_PARENTHESIS, "("});
				expr->children.push_back(parseExpression());
				newRoot->children.push_back(expr);
				nextToken();
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
					if (currentToken.type == TYPE) {
						postfixStack.push_back(parseType());
					} else {
						postfixStack.push_back(make(1, currentToken));
					}
				}

				for (auto rit = postfixStack.rbegin(); rit < postfixStack.rend(); rit++) {
					lastOp->children.push_back(*rit);
					lastOp = *rit;
				}
				lastOp->children.push_back(item);
			}

			n = newRoot;
			rootPrec = checkingPrec;
		} else {
			int currentPrec = rootPrec;
			while (currentPrec > checkingPrec) {
				parent = child;
				child = child->children[child->children.size() - 1];
				currentPrec = binPrecedence[child->tk.value];
			}

			node_t opNode = make(2, currentToken);

			opNode->children.push_back(child);
			std::replace(parent->children.begin(), parent->children.end(), child, opNode);

			if (nextToken() == OPEN_PARENTHESIS) {
				nextToken();
				node_t expr = make(1, {OPEN_PARENTHESIS, "("});
				expr->children.push_back(parseExpression());
				opNode->children.push_back(expr);
				nextToken();
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
					if (currentToken.type == TYPE) {
						postfixStack.push_back(parseType());
					} else {
						postfixStack.push_back(make(1, currentToken));
					}
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
	if (clok::VERBOSE) std::cout << "Parsing block statement" << std::endl;
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
	if (clok::VERBOSE) std::cout << "Parsing run statement" << std::endl;
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
	if (clok::VERBOSE) std::cout << "Parsing function" << std::endl;
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
		case NAMESPACE:
			root->children.push_back(parseNamespace());
			break;
		case FUNCTION:
			root->children.push_back(parseFunction());
			break;
		case RUN:
			root->children.push_back(parseRun());
			break;

		case SEMICOLON:
			break; // We don't care about trailing semicolons on things
		case UNKNOWN:
			throw ParseError(std::string("Unknown token: ") + yytext);
		default:
			throw ParseError(std::string("Unexpected token: ") + yytext);
		}
	}

	return root;
}

node_t clok::parse()
{
	return parseRoot();
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

void clok::printAST(node_t root)
{
	printNode(root, 0);
}
