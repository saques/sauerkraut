#include <iostream>
#include "node.h"
#include "codegen.h"
#include "string.h"
#include <fstream>
#include <iostream>
using namespace std;

extern int yyparse();
extern BlockNode * programStart;
void createCoreFunctions(CodeGenContext& context);

static cl::opt<std::string>
InputFilename(cl::Positional, cl::desc("<input brainf>"));

static cl::opt<std::string>
OutputFilename("o", cl::desc("Output filename"), cl::value_desc("filename"));

int main(int argc, char **argv)
{
	cl::ParseCommandLineOptions(argc, argv, " BrainF compiler\n");
	bool run = false;
	bool llvms = false;
	for (int i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-r")) {
			/* run flag set to on */
			run = true;
		} else if (strcmp(argv[i], "-s")) {
			/* compile to llvm set to on*/
			llvms = true;
		}
	}
	if (InputFilename != "") {
		if (freopen(InputFilename.c_str(), "r", stdin) == NULL) {
			std::cerr << "Input file was not found." << std::endl;
			exit(0);
		}
	}

	yyparse();

	cerr << ">>>Program start: " << programStart << endl;
	InitializeNativeTarget();
	InitializeNativeTargetAsmPrinter();
	InitializeNativeTargetAsmParser();
	CodeGenContext context;
	// createCoreFunctions(context);
	raw_ostream *out = &outs();
	if (OutputFilename == "") {
		OutputFilename = "output.ll";
	}
	std::error_code EC;
	out = new raw_fd_ostream(OutputFilename, EC, sys::fs::F_None);

	context.generateCode(*programStart, out);

	return 0;
}
