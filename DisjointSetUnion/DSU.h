/*
 * @Author: NZB 
 * @Date: 2022-04-29 22:38:25 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-05-02 00:11:27
 */

#ifndef DSU_H
#define DSU_H

#include <list>

/* 并查集类 DSU */
class DSU {

protected:
    typedef unsigned int Tsz;     	// 并查集的最大容量
    typedef const Tsz c_Tsz;    	// 常量Tsz

public:
	DSU(Tsz setSize);
	~DSU();

	inline Tsz size();						// 返回 并查集容量
	inline bool inSameSet(Tsz x, Tsz y);	// 判断 两个节点是否在一个集合中
	Tsz findRoot(Tsz x);					// 查找 根节点
	virtual void unite(Tsz x, Tsz y);		// 合并 两个节点

protected:
	const static Tsz max_size = 1e7;	// 并查集允许的最大容量
	Tsz _setSize;						// 并查集容量
	Tsz* const _parId;					// 存储 父节点编号
	Tsz* const _rank;					// 存储 以此为根节点的树深
};

/* 拓展并查集类 DSU_plus */
class DSU_plus : public DSU {

public:
	DSU_plus(Tsz setSize);
	~DSU_plus();

	std::list<Tsz> search_Siblings(Tsz x);	// 查找 同一集合中的其他节点
	std::list<Tsz> search_BySet(Tsz x);		// 查找 所有的节点，按集合分类

private:
	std::list<Tsz> x;
};

#endif
