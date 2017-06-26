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

void createCoreFunctions(CodeGenContext& context){
	appendCoreFunction(&context, "printi", 1);
}
