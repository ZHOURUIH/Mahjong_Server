#ifndef _TX_SET_H_
#define _TX_SET_H_

#include "txSTLBase.h"

template<typename T>
class txSet : public txSTLBase
{
public:
	typedef typename set<T>::iterator iterator;
public:
	txSet(){}
	virtual ~txSet(){ clear(); }
	iterator find(const T& elem) const{return mSet.find(elem);}
	iterator begin() const{return mSet.begin();}
	iterator end() const{return mSet.end();}
	pair<iterator, bool> insert(const T& elem)
	{
		checkLock();
		return mSet.insert(elem);
	}
	void erase(const iterator& iter)
	{
		checkLock();
		mSet.erase(iter);
	}
	bool tryErase(const T& value)
	{
		iterator iter = mSet.find(value);
		if (iter != mSet.end())
		{
			erase(iter);
			return true;
		}
		return false;
	}
	bool contains(const T& value){return mSet.find(value) != mSet.end();}
	void clear()
	{
		checkLock();
		mSet.clear();
	}
	int size() const{return mSet.size();}
	void clone(txSet<T>& temp)
	{
		temp = *this;
		temp.resetLock();
	}
protected:
	set<T> mSet;
};

#endif