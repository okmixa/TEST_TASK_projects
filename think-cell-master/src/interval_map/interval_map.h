#pragma once
#include <map>

template<typename K, typename V>
class interval_map {
	V m_valBegin;
	std::map<K,V> m_map;
public:
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {

		// Check if the range is not void
		if ( !(keyBegin < keyEnd) ) {
			return;
		}

		// Determine the interval
		auto end = std::end(m_map);
		auto begin = end;
		for(auto it = m_map.begin(); it != m_map.end(); ++it) {
			if (begin == end && keyBegin < it->first) {
				begin = it;
			}
			if (keyEnd < it->first) {
				end = it;
				break;
			}
		}

		// Save the last value
		const V valEnd = operator[](keyEnd);

		// Remove existed nodes on the interval
		for (auto it = begin; it != end && it != m_map.end();) {
			it = m_map.erase(it);
		}

		// Insert the interval of val
		m_map.insert_or_assign(keyBegin, val);

		// Add the last value to the end
		m_map.insert_or_assign(keyEnd, valEnd);

		// contacts:
		// skype: death_trojan
		// email: rylov@ex.ua
	}

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}

	size_t internalMapSize()
	{
		return m_map.size();
	}
};
