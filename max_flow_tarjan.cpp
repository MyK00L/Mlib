#include <iostream>
#include <vector>
#include <queue>
#include <cinttypes>

using namespace std;

using ll = int64_t;
const ll inf = 1e18;
class TMF {
	private:
		size_t n;
		vector<vector<ll> > w; // capacity of edge
		vector<ll> ecc; // eccess of node
		vector<int> label; // label
		vector<vector<size_t> > g; // graph with backedges
	public:
		TMF(size_t n) :n(n),w(n+1,vector<ll>(n+1,0ll)),ecc(n+1,0ll),label(n+1,0),g(n+1)
		{}
		void add_edge(size_t u, size_t v, ll wt) {
			if(u==v) return;
			if(u==1) ecc[1]+=wt;
			if(!w[u][v] && !w[v][u]) {
				g[u].push_back(v);
				g[v].push_back(u);
			}
			w[u][v]+=wt;
		}
		ll flow() {
			label[1]=n;
			auto comp = [&](size_t a, size_t b){return label[a]<label[b];};
			priority_queue<size_t,vector<size_t>, decltype(comp) > pq(comp);
			pq.push(1);
			while(!pq.empty()){
				size_t i=pq.top();
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
			return ecc[n];
		}
};

int main(){
	ios_base::sync_with_stdio(0);
	cin.tie(0);

	size_t n,m;
	cin>>n>>m;
	TMF tmf(n);
	for(size_t i=0; i<m; ++i) {
		size_t u,v;ll w;
		cin>>u>>v>>w;
		tmf.add_edge(u,v,w);
		// tmf.add_edge(v,u,w);
	}
	cout<<tmf.flow()<<endl;
	return 0;
}

