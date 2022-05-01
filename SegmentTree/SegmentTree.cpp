/*
 * @Author: NZB 
 * @Date: 2022-04-27 20:27:51 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-05-01 22:47:37
 */

#include "SegmentTree.h"

#include <cstring>
#include "../Trace.h"

Trace debug(false);
Trace error(true);

/**
 *	@brief	SegmentTree类 构造函数
 *	@param	c_Tsz dataSize							: 数据总量
 *	@param	const Type* const dataList				: 初始数据，为空代表初始值为全0
 *	@param	Type (*func)(const Type&, const Type&)	: 自定义加法函数，为空代表调用默认“+”运算符
**/
template<typename Type>
SegmentTree<Type>::SegmentTree(c_Tsz dataSize, c_Type* const dataList, Type (*func)(c_Type&, c_Type&)): 
_dataSize((dataSize > max_size) ? 0 : dataSize), _addition_point(func),
_root((_dataSize == 0) ? nullptr : new SegTreeNode(0, _dataSize-1, dataList, this)) {
	debug.print("外部类 SegmentTree 调用 构造");
	if(dataSize > max_size) error.print("Error: \"dataSize\"参数设置过大，不能超过\"max_size\"！");
}

/**
 *	@brief	SegmentTree类 析构函数
**/
template<typename Type>
SegmentTree<Type>::~SegmentTree() {
	debug.print("外部类 SegmentTree 调用 析构");
	delete _root;
}

/**
 *	@brief	SegmentTree类 接口函数，修改、查询 的功能
 *	@param	c_Tsz l				: 区间左边界
 *	@param	c_Tsz r				: 区间右边界
 *	@param	c_Tsz pos			: 单点位置
 *	@param	const Type val		: 变化量 覆盖量
 *	@param	const uptType type	: 更新方式 变化或覆盖
**/
template<typename Type> inline 
SegmentTree<Type>::Tsz SegmentTree<Type>::size() {
	return _dataSize;
}
template<typename Type> inline 
void SegmentTree<Type>::update(c_Tsz l, c_Tsz r, c_Type& val, c_uptType type) {
	if(!(l <= r && 0 <= l && r < _dataSize)) {
		error.print("Error: \"update(l, r, val, type)\"请检查参数范围！");
		return;
	}
	_root->updateNode(l, r, val, type);
}
template<typename Type> inline 
void SegmentTree<Type>::update(c_Tsz pos, c_Type& val, c_uptType type) {
	if(!(0 <= pos && pos < _dataSize)) {
		error.print("Error: \"update(pos, val, type)\" 请检查参数范围！");
		return;
	}
	_root->updateNode(pos, pos, val, type);
}
template<typename Type> inline 
Type SegmentTree<Type>::query(c_Tsz l, c_Tsz r) {
	if(!(l <= r && 0 <= l && r < _dataSize)) {
		error.print("Error: \"query(l, r)\" 请检查参数范围！");
		Type res;
		memset(&res, 0, sizeof(res));
		return res;
	}
	return _root->queryNode(l, r);
}
template<typename Type> inline 
Type SegmentTree<Type>::query(c_Tsz pos) {
	if(!(0 <= pos && pos < _dataSize)) {
		error.print("Error: \"query(pos)\" 请检查参数范围！");
		Type res;
		memset(&res, 0, sizeof(res));
		return res;
	}
	return _root->queryNode(pos, pos);
}

/**
 *	@brief	Type类 默认加法函数, 在用户不额外提供 加法函数 的情况下，调用 Type类 本身的"+"加法运算符
 *	@param	const Type& x	: 加数1
 *	@param	const Type& y	: 加数2
**/
template<typename Type> inline 
Type SegmentTree<Type>::addition(c_Type& x, c_Type& y) {
	return (_addition_point == nullptr) ? x + y : (*_addition_point)(x, y);
}

/**
 *	@brief	Type类 累加函数，类似于快速幂
 *	@param	Type tmp	: 基数
 *	@param	Tsz num		: 累加次数
**/
template<typename Type>
Type SegmentTree<Type>::multiplyBasedAdd(Type tmp, Tsz num) {
	Type res;
	bool isInit = false;
	while(num) {
		if(num & 1) {
			if(isInit) res = addition(res, tmp);
			else res = tmp;
		}
		tmp = addition(tmp, tmp);
		num >>= 1;
	}
	return res;
}

/**
 *	@brief	SegTreeNode类 构造函数
 *	@param	c_Tsz l						: 区间左边界
 *	@param	c_Tsz r						: 区间右边界
 *	@param	const Type* const dataList	: 初始数据
 *	@param	SegmentTree *tree			: 所属线段树类的指针
**/
template<typename Type>
SegmentTree<Type>::SegTreeNode::SegTreeNode(c_Tsz l, c_Tsz r, c_Type* const dataList, SegmentTree *tree): 
_l(l), _r(r), _isLazyWork(false), _lazyType(addend), _tree(tree),
_lChld((l == r) ? nullptr : new SegmentTree<Type>::SegTreeNode(l, (l + r) >> 1, dataList, tree)),
_rChld((l == r) ? nullptr : new SegmentTree<Type>::SegTreeNode(((l + r) >> 1) + 1, r, dataList, tree)) {
	debug.print("内部类 SegTreeNode 调用 构造");

	// 叶子节点
	if(l == r) {
		if(dataList != nullptr) _data = dataList[l];
		else memset(&_data, 0, sizeof(_data));
	}

	// 非叶子节点
	else _data = _tree->addition(_lChld->_data, _rChld->_data);
}

/**
 *	@brief	SegTreeNode类 析构函数
**/
template<typename Type>
SegmentTree<Type>::SegTreeNode::~SegTreeNode() {
	debug.print("内部类 SegTreeNode 调用 析构");
	if(_lChld) {
		delete _lChld;
		delete _rChld;
	}
}

/**
 *	@brief	更新区间[l, r]内所有数据的值
 *	@param	c_Tsz l				: 区间左边界
 *	@param	c_Tsz r				: 区间右边界
 *	@param	const Type val		: 变化量 覆盖量
 *	@param	const uptType type	: 更新方式 变化或覆盖
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::updateNode(c_Tsz l, c_Tsz r, c_Type& val, c_uptType type) {

	// 区间完全覆盖
	if(l <= _l && _r <= r) {
		updateData(val, type);
		return;
	}

	// 懒标记下发
	if(_lazyType) lazyTag_down();
					
	// 更新左右子节点
	c_Tsz mid = (_l + _r) >> 1;
	if(l <= mid) _lChld->updateNode(l, r, val, type);
	if(mid < r) _rChld->updateNode(l, r, val, type);

	// 更新当前节点
	_data = _tree->addition(_lChld->_data, _rChld->_data);
}

/**
 *	@brief	获取区间[l, r]内所有数据的加和
 *	@param	c_Tsz l		: 区间左边界
 *	@param	c_Tsz r		: 区间右边界	 
 *	@return 区间内所有数据的加和
**/
template<typename Type>
Type SegmentTree<Type>::SegTreeNode::queryNode(c_Tsz l, c_Tsz r) {

	// 区间完全覆盖
	if(l <= _l && _r <= r) return _data;

	// 懒标记下发
	if(_isLazyWork) lazyTag_down();

	// 合并左右子节点的值
	c_Tsz mid = (_l + _r) >> 1;
	if(l <= mid) {
		if(mid < r) return _tree->addition(_lChld->queryNode(l, r), _rChld->queryNode(l, r));
		else return _lChld->queryNode(l, r);
	}
	else return _rChld->queryNode(l, r);
}

/**
 *	@brief	下发懒标记
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::lazyTag_down() {
	
	// 非叶子节点 下发懒标记
	if(_lChld) {
		_lChld->updateData(_lazyTag, _lazyType);
		_rChld->updateData(_lazyTag, _lazyType);
	}

	// 清除懒标记
	_isLazyWork = false;
}

/**
 *	@brief	节点的数据更新 data 和 lazyTag
 *	@param	const Type val		: 变化量 覆盖量
 *	@param	const uptType type	: 更新方式 变化或覆盖
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::updateData(c_Type& val, c_uptType type) {

	// 对区间内的值 加上变化量
	if(type == addend) {
		_data = _tree->addition(_data, _tree->multiplyBasedAdd(val, _r - _l + 1));
		if(_isLazyWork) _lazyTag = _tree->addition(_lazyTag, val);
		else _lazyTag = val;
	}

	// 对区间内的值 赋予覆盖量
	else {
		_data = _tree->multiplyBasedAdd(val, _r - _l + 1);
		_lazyTag = val;
	}

	// 启用懒标记
	_isLazyWork = true;
	_lazyType = type;
}
