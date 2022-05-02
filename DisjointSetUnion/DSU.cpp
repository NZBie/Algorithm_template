/*
 * @Author: NZB 
 * @Date: 2022-04-29 22:38:31 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-05-03 00:42:03
 */

#include "DSU.h"

#include <cstring>
#include "../Trace.h"

Trace error(true);

/**
 *	@brief	并查集 DSU类 的构造函数
 *	@param	Tsz unionSize	: 并查集容量
**/
DSU::DSU(Tsz unionSize): 
_unionSize((unionSize > max_size) ? 0 : unionSize), _nodes(new Union[_unionSize]) {
	if(unionSize > max_size) error.print("Error: \"dataSize\"参数设置过大，不能超过\"max_size\"！");
	for(int i=0;i<_unionSize;i++) {
		_nodes[i].parent = _nodes + i;
		_nodes[i].rank = 1;
		_nodes[i].nodeList.push_back(i);
		_set.insert(_nodes + i);
	}
}

/**
 *	@brief	并查集 DSU类 的析构函数
**/
DSU::~DSU() {
	delete [] _nodes;
}

/**
 *	@brief	返回并查集当前的容量
 *	@return 并查集的容量
**/
inline
DSU::Tsz DSU::size() {
    return _unionSize;
}

/**
 *	@brief	返回并查集当前的集合数目
 *	@return 集合的个数
**/
inline
DSU::Tsz DSU::num_union() {
    return _set.size();
}

/**
 *	@brief	返回当前节点所在集合的大小
 *	@param	Tsz x	: 节点编号
 *	@return 当前所在集合的大小
**/
inline
DSU::Tsz DSU::size_union(Tsz x) {
	return findRoot(_nodes[x].parent)->nodeList.size();
}

/**
 *	@brief	返回同一集合中的所有节点
 *	@param	Tsz x	: 节点编号
 *	@return 当前所在集合内的所有节点
**/
inline
std::list<DSU::Tsz> DSU::search_siblings(Tsz x) {
	return findRoot(_nodes[x].parent)->nodeList;
}

/**
 *	@brief	返回最小集合内的节点个数
 *	@return 最小集合的大小
**/
inline
DSU::Tsz DSU::size_minSet() {
	return (*_set.begin())->nodeList.size();
}

/**
 *	@brief	以列表为容器返回最小集合内的所有节点
 *	@return 最小集合的所有节点
**/
inline
std::list<DSU::Tsz> DSU::search_minSet() {
	return (*_set.begin())->nodeList;
}

/**
 *	@brief	返回最大集合内的节点个数
 *	@return 最大集合的大小
**/
inline
DSU::Tsz DSU::size_maxSet() {
	return (*_set.rbegin())->nodeList.size();
}

/**
 *	@brief	以列表为容器返回最大集合内的所有节点
 *	@return 最大集合的所有节点
**/
inline
std::list<DSU::Tsz> DSU::search_maxSet() {
	return (*_set.rbegin())->nodeList;
}

/**
 *	@brief	判断两个节点是否在一个集合中
 *	@param	Tsz x	: 节点编号1
 *	@param	Tsz y	: 节点编号2
 *	@return	当前节点的根节点
**/
inline 
bool DSU::inSameSet(Tsz x, Tsz y) {
	return findRoot(_nodes[x].parent) == findRoot(_nodes[y].parent);
}

/**
 *	@brief	合并两个节点x、y
 *	@param	Tsz x	: 节点编号1
 *	@param	Tsz y	: 节点编号2
 *	@return	null
**/
void DSU::unite(Tsz x, Tsz y) {
	Union* node_x = findRoot(_nodes[x].parent);
	Union* node_y = findRoot(_nodes[y].parent);
	if(node_x == node_y) return;
	// y合并给x
	if(node_x->rank > node_y->rank) {
		uniteData(node_x, node_y);
	}
	// x合并给y
	else {
		uniteData(node_y, node_x);
		if(node_x->rank == node_y->rank) node_y->rank++;
	}
}

/**
 *	@brief	_set的比较函数，使用于union排序，方便查看并查集的整体情况
 *	@param	const Union*const& x	: 节点1
 *	@param	const Union*const& y	: 节点2
 *	@return	比较结果
**/
bool DSU::compare::operator () (const Union*const& x, const Union*const& y) const {
	if(x->nodeList.size() != y->nodeList.size()) return x->nodeList.size() < y->nodeList.size();
	else return x < y;
}

/**
 *	@brief	数据合并 y合并给x
 *	@param	Union* node_x	: 节点1
 *	@param	Union* node_y	: 节点2
 *	@return	null
**/
void DSU::uniteData(Union* node_x, Union* node_y) {
	_set.erase(node_x);
	_set.erase(node_y);
	node_y->parent = node_x;
	node_x->nodeList.merge(node_y->nodeList);
	// _nodes[y].nodeList.clear();
	_set.insert(node_x);
}

/**
 *	@brief	查找当前节点的根节点
 *	@param	Union* x	: 当前节点
 *	@return	当前节点的根节点
**/
DSU::Union* DSU::findRoot(Union* node_x) {
	return (node_x->parent == node_x) ? node_x : node_x->parent = findRoot(node_x->parent);
}
