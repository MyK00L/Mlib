#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using ll = long long;

//using fermat little theorem with mod prime
// a^-1 = a^m-2 mod m
ll pow(ll b, ll e, ll mod){
	b%=mod;
	ll res=1;
	while(e){
		if(e&1)res=(res*b)%mod;
		b=(b*b)%mod;
		e>>=1;
	}
	return res;
}

//using extended euclidean algorithm
pair<ll,ll> diofante(ll a, ll b){
	if(a==0)return {0,1};
	auto t = diofante(b%a,a);
	return {t.second-(b/a)*t.first,t.first};
}
ll modinv(ll x, ll mod){
	return (diofante(x,mod).first%mod+mod)%mod;
}

//get first n inverses in O(n) with mod prime
vector<ll> get_inv(int n, ll mod){
	vector<ll> res(n+1);
	res[1]=1;
	for(int i=2; i<=n; ++i){
		res[i]=(mod-(mod/i)*res[mod%i]%mod)%mod;
	}
	res[0]=1;
	return res;
}

// https://cp-algorithms.com/algebra/module-inverse.html
int main(){
	ll n,m;
	cin>>n>>m;
	
	//these 3 are the same if m is prime
	cout<<modinv(n,m)<<endl; //this works if n and m are coprime
	cout<<get_inv(n,m).back()<<endl; //this works if m is prime
	cout<<pow(n,m-2,m)<<endl; //this works if m is prime

	return 0;
}

