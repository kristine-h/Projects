#include "Phoenix.h"

int main(int argc , char*argv[]){
	if(argc < 2) {
		std::cout<< "Please specify the .txt file" <<std::endl;
		return 15;
	}
	Phoenix p;
	p.ReadFile(argv[1]);
	return 0;
}            
