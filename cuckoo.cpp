#include <iostream>
#include <functional>
#include <unordered_set>
#include <random>
#include <utility>

using namespace std;

template <typename T>
class Cuckoo{
public:
	Cuckoo(int n, T placeholder):_size(stress(n)),_empty(placeholder){
		_dataf=(T*)malloc(sizeof(T)*_size);
		_datag=(T*)malloc(sizeof(T)*_size);
		for(size_t i=0; i<_size; ++i)_dataf[i]=_datag[i]=_empty;
	}
	Cuckoo(T placeholder):Cuckoo(4,placeholder){}
	void insert(T v){
		size_t fh=f(v);
		size_t gh=g(v);
		if(_dataf[fh]==v || _datag[gh]==v)return;
		if(_dataf[fh]==_empty){
			_dataf[fh]=v;
			return;
		}
		if(_datag[gh]==_empty){
			_datag[gh]=v;
			return;
		}
		T inserting = v;
		swap(v,_datag[gh]);
		while(v!=_empty&&v!=inserting){
			swap(v,_dataf[f(v)]);
			if(v==_empty || v==inserting) break;
			swap(v,_datag[g(v)]);
		}
		if(v==inserting){
			T * old_dataf = _dataf;
			T * old_datag = _datag;
			size_t old_size = _size;
			_size<<=1;
			_dataf = (T*)malloc(sizeof(T)*_size);
			_datag = (T*)malloc(sizeof(T)*_size);
			for(size_t i=0; i<_size; ++i)_dataf[i]=_datag[i]=_empty;
			for(size_t i=0; i<old_size; ++i){
				if(old_dataf[i]!=_empty)insert(old_dataf[i]);
				if(old_datag[i]!=_empty)insert(old_datag[i]);
			}
			insert(inserting);
			free(old_dataf);
			free(old_datag);
		}
	}
	void erase(T v){
		size_t fh = f(v);
		size_t gh = g(v);
		if(_dataf[fh]==v)_dataf[fh]=_empty;
		else if(_datag[gh]==v)_datag[gh]=_empty;
	}
	bool find(T v){
		return ((_dataf[f(v)]==v) | (_datag[g(v)]==v));
	}
	void debug(){
		size_t i;
		cerr<<"f: ";
		for(i=0; i<_size; ++i)cerr<<_dataf[i]<<' ';
		cerr<<endl;
		cerr<<"g: ";
		for(i=0; i<_size; ++i)cerr<<_datag[i]<<' ';
		cerr<<endl;
	}
private:
	size_t stress(size_t x){
		return (x*3)>>1;
	}
	size_t f(T& x){
		return hash<T>{}(x)%_size;
	}
	size_t g(T& x){
		return (hash<T>{}(x)/_size)%_size;
	}
	T * _dataf;
	T * _datag;
	size_t _size;
	T _empty;
};

int main(){
	Cuckoo<int> c(-1);
	unordered_set<int> s;
	int n,m,i,cm,a;
	cin>>n>>m;
	mt19937 rng(42);
	/*
	for(i=0; i<m; ++i){
		//c.debug();
		cm=rng()%3;
		a=rng()%n;
		//cin>>cm>>a;
		if(cm==0){
			//cerr<<"find "<<a<<' '<<c.find(a)<<' '<<s.count(a)<<endl;
			if(c.find(a)!=s.count(a)) cerr<<"FUCK"<<endl;
		} else if(cm==1){
			//cerr<<"erase "<<a<<endl;
			c.erase(a);
			s.erase(a);
		} else {
			//cerr<<"insert "<<a<<endl;
			c.insert(a);
			s.insert(a);
		}
	}
	*/
	timespec st;
	timespec ed;
	
	rng.seed(666);
	clock_gettime(CLOCK_REALTIME,&st);
	for(i=0; i<m; ++i){
		cm=rng()%3;
		a=rng()%n;
		if(cm==0){
			c.find(a);
		} else if(cm==1){
			c.erase(a);
		} else {
			c.insert(a);
		}
	}
	clock_gettime(CLOCK_REALTIME,&ed);
	cout<<"Cuckoo: "<<ed.tv_nsec-st.tv_nsec<<endl;

	rng.seed(666);
	clock_gettime(CLOCK_REALTIME,&st);
	for(i=0; i<m; ++i){
		cm=rng()%3;
		a=rng()%n;
		if(cm==0){
			s.count(a);
		} else if(cm==1){
			s.erase(a);
		} else {
			s.insert(a);
		}
	}
	clock_gettime(CLOCK_REALTIME,&ed);
	cout<<"unordered_set: "<<ed.tv_nsec-st.tv_nsec<<endl;
	return 0;
}
