#include <stack>
#include <typeinfo>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/PassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

class BlockNode;

class CodeGenBlock {
public:
    BasicBlock *block;
    Value *returnValue;
    std::map<std::string, Value*> locals;


};

class CodeGenContext {
    std::stack<CodeGenBlock *> blocks;


public:
	Function *mainFunction;
    Module *module;
    CodeGenContext() { module = new Module("main", getGlobalContext()); }
    bool generateCode(BlockNode& root, raw_ostream * out);
    GenericValue runCode();
    std::map<std::string, Value*>& locals() { return blocks.top()->locals; }
    BasicBlock *currentBlock() { return blocks.top()->block; }

    void pushBlock(BasicBlock *block) {
		CodeGenBlock * oldTop = NULL;
		if (!blocks.empty()) {
			oldTop = blocks.top();
		}
		blocks.push(new CodeGenBlock());
		blocks.top()->returnValue = NULL;
		blocks.top()->block = block;
		if (oldTop != NULL) {
			blocks.top()->locals.insert(oldTop->locals.begin(), oldTop->locals.end());
		}
	}
	void pushBlock(BasicBlock *block, std::map<std::string, Value*> locals) {
		CodeGenBlock * oldTop = NULL;
		if (!blocks.empty()) {
			oldTop = blocks.top();
		}
		CodeGenBlock * newBlock = new CodeGenBlock();
		if (oldTop != NULL) {
			blocks.top()->locals.insert(oldTop->locals.begin(), oldTop->locals.end());
		}
		newBlock->locals.insert(locals.begin(), locals.end());
		newBlock->block = block;
		newBlock->returnValue = NULL;
		blocks.push(newBlock);
	}
    void popBlock() { CodeGenBlock *top = blocks.top(); blocks.pop(); delete top; }
    void setCurrentReturnValue(Value *value) { blocks.top()->returnValue = value; }
    Value* getCurrentReturnValue() { return blocks.top()->returnValue; }
	CodeGenBlock * currentCodeGenBlock() {blocks.top();}
	bool isMain(){
		return mainFunction == currentBlock()->getParent();
	}
};
