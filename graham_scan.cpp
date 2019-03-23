#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct P{
	long long x;
	long long y;
};

const int MN = 1000006;
int n;
P a[MN];

long long cross(P a, P b, P c){
	return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
long long dist2(P a, P b){
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

int main(){
	//triangoli pienotti
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	cin>>n;
	for(int i=0; i<n; ++i)cin>>a[i].x>>a[i].y;
	for(int i=1; i<n; ++i){
		if((a[i].x==a[0].x&&a[i].y<a[0].y) || a[i].x<a[0].x)swap(a[0],a[i]);
	}
	sort(a+1,a+n,
	[&](P b, P c){
		if(cross(a[0],b,c)==0){
			return dist2(a[0],b)<dist2(a[0],c);
		}
		return cross(a[0],b,c)>0;	
	});
	vector<P> ch;
	ch.push_back(a[0]);
	ch.push_back(a[1]);
	for(int i=2; i<n; ++i){
		while(ch.size()>=2 && 
cross(ch[ch.size()-2],ch.back(),a[i])<=0){
			ch.pop_back();
		}
		ch.push_back(a[i]);
	}
	cout<<ch.size()<<endl;
	return 0;
}
