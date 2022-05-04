/*
 * @Author: NZB
 * @Date: 2022-04-26 23:28:17
 * @Last Modified by: NZB
 * @Last Modified time: 2022-05-01 22:50:18
 */

#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

#include <cstring>
#include "../Trace.h"

enum uptType { addend/*变化量*/, cover/*覆盖量*/ };	// 定义更新类型

/* 线段树类SegmentNode */
template<typename Type>
class SegmentTree {

private:
	typedef unsigned int Tsz;			// 线段树上的最大容量
	typedef const Tsz c_Tsz;			// 常量Tsz
	typedef const Type c_Type;			// 常量Type
	typedef const uptType c_uptType;	// 常量uptType
	// typedef SegmentTree<Type> SegTreeType;

public:
	SegmentTree(c_Tsz dataSize, c_Type* const dataList = nullptr, Type (*func)(c_Type&, c_Type&) = nullptr);
	SegmentTree(const SegmentTree<Type>& tree);
	~SegmentTree();

	inline Tsz size();													// 返回线段树容量
	inline void update(c_Tsz l, c_Tsz r, c_Type& val, c_uptType type);	// 区间更新
	inline void update(c_Tsz pos, c_Type& val, c_uptType type);			// 单点更新
	inline Type query(c_Tsz l, c_Tsz r);								// 区间查询
	inline Type query(c_Tsz pos);										// 单点查询

private:
	class SegTreeNode;					// 内部节点类
	const static Tsz max_size = 1e6;	// 线段树允许的最大容量
	Tsz _dataSize;						// 数据量
	SegTreeNode* const _root;			// 根节点
	Type (*_addition_point)(c_Type&, c_Type&);		// 函数指针，指向Type类加法函数的指针

	inline Type addition(c_Type& x, c_Type& y);		// Type类 加法函数
	Type multiplyBasedAdd(Type tmp, Tsz num);		// Type类 累加函数
};

/* SegmentNode的内部类SegTreeNode */
template<typename Type>
class SegmentTree<Type>::SegTreeNode {

public:
	SegTreeNode(c_Tsz l, c_Tsz r, c_Type* const dataList, SegmentTree<Type>* tree);
	SegTreeNode(const SegTreeNode& node, SegmentTree<Type>* tree);
	~SegTreeNode();

	void updateNode(c_Tsz l, c_Tsz r, c_Type& val, c_uptType type);	// 区间更新
	Type queryNode(c_Tsz l, c_Tsz r);								// 区间查询

private:
	c_Tsz _l, _r;				// 该节点的区间范围
	SegmentTree* _tree;			// 所属线段树类的指针
	bool _isLazyWork;			// 懒标记是否可用
	uptType _lazyType;			// 懒标记类型
	SegTreeNode* const _lChld;	// 左子节点
	SegTreeNode* const _rChld;	// 右子节点
	Type _data;					// 数据
	Type _lazyTag;				// 懒标记

	void lazyTag_down();							// 懒标记下发
	void updateData(c_Type& val, c_uptType type);	// 节点的数据更新 data 和 lazyTag
};

#endif
