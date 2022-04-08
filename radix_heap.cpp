#include <vector>
#include <array>
#include <limits>
#include <utility>
#include <cinttypes>
#include <cassert>
#include <algorithm>

using namespace std;

template<typename K, typename V, size_t MV=0>
class RadixHeap {
	private:
		static constexpr size_t NB = sizeof(K)*8+1;
		array<vector<pair<K,V>>,NB> buck;
		array<array<uint32_t,2>, MV> pos; // pos of item with some value
		K lastpop;
		size_t _size;

		inline size_t get_bucket(const K& x) const {
			if(x==lastpop) return 0;
			return 64-__builtin_clzll(x^lastpop);
		}
	public:
		RadixHeap() {
			lastpop=numeric_limits<K>::min();
			fill(pos.begin(),pos.end(),array<uint32_t,2>({NB,NB}));
			_size=0;
		}
		inline size_t size() const {
			return _size;
		}
		inline bool empty() const {
			return !_size;
		}
		pair<K,V> pop() {
			// size decreases
			assert(_size);
			--_size;

			// find first non-empty bucket
			size_t bi=0;
			for(;bi<NB;++bi)if(!buck[bi].empty())break;

			// find smallest element in bucket
			size_t ri=0;
			lastpop=buck[bi][0].first;
			for(size_t i=1; i<buck[bi].size(); ++i) {
				if(buck[bi][i].first<lastpop) {
					lastpop=buck[bi][i].first;
					ri=i;
				}
			}
			const pair<K,V> ans = buck[bi][ri];
			pos[ans.second]={NB,NB};
			swap(buck[bi][ri],buck[bi].back());
			buck[bi].pop_back();

			if(bi==0) return ans;

			// move all other elements to new bucket
			for(size_t i=0; i<buck[bi].size(); ++i) {	
				const size_t nb = get_bucket(buck[bi][i].first);
				//assert(nb!=bi);
				pos[buck[bi][i].second]={(uint32_t)nb,(uint32_t)buck[nb].size()};
				buck[nb].push_back(buck[bi][i]);
			}
			buck[bi].clear();

			return ans;
		}
		void push(const K k, const V v) {
			++_size;
			const size_t nb = get_bucket(k);
			pos[v]={(uint32_t)nb,(uint32_t)buck[nb].size()};
			buck[nb].push_back({k,v});
		}
		void decrease_key(const K nk, const V v) {
			const size_t b = pos[v][0];
			if(b==NB) {
				push(nk,v);
				return;
			}
			const size_t i = pos[v][1];
			//assert(buck[b][i].second==v);
			if(nk>=buck[b][i].first) return;
			buck[b][i].first=nk;
			const size_t nb = get_bucket(nk);
			if(nb==b) return;
			pos[buck[b].back().second][1]=(uint32_t)i;
			pos[v]={(uint32_t)nb,(uint32_t)buck[nb].size()};
			buck[nb].push_back(buck[b][i]);
			swap(buck[b][i],buck[b].back());
			buck[b].pop_back();
		}
};

