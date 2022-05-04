/*
 * @Author: NZB 
 * @Date: 2022-04-29 22:38:25 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-05-03 00:28:11
 */

#ifndef DSU_H
#define DSU_H

#include <list>
#include <set>
#include <vector>
#include "../Trace.h"

/* 并查集类 DSU */
class DSU {

protected:
	struct Union;		// 并查集内部节点结构
	struct compare {	// _set的比较函数，使用于union排序，方便查看并查集的整体情况
		bool operator () (const Union*const&, const Union*const&) const;
	};

    typedef unsigned int Tsz;     				// 并查集的最大容量
    typedef const Tsz c_Tsz;    				// 常量Tsz
	typedef std::list<Tsz> list;
	typedef std::vector<int> vctInt;
	typedef std::vector<list> vctList;
	typedef std::set<Union*, compare> set;

public:
	DSU(Tsz unionSize);
	DSU(const DSU& dsu);
	~DSU();

	DSU operator = (const DSU& dsu);

	inline Tsz size();							// 返回 并查集中节点个数
	inline Tsz num_union();						// 返回 集合的个数
	
	inline bool inSameUnion(Tsz x, Tsz y);		// 判断 两个节点是否在一个集合中
	void unite(Tsz x, Tsz y);					// 合并 两个节点所在的集合

	inline Tsz size_union(Tsz x);				// 返回 当前节点所在集合的大小
	inline const list& search_siblings(Tsz x);	// 返回 当前集合所在链表的引用
	inline Tsz size_minUnion();					// 返回 最小集合的大小
	inline const list& search_minUnion();		// 返回 最小集合所在链表的引用
	inline Tsz size_maxUnion();					// 返回 最大集合的大小
	inline const list& search_maxUnion();		// 返回 最大集合所在链表的引用
	inline vctInt size_allUnions();				// 返回 所有集合的大小，存储于vector返回
	inline const vctList search_allUnions();	// 返回 所有集合所在链表的引用，存储于vector返回

public:
	static c_Tsz max_size = 1 << 22;	// 并查集允许的最大容量
	c_Tsz _unionSize;					// 并查集容量
	Union* const _nodes;				// 存储 所有集合信息
	set _set;							// 动态存储 所有集合整体信息

	void uniteData(Union* node_x, Union* node_y);	// 数据合并 y合并给x
	Union* findRoot(Union* x);						// 查找 所在集合的根节点
};

/* 并查集内部 集合类 DSU */
struct DSU::Union {

	Union* parent;		// 存储 父节点的地址
	Tsz rank;			// 存储 集合构成树的树深
	list nodeList;		// 存储 集合内的所有节点，非根节点均为空
};

#endif
