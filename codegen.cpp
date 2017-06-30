#include "node.h"
#include "codegen.h"
#include "sauerkraut.hpp"
#include <deque>
using namespace std;
extern LLVMContext TheContext;
IRBuilder<> builder(TheContext);
Value * createCharArray(CodeGenContext& context, std::string stri);
Value* expressionListPointerArray(CodeGenContext& context, ExpressionList elements);
Value * eval(CodeGenContext& context, Value * value);

/* Compile the AST into a module */
bool CodeGenContext::generateCode(BlockNode& root, raw_ostream * out)
{
	std::cerr << "Generating code...\n";

	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getInt64Ty(TheContext), makeArrayRef(argTypes), false);
	mainFunction = Function::Create(ftype, GlobalValue::ExternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(TheContext, "entry", mainFunction, 0);
	builder.SetInsertPoint(bblock);
	/* Push a new variable/block context */
	pushBlock(bblock);
	 /* emit bytecode for the toplevel block */
	if (root.codeGen(*this) != NULL) {
		builder.CreateRet(ConstantInt::get(Type::getInt64Ty(TheContext), 0, true));

		popBlock();
		/* Print the bytecode in a human-readable format
		   to see if our program compiled properly
		 */
		std::cerr << "Code is generated.\n";
		*out << *module;
		out->flush();
		return true;
	} else {
		return false;
	}
}

/* -- Code Generation -- */

Value* IntegerNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating integer: " << value << endl;
	Function *function = context.module->getFunction("newIntegerObj");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "newIntegerObj"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), value, true));
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}

Value* StringNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating string: " << s << endl;
	Function *function = context.module->getFunction("newStringObj");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "newStringObj"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(createCharArray(context,s));
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}


Value* expressionListPointerArray(CodeGenContext& context, ExpressionList elements){
	ExpressionList::const_iterator it;
	int i=0;
	Type * voidp = PointerType::get(IntegerType::get(TheContext, 8), 0);
	ArrayType* arrayType = ArrayType::get(voidp, elements.size());

	Value* arr_alloc = new AllocaInst(
		   arrayType, "arrargs", context.currentBlock()
	);
	auto zero = ConstantInt::get(TheContext, llvm::APInt(64, 0, true));
	for (it = elements.begin(); it != elements.end(); it++, i++) {
		auto index = ConstantInt::get(TheContext, llvm::APInt(32, i, true));
		auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
		auto store = new llvm::StoreInst(
			(**it).codeGen(context),
			ptr,
			false,
			context.currentBlock()
		);
	}
	auto index = ConstantInt::get(TheContext, llvm::APInt(32, 0, true));
	return GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
}

Value* ArrayCreationNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating array" << endl;
	Function *function = context.module->getFunction("newArrayObj");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "newArrayObj"<< endl;
	}

	std::vector<Value*> args;

	args.push_back(expressionListPointerArray(context,elements));

	args.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), elements.size(), true));

	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}


Value* KVObjectCreationNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating object" << endl;
	Function *function = context.module->getFunction("newKVObjectObj");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "newKVObjectObj"<< endl;
	}

	std::vector<Value*> args;

	args.push_back(expressionListPointerArray(context,keys));
	args.push_back(expressionListPointerArray(context,values));

	args.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), keys.size(), true));

	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}


Value* IdentifierNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating identifier reference: " << name << endl;
	if (context.locals().find(name) == context.locals().end()) {
		std::cerr << "undeclared variable " << name << endl;
		return NULL;
	}
	return new LoadInst(context.locals()[name], "", false, context.currentBlock());
}

Value* AssignmentNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating assignment for " << ident.name << endl;
	if (context.locals().find(ident.name) == context.locals().end()) {
		std::cerr << "undeclared variable " << ident.name << endl;
		return NULL;
	}
	return new StoreInst(value.codeGen(context), context.locals()[ident.name], false, context.currentBlock());
}

Value* VariableDeclarationNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Creating variable declaration " << id.name << endl;
	if (context.locals().find(id.name) == context.locals().end()) {
		Type * type = PointerType::get(IntegerType::get(TheContext, 8), 0);
		AllocaInst *alloc = new AllocaInst(type
				, id.name.c_str(), context.currentBlock());
		context.locals()[id.name] = alloc;
		if (assignmentExpr != NULL) {
			AssignmentNode assn(id, *assignmentExpr);
			assn.codeGen(context);
		}
		return alloc;
	} else {
		std::cerr << "Duplicate variable declaration " << id.name << "; compilation error." << endl;
		return NULL;
	}

}

Value* BlockNode::codeGen(CodeGenContext& context)
{
	StatementList::const_iterator it;
	Value *last = NULL;
	for (it = statements.begin(); it != statements.end(); it++) {
		std::cerr << "Generating code for " << typeid(**it).name() << endl;
		last = (**it).codeGen(context);
		if (last == NULL) {
			break;
		}
	}
	return last;
}

Value* ExpressionStatementNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Generating code for " << typeid(expression).name() << endl;
	return expression.codeGen(context);
}

Value* ExternalFunctionDeclarationNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Declaring external function " << id.name << endl;
	vector<Type*> argTypes;
    VariableList::const_iterator it;
    for (int i = 0; i < arguments; i++) {
        //argTypes.push_back(typeOf((**it).type));
		argTypes.push_back(Type::getInt64Ty(TheContext));
    }
    FunctionType *ftype = FunctionType::get(Type::getInt64Ty(TheContext), makeArrayRef(argTypes), false);
    Function *function = Function::Create(ftype, GlobalValue::ExternalLinkage, id.name, context.module);
    return function;
}

Value* FunctionCallNode::codeGen(CodeGenContext& context)
{
	Function *function = context.module->getFunction(id.name);
	if (function == NULL) {
		std::cerr << "no such function " << id.name << endl;
	}
	std::vector<Value*> args;
	ExpressionList::const_iterator it;
	for (it = arguments.begin(); it != arguments.end(); it++) {
		args.push_back((**it).codeGen(context));
	}
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	std::cerr << "Creating method call: " << id.name << endl;
	return call;
}

Value * createCharArray(CodeGenContext& context, std::string stri)
{
	const char * str = stri.c_str();
	int len = strlen(str);
	Type * chart = Type::getInt8Ty(TheContext);
	ArrayType* arrayType = ArrayType::get(chart,len + 1);

	Value* arr_alloc = new AllocaInst(
	    arrayType, "chararray", context.currentBlock()
	);

	auto zero = ConstantInt::get(TheContext, llvm::APInt(64, 0, true));
	for (int i = 0; i < len; i++ ) {
		auto index = ConstantInt::get(TheContext, llvm::APInt(32, i, true));
		Type * voidp = PointerType::get(IntegerType::get(TheContext, 8), 0);
		auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
		auto store = new llvm::StoreInst(
			ConstantInt::get(Type::getInt8Ty(TheContext), str[i], true),
			ptr,
			false,
			context.currentBlock()
		);
	}
	auto index = ConstantInt::get(TheContext, llvm::APInt(32, len, true));
	Type * voidp = PointerType::get(IntegerType::get(TheContext, 8), 0);
	auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	auto store = new llvm::StoreInst(
		ConstantInt::get(Type::getInt8Ty(TheContext), 0, true),
		ptr,
		false,
		context.currentBlock()
	);
	index = ConstantInt::get(TheContext, llvm::APInt(32, 0, true));
	ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	return ptr;
}

Value * MethodInvocationNode::codeGen(CodeGenContext& context){
	Function * function = context.module->getFunction("funcexec");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "funcexec"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(object.codeGen(context));
	args.push_back(createCharArray(context,id.name));
	args.push_back(expressionListPointerArray(context,arguments));
	args.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), arguments.size(), true));
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}


Value * UnaryOperationNode::codeGen(CodeGenContext& context)
{
	Function * function = context.module->getFunction("funcexec");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "funcexec"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(lhs.codeGen(context));
	auto zero = ConstantInt::get(TheContext, llvm::APInt(64, 0, true));
	auto index = ConstantInt::get(TheContext, llvm::APInt(32, 0, true));

	args.push_back(createCharArray(context, methodName.c_str()));
	Type * voidp = PointerType::get(IntegerType::get(TheContext, 8), 0);
	ArrayType* arrayType = ArrayType::get(voidp, 2);

	Value* arr_alloc = new AllocaInst(
			arrayType, "sumarray", context.currentBlock()
	);

	auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	args.push_back(ptr);
	// args.push_back(ConstantDataArray::get(TheContext, makeArrayRef(funargs));
	/* push arguments count (1) */
	args.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 0, true));
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}

Value * BinaryOperationNode::codeGen(CodeGenContext& context)
{
	Function * function = context.module->getFunction("funcexec");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "funcexec"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(lhs.codeGen(context));
	auto zero = ConstantInt::get(TheContext, llvm::APInt(64, 0, true));
	auto index = ConstantInt::get(TheContext, llvm::APInt(32, 0, true));
	/* push function name (sum) */
	args.push_back(createCharArray(context, methodName.c_str()));
	// std::vector<Value *> funargs;
	// funargs.push_back(rhs.codeGen(context));
	/* push function arguments {rhs exprssion} */
	Type * voidp = PointerType::get(IntegerType::get(TheContext, 8), 0);
	ArrayType* arrayType = ArrayType::get(voidp, 2);

	Value* arr_alloc = new AllocaInst(
	    arrayType, "sumarray", context.currentBlock()
	);


	auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	auto store = new llvm::StoreInst(rhs.codeGen(context), ptr, false, context.currentBlock());
	args.push_back(ptr);
	// args.push_back(ConstantDataArray::get(TheContext, makeArrayRef(funargs));
	/* push arguments count (1) */
	args.push_back(ConstantInt::get(Type::getInt64Ty(TheContext), 1, true));
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}

Value * FunctionDeclarationNode::codeGen(CodeGenContext& context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	Type * voidp = PointerType::get(IntegerType::get(TheContext, 8), 0);
	for (it = arguments->begin(); it != arguments->end(); it++) {
		argTypes.push_back(voidp);
	}
	FunctionType *ftype = FunctionType::get(voidp, makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name, context.module);
	BasicBlock *bblock = BasicBlock::Create(TheContext, "entry", function, 0);
	builder.SetInsertPoint(bblock);
	context.pushBlock(bblock);

	Function::arg_iterator argsValues = function->arg_begin();
    Value* argumentValue;

	for (it = arguments->begin(); it != arguments->end(); it++) {
		(**it).codeGen(context);

		argumentValue = &*argsValues++;
		argumentValue->setName((*it)->id.name);
		StoreInst *inst = new StoreInst(argumentValue, context.locals()[(*it)->id.name], false, bblock);
	}

	block.codeGen(context);
	if (context.getCurrentReturnValue() == NULL) {
		builder.CreateRet(IntegerNode(0).codeGen(context));
	}

	context.popBlock();
	builder.SetInsertPoint(context.currentBlock());
	std::cerr << "Creating function: " << id.name << endl;
	return function;
}

Value * ReturnNode::codeGen(CodeGenContext& context)
{
	Value *returnValue = expression.codeGen(context);
	if (returnValue != NULL){
		context.setCurrentReturnValue(returnValue);
		/* if return statement is inside main, Integer conversion is necessary */
		if (context.isMain())  {
			builder.CreateRet(eval(context, returnValue));
		} else {
			builder.CreateRet(returnValue);
		}
		return returnValue;
	}
}

Value * eval(CodeGenContext& context, Value * value)
{
	Function * function = context.module->getFunction("eval");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "eval"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(value);
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}


Value * IfNode::codeGen(CodeGenContext& context)
{
	std::cerr << "Generating if code " << endl;

	std::map <std::string, Value *> oldLocals;
	oldLocals.insert(context.locals().begin(), context.locals().end());
	Function * function = context.currentBlock()->getParent();
	BasicBlock *ThenBB =BasicBlock::Create(TheContext, "then", function, 0);
	BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else", function, 0);
	BasicBlock *MergeBB = BasicBlock::Create(TheContext, "ifcont", function, 0);

	Value * condVal = eval(context, expression.codeGen(context));
	if (condVal == NULL) {
		return NULL;
	}
	condVal = builder.CreateICmpNE(
		condVal,
		ConstantInt::get(Type::getInt64Ty(TheContext), 0, true),
		"ifcond"
	);
	builder.CreateCondBr(condVal, ThenBB, ElseBB);
	context.pushBlock(ThenBB);
	builder.SetInsertPoint(ThenBB);
	Value * ThenV = thenBlock.codeGen(context);
	ThenBB = builder.GetInsertBlock();
	bool thenReturns = context.getCurrentReturnValue() != NULL ? true : false;
	context.popBlock();
	if (!thenReturns) {
		builder.CreateBr(MergeBB);
	}
	context.pushBlock(ElseBB);
	builder.SetInsertPoint(ElseBB);
	Value *ElseV = elseBlock.codeGen(context);
	bool elseReturns = context.getCurrentReturnValue() != NULL ? true : false;
	context.popBlock();
	context.popBlock();
	if (!elseReturns) {
		builder.CreateBr(MergeBB);
	}
	ElseBB = builder.GetInsertBlock();

	context.pushBlock(MergeBB, oldLocals);
	builder.SetInsertPoint(MergeBB);
	return MergeBB;
}

Value * WhileNode::codeGen(CodeGenContext& context) {

	std::cerr << "Generating while code " << endl;

	std::map <std::string, Value *> oldLocals;
	oldLocals.insert(context.locals().begin(), context.locals().end());

	Function * function = context.currentBlock()->getParent();
	BasicBlock * LoopCond =  BasicBlock::Create(TheContext, "condition", function, 0);
	BasicBlock * BodyBlock = BasicBlock::Create(TheContext, "body", function, 0);
	BasicBlock * AfterBlock = BasicBlock::Create(TheContext, "afterwhile", function, 0);
	builder.CreateBr(LoopCond);
	builder.SetInsertPoint(LoopCond);
	context.pushBlock(LoopCond);
	Value * condVal = eval(context, expression.codeGen(context));
	if (condVal == NULL) {
		return NULL;
	}
	condVal = builder.CreateICmpNE(
		condVal,
		ConstantInt::get(Type::getInt64Ty(TheContext), 0, true),
		"ifcond"
	);
	builder.CreateCondBr(condVal, BodyBlock, AfterBlock);
	context.popBlock();

	context.pushBlock(BodyBlock);
	builder.SetInsertPoint(BodyBlock);
	Value * BodyV = thenBlock.codeGen(context);
	bool bodyReturns = context.getCurrentReturnValue() != NULL ? true : false;
	context.popBlock();
	context.popBlock();
	if(!bodyReturns) {
		builder.CreateBr(LoopCond);
	}
	builder.SetInsertPoint(AfterBlock);
	context.pushBlock(AfterBlock, oldLocals);
	return AfterBlock;
}
