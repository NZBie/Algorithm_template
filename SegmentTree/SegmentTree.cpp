#include "SegmentTree.h"

#include <cstring>
#include "../Trace.h"

Trace debug(false);
Trace error(true);

/**
 *	@brief  SegmentTree类 构造函数
 *	@param	unsigned dataSize           : 数据总量
 *	@param  Type* dataList              : 数据
 *	@param  Type (*func)(Type, Type)    : 自定义加法函数
**/
template<typename Type>
SegmentTree<Type>::SegmentTree(unsigned dataSize, Type* dataList, Type (*func)(Type, Type))
: _dataSize(dataSize), _addition_point(func) {
    if(_dataSize == 0) _root = nullptr;
    else _root = new SegTreeNode(0, _dataSize-1, dataList, this);
}

/**
 *	@brief  SegmentTree类 析构函数
**/
template<typename Type>
SegmentTree<Type>::~SegmentTree() {
    debug.print("外部类 SegmentTree 调用 析构");
    delete _root;
    _root = nullptr;
}

/**
 *	@brief  SegmentTree类 接口函数
**/
template<typename Type>
void SegmentTree<Type>::update(index l, index r, Type val, updateType type) {
    if(!(l <= r && 0 <= l && r < _dataSize)) {
        error.print("Error: update(l, r, val) 请检查参数范围");
        return;
    }
    _root->updateNode(l, r, val, type);
}
template<typename Type>
void SegmentTree<Type>::update(index pos, Type val, updateType type) {
    if(!(0 <= pos && pos < _dataSize)) {
        error.print("Error: update(pos, val) 请检查参数范围");
        return;
    }
    _root->updateNode(pos, pos, val, type);
}
template<typename Type>
Type SegmentTree<Type>::query(index l, index r) {
    if(!(l <= r && 0 <= l && r < _dataSize)) {
        error.print("Error: query(l, r) 请检查参数范围");
        Type res;
        memset(&res, 0, sizeof(res));
        return res;
    }
    return _root->queryNode(l, r);
}
template<typename Type>
Type SegmentTree<Type>::query(index pos) {
    if(!(0 <= pos && pos < _dataSize)) {
        error.print("Error: query(pos) 请检查参数范围");
        Type res;
        memset(&res, 0, sizeof(res));
        return res;
    }
    return _root->queryNode(pos, pos);
}

/**
 *	@brief  Type类 默认加法函数, 在用户不额外提供 加法函数 的情况下，调用 Type类 本身的"+"加法运算符
 *	@param	Type x  : 加数1
 *	@param  Type y  : 加数2
**/
template<typename Type>
Type SegmentTree<Type>::addition(Type x, Type y) {
    if(_addition_point == nullptr) return x + y;
    else return (*_addition_point)(x, y);
}

/**
 *	@brief  Type类 累加函数，类似于快速幂
 *	@param	Type tmp        : 基数
 *	@param  unsigned num    : 累加次数
**/
template<typename Type>
Type SegmentTree<Type>::multiplyBasedAdd(Type tmp, unsigned num) {
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
 *	@brief  SegTreeNode类 构造函数
 *	@param	index l             : 区间左边界
 *	@param  index r             : 区间右边界
 *	@param  SegmentTree *tree   : // 所属线段树类的指针
**/
template<typename Type>
SegmentTree<Type>::SegTreeNode::SegTreeNode(index l, index r, Type* dataList, SegmentTree *tree)
: _l(l), _r(r), _isLazyWork(false), _lazyType(addend), _tree(tree) {

	// 初始化
    // memset(&_lazyTag, 0, sizeof(_lazyTag));

    // 叶子节点
    if(l == r) {
        _lChld = _rChld = nullptr;
        if(dataList != nullptr) _data = dataList[l];
        else memset(&_data, 0, sizeof(_data));
    }

    // 非叶子节点
    else {
        index mid = (l + r) >> 1;
        _lChld = new SegmentTree<Type>::SegTreeNode(l, mid, dataList, tree);
        _rChld = new SegmentTree<Type>::SegTreeNode(mid + 1, r, dataList, tree);
        _data = _tree->addition(_lChld->_data, _rChld->_data);
    }
}

/**
 *	@brief  SegTreeNode类 析构函数
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
 *	@brief  更新区间[l, r]内所有数据的值
 *	@param	index l         : 区间左边界
 *	@param  index r         : 区间右边界
 *	@param  Type val        : 变化量 覆盖量
 *	@param  updateType type : 更新方式 变化或覆盖
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::updateNode(index l, index r, Type val, updateType type) {

    // 区间完全覆盖
	if(l <= _l && _r <= r) {
        updateData(val, type);
		return;
	}

    // 懒标记下发
	if(_lazyType) lazyTag_down();
                    
    // 更新左右子节点
    index mid = (_l + _r) >> 1;
    if(l <= mid) _lChld->updateNode(l, r, val, type);
    if(mid < r) _rChld->updateNode(l, r, val, type);

    // 更新当前节点
    _data = _tree->addition(_lChld->_data, _rChld->_data);
}
// template<typename Type>
// void SegmentTree<Type>::SegTreeNode::updateNode(index pos, Type val, updateType type) {

// }

/**
 *	@brief  获取区间[l, r]内所有数据的加和
 *	@param  index l  : 区间左边界
 *	@param	index r  : 区间右边界     
 *	@return 区间内所有数据的加和
**/
template<typename Type>
Type SegmentTree<Type>::SegTreeNode::queryNode(index l, index r) {

    // 区间完全覆盖
	if(l <= _l && _r <= r) return _data;

    // 懒标记下发
	if(_isLazyWork) lazyTag_down();

    // 合并左右子节点的值
	Type res;
    index mid = (_l + _r) >> 1;
	if(l <= mid && mid < r) res = _tree->addition(_lChld->queryNode(l, r), _rChld->queryNode(l, r));
	else if(l <= mid) res = _lChld->queryNode(l, r);
    else if(mid < r) res = _rChld->queryNode(l, r);
	return res;
}
// template<typename Type>
// Type SegmentTree<Type>::SegTreeNode::queryNode(index pos) {

// }

/**
 *	@brief  下发懒标记
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
    // memset(&_lazyTag, 0, sizeof(_lazyTag));
    _isLazyWork = false;
}

/**
 *	@brief  节点的数据更新 data 和 lazyTag
 *	@param  Type val        : 变化量 覆盖量
 *	@param  updateType type : 更新方式 变化或覆盖
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::updateData(Type val, updateType type) {

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

    _isLazyWork = true;
    _lazyType = type;
}
