#include <iostream>
#include "codegen.h"
#include "node.h"

using namespace std;

void appendCoreFunction(CodeGenContext * context, const char * name, int arguments)
{
	vector<Type*> argTypes;
	for (int i = 0; i < arguments; i++) {
		argTypes.push_back(PointerType::get(IntegerType::get(getGlobalContext(), 8), 0));
	}
	FunctionType *ftype = FunctionType::get(PointerType::get(IntegerType::get(getGlobalContext(), 8), 0),
							makeArrayRef(argTypes), false);
	Function * function = Function::Create(ftype, GlobalValue::ExternalLinkage,name, context->module);
}

void appendIntInit(CodeGenContext * context, const char * name)
{
	vector<Type*> argTypes;
	argTypes.push_back(IntegerType::get(getGlobalContext(), 64));
	FunctionType * ftype = FunctionType::get(PointerType::get(IntegerType::get(getGlobalContext(), 8), 0),
							makeArrayRef(argTypes), false);
	Function * function = Function::Create(ftype, GlobalValue::ExternalLinkage, name, context->module);
}


void appendFuncexec(CodeGenContext* context, const char * name)
{
	vector<Type*> argTypes;
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	Type * voidpp = PointerType::get(voidp, 0);
	Type * intty = IntegerType::get(getGlobalContext(), 64);
	argTypes.push_back(voidp);
	argTypes.push_back(voidp);
	argTypes.push_back(voidpp);
	argTypes.push_back(intty);
	FunctionType * ftype = FunctionType::get(voidp,
		makeArrayRef(argTypes), false);
		Function * function = Function::Create(ftype, GlobalValue::ExternalLinkage, name, context->module);
}

void appendArrayInit(CodeGenContext* context, const char * name)
{
	vector<Type*> argTypes;
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	Type * voidpp = PointerType::get(voidp, 0);
	Type * intty = IntegerType::get(getGlobalContext(), 64);
	argTypes.push_back(voidpp);
	argTypes.push_back(intty);
	FunctionType * ftype = FunctionType::get(voidp,
		makeArrayRef(argTypes), false);
		Function * function = Function::Create(ftype, GlobalValue::ExternalLinkage, name, context->module);
}

void appendKVObjectInit(CodeGenContext* context, const char * name)
{
	vector<Type*> argTypes;
	Type * voidp = PointerType::get(IntegerType::get(getGlobalContext(), 8), 0);
	Type * voidpp = PointerType::get(voidp, 0);
	Type * intty = IntegerType::get(getGlobalContext(), 64);
	argTypes.push_back(voidpp);
	argTypes.push_back(voidpp);
	argTypes.push_back(intty);
	FunctionType * ftype = FunctionType::get(voidp,
		makeArrayRef(argTypes), false);
		Function * function = Function::Create(ftype, GlobalValue::ExternalLinkage, name, context->module);
}


void appendStringInit(CodeGenContext * context, const char * name)
{
	vector<Type*> argTypes;
	argTypes.push_back(PointerType::get(IntegerType::get(getGlobalContext(), 8), 0));
	FunctionType *ftype = FunctionType::get(PointerType::get(IntegerType::get(getGlobalContext(), 8), 0),
							makeArrayRef(argTypes), false);
	Function * function = Function::Create(ftype, GlobalValue::ExternalLinkage, name, context->module);
}


void createCoreFunctions(CodeGenContext& context){
	appendIntInit(&context, "newIntegerObj");
	appendStringInit(&context, "newStringObj");
	appendArrayInit(&context,"newArrayObj");
	appendKVObjectInit(&context,"newKVObjectObj");
	appendCoreFunction(&context, "printi", 1);
	appendFuncexec(&context, "funcexec");
	appendCoreFunction(&context,"print",1);
	appendCoreFunction(&context,"readi",0);
	appendCoreFunction(&context,"read",0);
}
