
# Requisitos: 
	* llvm  -> sudo apt install llvm  
	* clang -> sudo apt install clang 
# Uso:
 	1. make
	2. ./sauerkraut <archivo del programa> -o <programa de salida en LLVM IR>  
	3. clang <programa de salida en LLVM IR> <archivos .o para linkeditar>-o <programa de salida en binario>
	4. ./<programa de salida> 
