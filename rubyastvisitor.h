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


#ifndef RUBYASTVISITOR_H
#define RUBYASTVISITOR_H

#include <fstream>
#include "node.h"
// Ruby



namespace Ruby
{

/**
 * @class RubyAstVisitor
 *
 * This class represents the Visitor of the RubyAst class.
 */
class  RubyAstVisitor
{
public:
    /// Constructor.
    RubyAstVisitor(Node *tree_head, struct options_t *opts);

    /// Destructor.
    virtual ~RubyAstVisitor();

    /**
     * This is the entry point of the visitor. It means that to visit a
     * valid RubyAst we should call this method.
     *
     * @param node The root of the AST.
     */
    void visitCode();

    /**
     * Given a node, it calls the appropriate method in order to successfully
     * visit this node.
     *
     * @param node The given node.
     */
    void visitNode(Node *node);

protected:
    /// And the following is a list of virtual methods that can be overriden.

    virtual void visitName(Node *node);
    virtual void visitString(Node *node);
	virtual void  visitBareString(Node *node);
    virtual void visitRegexp(Node *node);
    virtual void visitNumeric(Node *node);
    virtual void visitSymbol(Node *node);
    virtual void visitBody(Node *node);
    virtual void visitBinary(Node *node);
    virtual void visitBoolean(Node *node);
	virtual void visitBitwise(Node *node);
    virtual void visitRange(Node *node);
    virtual void visitUnary(Node *node);
    virtual void visitArray(Node *node);
	virtual void visitArrayStatements(Node *list);
    virtual void visitArrayValue(Node *node);
    virtual void visitHash(Node *node);
    virtual void visitReturnStatement(Node *node);
    virtual void visitUndefStatement(Node *node);
    virtual void visitAliasStatement(Node *node);
    virtual void visitYieldStatement(Node *node);
    virtual void visitAssignmentStatement(Node *node);
    virtual void visitIfStatement(Node *node);
    virtual void visitCaseStatement(Node *node);
    virtual void visitBeginStatement(Node *node);
    virtual void visitUpBeginEndStatement(Node *node);
    virtual void visitWhileStatement(Node *node);
    virtual void visitForStatement(Node *node);
    virtual void visitMethodStatement(Node *node);
    virtual void visitMethodArguments(Node *node);
    virtual void visitParameter(Node *node);
    virtual void visitClassStatement(Node *node);
    virtual void visitSingletonClass(Node *node);
    virtual void visitModuleStatement(Node *node);
    virtual void visitMethodCall(Node *node);
    virtual void visitSuper(Node *node);
    virtual void visitLambda(Node *node);
    virtual void visitBlock(Node *node);
    virtual void visitBlockVariables(Node *node);
    virtual void visitRequire(Node *node, bool relative = false);
    virtual void visitMixin(Node *node, bool include);
    virtual void visitDefined(Node *node);
    virtual void visitTrue(Node *node);
    virtual void visitFalse(Node *node);
    virtual void visitNil(Node *node);
    virtual void visitLine(Node *node);
    virtual void visitFile(Node *node);
    virtual void visitEncoding(Node *node);
    virtual void visitSelf(Node *node);
    virtual void visitAccessSpecifier(short policy);
    virtual void visitClassName(Node *node);
    virtual void visitRescue(Node *node);
    virtual void visitRescueArg(Node *node);
    virtual void visitEnsure(Node *node);
	static std::string escapeJsonString(const std::string& input); 

private:
	Node *head;
	struct options_t *options;
	std::ofstream file;
    /**
     * @internal Helper method useful to visit a list of statements.
     *
     * @param list A node that is the first item of a list of nodes.
     */
    void visitStatements(Node *list);

    /**
     * @internal Helper method used for the visitIfStatement method. Its
     * goal is to visit a tail of an if statement.
     *
     * @param tail The tail of an if statement.
     */
    void visitIfTail(Node *tail);

    /**
     * @internal Helper method that goes through a list of when statements.
     * Obviously, this method is used by the visitCaseStatement method.
     *
     * @param list A node that is the first item of a list of when statements.
     */
    void visitWhenStatements(Node *list);

    /**
     * @internal Helper method that checks if this is either a require, an
     * include/extend or just a regular method call.
     *
     * @param mc The given method call node.
     */
    void checkMethodCall(Node *mc);
};

} // End of namespace Ruby


#endif // RUBYASTVISITOR_H
