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

int yycol = 1;

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

token currentToken = {NONE, ""};
tokenType nextToken()
{
	yycol += currentToken.value.length();
	tokenType ttp;
	while ((ttp = static_cast<tokenType>(yylex())) == WHITESPACE || ttp == NEWLINE) if (ttp == NEWLINE) {yylineno++; yycol = 1;} else yycol += strlen(yytext);
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

ParseError unexpected(std::string expected)
{
	return ParseError(std::string("Unexpected token: '") + currentToken.value + "'. Expected " + expected);
}

node_t parseType()
{
	if (clok::VERBOSE) std::cout << "Parsing type" << std::endl;
	if (currentToken.type != TYPE) throw unexpected("type");

	std::string type = currentToken.value;

	token t = {NONE, ""};
	int modifierEnd = type.find('<');
	if (modifierEnd == 0) {
		t = {TYPE, type.substr(1, type.length() - 2)};
	} else {
		t = {TYPE, type.substr(0, modifierEnd) + type.substr(modifierEnd + 1, type.length() - 2)};
	}

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
	while (nextToken() == OPERATOR_POST_UN || currentToken.type == TYPE || currentToken.type == OPEN_SQUARE) {
		if (currentToken.type == OPEN_SQUARE) {
			node_t params = make(currentToken);
			while (nextToken() != CLOS_SQUARE) {
				params->children.push_back(parseExpression());
			}
		} else if (currentToken.type == TYPE) {
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

	while (currentToken.type != SEMICOLON && currentToken.type != CLOS_PARENTHESIS) {
		int checkingPrec;
		try {
			checkingPrec = binPrecedence.at(currentToken.value);
		} catch (std::out_of_range e) {
			throw unexpected("operator");
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
				while (nextToken() == OPERATOR_POST_UN || currentToken.type == TYPE || currentToken.type == OPEN_SQUARE) {
					if (currentToken.type == OPEN_SQUARE) {
						node_t params = make(currentToken);
						while (nextToken() != CLOS_SQUARE) {
							params->children.push_back(parseExpression());
						}
					} else if (currentToken.type == TYPE) {
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
				while (nextToken() == OPERATOR_POST_UN || currentToken.type == TYPE || currentToken.type == OPEN_SQUARE) {
					if (currentToken.type == OPEN_SQUARE) {
						node_t params = make(currentToken);
						while (nextToken() != CLOS_SQUARE) {
							params->children.push_back(parseExpression());
						}
					} else if (currentToken.type == TYPE) {
						postfixStack.push_back(parseType());
					} else {
						postfixStack.push_back(make(1, currentToken));
					}
				}

				for (auto rit = postfixStack.rbegin(); rit < postfixStack.rend(); rit++) {
					lastOp->children.push_back(*rit);
					lastOp = *rit;
				}
				opNode->children.push_back(item);
			}
		}
	}

	return n;
}

node_t parseBlock();

node_t parseBlockSegment()
{
	switch (currentToken.type) {
	case RETURN: {
		node_t rt = make(1, currentToken);
		if (nextToken() != SEMICOLON) {
			rt->children.push_back(parseExpression());
		}
		nextToken();

		return rt;
	}
	case IF: {
		node_t fi = make(2, currentToken);

		node_t expr;
		if (nextToken() == OPEN_SQUARE) {
			while (nextToken() != CLOS_SQUARE) {
				expr = parseExpression();
			}
		} else {
			expr = parseExpression();
		}

		if (expr == nullptr) throw ParseError("Expected expression after if");
		fi->children.push_back(expr);

		if (nextToken() == OPEN_BRACE) fi->children.push_back(parseBlock());
		else fi->children.push_back(parseExpression());

		if (nextToken() == ELSE) {
			node_t esle = make(1, currentToken);
			nextToken();
			esle->children.push_back(parseBlockSegment());
			fi->children.push_back(esle);
		}

		return fi;
	}
	case FOR: {
		node_t rof = make(4, currentToken);

		if (nextToken() == OPEN_SQUARE) nextToken();

		rof->children.push_back(parseExpression()); nextToken();
		rof->children.push_back(parseExpression()); nextToken();
		rof->children.push_back(parseExpression());

		if (nextToken() == CLOS_SQUARE) nextToken();

		rof->children.push_back(parseBlockSegment());

		return rof;
	}
	case REPEAT: {
		node_t rpt = make(2, currentToken);

		if (nextToken() == OPEN_SQUARE) nextToken();

		rpt->children.push_back(parseExpression());

		if (nextToken() == CLOS_SQUARE) nextToken();

		rpt->children.push_back(parseBlockSegment());

		return rpt;
	}
	case WHILE: {
		node_t elihw = make(2, currentToken);

		if (nextToken() == OPEN_SQUARE) nextToken();

		elihw->children.push_back(parseExpression());

		if (nextToken() == CLOS_SQUARE) nextToken();

		elihw->children.push_back(parseBlockSegment());

		return elihw;
	}
	case OPEN_BRACE: {
		node_t block = parseBlock();
		nextToken();
		return block;
	}
	default: {
		node_t expr = parseExpression();
		nextToken();
		return expr;
	}
	}
}

node_t parseBlock()
{
	if (clok::VERBOSE) std::cout << "Parsing block statement" << std::endl;
	if (currentToken.type != OPEN_BRACE) {
		throw ParseError("Expected '{' at start of block statement");
	}

	node_t n = make({NONE, "block"});

	nextToken();

	while (currentToken.type != CLOS_BRACE) {
		n->children.push_back(parseBlockSegment());
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
	node_t n = make(4, currentToken);

	if (nextToken() != IDENTIFIER) throw ParseError("Expected identifier after fn");

	n->children.push_back(make(currentToken));

	if (nextToken() == TYPE) {
		n->children.push_back(parseType());
		nextToken();
	}

	if (currentToken.type == OPEN_SQUARE) {
		nextToken();
		do {
			n->children.push_back(parseExpression());
			nextToken();
		} while (currentToken.type != CLOS_SQUARE);
		nextToken();
	}

	while (currentToken.type == FUNCTION_MOD || currentToken.type == ACCESS_MOD) {
		n->children.push_back(make(0, currentToken));
		nextToken();
	}

	n->children.push_back(parseBlock());

	return n;
}

node_t parseDeclaration()
{
	if (clok::VERBOSE) std::cout << "Parsing declaration" << std::endl;
	node_t root = make(3, currentToken);

	nextToken();
	root->children.push_back(parseType());

	while (nextToken() == ACCESS_MOD) root->children.push_back(make(currentToken));

	if (currentToken.value != "=") throw unexpected("=");

	nextToken();
	root->children.push_back(parseExpression());

	return root;
}

node_t parseClass()
{
	if (clok::VERBOSE) std::cout << "Parsing class/struct" << std::endl;
	node_t root = make(3, currentToken);

	nextToken();
	root->children.push_back(parseType());

	if (nextToken() == ACCESS_MOD) {
		root->children.push_back(make(currentToken));
		nextToken();
	} else {
		root->children.push_back(make(token(ACCESS_MOD, "public")));
	}

	if (currentToken.type == EXTENDS) {
		root->children.push_back(make(currentToken));
		root->children.push_back(parseType());

		token superAccess = token(ACCESS_MOD, "public");
		if (nextToken() == ACCESS_MOD) superAccess = currentToken;

		root->children.push_back(make(superAccess));
	}

	if (currentToken.type != OPEN_BRACE) throw unexpected("brace");

	while (nextToken() != CLOS_BRACE) {
		if (currentToken.type == FUNCTION) root->children.push_back(parseFunction());
		else if (currentToken.type == IDENTIFIER) root->children.push_back(parseDeclaration());
		else throw ParseError(std::string("Unexpected token: ") + currentToken.value);
	}

	return root;
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
		case CLASS:
		case STRUCT:
			root->children.push_back(parseClass());
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

void printNode(node_t n, int depth, std::ostream *stream)
{
	if (depth > 0) {
		for (int i = 1; i < depth; i++) {
			*stream << " ";
		}
		*stream << "-";
	}
	*stream << n->tk.value << std::endl;

	for (auto i = n->children.begin(); i < n->children.end(); i++) {
		printNode(*i, depth + 1, stream);
	}
}

void clok::printAST(node_t root, std::ostream *stream)
{
	printNode(root, 0, stream);
}
