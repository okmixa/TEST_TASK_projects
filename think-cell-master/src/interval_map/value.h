#pragma once

template<typename V>
class Value
{
public:
	Value(V val) : _val(val) {}
    
    // copyable
	Value(V& v) : _val(v._val) {}
    
    // assignable
	void operator=(const Value<V>& v) { _val = v._val; }

    // comparable via operator==
    bool operator==(const Value<V>& v) const { return _val == v._val; }

private:
    V _val;
	Value() = delete;
};

