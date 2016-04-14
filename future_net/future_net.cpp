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
struct Graph{
	int mat[maxn][maxn],degI[maxn],degO[maxn];
	int dfn[maxn],low[maxn],belong[maxn],Priority[maxn];
	int dist_to_terminate[maxn];
	int Nc,Ec,DEMc;
	map<int,int> M,getedge;
	int revhash[maxn];
	vector<int> E[maxn],F[maxn],RE[maxn],RF[maxn];
	vector<int> path;
	bool vis[maxn],isprimary[maxn],vis_tar[maxn],instack[maxn];
	int TM = 0,STRONG_CONNECT = 0;
	stack<int> ST;
	public:
	int Start,Terminate;
	_line line[maxn*8];
	Graph(){
		memset(mat,0x33,sizeof(mat));
		memset(degI,0,sizeof(degI));
		memset(degO,0,sizeof(degO));
		memset(line,0,sizeof(line));
		memset(revhash,0x22,sizeof(revhash));
		memset(vis,0,sizeof(vis));
		memset(isprimary,0,sizeof(isprimary));
		memset(vis_tar,0,sizeof(vis_tar));
		memset(instack,0,sizeof(instack));
		memset(dist_to_terminate,0x33,sizeof(dist_to_terminate));
		Ec = Nc = DEMc = 0;
		getedge.clear();
	}
	void input_topo(FILE* fin);
	int input_demand(FILE* fin);
	void _Add_Constraint(int p,char type);
	void InitSolver();
	void WriteMPSFile(FILE* fout);
}G;
bool cmp_pn_1(int x,int y){
    return x > y;
}
bool cmp_pn_2(int x,int y){
    return rand() < rand();
}
bool cmp_pn_3(int x,int y){
    return G.dist_to_terminate[x] < G.dist_to_terminate[y];
}
void Graph::WriteMPSFile(FILE *fout){
	fprintf(fout,"NAME\tcase\n");
	fprintf(fout,"ROWS\n");
	fprintf(fout," N\tobj\n");
	for(int i = 1; i <= Nc; i++){
		if(isnormal(i)){
			fprintf(fout," E\tC(B.%03d)\n",i);
			fprintf(fout," L\tC(L.%03d)\n",i);
		}
		else{
			fprintf(fout," E\tC(I.%03d)\n",i);
			fprintf(fout," E\tC(O.%03d)\n",i);
		}
	}
	fprintf(fout,"COLUMNS\n");
	for(int i = 0; i < Ec; i++){
		int uu = M[line[i].u], vv = M[line[i].v],valval = line[i].val;
		fprintf(fout," X(%03d)\t\tobj\t\t%d\n",i,valval);
		if(isnormal(uu)){
			fprintf(fout," X(%03d)\t\tC(B.%03d)\t-1\n",i,uu);
			fprintf(fout," X(%03d)\t\tC(L.%03d)\t1\n",i,uu);
		}
		else{
			fprintf(fout," X(%03d)\t\tC(O.%03d)\t1\n",i,uu);
		}
		if(isnormal(vv)){
			fprintf(fout," X(%03d)\t\tC(B.%03d)\t1\n",i,vv);
		}
		else{
			fprintf(fout," X(%03d)\t\tC(I.%03d)\t1\n",i,vv);
		}
	}
	fprintf(fout,"RHS\n");
	for(int i = 1; i <= Nc; i++){
		if(isnormal(i)){
			fprintf(fout," rhs\t\tC(B.%03d)\t1\n",i);
			fprintf(fout," rhs\t\tC(L.%03d)\t1\n",i);
		}
		else{
			int ci = 1,co = 1;
			if(i==Start)ci--;
			if(i==Terminate)co--;
			fprintf(fout," rhs\t\tC(I.%03d)\t%d\n",i,ci);
			fprintf(fout," rhs\t\tC(O.%03d)\t%d\n",i,co);
		}
	}
	fprintf(fout,"BOUNDS\n");
	for(int i = 0; i < Ec; i++){
		fprintf(fout," BV bnd\t\tX(%03d)\n",i);
	}
	fprintf(fout,"ENDATA");
}
		
void Graph::input_topo(FILE* fin){
	int T = 0;
	getedge.clear();
	while(fscanf(fin,"%d,%d,%d,%d",&line[T].num,&line[T].u,&line[T].v,&line[T].val) == 4){
        T++;
	}
	M.clear();
	for(int i = 0; i < T; i++){
		if(M.find(line[i].u) == M.end())M.insert(make_pair(line[i].u,0));
		if(M.find(line[i].v) == M.end())M.insert(make_pair(line[i].v,0));
	}
	int cc;
	map<int,int>::iterator it;
	for(it = M.begin(),cc = 1; it != M.end(); it++,cc++){
		it->second = cc;
		revhash[cc] = it->first;
	}
	for(int i = 0; i < T; i++){
		int numnum = line[i].num,uu = M[line[i].u],vv = M[line[i].v],valval = line[i].val;
		getedge[uu*1000+vv] = numnum;
		if(mat[uu][vv]==inf){
			degI[vv]++;
			degO[uu]++;
			E[uu].push_back(vv);
			F[uu].push_back(i);
			RE[vv].push_back(uu);
			RF[vv].push_back(i);
		}
		mat[uu][vv] = min(mat[uu][vv],valval);
	}
	Ec = T;
	Nc = M.size();
};
int Graph::input_demand(FILE* fin){
    char str[90000];
    int x = 0;
    fscanf(fin,"%d,%d,",&Start,&Terminate);
    if(M.find(Start) == M.end() || M.find(Terminate) == M.end()){
        return _UNSOLVABLE_START_END_NOT_CONNECTED;
    }
    Start = M[Start];
    Terminate = M[Terminate];
    fscanf(fin,"%s",str);
    for(int i = 0; i < strlen(str); i++){
        if(str[i] >= '0' && str[i] <= '9'){
            x = x * 10 + str[i] - 48;
        }
        else{
            if(str[i] == '|'){
                if(M.find(x) == M.end())return _A_WRONG_DEMANDED_POINT_NUMBER;
                isprimary[M[x]] = true;
                x = 0;
                DEMc++;
            }
            else x = 0;
        }
    }
    if(M.find(x) == M.end())return _A_WRONG_DEMANDED_POINT_NUMBER;
    isprimary[M[x]] = true;
    DEMc++;
    return _OK;
}
/* ********************************
   *******************************
   ******************************
   *****************************
*/
/* Defining variables on CBC solver. */
OsiClpSolverInterface solver1;
OsiSolverInterface *solver = &solver1;
CoinModel build;
/* End */
/* ********************************
   *******************************
   ******************************
   *****************************
*/
void Graph::InitSolver(){
	for(int i = 0; i < Ec; i++){
		build.setColumnBounds(i,0,1);
		build.setObjective(i,line[i].val);
		build.setInteger(i);
	}
}
void Graph::_Add_Constraint(int p,char type){
	int cc = E[p].size(), rcc = RE[p].size();
	double row[50];
	int colno[50];
	if(type <= 2){
		int ic = (type==0?0:1), oc = (type==1?0:1);
		for(int i = 0; i < rcc; i++){
			row[i] = 1;
			colno[i] = RF[p][i];
		}
		build.addRow(rcc,colno,row,ic,ic);
		for(int i = 0; i < cc; i++){
			row[i] = 1;
			colno[i] = F[p][i];
		}
		build.addRow(cc,colno,row,oc,oc);
	}
	else{
		for(int i = 0; i < rcc; i++){
			row[i] = 1;
			colno[i] = RF[p][i];
		}
		build.addRow(rcc,colno,row,0,1);
		for(int i = 0; i < cc; i++){
			row[rcc+i] = -1;
			colno[rcc+i] = F[p][i];
		}
		build.addRow(cc+rcc,colno,row,0,0);
	}
}
double ans[20000];
void Print_Answer(FILE *fout,CbcModel mip){
	int pres = G.Start;
	int ConstraintC = mip.getNumRows(), VariableC = mip.getNumCols();
	int longsam = (int)(mip.getObjValue());
	double x[700];
	memcpy(x,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	while(pres != G.Terminate){
#ifdef TEST_LOCAL973
		printf("PRES = %d REALPRES = %d\n",pres+G.Ec,G.revhash[pres]);
#endif
		for(int i = 0; i < G.F[pres].size(); i++){
			if(int(x[G.F[pres][i]]+0.5) == 1){
				if(pres != G.Start){
					fprintf(fout,"|");
				}
				fprintf(fout,"%d",G.F[pres][i]);
#ifdef TEST_LOCAL973
				printf("Choose Line %d\n",G.F[pres][i]);
#endif
				pres = G.E[pres][i];
				break;
			}
		}
#ifdef TEST_LOCAL973
		printf("PRES = %d REALPRES = %d\n",pres+G.Ec,G.revhash[pres]);
#endif
	}
#ifdef TEST_LOCAL973
	fprintf(fout,"\nLength = %d\n",longsam);
#endif
}

void printlist(CbcModel mip){
	puts("Objective Coeffecients:");
	const double *co = mip.getObjCoefficients();
	for(int i = 0; i < mip.getNumCols(); i++){
		printf("%.3lf ",co[i]);
	}
	puts("");
} 
bool hasring(CbcModel mip,double *sol){
	int Cc = mip.getNumRows(), Vc = mip.getNumCols();	
	int pres = G.Start;
	int cnt;
	bool retval=false,visited[700];
	double row1[700];
	int colno1[700];
	memset(visited,0,sizeof(visited));
	visited[G.Start] = true;
	double *x = sol;
	//printlist(mip);
	while(pres != G.Terminate){
#ifdef TEST_LOCAL973
		printf("PRES = %d REALPRES = %d\n",pres+G.Ec,G.revhash[pres]);
#endif
		for(int i = 0; i < G.F[pres].size(); i++){
			if(int(x[G.F[pres][i]]+0.5) == 1){
#ifdef TEST_LOCAL973
				printf("Choose Line %d\n",G.F[pres][i]);
#endif
				pres = G.E[pres][i];
				break;
			}
		}
#ifdef TEST_LOCAL973
		printf("PRES = %d REALPRES = %d\n",pres+G.Ec,G.revhash[pres]);
#endif
		if(visited[pres]){
			retval=true;
			break;
		}
		visited[pres]=true;
	}
	for(int i = 1; i <= G.Nc; i++){
		if(!visited[i] && (int)(ans[Cc+G.Ec+2*i]+0.5) == 1){
#ifdef TEST_LOCAL973
			printf("Node %d is in a circle.\n",i);
#endif
			cnt = 0;
			retval = true;
			visited[pres=i] = true;
			while(1){
				for(int j = 0; j < G.F[pres].size(); j++){
#ifdef TEST_LOCAL973
					printf("Line %d: X(j) == %.3lf\n",G.F[pres][j],x[j]);
#endif
					if(int(x[G.F[pres][j]]+0.5) == 1){
						row1[cnt] = 1;
						colno1[cnt] = G.F[pres][j];
						cnt++;
						pres = G.E[pres][j];
						break;
					}
				}
#ifdef TEST_LOCAL973
				printf("PRES = %d\n",pres);
				puts("");
#endif
				if(visited[pres])break;
				visited[pres] = true;
			}
#ifdef TEST_LOCAL973
			printf("CNT = %d\n",cnt);
#endif
			if(cnt>0)build.addRow(cnt,colno1,row1,0,cnt-1);
			mip.solver()->loadFromCoinModel(build);
		}
	}
	printf("This solution is %s\n",retval?"OK":"NOT OK");
	return retval;
}
int LMT;
int main(int argc,char* argv[]){
	//freopen("logs.txt","w",stdout);
	double row[9000],row3[4];
	int colno3[4];
	int _TIME = 18;
    srand((unsigned int)time(0));
    FILE* fin0 = fopen(argv[1],"r");
    FILE* fin1 = fopen(argv[2],"r");
    FILE* fout0 = fopen(argv[3],"w");
	FILE* mps = fopen("dat2.mps","w");
    G.input_topo(fin0);
    G.input_demand(fin1);
    fclose(fin0);
    fclose(fin1);
	G.WriteMPSFile(mps);
	fclose(mps);
	G.InitSolver();
	LMT = min(G.Nc,150);
#ifdef TEST_LOCAL973
	puts("Success in building");
 #endif
	/*for(int i = 1; i <= G.Nc; i++){
		if(i == G.Start || i == G.Terminate){
			if(i == G.Start)G._Add_Constraint(i,0);
			else G._Add_Constraint(i,1);
		}
		else{
			if(G.isprimary[i])G._Add_Constraint(i,2);
			else G._Add_Constraint(i,3);
		}
	}
	double row1[10];
	int colno1[10];
	for(int i = 1; i <= G.Nc; i++){
		for(int j = 1; j <= G.Nc; j++){
			if(i==j)continue;
			if(G.mat[i][j] != inf && G.mat[j][i] != inf){
				int ij = G.getedge[i*1000+j]+1, ji = G.getedge[j*1000+i]+1;
				row1[0]=row1[1] = 1;
				colno1[0]=ij,colno1[1]=ji;
				build.addRow(2,colno1,row1,0,1);
			}
		}
	}				
	solver->loadFromCoinModel(build);
	*/
	solver1.readMps("dat2.mps","");
	OsiSolverInterface *solver2(&solver1);
	CbcModel mip(*solver2);
	//mip.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
	mip.setMaximumSeconds(9.7-(clock()-t1)/CLOCKS_PER_SEC);
//CbcModel model(*solver);
  model.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);


  // Set up some cut generators and defaults
  // Probing first as gets tight bounds on continuous

  CglProbing generator1;
  generator1.setUsingObjective(true);
  generator1.setMaxPass(3);
  generator1.setMaxProbe(100);
  generator1.setMaxLook(50);
  generator1.setRowCuts(3);
  //  generator1.snapshot(*model.solver());
  //generator1.createCliques(*model.solver(),2,1000,true);
  //generator1.setMode(0);

  CglGomory generator2;
  // try larger limit
  generator2.setLimit(300);

  CglKnapsackCover generator3;

  CglOddHole generator4;
  generator4.setMinimumViolation(0.005);
  generator4.setMinimumViolationPer(0.00002);
  // try larger limit
  generator4.setMaximumEntries(200);

  CglClique generator5;
  generator5.setStarCliqueReport(false);
  generator5.setRowCliqueReport(false);

  CglMixedIntegerRounding mixedGen;
  CglFlowCover flowGen;
  
  // Add in generators
  model.addCutGenerator(&generator1,-1,"Probing");
  model.addCutGenerator(&generator2,-1,"Gomory");
  model.addCutGenerator(&generator3,-1,"Knapsack");
  model.addCutGenerator(&generator4,-1,"OddHole");
  model.addCutGenerator(&mixedGen,-1,"MixedIntegerRounding");

  OsiClpSolverInterface * osiclp = dynamic_cast< OsiClpSolverInterface*> (model.solver());
  // go faster stripes
  if (osiclp->getNumRows()<300&&osiclp->getNumCols()<500) {
    osiclp->setupForRepeatedUse(2,0);
    printf("trying slightly less reliable but faster version (? Gomory cuts okay?)\n");
    printf("may not be safe if doing cuts in tree which need accuracy (level 2 anyway)\n");
  }

  // Allow rounding heuristic

  CbcRounding heuristic1(model);
  model.addHeuristic(&heuristic1);

  // And local search when new solution found

  CbcHeuristicLocal heuristic2(model);
  model.addHeuristic(&heuristic2);


  // Do initial solve to continuous
  model.initialSolve();

  // Could tune more
  model.setMinimumDrop(CoinMin(1.0,
			     fabs(model.getMinimizationObjValue())*1.0e-3+1.0e-4));

  if (model.getNumCols()<500)
    model.setMaximumCutPassesAtRoot(-100); // always do 100 if possible
  else if (model.getNumCols()<5000)
    model.setMaximumCutPassesAtRoot(100); // use minimum drop
  else
    model.setMaximumCutPassesAtRoot(20);
  //model.setMaximumCutPasses(5);

  // Switch off strong branching if wanted
  // model.setNumberStrong(0);
  // Do more strong branching if small
  if (model.getNumCols()<5000)
    model.setNumberStrong(10);
	mip.initialSolve();
	mip.branchAndBound();
	memcpy(ans,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	printf("Result is %s\n",mip.isProvenOptimal()?"OPTIMAL":"NOT OPTIMAL");
	bool ok1;
	while((ok1=(hasring(mip,ans)) )){
		if(clock()-t1>9.7*CLOCKS_PER_SEC)break;
		mip.setMaximumSeconds(9.7-(clock()-t1)/CLOCKS_PER_SEC);
		mip.branchAndBound();
		memcpy(ans,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	}
	if(mip.getSolutionCount()>0){
		Print_Answer(fout0,mip);
	}
	else{
		fprintf(fout0,"NA");
	}
	fclose(fout0);
    return 0;
}
