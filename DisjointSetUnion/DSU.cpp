/*
 * @Author: NZB 
 * @Date: 2022-04-29 22:38:31 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-05-01 22:05:06
 */

#include "DSU.h"

#include <cstring>
#include "../Trace.h"

Trace error(true);

/**
 *	@brief	并查集 DSU类 的构造函数
 *	@param	Tsz setSize		: 并查集容量
**/
DSU::DSU(Tsz setSize): 
_setSize((setSize > max_size) ? 0 : setSize), _parId(new Tsz[_setSize]), _rank(new Tsz[_setSize]) {
	if(setSize > max_size) error.print("Error: \"dataSize\"参数设置过大，不能超过\"max_size\"！");
	memset(_rank, 0, sizeof(_rank));
	for(int i=0;i<setSize;i++) {
		_parId[i] = i;
	}
}

/**
 *	@brief	并查集 DSU类 的析构函数
**/
DSU::~DSU() {
	delete [] _parId;
	delete [] _rank;
}

/**
 *	@brief	返回并查集的容量
 *	@return 当前并查集的容量
**/
inline
DSU::Tsz DSU::size() {
    return _setSize;
}

/**
 *	@brief	判断两个节点是否在一个集合中
 *	@param	Tsz x		: 节点1
 *	@param	Tsz y		: 节点2
 *	@return	当前节点的根节点
**/
inline 
bool DSU::inSameSet(Tsz x, Tsz y) {
	return findRoot(x) == findRoot(y);
}

/**
 *	@brief	查找当前节点的根节点
 *	@param	c_Tsz x		: 当前节点
 *	@return	当前节点的根节点
**/
DSU::Tsz DSU::findRoot(c_Tsz x) {
	return (_parId[x] == x) ? x : _parId[x] = findRoot(_parId[x]);
}

/**
 *	@brief	合并两个节点x、y
 *	@param	Tsz x		: 节点1
 *	@param	Tsz y		: 节点2
 *	@return	null
**/
void DSU::unite(Tsz x, Tsz y) {
	x = findRoot(x);
	y = findRoot(y);
	if(x == y) return;
	if(_rank[x] > _rank[y]) _parId[y] = x;
	else {
		_parId[x] = y;
		if(_rank[x] == _rank[y]) _rank[y]++;
	}
}
