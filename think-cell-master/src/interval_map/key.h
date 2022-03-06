#pragma once

template<typename K>
class Key
{
public:
    Key(K val) : _val(val) {}
    
    // copyable
    Key(K& k) : _val(k._val) {}
    
    // assignable
	void operator=(const Key<K>& k) { _val = k._val; }

    // less than
    bool operator<(const Key<K>& k) const { return _val < k._val; }

private:
    K _val;
	Key() = delete;
};

