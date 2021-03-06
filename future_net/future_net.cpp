#include "./lib/ALib.hpp"
//#define TEST_LOCAL973 233
#define YStart G.Ec
#define isnormal(x) !G.isprimary[x]&&x!=G.Start&&x!=G.Terminate
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
	printf("writecol\n");
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
/* End */
/* ********************************
   *******************************
   ******************************
   *****************************
*/
double ans[20000];
void Print_Answer(FILE *fout,CbcModel mip){
	int pres = G.Start;
	int ConstraintC = mip.getNumRows(), VariableC = mip.getNumCols();
	int longsam = (int)(mip.getObjValue());
	printf("Longsam = %d\n",longsam);
	//memcpy(x,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	const double *x = mip.getColSolution();
	puts("CPY FINISH");
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
int main(int argc,char* argv[]){
	freopen("logs.txt","w",stdout);
	double row[90];
	int colno[30];
	int _TIME = 18;
	
	static OsiClpSolverInterface solver1;
	static OsiSolverInterface *solver = &solver1;
	static CoinModel build;
    srand((unsigned int)time(0));
    FILE* fin0 = fopen(argv[1],"r");
    FILE* fin1 = fopen(argv[2],"r");
    FILE* fout0 = fopen(argv[3],"w");
	FILE* mps = fopen("dat3.mps","w");
    G.input_topo(fin0);
    G.input_demand(fin1);
    fclose(fin0);
    fclose(fin1);
	int LMT = G.Nc;
	for(int i = 0; i < G.Ec; i++){
		build.setColumnBounds(i,0,1);
		build.setObjective(i,G.line[i].val);
		build.setInteger(i);
	}
	for(int i = G.Ec; i < G.Ec+G.Nc; i++){
		build.setInteger(i);
		build.setObjective(i,0.0);
		int rela_v = i-G.Ec+1;
		if(rela_v == G.Start){
			build.setColumnBounds(i,1,1);
		}
		else{
			if(rela_v == G.Terminate || G.isprimary[rela_v]){
				build.setColumnBounds(i,1,LMT);
			}
			else{
				build.setColumnBounds(i,0,LMT);
			}
		}
	}
	for(int i = G.Ec+G.Nc; i < G.Ec+G.Nc*3; i++){
		build.setInteger(i);
		build.setObjective(i,0.0);
	}
	for(int i = 1; i <= G.Nc; i++){
		int delta=G.Nc+G.Ec-2;
		if(isnormal(i)){
			build.setColumnBounds(2*i+delta,0,1);
			build.setColumnBounds(2*i+delta+1,0,1);
			row[0]=1,row[1]=-1;
			colno[0]=2*i+delta,colno[1]=2*i+delta+1;
			build.addRow(2,colno,row,0,0);
		}
		else{
			int ci = 1,co = 1;
			if(i == G.Start)ci--;
			if(i == G.Terminate)co--;
			build.setColumnBounds(2*i+delta,ci,ci);
			build.setColumnBounds(2*i+delta+1,co,co);
		}
	}			
#ifdef TEST_LOCAL973
	puts("Success in building");
 #endif
	puts("WOC");
	for(int i = 1; i <= G.Nc; i++){
		printf("I=%d\n",i);
		int rcc = G.RF[i].size(), cc = G.F[i].size();
		int delta = G.Nc+G.Ec-2;
		for(int j = 0; j < rcc; j++){
			row[j] = 1;
			colno[j] = G.RF[i][j];
		}
		row[rcc] = -1;
		colno[rcc] = 2*i+delta;
		build.addRow(rcc+1,colno,row,0,0);
		for(int j = 0; j < cc; j++){
			row[j] = 1;
			colno[j] = G.F[i][j];
		}
		row[cc] = -1;
		colno[cc] = 2*i+delta+1;
		build.addRow(cc+1,colno,row,0,0);
	}
	for(int i = 0; i < G.Ec; i++){
		int uu = G.M[G.line[i].u], vv = G.M[G.line[i].v];
		row[0] = 1;
		row[1] = -1;
		row[2] = LMT+1;
		colno[0] = G.Ec-1+uu;
		colno[1] = G.Ec-1+vv;
		colno[2] = i;
		build.addRow(3,colno,row,-inf,LMT);
	}
	for(int i = 1; i <= G.Nc; i++){
		if(i == G.Start || i == G.Terminate)continue;
		int delta = G.Nc + G.Ec - 2;
		row[0] = 1;
		row[1] = -(LMT+1);
		colno[0] = G.Ec-1+i;
		colno[1] = 2*i+delta;
		build.addRow(2,colno,row,-inf,0);
	}
	for(int i = 1; i <= G.Nc; i++){
		for(int j = 1; j <= G.Nc; j++){
			if(i==j)continue;
			if(G.mat[i][j] != inf && G.mat[j][i] != inf){
				int ij = G.getedge[i*1000+j], ji = G.getedge[j*1000+i];
				row[0]=row[1] = 1;
				colno[0]=ij,colno[1]=ji;
				build.addRow(2,colno,row,0,1);
			}
		}
	}				
	solver->loadFromCoinModel(build);
	CbcModel mip(*solver);
	//mip.solver()->setHintParam(OsiDoReducePrint,true,OsiHintTry);
	mip.setMaximumSeconds(9.7);
	mip.initialSolve();
//	puts("Initial Solve FInish.");
	mip.branchAndBound();
//	puts("BAB finish.");
	memcpy(ans,mip.getColSolution(),sizeof(double)*mip.getNumCols());
//	printf("Result is %s\n",mip.isProvenOptimal()?"OPTIMAL":"NOT OPTIMAL");
	bool ok1;
	memcpy(ans,mip.getColSolution(),sizeof(double)*mip.getNumCols());
	if(mip.getSolutionCount()>0){
//		puts("Print Answer!");
		Print_Answer(fout0,mip);
	}
	else{
		fprintf(fout0,"NA");
	}
	fclose(fout0);
    return 0;
}
