#include <iostream>
#include <utility>

using namespace std;
using ll = long long;

pair<ll,ll> diofante(ll a, ll b){
	if(a==0)return {0,1};
	auto t = diofante(b%a,a);
	return {t.second-(b/a)*t.first,t.first};
}

ll modinv(ll x, ll mod){
	return (diofante(x,mod).first%mod+mod)%mod;
}

ll gcd(ll a, ll b){
	return a?gcd(b%a,a):b;
}

int main(){
	ll a,b;
	cin>>a>>b;
	cout<<modinv(a,b)<<endl;
	cout<<gcd(a,b)<<endl;
	return 0;
}

