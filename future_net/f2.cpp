#include "./lib/ALib.hpp"
//#define TEST_LOCAL973 233
#define YStart G.Ec
#define isnormal(x) !isprimary[x]&&x!=Start&&x!=Terminate
#define model mip
using namespace std;
struct _line{
	int num,u,v,val;
};
double t1 = clock();
//OsiClpSolverInterface solver1;
//OsiSolverInterface *solver = &solver1;
//CoinModel build;
int LMT;
int main(int argc,char* argv[]){
	//freopen("logs.txt","w",stdout);
	double row[9000],row3[4];
	int colno3[4];
	int _TIME = 18;
	//OsiClpSolverInterface solver1;
	//G.InitSolver();
	//LMT = min(G.Nc,150);
#ifdef TEST_LOCAL973
	puts("Success in building");
 #endif
	puts("WOC");
	solver1.readMps("dat2.mps","");
	solver1.initialSolve();
	OsiSolverInterface *solver2(&solver1);
	CbcModel mip(*solver2);
	//mip.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
	mip.initialSolve();
	mip.branchAndBound();
	//memcpy(ans,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	printf("Result is %s\n",mip.isProvenOptimal()?"OPTIMAL":"NOT OPTIMAL");
	bool ok1;
	/*while((ok1=(hasring(mip,ans)) )){
		if(clock()-t1>9.7*CLOCKS_PER_SEC)break;
		mip.setMaximumSeconds(9.7-(clock()-t1)/CLOCKS_PER_SEC);
		mip.branchAndBound();
		memcpy(ans,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	}*/
	//if(mip.getSolutionCount()>0){
	//	Print_Answer(fout0,mip);
	//}
	//else{
//		fprintf(fout0,"NA");
//	}
    return 0;
}
