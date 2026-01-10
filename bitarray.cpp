#include <cstdint>
#include <cassert>
#include <array>

using namespace std;

template<const size_t N> struct Bitarr {
	array<uint64_t,N> data;
	Bitarr(bool b) {
		if(b) fill(data.begin(),data.end(),uint64_t(-1));
		else fill(data.begin(),data.end(),0);
	}
	Bitarr(): Bitarr(0) {}
	bool operator[](const size_t i) const {
		return (data[i/64]>>(i%64))&1;
	}
	void set1(const size_t i) {
		data[i/64]|=uint64_t(1)<<(i%64);
	}
	void set0(const size_t i) {
		data[i/64]&=uint64_t(-1)^(uint64_t(1)<<(i%64));
	}
	void set(const size_t i, bool v) {
		if(v) set1(i);
		else set0(i);
	}
	void flip(const size_t i) {
		data[i/64]^=uint64_t(1)<<(i%64);
	}
	void operator&=(const Bitarr& o) {
		for(size_t i=0; i<N; ++i) data[i]&=o.data[i];
	}
	Bitarr<N> operator&(const Bitarr<N>& o) const {
		Bitarr<N> ans(*this);
		ans&=o;
		return ans;
	}
	void operator|=(const Bitarr& o) {
		for(size_t i=0; i<N; ++i) data[i]|=o.data[i];
	}
	Bitarr<N> operator|(const Bitarr<N>& o) const {
		Bitarr<N> ans(*this);
		ans|=o;
		return ans;
	}
	void operator^=(const Bitarr& o) {
		for(size_t i=0; i<N; ++i) data[i]^=o.data[i];
	}
	Bitarr<N> operator^(const Bitarr<N>& o) const {
		Bitarr<N> ans(*this);
		ans^=o;
		return ans;
	}
	void flipall() {
		for(size_t i=0; i<N; ++i) data[i]=~data[i];
	}
	void flip_upto(size_t n) {
		if(n>=N*64) {
			flipall();
			return;
		}
		size_t i;
		for(i=0; i<n/64; ++i) data[i]=~data[i];
		data[i]^=(uint64_t(1)<<(n%64))-1;
	}

	size_t cnt() const {
		size_t ans=0;
		for(size_t i=0; i<N; ++i) {
			ans+=__builtin_popcountll(data[i]);
		}
		return ans;
	}
	/*
	size_t random_one() const {
		size_t m = cnt();
		assert(m!=0);
		auto it = begin();
		it.skip(rng()%m);
		return *it;
	}
	*/
	struct const_iterator {
		using iterator_category = std::forward_iterator_tag;
		const array<uint64_t,N>& data;
		uint64_t b;
		size_t idx;
		const_iterator(const array<uint64_t,N>& _data): data(_data), idx(0) {
			b=data[0];
			while(b==0 && idx<N) {
				idx+=1;
				if(idx!=N) b=data[idx];
			}
		}
		const_iterator(const array<uint64_t,N>& _data, uint64_t _b, size_t _idx): data(_data), b(_b), idx(_idx) {}
		const_iterator operator++() {
			b^=(b&(-b));
			while(b==0 && idx<N) {
				idx+=1;
				if(idx!=N) b=data[idx];
			}
			return *this;
		}
		void skip(size_t diff) {
			for(;;) {
				size_t x = __builtin_popcountll(b);
				if(x>diff) break;
				diff-=x;
				idx+=1;
				b=data[idx];
			}
			for(size_t _=0; _<diff; ++_) b^=(b&(-b));
		}
		size_t operator*() {
			return idx*64 + __builtin_ctzll(b&(-b));
		}
		bool operator!=(const const_iterator& o) const {
			return idx!=o.idx || b!=o.b;
		}
	};
	const_iterator begin() const {
		return const_iterator(data);
	}
	const_iterator end() const {
		return const_iterator{data,0,N};
	}
};
