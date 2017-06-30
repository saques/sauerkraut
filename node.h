#include <iostream>
#include <vector>
#include <llvm/IR/Value.h>
#include <deque>
class CodeGenContext;
class StatementNode;
class IdentifierNode;
class ExpressionNode;
class VariableDeclarationNode;

typedef std::deque<StatementNode*> StatementList;
typedef std::deque<VariableDeclarationNode*> VariableList;
typedef std::deque<ExpressionNode*> ExpressionList;

class Node {
public:
	virtual ~Node() {}
	virtual llvm::Value* codeGen(CodeGenContext& context) { return NULL; }
};

class StatementNode : public Node{

};

class ExpressionNode : public Node{

};

class ExpressionStatementNode : public StatementNode{
public:
		ExpressionNode& expression;
		ExpressionStatementNode(ExpressionNode& expression):
			expression(expression){}
		virtual llvm::Value* codeGen(CodeGenContext& context);
};

class IdentifierNode :public ExpressionNode{
public:
	std::string name;
	IdentifierNode(const std::string& name) : name(name){}
	virtual llvm::Value * codeGen(CodeGenContext& context);
};

class StringNode :public ExpressionNode{
public:
	std::string s;
	StringNode(const std::string& s) : s(s){}
	virtual llvm::Value * codeGen(CodeGenContext& context);
};

class IntegerNode :public ExpressionNode{
public:
	int value;
	IntegerNode(int value) : value(value){}
	virtual llvm::Value * codeGen(CodeGenContext& context);
};

class AssignmentNode :public ExpressionNode{
public:
	IdentifierNode& ident;
	ExpressionNode& value;

	AssignmentNode(IdentifierNode& ident, ExpressionNode& value):
		ident(ident), value(value) {}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class BlockNode : public ExpressionNode {
public:
	StatementList statements;
	BlockNode() { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class VariableDeclarationNode : public StatementNode {
public:
	IdentifierNode& id;
	ExpressionNode * assignmentExpr;
	VariableDeclarationNode(IdentifierNode& id, ExpressionNode* assignmentExpr) :
		id(id), assignmentExpr(assignmentExpr){}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ExternalFunctionDeclarationNode : public StatementNode {
public:
	IdentifierNode& id;
	int arguments;
	ExternalFunctionDeclarationNode(IdentifierNode& id, int arguments) :
		id(id), arguments(arguments) {}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};


class FunctionCallNode : public ExpressionNode {
public:
	const IdentifierNode& id;
	ExpressionList arguments;
	FunctionCallNode(const IdentifierNode& id, ExpressionList& arguments) :
		id(id), arguments(arguments) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class MethodInvocationNode : public ExpressionNode {
public:
	ExpressionNode& object;
	const IdentifierNode& id;
	ExpressionList arguments;
	MethodInvocationNode(ExpressionNode& object,const IdentifierNode& id,
	                     ExpressionList& arguments):
	object(object), id(id), arguments(arguments) {}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class ArrayCreationNode : public ExpressionNode {
public:
	ExpressionList elements;
	ArrayCreationNode(ExpressionList& elements) :
		elements(elements) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class KVObjectCreationNode : public ExpressionNode {
public:
	ExpressionList keys;
	ExpressionList values;
	KVObjectCreationNode(ExpressionList& keys, ExpressionList& values) :
		keys(keys), values(values) { }
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class FunctionDeclarationNode : public StatementNode {
public:
	IdentifierNode& id;
	VariableList* arguments;
	BlockNode& block;
	FunctionDeclarationNode(IdentifierNode& id, VariableList* arguments, BlockNode& block) :
		id(id), arguments(arguments), block(block) {}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class BinaryOperationNode : public ExpressionNode {
public:
	ExpressionNode& lhs;
	ExpressionNode& rhs;
	std::string methodName;
	BinaryOperationNode(ExpressionNode& lhs, ExpressionNode& rhs,std::string methodName) :
		lhs(lhs), rhs(rhs), methodName(methodName) {}
	virtual llvm::Value * codeGen(CodeGenContext& context);
};

class TertiaryOperationNode : public ExpressionNode {
public:
	ExpressionNode& lhs;
	ExpressionNode& mhs;
	ExpressionNode& rhs;
	std::string methodName;
	TertiaryOperationNode(ExpressionNode& lhs, ExpressionNode& mhs, ExpressionNode& rhs,std::string methodName) :
		lhs(lhs), mhs(mhs), rhs(rhs), methodName(methodName) {}
	virtual llvm::Value * codeGen(CodeGenContext& context);
};

class UnaryOperationNode : public ExpressionNode {
public:
    ExpressionNode& lhs;
    std::string methodName;
    UnaryOperationNode(ExpressionNode& lhs,std::string methodName) :
            lhs(lhs), methodName(methodName) {}
    virtual llvm::Value * codeGen(CodeGenContext& context);
};

class ReturnNode : public StatementNode {
public:
	ExpressionNode& expression;
	ReturnNode(ExpressionNode& expression) : expression(expression) {}
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class IfNode : public StatementNode {
public:
	ExpressionNode& expression;
	BlockNode& thenBlock;
	BlockNode& elseBlock;
	IfNode(ExpressionNode& expression, BlockNode& thenBlock, BlockNode& elseBlock)
		:	expression(expression), thenBlock(thenBlock), elseBlock(elseBlock) {};
	virtual llvm::Value* codeGen(CodeGenContext& context);
};

class WhileNode : public StatementNode {
public:
	ExpressionNode& expression;
	BlockNode& thenBlock;
	WhileNode(ExpressionNode& expression, BlockNode& thenBlock) :
		expression(expression), thenBlock(thenBlock) {} ;
	virtual llvm::Value * codeGen(CodeGenContext& context);
};
