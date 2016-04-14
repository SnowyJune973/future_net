#include "./lib/ALib.hpp"
#define BUILD 1
using namespace std;
int main(){
	OsiClpSolverInterface solver1;
	OsiSolverInterface *solver = &solver1;
	std::string mpsFileName="./dat2.mps";
	CoinModel build;
#if BUILD==1
	solver1.readMps(mpsFileName.c_str(),"");
	double t1 = clock();
	solver1.initialSolve();
	OsiSolverInterface *solver2 = &solver1;
	CbcModel model(*solver2);
	model.initialSolve();
	model.branchAndBound();
#endif
	return 0;
}
