#include <iostream>
#include <vector>
#include <queue>
#include <cinttypes>

using namespace std;

using ll = int64_t;
class TMF { // s=0 t=n-1
	private:
		size_t n;
		vector<vector<ll> > w; // capacity of edge
		vector<ll> ecc; // eccess of node
		vector<int> label; // label
		vector<vector<size_t> > g; // graph with backedges
	public:
		TMF(size_t n) :n(n),w(n,vector<ll>(n,0ll)),ecc(n,0ll),label(n,0),g(n)
		{}
		void add_edge(size_t u, size_t v, ll wt) {
			if(u==v) return;
			if(u==0) ecc[0]+=wt;
			if(!w[u][v] && !w[v][u]) {
				g[u].push_back(v);
				g[v].push_back(u);
			}
			w[u][v]+=wt;
		}
		ll flow() {
			label[0]=n;
			auto comp = [&](size_t a, size_t b){return label[a]<label[b];};
			priority_queue<size_t,vector<size_t>, decltype(comp) > pq(comp);
			pq.push(0);
			while(!pq.empty()){
				size_t i=pq.top();
				pq.pop();
				while(ecc[i]){
					for(auto j:g[i]){
						if(w[i][j]&&(i==0 || label[i]==label[j]+1)){
							ll tp = min(w[i][j],ecc[i]);
							ecc[i]-=tp;
							ecc[j]+=tp;
							w[i][j]-=tp;
							w[j][i]+=tp;
							if(j!=0&&j!=n-1){
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
			return ecc[n-1];
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

