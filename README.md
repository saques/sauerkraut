
<p align="center">
  <img src="sklogo.png" alt="logo"/>
</p>

# Requisitos: 
	* llvm  -> sudo apt install llvm  (3.8)
	* clang -> sudo apt install clang (3.8)
	* bison -> sudo apt-get install bison
	* and stuff -> sudo apt-get install lib32ncurses5-dev zlib1g-dev 
# Uso:
 	1. make
	2. ./sauerkraut <archivo del programa> -o <programa de salida en LLVM IR>  
	3. clang <programa de salida en LLVM IR> <archivos .o para linkeditar>-o <programa de salida en binario>
	4. ./<programa de salida> 
