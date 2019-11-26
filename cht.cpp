#include <set>
#include <iostream>

using namespace std;
using ll = long long;
const ll inf = 1e15;

struct cht{
	struct line{
		mutable ll x1;
		ll m;
		ll q;
		bool query;
		bool operator<(const line& o)const{
			return (query||o.query)?x1<o.x1:
				m>o.m; //min cht
				//m<o.m; //max cht
		}
	};
	ll intersect(const line& a, const line& b){
		if(a.m==b.m)return inf;
		return (a.q-b.q)/(b.m-a.m);
	}
	set<line> lines;
	bool useless(set<line>::iterator it){
		if(it==lines.begin()||next(it)==lines.end())return 0;
		return (intersect(*prev(it),*it)>=intersect(*it,*next(it)));
	}
	void recalc(set<line>::iterator it){
		it->x1 = (next(it)==lines.end()?inf:intersect(*it,*next(it)));
		if(it!=lines.begin())prev(it)->x1 = intersect(*prev(it),*it);
	}
	void insert(ll m, ll q){
		line nl = line{-1,m,q,0};
		auto it = lines.insert(nl).first;
		if(useless(it)){
			lines.erase(it);
			return;
		}
		while(next(it)!=lines.end() && useless(next(it)))it=prev(lines.erase(next(it)));
		while(it!=lines.begin() && useless(prev(it)))it=lines.erase(prev(it));
		recalc(it);
	}
	set<line>::iterator find(ll x){
		line query = line{x,0,0,1};
		return lines.lower_bound(query);
	}
	ll at(ll x){
		auto t = find(x);
		if(t==lines.end())return 666;
		return (t->m)*x+t->q;
	}
	void debug(){
		for(auto i: lines){
			cerr<<i.m<<' '<<i.q<<' '<<i.x1<<endl;
		}
	}
};

// https://codeforces.com/problemset/problem/319/C
#include <vector>
int main(){
	int n;
	cin>>n;
	vector<ll> a(n);
	vector<ll> b(n);
	for(auto& i:a)cin>>i;
	for(auto& i:b)cin>>i;
	cht c;
	c.insert(b[0],0);
	for(int i=1; i<n-1; ++i){
		ll time = c.at(a[i]);
		c.insert(b[i],time);
	}
	cout<<c.at(a[n-1])<<endl;
	return 0;
}

