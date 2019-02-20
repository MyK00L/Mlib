#include <iostream>
#include <vector>

using namespace std;

template <typename T>
class Avl{
public:
	Avl(T* b, T* e){
		root = build(b,e);
	}
	Avl(vector<T>& v):Avl(v.data(),v.data()+v.size()){}
	Avl():root(0){}
	void erase(int pos){
		root=erase(root,pos);
	}
	void insert(int pos, T v){
		root=insert(root,pos,v);
	}
	T find(int pos){
		return find(root,pos);
	}
	void debug(){
		inorder(root);
		cerr<<endl;
	}
private:
	struct node {
		node(T v):l(0),r(0),v(v),h(1),n(1){}
		node * l;
		node * r;
		T v;
		int h;
		int n;
	};
	inline node * newnode(T v){
		return new node(v);
	}
	inline void deletenode(node * p){
		delete p;
	}
	int h(node * p){return p?p->h:0;}
	int siz(node * p){return p?p->n:0;}
	void calch(node * p){
		p->h=max(h(p->l),h(p->r))+1;
		p->n=siz(p->l)+siz(p->r)+1;
	}
	int bf(node * p){
		return h(p->r)-h(p->l);
	}
	node * rotl(node * p){
		node * f = p->r;
		p->r=f->l;
		f->l=p;
		calch(p);
		calch(f);
		return f;
	}
	node * rotr(node * p){
		node * f = p->l;
		p->l=f->r;
		f->r=p;
		calch(p);
		calch(f);
		return f;
	}
	node * balance(node * p){
		if(!p)return 0;
		calch(p);
		if(bf(p)>=2){
			if(bf(p->r)<0) p->r=rotr(p->r);
			return rotl(p);
		}
		if(bf(p)<=-2){
			if(bf(p->l)>0) p->l=rotl(p->l);
			return rotr(p);
		}
		return p;
	}
	node * insert(node * p, const int pos, const T& v){
		if(!p)return newnode(v);
		if(siz(p->l)<pos) p->r=insert(p->r,pos-siz(p->l)-1,v);
		else p->l=insert(p->l,pos,v);
		return balance(p);
	}
	node * findmin(node * p){
		return p->l?findmin(p->l):p;
	}
	node * removemin(node * p){
		if(!p->l)return p->r;
		p->l=removemin(p->l);
		calch(p);
		return p;
	}
	node * erase(node * p, const int pos){
		if(siz(p->l)<pos) p->r=erase(p->r,pos-siz(p->l)-1);
		else if(siz(p->l)>pos) p->l=erase(p->l,pos);
		else {
			node * l = p->l;
			node * r = p->r;
			deletenode(p);
			if(!r)return l;
			p=findmin(r);
			p->r=removemin(r);
			p->l=l;
		}
		return balance(p);
	}
	T find(node * p, const int pos){
		if(siz(p->l)<pos)return find(p->r,pos-siz(p->l)-1);
		if(siz(p->l)>pos)return find(p->l,pos);
		return p->v;
	}
	node * build(T* b, T* e){
		if(b>=e)return 0;
		T* m = b+((e-b)>>1);
		node * p = newnode(*m);
		p->l=build(b,m);
		p->r=build(m+1,e);
		calch(p);
		return p;
	}
	void inorder(node * p){
		if(!p)return;
		inorder(p->l);
		cerr << '(' << p->v << ' ' << siz(p) << ' ' << (p->l?p->l->v:-1) << ' ' << (p->r?p->r->v:-1) << ") ";
		inorder(p->r);
	}
	node * root;
};

vector<int> v;

int main(){
	//vasi
	ios_base::sync_with_stdio(0);
	cin.tie(0);
	int n,m;
	cin>>n>>m;
	v.resize(n);
	for(int i=0; i<n; ++i)v[i]=i;
	Avl<int> avl(v);
	char c;
	int a,b,v;
	for(;m;--m){
		cin>>c>>a;
		if(c=='c'){
			cout<<avl.find(a)<<' ';
		} else if(c=='s'){
			cin>>b;
			v = avl.find(a);
			avl.erase(a);
			avl.insert(b,v);
		}
	}
	return 0;
}
