#include "node.h"
#include "codegen.h"
#include "sauerkraut.hpp"
#include <deque>
using namespace std;

Value * createCharArray(CodeGenContext& context, std::string stri);

/* Compile the AST into a module */
bool CodeGenContext::generateCode(BlockNode& root, raw_ostream * out)
{
	std::cerr << "Generating code...\n";

	/* Create the top level interpreter function to call as entry */
	vector<Type*> argTypes;
	FunctionType *ftype = FunctionType::get(Type::getVoidTy(getGlobalContext()), makeArrayRef(argTypes), false);
	mainFunction = Function::Create(ftype, GlobalValue::ExternalLinkage, "main", module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", mainFunction, 0);

	/* Push a new variable/block context */
	pushBlock(bblock);
	 /* emit bytecode for the toplevel block */
	if (root.codeGen(*this) != NULL) {
		ReturnInst::Create(getGlobalContext(), bblock);
		popBlock();
		/* Print the bytecode in a human-readable format
		   to see if our program compiled properly
		 */
		std::cerr << "Code is generated.\n";
		PassManager<Module> pm;
		pm.addPass(PrintModulePass(*out));
		pm.run(*module);
		return true;
	} else {
		return false;
	}
}

/* Executes the AST by running the main function */
GenericValue CodeGenContext::runCode() {
	if (module != NULL) {
		std::cerr << "Running code...\n";

		ExecutionEngine *ee = EngineBuilder( unique_ptr<Module>(module) ).create();
		ee->finalizeObject();
		vector<GenericValue> noargs;
		GenericValue v = ee->runFunction(mainFunction, noargs);
		std::cerr << "Code was run.\n";
		return v;
	} else {
		GenericValue a;
		return a;
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
	args.push_back(ConstantInt::get(Type::getInt64Ty(getGlobalContext()), value, true));
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
		Type * type = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
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
		argTypes.push_back(Type::getInt64Ty(getGlobalContext()));
    }
    FunctionType *ftype = FunctionType::get(Type::getInt64Ty(getGlobalContext()), makeArrayRef(argTypes), false);
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
	Type * chart = Type::getInt8Ty(getGlobalContext());
	ArrayType* arrayType = ArrayType::get(chart,len + 1);

	Value* arr_alloc = new AllocaInst(
	    arrayType, "chararray", context.currentBlock()
	);

	auto zero = ConstantInt::get(getGlobalContext(), llvm::APInt(64, 0, true));
	for (int i = 0; i < len; i++ ) {
		auto index = ConstantInt::get(getGlobalContext(), llvm::APInt(32, i, true));
		Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
		auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
		auto store = new llvm::StoreInst(
			ConstantInt::get(Type::getInt8Ty(getGlobalContext()), str[i], true),
			ptr,
			false,
			context.currentBlock()
		);
	}
	auto index = ConstantInt::get(getGlobalContext(), llvm::APInt(32, len, true));
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	auto store = new llvm::StoreInst(
		ConstantInt::get(Type::getInt8Ty(getGlobalContext()), 0, true),
		ptr,
		false,
		context.currentBlock()
	);
	index = ConstantInt::get(getGlobalContext(), llvm::APInt(32, 0, true));
	ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	return ptr;
}

Value * UnaryOperationNode::codeGen(CodeGenContext& context)
{
	Function * function = context.module->getFunction("funcexec");
	if (function == NULL) {
		std::cerr << "no such function (coreCoreFunctionFail) " << "funcexec"<< endl;
	}
	std::vector<Value*> args;
	args.push_back(lhs.codeGen(context));
	auto zero = ConstantInt::get(getGlobalContext(), llvm::APInt(64, 0, true));
	auto index = ConstantInt::get(getGlobalContext(), llvm::APInt(32, 0, true));

	args.push_back(createCharArray(context, methodName.c_str()));
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	ArrayType* arrayType = ArrayType::get(voidp, 2);

	Value* arr_alloc = new AllocaInst(
			arrayType, "sumarray", context.currentBlock()
	);

	auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	args.push_back(ptr);
	// args.push_back(ConstantDataArray::get(getGlobalContext(), makeArrayRef(funargs));
	/* push arguments count (1) */
	args.push_back(ConstantInt::get(Type::getInt64Ty(getGlobalContext()), 0, true));
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
	auto zero = ConstantInt::get(getGlobalContext(), llvm::APInt(64, 0, true));
	auto index = ConstantInt::get(getGlobalContext(), llvm::APInt(32, 0, true));
	/* push function name (sum) */
	args.push_back(createCharArray(context, methodName.c_str()));
	// std::vector<Value *> funargs;
	// funargs.push_back(rhs.codeGen(context));
	/* push function arguments {rhs exprssion} */
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	ArrayType* arrayType = ArrayType::get(voidp, 2);

	Value* arr_alloc = new AllocaInst(
	    arrayType, "sumarray", context.currentBlock()
	);


	auto ptr = GetElementPtrInst::Create(arrayType, arr_alloc, { zero, index }, "", context.currentBlock());
	auto store = new llvm::StoreInst(rhs.codeGen(context), ptr, false, context.currentBlock());
	args.push_back(ptr);
	// args.push_back(ConstantDataArray::get(getGlobalContext(), makeArrayRef(funargs));
	/* push arguments count (1) */
	args.push_back(ConstantInt::get(Type::getInt64Ty(getGlobalContext()), 1, true));
	CallInst *call = CallInst::Create(function, makeArrayRef(args), "", context.currentBlock());
	return call;
}

Value * FunctionDeclarationNode::codeGen(CodeGenContext& context)
{
	vector<Type*> argTypes;
	VariableList::const_iterator it;
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	for (it = arguments->begin(); it != arguments->end(); it++) {
		argTypes.push_back(voidp);
	}
	FunctionType *ftype = FunctionType::get(voidp, makeArrayRef(argTypes), false);
	Function *function = Function::Create(ftype, GlobalValue::InternalLinkage, id.name, context.module);
	BasicBlock *bblock = BasicBlock::Create(getGlobalContext(), "entry", function, 0);

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
		ReturnInst::Create(getGlobalContext(), IntegerNode(0).codeGen(context), bblock);
	} else {
		ReturnInst::Create(getGlobalContext(), context.getCurrentReturnValue(), bblock);
	}

	context.popBlock();
	std::cerr << "Creating function: " << id.name << endl;
	return function;
}

Value * ReturnNode::codeGen(CodeGenContext& context)
{
	Value *returnValue = expression.codeGen(context);
	printf("generating return\n ");
	if (context.getCurrentReturnValue() != NULL) {
		std::cerr << "Compilation error: multiple return statements" << endl;
		return NULL;
	} else {
		context.setCurrentReturnValue(returnValue);
		return returnValue;
	}
}
