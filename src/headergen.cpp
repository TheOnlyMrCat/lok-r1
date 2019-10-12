//
//  parse.cpp
//  Lok
//
//  Created by TheOnlyMrCat on 24/9/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "asmnodes.hpp"
#include "astnode.hpp"
#include "clok.hpp"

#include <memory>

std::vector<header_node_t> clok::header_gen(node_t program)
{
    header_node_t currentNamespace = nullptr;

    for (node_t node : program->children) {
        switch (node->tk.type) {
        case NAMESPACE: {
            std::string type = node->children[0]->tk.value;
            currentNamespace = std::unique_ptr<header_node>(new namespace_node(type));
            break;
        }
        case CLASS: {
            class_node cls(node->children[0]->tk.value, node->children[1]->tk.value);
            
        }
        }
    }
}
