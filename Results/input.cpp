#include <fstream>
#include <cmath>
#include <cstdlib>

int main(){
	// Random training sets for XOR; two inputs and one output
	std::ofstream fout("input.txt");
	fout<<"Topology: 2 4 1"<<std::endl;
	for(int i=2000; i>=0;i--){
		int n1= (int)(2.0*rand()/double(RAND_MAX));
		int n2= (int)(2.0*rand()/double(RAND_MAX));
		
		int t= n1^n2;
		fout<<"in: "<<n1<<".0 "<<n2<<".0 "<<std::endl;
		fout<<"out: "<<t<<".0 "<<std::endl;

	}
	
	fout.close();
}
