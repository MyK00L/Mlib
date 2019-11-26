#include <iostream>
#include <vector>

using namespace std;
using ll = long long;

pair<ll,ll> diofante(ll a, ll b){
	if(!a)return {0,1};
	auto t = diofante(b%a,a);
	return {t.second-(b/a)*t.first,t.first};
}
ll modinv(ll x, ll m){
	return (diofante(x,m).first%m+m)%m;
}

template<class T, ll a, ll mod>
struct prefix_hash{
	int n;
	vector<ll> a_inv;
	vector<ll> psum;
	prefix_hash(T arr, int n):n(n),a_inv(n),psum(n+1){
		a_inv[0]=1;
		a_inv[1]=modinv(a,mod);
		for(int i=2; i<n; ++i){
			a_inv[i]=(a_inv[1]*a_inv[i-1])%mod;
		}
		psum[n]=0;
		psum[n-1]=arr[n-1];
		ll powa=a;
		for(int i=n-2; i>=0; --i){
			psum[i]=(psum[i+1]+powa*arr[i]%mod)%mod;
			powa=(powa*a)%mod;
		}
	}
	ll operator()(int l, int r){
		return (((psum[l]-psum[r+1]+mod)%mod)*a_inv[n-r-1])%mod;
	}
};

// https://training.olinfo.it/#/task/unimi_glitch/statement
#include <unordered_set>
int main(){
	int n;
	string s;
	cin>>n>>s;
	prefix_hash<string,103,10000007> pa1(s,n);
	prefix_hash<string,257,10000009> pa2(s,n);
	auto feas = [&](int m)->bool{
		unordered_set<ll> us;
		for(int i=0; i+m-1<n; ++i){
			int h1=pa1(i,i+m-1),h2=pa2(i,i+m-1);
			ll htot = ((ll)h1<<32)|h2;
			if(us.count(htot))return 1;
			us.insert(htot);
		}
		return 0;
	};
	int lb=0,ub=n,m;
	while(lb<ub-1){
		m=(lb+ub)/2;
		if(feas(m))lb=m;
		else ub=m;
	}
	cout<<lb<<endl;
	return 0;
}

