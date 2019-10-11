//
//  astnode.hpp
//  Lok
//
//  Created by TheOnlyMrCat on 26/9/19.
//  Copyright © 2019 Dockdev. All rights reserved.
//

#include "asmnodes.hpp"

header_node::~header_node() {}
std::string header_node::c_gen() {return "";}

std::string namespace_node::c_gen()
{
     std::string buffer = "namespace ";
     buffer += type;
     buffer += "{";
     for (auto &&i : sub_elements) {
          buffer += i->c_gen();
     }
     buffer += "}";
     return buffer;
}

std::string class_node::c_gen()
{
     std::string buffer = "class ";
     buffer += type;
     buffer += "{";

     if (public_sub.size() > 0 | internal_sub.size() > 0) {
          buffer += "public:";
          for (auto sub = public_sub.begin(); sub != public_sub.end(); sub++) {
               buffer += (*sub)->c_gen();
          }
          for (auto sub = internal_sub.begin(); sub != internal_sub.end(); sub++) {
               buffer += (*sub)->c_gen();
          }
     }

     if (protect_sub.size() > 0) {
          buffer += "protected:";
          for (auto sub = protect_sub.begin(); sub != protect_sub.end(); sub++) {
               buffer += (*sub)->c_gen();
          }
     }

     if (private_sub.size() > 0) {
          buffer += "private:";
          for (auto sub = private_sub.begin(); sub != private_sub.end(); sub++) {
               buffer += (*sub)->c_gen();
          }
     }

     buffer += "};";
     return buffer;
}

std::string fn_node::c_gen()
{
     std::string buffer = return_type;
     buffer += " ";
     buffer += name;
     buffer += "(";
     for (auto param = parameters.begin(); param != parameters.end(); param++) {
          buffer += (*param).first;
          buffer += " ";
          buffer += (*param).second;
     }
     buffer += ");";
     return buffer;
}