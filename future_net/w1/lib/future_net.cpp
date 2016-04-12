#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <fstream>
#include <set>
#include <map>
#include <ctime>
#include <queue>
#include <stack>
#include "./lib/lp_lib.h"
#include "./lib/defines.h"
//#include "defines.h"
#define DFS_MAXCNT 97000000
//Real should be 3e6
using namespace std;
struct _line{
	int num,u,v,val;
};
int Route[800],MinRoute[800];
struct miniGraph{
public:
	int mat[maxn][maxn],degI[maxn];
	int Priority[maxn];
	miniGraph(){
		memset(mat,0,sizeof(mat));
		memset(degI,0,sizeof(degI));
		memset(Priority,1,sizeof(Priority));
	}
	void init(){
		memset(mat,0,sizeof(mat));
		memset(degI,0,sizeof(degI));
		memset(Priority,1,sizeof(Priority));
	}
	void add_edge(int u,int v){
		degI[v] ++;
		mat[u][v] = 1;
	}
	void topo(int l,int r){
		//memset(mat,0,sizeof(mat));
		queue<int> QQ;
		//printf("L = %d , R= %d\n",l,r);
		while(!QQ.empty())QQ.pop();
		for(int i = l; i <= r; i++){
			if(degI[i] == 0){
				QQ.push(i);
				Priority[i] = 1;
			}
		}
		while(!QQ.empty()){
			int i = QQ.front();
			//printf("%d is front.\n",i);
			for(int j = l; j <= r; j++){
                //printf("deg of %d is %d\n",j,degI[j]);
			}
			for(int j = l; j <= r; j++){
				if(mat[i][j]){
					degI[j] --;
					mat[i][j] = 0;
					if(degI[j] == 0){
                        Priority[j] = Priority[i] + 1;
                        QQ.push(j);
					}
				}
				//mat[i][j] = 0;
			}
			QQ.pop();
		}
	}
}G2;
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
	bool findpath(int pres,int dem,int* limit,int* limite,int* Path);
	void fillGraph2();
	void getTarjan(int x);
	void doTarjan();
	int checkvalid(int* Path);
	int countpath(vector<int> V);
	void erasepoint(int x);
	int cal_distT(int p);
	void _Add_Constraint(int p,char type);
}G;
int Graph::cal_distT(int p){
    if(p == Terminate)return dist_to_terminate[p] = 0;
    if(dist_to_terminate[p] != 0x33)return dist_to_terminate[p];
    for(vector<int>::iterator it = E[p].begin(); it != E[p].end(); it++){
        dist_to_terminate[p] = min(dist_to_terminate[p],cal_distT(*it)+1);
    }
    return dist_to_terminate[p];
}
bool cmp_pn_1(int x,int y){
    return x > y;
}
bool cmp_pn_2(int x,int y){
    return rand() < rand();
}
bool cmp_pn_3(int x,int y){
    return G.dist_to_terminate[x] < G.dist_to_terminate[y];
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
void Graph::getTarjan(int x){
    dfn[x] = low[x] = (++TM);
    ST.push(x);
    instack[x] = true;
    vis_tar[x] = true;
    for(int i = 1; i <= Nc; i++){
        if(mat[x][i] != inf){
            if(!vis_tar[i]){
                getTarjan(i);
                low[x] = min(low[x],low[i]);
            }
            else{
                if(instack[i]){
                    low[x] = min(low[x],dfn[i]);
                }
            }
        }
    }
    if(low[x] == dfn[x]){
        STRONG_CONNECT ++;
        while(ST.top() != x){
            belong[ST.top()] = STRONG_CONNECT;
            instack[ST.top()] = false;
            ST.pop();
        }
        belong[x] = STRONG_CONNECT;
        instack[x] = false;
        ST.pop();
    }
}
void Graph::doTarjan(){
    memset(vis_tar,0,sizeof(vis_tar));
    memset(instack,0,sizeof(instack));
    memset(belong,0,sizeof(belong));
    TM = STRONG_CONNECT = 0;
    for(int i = 1;i <= Nc; i++){
        ;if(belong[i] == 0)getTarjan(i);
    }
}
void Graph::fillGraph2(){
    G2.init();
    for(int i = 1; i <= Nc; i++){
        for(vector<int>::iterator it = E[i].begin(); it != E[i].end(); it++){
            if(belong[i] != belong[*it] && !G2.mat[belong[i]][belong[*it]])G2.add_edge(belong[i],belong[*it]);
        }
    }
    G2.topo(1,STRONG_CONNECT);
    for(int i = 1; i <= Nc; i++){
        Priority[i] = G2.Priority[belong[i]];
    }
}
void printgraph(Graph g){
    for(int i = 1; i <= g.Nc; i++){
        for(int j = 1; j <= g.Nc; j++){
            printf("%d ",g.mat[i][j]);
        }
        puts("");
    }
    puts("");
    for(int i = 1; i <= g.Nc; i++){
        for(int j = 0; j < g.E[i].size(); j++){
            printf("%d ",g.E[i][j]);
        }
        puts("");
    }
}
void printpath(int* Path,FILE* fout){
    fputs("",fout);
    for(int i = 1; i <= Path[0]; i++){
        if(i != 1)fputc('|',fout);
        fprintf(fout,"%d",Path[i]);
    }
    fprintf(fout,"\n");
}
int Graph::checkvalid(int* Path){
    if(Path[0] == 0)return 2;
    if(Path[700] == 0)return 3;
    bool visited[maxn];
    visited[line[Path[1]].u] = true;
    memset(visited,0,sizeof(visited));
    for(int i = 1; i <= Path[0]; i++){
        if(visited[line[Path[1]].v])return 1;
        visited[line[Path[1]].v] = true;
    }
    int demcnt = 0;
    if(isprimary[line[Path[1]].u])demcnt ++;
    for(int i = 1; i <= Path[0]; i++){
        if(isprimary[line[Path[1]].v])demcnt ++;
    }
    if(demcnt != DEMc)return 4;
    return 0;
}
int Graph::countpath(vector<int> V){
    int sum = 0;
    for(int i = 0; i < V.size() - 1; i++){
        sum += mat[V[i]][V[i+1]];
    }
    return sum;
}
void Graph::erasepoint(int x){
    E[x].clear();
    for(int i = 1; i <= Nc; i++){
        if(mat[i][x] != inf){
            for(vector<int>::iterator it = E[i].begin(); it != E[i].end(); it++){
                if(*it == x){
                    E[i].erase(it);
                    break;
                }
            }
        }
    }
    for(int i = 1; i <= Nc; i++){
        mat[i][x] = mat[x][i] = inf;
    }
}
bool Dealing1(Graph g){
    for(int i = 1; i <= g.Nc; i++){
        if(!g.isprimary[i])continue;
        for(int j = 1; j <= g.Nc; j++){
            if(!g.isprimary[j])continue;
            if(g.Priority[i] == g.Priority[j] && g.belong[i] != g.belong[j])return false;
        }
    }
    bool hasPrimary[maxn];
    memset(hasPrimary,0,sizeof(hasPrimary));
    for(int i = 1; i <= g.Nc; i++){
        if(g.isprimary[i]){
            hasPrimary[g.Priority[i]] = true;
        }
    }
    for(int i = 1; i <= g.Nc; i++){
        if(!g.isprimary[i] && hasPrimary[g.Priority[i]])g.erasepoint(i);
    }
    return true;
}
lprec* lp;
void Graph::_Add_Constraint(int p,char type){
	//memset(row,0,sizeof(row));
	//0: Start Point
	//1: Terminate Point
	//2: Designated Point
	//3: Normal Point
	int cc = E[p].size(), rcc = RE[p].size();
	puts("");
	puts("***********************");
	printf("Node %d. CC = %d , RCC = %d\n",p,cc,rcc);
	puts("CC:");
	for(int i = 0; i < cc; i++){
		printf("%d -> %d : Line Number %d.\n",p,E[p][i],F[p][i]);
	}
	puts("RCC:");
	for(int i = 0; i < rcc; i++){
		printf("%d -> %d : Line Number %d.\n",RE[p][i],p,RF[p][i]);
	}
	puts("***********************");
	puts("");
	REAL row[5000];
	int colno[20];
	if(type == 0){
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < rcc; i++){
			row[i] = 1;
			colno[i] = RF[p][i]+1;
			printf("COLNO[%d] = %d,\n",i,colno[i]);
		}
		for(int i = 1; i <= rcc; i++){
			printf("Variable %d : Line No.%d , Multiple %.2lf\n",i,colno[i],row[i]);
		}
		add_constraintex(lp,rcc,row,colno,EQ,0);
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < cc; i++){
			row[i] = 1;
			colno[i] = F[p][i]+1;
			printf("COLNO[%d] = %d.\n",i,colno[i]);
		}
		
		for(int i = 1; i <= cc; i++){
			printf("Variable %d : Line No.%d , Multiple %.2lf\n",i,colno[i],row[i]);
		}
		add_constraintex(lp,cc,row,colno,EQ,1);
	}
	if(type == 1){
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < rcc; i++){
			row[i] = 1;
			colno[i] = RF[p][i]+1;
		}
		add_constraintex(lp,rcc,row,colno,EQ,1);
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < cc; i++){
			row[i] = 1;
			colno[i] = F[p][i]+1;
		}
		add_constraintex(lp,cc,row,colno,EQ,0);
	}
	if(type == 2){
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < rcc; i++){
			row[i] = 1;
			colno[i] = RF[p][i]+1;
		}
		add_constraintex(lp,rcc,row,colno,EQ,1);
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < cc; i++){
			row[i] = 1;
			colno[i] = F[p][i]+1;
		}
		add_constraintex(lp,cc,row,colno,EQ,1);
	}
	if(type == 3){
		memset(row,0,sizeof(row));
		memset(colno,0,sizeof(colno));
		for(int i = 0; i < rcc; i++){
			row[i] = 1;
			colno[i] = RF[p][i]+1;
		}
		add_constraintex(lp,rcc,row,colno,LE,1);
		for(int i = 0; i < cc; i++){
			row[rcc+i] = -1;
			colno[rcc+i] = F[p][i]+1;
		}
		add_constraintex(lp,rcc+cc,row,colno,EQ,0);
	}
}
int main(int argc,char* argv[]){
	REAL row[9000];
    srand((unsigned int)time(0));
    FILE* fin0 = fopen(argv[1],"r");
    FILE* fin1 = fopen(argv[2],"r");
    FILE* fout0 = fopen(argv[3],"w");
    G.input_topo(fin0);
    G.input_demand(fin1);
    G.doTarjan();
    G.fillGraph2();
    fclose(fin0);
    fclose(fin1);
    if(!Dealing1(G)){
        fprintf(fout0,"NA");
        fclose(fout0);
        return 0;
    }
    lp = make_lp(0,G.Ec);
	if(lp == NULL){
		printf("FKU,Unable to create LP model\n");
		return 0;
	}
	puts("Success in building");
	memset(row,0,sizeof(row));
	for(int i = 1; i <= G.Ec; i++){
		row[i] = G.line[i-1].val;
		set_binary(lp,i,TRUE);
	}
	set_obj_fn(lp,row);
	set_timeout(lp,9);
	set_add_rowmode(lp,TRUE);
	for(int i = 1; i <= G.Nc; i++){
		if(i == G.Start || i == G.Terminate){
			if(i == G.Start)G._Add_Constraint(i,0);
			else G._Add_Constraint(i,1);
		}
		else{
			if(G.isprimary[i])G._Add_Constraint(i,2);
			else G._Add_Constraint(i,3);
		}
	}
	//set_print_sol(lp,2);
	set_add_rowmode(lp,FALSE);
	set_debug(lp,TRUE);
	print_lp(lp);
	//print_duals(lp);
	//puts("*********************");
	//puts("*********************");
	int ret = solve(lp);
	//print_solution(lp,1);
	if(ret == SUBOPTIMAL || ret == OPTIMAL){
		fprintf(fout0,"GET A ANSWER");
	}
	else{
		if(ret == TIMEOUT)fprintf(fout0,"TIMEOUT");
		if(ret == UNBOUNDED)fprintf(fout0,"UNBOUNDED");
	}
	delete_lp(lp);
	//fprintf(fout0,"\n%d",MinRoute[700]);
    fclose(fout0);
    return 0;
}
