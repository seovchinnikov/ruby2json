/*
 * Ruby AST to JSON
 * Copyright (C) 2014 Sergei Ovchinnikov <rubasketcom@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <cstddef>
#include <iostream>
#include <sstream>
#include <fstream>
#include "rubyastvisitor.h"

/*
 * Note that each visitor method has a comment that explains how the AST
 * is structured for the specific statement. Only the following attributes
 * will be considered: l, r, cond and ensure. If any of these pointers are not
 * specified, it means that its value is just NULL.
 */


namespace Ruby
{

RubyAstVisitor::RubyAstVisitor(Node *tree_head, struct options_t *opts)
{
    /* There's nothing to do here */
	head = tree_head;
	options = opts;
	std::cout<<"constructor"<<std::endl;
}

RubyAstVisitor::~RubyAstVisitor()
{
    /* There's nothing to do here */
}

void RubyAstVisitor::visitCode()
{
    //debug() << "Visiting Code...";
	file.open (options->to);
    std::cout<<"visiting"<<std::endl;
	
	if(head->next!=NULL){
		visitStatements(head);
	}
	else{
		visitNode(head);
	}
	
	file.close();

}



void RubyAstVisitor::visitName(Node *node)
{
	
	switch(node->flags){
		case var: case ivar: case cvar: case constant: case global:
			
		file << "{ \"type\":\"variable-reference\",";
		file << "\"variable-name\": \"" << node->name << "\" ,";
		file << "\"variable-type\":";
		switch(node->flags){
			case var: file << " \"local_var\" "; break;
			case ivar: file << " \"instance_var\" "; break;
			case cvar: file << " \"class_var\" "; break;
			case constant: file << " \"constant\" "; break;
			case global: file << " \"global_var\" "; break;
			case kwrest: file << " \"rest_splat_arg\" "; break;
			case star: file << " \"just_a_star_arg\" "; break;
			case label: file << " \"label-arg\" "; break;
			case opt: file << " \"default_value\" "; break;
			case block: file << " \"block_argument\" "; break;
		}
		file << "}";
		break;
		
		//default: std::cout<<"name"<<node->name<<node->flags<<std::endl;
	}
	
}

void RubyAstVisitor::visitNumeric(Node *node)
{
    
	std::cout<<"numeric"<<node->flags<<std::endl;
	file << "{\"type\":\"literal-constant\",\"constant-type\":";
	switch(node->flags){
		case int_l: file << " \"integer\" "; break;
		case float_l: file << " \"float\" "; break;
		case rational_l: file << " \"rational\" "; break;
		case imaginary_l: file << " \"imaginary\" "; break;
	}
	file << ",\"constant-value\":\"";
	file << escapeJsonString(node->value) ;
	file << "\"}";
}

void RubyAstVisitor::visitRegexp(Node *node)
{
   std::cout<<"regexp"<<std::endl;
   file << "{\"type\": \"regular-expression\"}";
}

void RubyAstVisitor::visitString(Node *node)
{
    /*
     * l -> list of variables contained inside the string (by using #{var}).
     */
	std::cout<<"string"<<std::endl;
	if(node->l!=NULL){
		file << "{\"type\":\"string-statement\",\"value\":";
		visitStatements(node->l);
		file << "}";
	}
	else{
		file << "{\"type\":\"string-statement\",\"value\":\"";
		file << "\"}";
	
	}
    
}

void RubyAstVisitor::visitBareString(Node *node)
{
    /*
     * l -> list of variables contained inside the string (by using #{var}).
     */
	std::cout<<"constSTRING"<<std::endl;
	file << "{\"type\":\"literal-constant\",\"constant-type\":\"string\",\"constant-value\":\"";
	file << escapeJsonString(node->value) ;
	file << "\"}";
    
}

void RubyAstVisitor::visitSymbol(Node *node)
{
    std::cout<<"symbol"<<node->name<<std::endl;
	file << "{\"type\":\"literal-constant\",\"constant-type\":\"symbol\",\"constant-value\":\"";
	file << escapeJsonString(node->name) ;
	file << "\"}";
}

void RubyAstVisitor::visitBody(Node *node)
{
    /*
     * l -> list of inner statements.
     * r -> optional rescue statement.
     * cond -> optional else statement.
     * ensure -> optional ensure statement.
     */
	std::cout<<"body"<<std::endl;
    if (!node)
        return;
	
	file << "{\"type\":\"body-block\",";
	file << "\"block\":";
	visitStatements(node->l);
	if(node->r!= NULL){
		file << ", \"rescue\":";
		visitNode(node->r);
	}
	if(node->r!= NULL){
		file << ", \"else\":";
		visitNode(node->cond->l);
	}
	if(node->r!= NULL){
		file << ", \"ensure\":";
		visitNode(node->ensure);
	}
	file << "}";

}

void RubyAstVisitor::visitBinary(Node *node)
{
    /*
     * l -> left operator.
     * r -> right operator.
     */
	std::cout<<"binary"<<std::endl;
	switch(node->kind){
		case token_minus: case token_mul: case token_div: case token_mod: case token_pow:
			file << "{ \"type\":\"";
			switch(node->kind){
				case token_minus:
					file << "difference"; break;
				case token_mul:
					file << "product"; break;
				case token_div:
					file << "quotient"; break;
				case token_mod:
					file << "modulo"; break;
				case token_pow:
					file << "power"; break;
			
			}
			file << "\",";
			file <<"\"left\":";
			visitNode(node->l);
			file<<",";
			file <<"\"right\":";
			visitNode(node->r);
			file<<"}";
			break;
	
		case token_rshift:
			file << "{\"type\":\"arithmetic-bitwise-shift-right\",\"result-type\":\"int\",";
			file << "\"amount\":";
			visitNode(node->r);
			file << ", \"argument\":";
			visitNode(node->l);
			file << "}";
			break;
			
		case token_plus:
			if(node->l->kind == token_string || node->r->kind == token_string){
				file << "{\"type\":\"string-concatenation\",\"result-type\":\"string\",";
				file << "\"left\":";
				visitNode(node->l);
				file << ", \"right\":";
				visitNode(node->r);
				file << "}";
			}
			else{
				file << "{\"type\":\"sum\",";
				file << "\"left\":";
				visitNode(node->l);
				file << ", \"right\":";
				visitNode(node->r);
				file << "}";
			
			}
			break;
			
		case token_lshift:
			if(node->l->kind == token_string || node->r->kind == token_string){
				file << "{\"type\":\"string-concatenation\",\"result-type\":\"string\",";
				file << "\"left\":";
				visitNode(node->l);
				file << ", \"right\":";
				visitNode(node->r);
				file << "}";
			}
			else{
				file << "{\"type\":\"bitwise-shift-left\",\"result-type\":\"int\",";
				file << "\"amount\":";
				visitNode(node->r);
				file << ", \"argument\":";
				visitNode(node->r);
				file << "}";
			}
			break;
			
		default:
			file << "{\"type\":\"binary-operator\",";
			file << "\"left\":";
			visitNode(node->l);
			file << ", \"right\":";
			visitNode(node->r);
			file << "}";
	}
		//file << "power"; break;
    //visitNode(node->l);
    //visitNode(node->r);
}


void RubyAstVisitor::visitBitwise(Node *node)
{
	file << "{\"type\":\"bitwise-";
	switch(node->kind){
		case token_bit_and: file << "and"; break;
		case token_bit_or: file << "or"; break;
		case token_bit_xor: file << "xor"; break;
	}
	
	file << "\", \"result-type\":\"int\", ";
	file << "\"left\":";
	visitNode(node->r);
	file << ", \"right\":";
	visitNode(node->r);
	file << "}";
	
}
void RubyAstVisitor::visitBoolean(Node *node)
{
    /* Same as for the visitBinary method  */
	std::cout<<"boolean"<<std::endl;
	if(node->kind == token_kw_not || node->kind == token_not){
	
		file << "{\"type\":\"logical-not\", \"result-type\":\"boolean\", ";
		file << "\"argument\":";
		visitNode(node->l);
		file << "}";
	}
	else{
			file << "{\"type\":\"";
			switch(node->kind){
				case token_kw_and: case token_and:
					file << "logical-and"; break;
				case token_kw_or: case token_or:
					file << "logical-or"; break;
				case token_cmp:
					file << "compare"; break;
				case token_eq:
					file << "equals"; break;
				case token_eqq:
					file << "equals-with-type"; break;
				case token_match:
					file << "matches"; break;
				case token_nmatch:
					file << "not_match"; break;
				case token_greater:
					file << "greater"; break;
				case token_geq:
					file << "greater-or-equal"; break;
				case token_lesser:
					file << "less"; break;
				case token_leq:
					file << "less-or-equal"; break;
				case token_neq:
					file << "not-equal"; break;
				default: 
					file << "unknown";
			}
			file << "\", \"result-type\":\"boolean\", ";
			file << "\"left\":";
			visitNode(node->l);
			file << ", \"right\":";
			visitNode(node->r);
			file << "}";
			
	}

}

void RubyAstVisitor::visitRange(Node *node)
{
    /* Same as for the visitBinary method  */
	std::cout<<"range"<<std::endl;
	file << "{\"type\":\"range\", ";
	file << "\"from\":";
	visitNode(node->l);
	file << ", \"to\":";
	visitNode(node->r);
	file << "}";
}

void RubyAstVisitor::visitUnary(Node *node)
{
    /*
     * l -> the operator.
     */
    
	file << "{\"type\":\"";
	switch(node->kind){
		case token_unary_plus:
			file << "unary-plus"; break;
		case token_unary_minus:
			file << "unary-minus"; break;
		case token_neg:
			file << "flip-bits"; break;
	}
	file << "\", ";
	file << "\"argument\":";
	visitNode(node->l);
	file << "}";
	std::cout<<"unary"<<std::endl;
}

void RubyAstVisitor::visitArray(Node *node)
{
    /*
     * l -> list of statements (the items of the array).
     */
	std::cout<<"array"<<std::endl;
	file << "{\"type\":\"array\", \"values\":";
    visitArrayStatements(node->l);
	file << "}";
}

void RubyAstVisitor::visitArrayValue(Node *node)
{
    /*
     * l -> the node containing the Array object.
     * r -> the index expression.
     */
	file << "{\"type\":\"array-reference\", \"index\":";
    visitStatements(node->r);
	file << ", \"array-name\": ";
	visitNode(node->l);
	file << "}";
	std::cout<<"arrayValue"<<std::endl;

}

void RubyAstVisitor::visitHash(Node *node)
{
    /*
     * l -> list of hash items.
     */
	std::cout<<"hash"<<std::endl;
	file << "{\"type\":\"hash\", \"values\": [";
    for (Node *copy = node->l; copy != NULL; copy = copy->next){
        visitBinary(copy);
		if(copy->next!=NULL)
			file << ",";
	}
	file << " ] }";
}

void RubyAstVisitor::visitReturnStatement(Node *node)
{
    /*
     * l -> the return expression.
     */
	std::cout<<"return"<<std::endl;
	file << "{\"type\":\"return-statement\"";
	if(node->l!=NULL){
		file << ", \"expression\": ";
		visitNode(node->l);
	}
	file << " }";
}

void RubyAstVisitor::visitUndefStatement(Node *node)
{
    /*
     * l -> list of undef items.
     */
	std::cout<<"undef"<<std::endl;
	file << "{\"type\":\"undef-statement\", \"methods\": ";
	visitStatements(node->r);
    //visitNode(node->l);
	file << " }";
  
}

void RubyAstVisitor::visitAliasStatement(Node *node)
{
    /* Same as for the visitBinary method. */
	std::cout<<"alias"<<std::endl;
	file << "{\"type\":\"func-alias\",";
	file << "\"new-name\":";
	visitNode(node->l);
	file << ", \"old-name\":";
	visitNode(node->r);
	file << "}";

}

void RubyAstVisitor::visitYieldStatement(Node *node)
{
    /*
     * l -> the yield expression.
     */
	std::cout<<"yield"<<std::endl;
    file << "{\"type\":\"yield-statement\", \"expression\": ";
    visitNode(node->l);
	file << " }";
}

void RubyAstVisitor::visitAssignmentStatement(Node *node)
{
    /*
     * l -> the left side of the assignment.
     * r -> the right side of the assignment.
     */
	std::cout<<"assign"<<std::endl;
	if(node->l->next){
		file << "{ \"type\":\"multiple-assignment\",";
		file << "\"variables\":";
		visitStatements(node->l);
		file << ", ";
		file << "\"expression\":";
		visitStatements(node->r);
		file << "}";
	
	} else{
		file << "{ \"type\":\"assignment\"";
		if(node->l!=NULL){
			file << ",\"variable\":";
			visitStatements(node->l);
		}
		file << ", ";
		file << "\"expression\":";
		visitStatements(node->r);
		file << "}";
	}
}

void RubyAstVisitor::visitIfStatement(Node *node)
{
    /*
     * l -> list of inner statements.
     * r -> the "tail": optional list of elsif's and an optional else.
     * cond -> the condition expression.
     */
	std::cout<<"if"<<std::endl;
	file << "{ \"type\":\"conditional-statement\",";
	file << "\"condition\":";
	visitNode(node->cond);
	file << ", ";
	file << "\"if-true\":";
	visitStatements(node->l);
	if(node->r!=NULL){
		file << ", ";
		file << "\"if-false\":";
		visitIfTail(node->r);
	}
	file << "}";
}

void RubyAstVisitor::visitCaseStatement(Node *node)
{
    /*
     * l -> the case body: list of when statements and an optional else
     * statement at the end.
     * cond -> the condition expression.
     */
	std::cout<<"case"<<std::endl;
	
	
	file << "{ \"type\":\"choose-statement\",";
	file << "\"expression-to-analyze\":";
	visitNode(node->cond);
	file << ", ";
	file << "\"variants\": [";
	visitWhenStatements(node->l);
	file << "]}";
   
}

void RubyAstVisitor::visitBeginStatement(Node *node)
{
    /*
     * l -> the body of the begin statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     */
	
	std::cout<<"begin"<<std::endl;
	file << "{ \"type\":\"begin-block\",";
	file << "\"block\":";
	visitBody(node->l);
	file << "}";
   
}

void RubyAstVisitor::visitUpBeginEndStatement(Node *node)
{
    /*
     * l -> list of inner statements.
     */
	std::cout<<"visitUpBeginEndStatement"<<std::endl;
	file << "{ \"type\":\"begin-end-block\",";
	file << "\"body-block\":";
	visitStatements(node->l);
	file << "}";
   
}

void RubyAstVisitor::visitWhileStatement(Node *node)
{
    /*
     * l -> list of inner statements.
     * cond -> the condition expression.
     */
	std::cout<<"while"<<std::endl;
	file << "{ \"type\":\"while-statement\",";
	file << "\"condition\":";
	visitNode(node->cond);
	file << ", ";
	file << "\"block\": ";
	visitStatements(node->l);
	file << "}";
    
}

void RubyAstVisitor::visitForStatement(Node *node)
{
    /*
     * l -> list of inner statements.
     * r -> list of variables declared before the "in" keyword.
     * cond -> the expression after the "in" keyword.
     */
	std::cout<<"for"<<std::endl;
	
	file << "{ \"type\":\"for-statement\",";
	file << "\"block\":";
	visitStatements(node->l);
	file << ", ";
	file << "\"before-in\": ";
	visitStatements(node->r);
	file << ", ";
	file << "\"after-in\": ";
	visitNode(node->cond);
	file << "}";
	
}

void RubyAstVisitor::visitMethodStatement(Node *node)
{
    /*
     * l -> list of inner statements.
     * r -> list of method arguments.
     */

    if (!node)
        return;
	std::cout<<"method"<<std::endl;
	file << "{ \"type\":\"method-statement\",";
	file << "\"inner-code\":";
	visitBody(node->l);
	file << ", ";
	file << "\"arguments\": [";
	visitMethodArguments(node->r);
	file << "] ";
	if(node->cond!=NULL && node->cond->name!=NULL){
		file << ", ";
		file << "\"name\": ";
		file<< "\""<<node->cond->name<<"\"";
	}
	file << "}";
 
}

void RubyAstVisitor::visitMethodArguments(Node *node)
{
    /* Just iterate over the "next" pointer. */
	std::cout<<"Methodparams"<<std::endl;
    for (Node *n = node; n != NULL; n = n->next) {
        visitParameter(n);
		if(n->next!=NULL){
			file << ",";
		}
    }

}

void RubyAstVisitor::visitParameter(Node *node)
{
   std::cout<<"parameter"<<std::endl;
   visitNode(node);
}

void RubyAstVisitor::visitClassStatement(Node *node)
{
    /*
     * l -> the body of the class statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     * r -> the name of the class.
     * cond -> the superclass node.
     */
	//std::cout<<node->name;
	
	file << "{\"type\":\"struct-type\" ";
	
	if(node->l->l != NULL){
		file << ", \"body\" : "<<std::endl;
		visitBody(node->l);
	}
	
	if(node->cond != NULL){
		file << ", \"base-structs:\": \"";
		//visitNode(node->cond);
		file<<node->cond->name;
		file << "\" ";
	}
	
	if(node->r != NULL){
		file << ", \"name\": \"";
		visitClassName(node->r);
		file << "\" ";
	}
	
	std::cout<<"classSt"<<std::endl;
	file << "}" << std::endl;
}

void RubyAstVisitor::visitSingletonClass(Node *node)
{
    /*
     * l -> the body of the class statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     * r -> the expression after the lshift ("<<") operator.
     */
	 
	file << "{ \"type\":\"singletone-class\",";
	file << "\"body\":";
	visitBody(node->l);
	file << ", ";
	file << "\"lshift\": ";
	visitNode(node->r);
	file << "}";
	std::cout<<"singleton"<<std::endl;
}

void RubyAstVisitor::visitModuleStatement(Node *node)
{
    /*
     * l -> the body of the class statement. Note that this body is not just
     * a list of "regular" statement, check the visitBody method for more info.
     * r -> the name of the module.
     */
	std::cout<<"module"<<std::endl;
	file << "{ \"type\":\"module-type\",";
	file << "\"body\":";
	visitBody(node->l);
	if(node->r!=NULL){
		file << ", ";
		file << "\"name\": \"";
		visitClassName(node->r);
		file << "\"";
	}
	file << "}";
   
}

void RubyAstVisitor::visitMethodCall(Node *node)
{
    /*
     * l -> the caller (note that it's a list, i.e. A::B::foo).
     * r -> the arguments for this method call.
     * cond -> an optional Ruby block.
     */

    /*
     * Note that the l pointer can contain again another method call.
     * This happens for example here:
     *  Class.new { def foo(a, b); end }.new.foo(1, 2)
     * In order to get everything straight, all builders end up
     * re-implementing this method. This is why this method has no
     * implementation by default.
     */
	Node *aux;
	std::cout<<"methodcall"<<std::endl;
	file << "{\"type\":\"function-call\"";
	
	if(node->l->next == NULL ){
		if(node->l->name!=NULL){
			file << ",\"function-name\":\"";
			file << node->l->name;
			file << "\"";
		}
	}
	else{
		file << ",\"function-call-tree\":[";
		for (aux = node->l; aux->next != NULL; aux = aux->next) {
			visitNode(aux);
			if(aux->next->next !=NULL)
				file << ",";
		}
		file << "]";
		if(aux->name!=NULL){
			file << ",\"name\":\"";
			file << aux->name;
			file << "\"";
		}
	}
	//file << node->l->name;
	if(node->r!=NULL){
		file << ", \"arguments\":";
		visitStatements(node->r);
	}
	file << "}";
	//visitNode(node->r);
}

void RubyAstVisitor::visitSuper(Node *node)
{
    /*
     * l -> the arguments passed to the super call.
     */

	std::cout<<"supercall"<<std::endl;
	file << "{ \"type\":\"super-call\",";
	file << "\"arguments\": [";
    for (Node *aux = node->l; aux != NULL; aux = aux->next) {
        visitNode(aux);
		if(aux->next!=NULL){
			file << ",";
		}
    }
	file << "] ";
	file << "}";

}

void RubyAstVisitor::visitLambda(Node *node)
{
    /*
     * cond -> the block of this lambda expression.
     */
	std::cout<<"lambda"<<std::endl;
    visitBlock(node->cond);
}

void RubyAstVisitor::visitBlock(Node *node)
{
    /*
     * l -> list of inner statements.
     * r -> list of block variables.
     */

    if (!node)
        return;
	
	std::cout<<"block"<<std::endl;
	file << "{ \"type\":\"block\",";
	file << "\"block-vars\": [";
    visitBlockVariables(node->r);
	file << "] ";
	file << ", \"body\": ";
    visitStatements(node->l);
	file << "}";
}

void RubyAstVisitor::visitBlockVariables(Node *node)
{
    /*
     * Just iterate over the next pointer.
     */
	std::cout<<"blockvars"<<std::endl;
    for (Node *n = node; n != NULL; n = n->next) {
        visitNode(n);
		if(n->next!=NULL){
			file << ",";
		}
    }
}

void RubyAstVisitor::visitRequire(Node *node, bool relative)
{
    std::cout<<"require"<<std::endl;
	file << "{\"type\":\"require-directive\",";
	file << "\"relative\":";
	file << relative;
	file << ", \"params\":";
	visitNode(node->r);
	file << "}";
}

void RubyAstVisitor::visitMixin(Node *node, bool include)
{
	std::cout<<"mixin"<<std::endl;
	if(include)
		file << "{\"type\":\"include-directive\",";
	else
		file << "{\"type\":\"extend-directive\",";

	file << "\"params\":";
	visitNode(node->r);
	file << "}";
}

void RubyAstVisitor::visitDefined(Node *node)
{
    /*
     * l -> the expression from the "defined" statement.
     */

	file << "{\"type\":\"ifdefined\",";
	file << "\"expression\":";
	visitNode(node->l);
	file << "}";

}

void RubyAstVisitor::visitTrue(Node *node)
{
	file << "{\"type\":\"true\"}";
}

void RubyAstVisitor::visitFalse(Node *node)
{
	file << "{\"type\":\"false\"}";
}

void RubyAstVisitor::visitNil(Node *node)
{
	file << "{\"type\":\"nil\"}";
}

void RubyAstVisitor::visitFile(Node *node)
{
	file << "{\"type\":\"this_file\"}";
}

void RubyAstVisitor::visitLine(Node *node)
{
	file << "{\"type\":\"this_line\"}";
}

void RubyAstVisitor::visitEncoding(Node *node)
{
	file << "{\"type\":\"this_encoding\"}";
}

void RubyAstVisitor::visitSelf(Node *node)
{
	file << "{\"type\":\"self\"}";
}

void RubyAstVisitor::visitAccessSpecifier(short int policy)
{
	switch(policy){
		case 0:
		file << "{\"type\":\"public-access\"}"; break;
		case 1:
		file << "{\"type\":\"protected-access\"}"; break;
		case 2:
		file << "{\"type\":\"private-access\"}"; break;
	}
}

void RubyAstVisitor::visitClassName(Node *node)
{
	file<<node->name;
}

void RubyAstVisitor::visitRescue(Node *node)
{
    /*
     * l -> rescue arg.
     * r -> list of inner statement.
     */
	
	file << "{\"type\":\"rescue\",";
	file << "\"arg\":";
	visitNode(node->l);
	file << ", \"inner\":";
	visitStatements(node->r);
	file << "}";
  

}

void RubyAstVisitor::visitRescueArg(Node *node)
{
    /*
     * l -> Left part of the rescue argument, could be a list.
     * r -> Right part of the rescue argument, only the DeclarationBuilder
     * wants to access this part.
     */

	file << "{\"type\":\"rescue-arg\",";
	file << "\"left\":";
	visitStatements(node->l);
	file << ", \"right\":";
	visitNode(node->r);
	file << "}";
  
}

void RubyAstVisitor::visitEnsure(Node *node)
{
    /*
     * l -> The inner statements.
     */
    
	file << "{\"type\":\"ensure\",";
	file << " \"inner\":";
	visitStatements(node->l);
	file << "}";


}

void RubyAstVisitor::visitNode(Node *node)
{
	//std::cout<<"fail"<<node->kind<<std::endl;
    char* name;
	//std::cout<<node->kind<<"|";
    /* This is not a valid node */
    if (!node || node->kind == token_invalid)
        return;
    switch (node->kind) {
        case token_return: visitReturnStatement(node); break;
        case token_yield: visitYieldStatement(node); break;
        case token_alias: visitAliasStatement(node); break;
        case token_undef: visitUndefStatement(node); break;
        case token_if: case token_unless: case token_ternary:
          visitIfStatement(node);
          break;
        case token_begin: visitBeginStatement(node); break;
        case token_up_begin:
        case token_up_end: visitUpBeginEndStatement(node); break;
        case token_case: visitCaseStatement(node); break;
        case token_while: case token_until: visitWhileStatement(node); break;
        case token_for: visitForStatement(node); break;
        case token_class: visitClassStatement(node); break;
        case token_singleton_class: visitSingletonClass(node); break;
        case token_module: visitModuleStatement(node); break;
        case token_function: visitMethodStatement(node); break;
        case token_super: visitSuper(node); break;
        case token_method_call: checkMethodCall(node); break;
        case token_assign:
        case token_op_assign: visitAssignmentStatement(node); break;
        case token_object:
            name = node->name;
			std::cout<<name<<std::endl;
            if (name == "public")
                visitAccessSpecifier(0);
            else if (name == "protected")
                visitAccessSpecifier(1);
            else if (name == "private")
                visitAccessSpecifier(2);
            else
                visitName(node);
            break;
        case token_hash: visitHash(node); break;
        case token_array: visitArray(node); break;
        case token_array_value: visitArrayValue(node); break;
        case token_defined: visitDefined(node); break;
        case token_unary_plus: case token_unary_minus: case token_neg:
            visitUnary(node);
            break;
        case token_plus: case token_minus: case token_mul: case token_div:
        case token_mod: case token_lshift: case token_rshift: case token_pow:
            visitBinary(node);
            break;
        case token_dot2: case token_dot3:
            visitRange(node);
            break;
        case token_cmp: case token_eq: case token_eqq: case token_match:
        case token_nmatch: case token_greater: case  token_geq:
        case token_lesser:  case token_leq:
        case token_or: case token_and: case token_kw_and: case token_not:
        case token_kw_not: case token_kw_or: case token_neq:
            visitBoolean(node);
            break;
		case token_bit_or: case token_bit_and: token_bit_xor: visitBitwise(node); break;
        case token_numeric: visitNumeric(node); break;
        case token_string: case token_heredoc: visitString(node); break;
		case token_bare_string: visitBareString(node); break;
        case token_regexp: visitRegexp(node); break;
        case token_nil: visitNil(node); break;
        case token_true: visitTrue(node); break;
        case token_false: visitFalse(node); break;
        case token_line: visitLine(node); break;
        case token_file: visitFile(node); break;
        case token_encoding: visitEncoding(node); break;
        case token_self: visitSelf(node); break;
        case token_symbol: case token_key: visitSymbol(node); break;
        case token_rescue: visitRescue(node); break;
        case token_rescue_arg: visitRescueArg(node); break;
        case token_ensure: visitEnsure(node); break;
        case token_break: case token__end__: case token_next:
        case token_redo: case token_retry:
            return;
    }
}

void RubyAstVisitor::visitStatements(Node *list)
{	
	if(list==NULL){
		file<<"{ \"type\":\"empty-statements-block\"}"<<std::endl;
		return;
	}
		
	if (list->next==NULL){
		visitNode(list);
		return;
	}
		
	file<<"{ \"type\":\"compound-statement\", \"statements\": [ "<<std::endl;
    for (Node *n = list; n; n = n->next) {
        visitNode(n);
		if(n->next!=NULL)
			file<<", ";
    }
	file<<std::endl<<" ]}";

}


void RubyAstVisitor::visitArrayStatements(Node *list)
{	
		
	file<<"{ ";
	int i=0;
    for (Node *n = list; n; n = n->next) {
		file<<"\""<<i<<"\""<<":";
        visitNode(n);
		if(n->next!=NULL)
			file<<", ";
		i++;
    }
	file<<std::endl<<" }";

}

void RubyAstVisitor::visitIfTail(Node *tail)
{
    if (tail == NULL)
        return;

    /* Check if this is an elsif or an else statement */
    if (tail->cond == NULL) {
        visitStatements(tail->l);
    } else
        visitIfStatement(tail);
  
}

void RubyAstVisitor::visitWhenStatements(Node *list)
{

    if (!list)
        return;
	std::cout<<"whenCase"<<std::endl;
    /* Check whether this is a when or an else statement */
    if (list->kind == token_when) {
		file << "{ \"type\":\"choose-variant\",";
		file << "\"condition-label\":";
		visitNode(list->cond); 	// expr
		file << ", ";
		file << "\"variant-body\":";
		visitStatements(list->l); 	// stmt
		file << "}";
		if(list->r){
			file << ",";
		}
        visitWhenStatements(list->r);	// next when stmt section 
    } else {
		std::cout<<"whenelseCase"<<std::endl;
		file << "{ \"type\":\"default-variant\",";
		file << "\"variant-body\":";
		visitStatements(list->l); 	// stmt
		file << "}";
    }
}

void RubyAstVisitor::checkMethodCall(Node *mc)
{
    /*
     * The method call body resides in the left child. Check if this
     * is either a require, an include/extend or just a normal method call.
     * If the left child is NULL, this is not a method call but a lambda
     * expression.
     */
	std::string name;
	if(mc->l->name != NULL)
			name = mc->l->name;
		else
			name = "";

    if (mc->l != NULL && name != "lambda") {
			
        if (name == "require")
            visitRequire(mc);
        else if (name == "include")
            visitMixin(mc, true);
        else if (name == "extend")
            visitMixin(mc, false);
        else if (name == "require_relative")
            visitRequire(mc, true);
        else
            visitMethodCall(mc);
    } else
        visitLambda(mc);
}

std::string RubyAstVisitor::escapeJsonString(const std::string& input) {
    std::ostringstream ss;
    for (std::string::const_iterator iter = input.begin(); iter != input.end(); iter++) {
        switch (*iter) {
            case '\\': ss << "\\\\"; break;
            case '"': ss << "\\\""; break;
            case '/': ss << "\\/"; break;
            case '\b': ss << "\\b"; break;
            case '\f': ss << "\\f"; break;
            case '\n': ss << "\\n"; break;
            case '\r': ss << "\\r"; break;
            case '\t': ss << "\\t"; break;
            default: ss << *iter; break;
        }
    }
    return ss.str();
}

} // End of namespace Ruby
