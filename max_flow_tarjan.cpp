#include <iostream>
#include <vector>
#include <queue>

using namespace std;

const int MN = 1003;

using ll = long long;

const ll inf = 1e18;

ll w[MN][MN]; //capacity of edje
ll ecc[MN]; //eccess of node
int label[MN];
vector<int> g[MN]; //graph description (with backedjes)

int n,m;

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cin>>n>>m;
	for(int i=0; i<m; ++i){
		int u,v,wt;
		cin>>u>>v>>wt;
		if(u==v)continue;
		if(u==1)ecc[1]+=wt;
		if(!w[u][v]){
			g[u].push_back(v);
			g[v].push_back(u);
		}
		w[u][v]+=wt;
	}
	label[1]=n;
	auto comp = [&](int a, int b){return label[a]<label[b];};
	priority_queue<int,vector<int>, decltype(comp) > pq(comp);
	pq.push(1);
	while(!pq.empty()){
		int i=pq.top();
		pq.pop();
		while(ecc[i]){
			for(auto j:g[i]){
				if(w[i][j]&&(i==1 || label[i]==label[j]+1)){
					ll tp = min(w[i][j],ecc[i]);
					ecc[i]-=tp;
					ecc[j]+=tp;
					w[i][j]-=tp;
					w[j][i]+=tp;
					if(j!=1&&j!=n){
						pq.push(j);
					}
				}
			}
			if(ecc[i]>0){
				label[i]=(n<<1)|1;
				for(auto j:g[i])
					if(w[i][j])label[i]=min(label[i],label[j]+1);
			}
		}
	}
	cout<<ecc[n]<<endl;
	return 0;
}
