#include <random>
#include <utility>
#include <chrono>
#include <iostream>

using namespace std;

mt19937_64 rng;

template<typename T>
struct treap{
	struct node{
		node():l(0),r(0),f(0),y(rng()),siz(1),rot(0){}
		node * l;
		node * r;
		node * f;
		T v;
		unsigned long long y;
		int siz;
		bool rot;
	};
	node * nn;
	treap(node * nn):nn(nn){
		//rng.seed(chrono::steady_clock::now().time_since_epoch().count());
	}
	int siz(node * p){
		return p?p->siz:0;
	}
	void prop(node * p){
		if(!p)return;
		p->siz=1+siz(p->l)+siz(p->r);
		if(p->rot){
			if(p->l)p->l->rot^=1;
			if(p->r)p->r->rot^=1;
			swap(p->l,p->r);
			p->rot=0;
		}
		if(p->l)p->l->f=p;
		if(p->r)p->r->f=p;
	}
	T find(node * p, int pos){
		if(siz(p->l)<pos)return find(p->r,pos-siz(p->l)-1);
		if(siz(p->l)>pos)return find(p->l,pos);
		return p->v;
	}
	pair<int,node*> getposroot(node * p){
		pair<int,node*> res = {siz(p->l),0};
		while(p->f){
			if(p==p->f->r){
				res.first+=siz(p->f->l)+1;
			}
			p=p->f;
		}
		res.second=p;
		return res;
	}
	pair<node*, node*> split(node* p, int pos){
		if(!p)return {0,0};
		prop(p);
		pair<node*,node*> res;
		if(siz(p->l)>pos){
			auto t = split(p->l,pos);
			p->l=t.second;
			res = {t.first,p};
		}
		else if(siz(p->l)<pos){
			auto t = split(p->r,pos-siz(p->l)-1);
			p->r=t.first;
			res = {p,t.second};
		} else {
			res = {p->l,p};
			p->l=0;
		}
		prop(res.first);
		prop(res.second);
		return res;
	}
	node * merge(node * l, node * r){
		if(!l)return r;
		if(!r)return l;
		prop(l);
		prop(r);
		if(l->y<r->y){
			l->r=merge(l->r,r);
			prop(l);
			return l;
		} else {
			r->l=merge(l,r->l);
			prop(r);
			return r;
		}
	}
	node * newnode(T v){
		nn->v = v;
		return nn++;
	}
	node * insert(node * p, int pos, T v){
		auto t = split(p,pos);
		return merge(merge(t.first,newnode(v)),t.second);
	}
	node * erase(node * p, int pos){
		auto t1 = split(p,pos);
		auto t2 = split(t1.second,1);
		return merge(t1.first,t2.second);
	}
	node * rotate(node * p, int posl, int posr){
		posr++;
		auto t2 = split(p,posr);
		auto t1 = split(t2.first,posl);
		if(t1.second)t1.second->rot ^= 1;
		return merge(merge(t1.first,t1.second),t2.second);
	}
	void inorder_cerr(node * p){
		if(!p)return;
		prop(p);
		inorder_cerr(p->l);
		cerr<<p->v<<' ';
		inorder_cerr(p->r);
	}
	void debug(node * p){
		inorder_cerr(p);
		cerr<<endl;
	}
};

const int MN = 1003;

treap<int>::node data[MN];
treap<int>::node * data_it;

int main(){
	rng.seed(chrono::steady_clock::now().time_since_epoch().count());
	data_it = data;
	treap<int> trp(data_it);
	treap<int>::node * root = 0;
	while(1){
		char c;
		cin>>c;
		if(c=='i'){
			int pos,v;
			cin>>pos>>v;
			root = trp.insert(root,pos,v);
		} else if(c=='e'){
			int pos;
			cin>>pos;
			root = trp.erase(root,pos);
		} else if (c=='r'){
			int posl,posr;
			cin>>posl>>posr;
			root = trp.rotate(root,posl,posr);
		} else if (c=='f'){
			int pos;
			cin>>pos;
			cout<<trp.find(root,pos)<<endl;
		} else if(c=='t'){
			int i;
			cin>>i;
			cout<<"V: "<<data[i].v<<endl;
			auto t = trp.getposroot(&data[i]);
			cout<<"res: "<<t.first<<' '<<t.second<<endl;
			cout<<"find: "<<trp.find(t.second,t.first)<<endl;
		} else if(c=='!'){
			break;
		}
		trp.debug(root);
	}
	return 0;
}

