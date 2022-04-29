#include "SegmentTree.h"

#include <cstring>
#include "../Trace.h"

Trace debug(false);
Trace error(true);

/**
 *	@brief  SegmentTree�� ���캯��
 *	@param	unsigned dataSize           : ��������
 *	@param  Type* dataList              : ����
 *	@param  Type (*func)(Type, Type)    : �Զ���ӷ�����
**/
template<typename Type>
SegmentTree<Type>::SegmentTree(unsigned dataSize, Type* dataList, Type (*func)(Type, Type))
: _dataSize(dataSize), _addition_point(func) {
    if(_dataSize == 0) _root = nullptr;
    else _root = new SegTreeNode(0, _dataSize-1, dataList, this);
}

/**
 *	@brief  SegmentTree�� ��������
**/
template<typename Type>
SegmentTree<Type>::~SegmentTree() {
    debug.print("�ⲿ�� SegmentTree ���� ����");
    delete _root;
    _root = nullptr;
}

/**
 *	@brief  SegmentTree�� �ӿں���
**/
template<typename Type>
void SegmentTree<Type>::update(index l, index r, Type val, updateType type) {
    if(!(l <= r && 0 <= l && r < _dataSize)) {
        error.print("Error: update(l, r, val) ���������Χ");
        return;
    }
    _root->updateNode(l, r, val, type);
}
template<typename Type>
void SegmentTree<Type>::update(index pos, Type val, updateType type) {
    if(!(0 <= pos && pos < _dataSize)) {
        error.print("Error: update(pos, val) ���������Χ");
        return;
    }
    _root->updateNode(pos, pos, val, type);
}
template<typename Type>
Type SegmentTree<Type>::query(index l, index r) {
    if(!(l <= r && 0 <= l && r < _dataSize)) {
        error.print("Error: query(l, r) ���������Χ");
        Type res;
        memset(&res, 0, sizeof(res));
        return res;
    }
    return _root->queryNode(l, r);
}
template<typename Type>
Type SegmentTree<Type>::query(index pos) {
    if(!(0 <= pos && pos < _dataSize)) {
        error.print("Error: query(pos) ���������Χ");
        Type res;
        memset(&res, 0, sizeof(res));
        return res;
    }
    return _root->queryNode(pos, pos);
}

/**
 *	@brief  Type�� Ĭ�ϼӷ�����, ���û��������ṩ �ӷ����� ������£����� Type�� �����"+"�ӷ������
 *	@param	Type x  : ����1
 *	@param  Type y  : ����2
**/
template<typename Type>
Type SegmentTree<Type>::addition(Type x, Type y) {
    if(_addition_point == nullptr) return x + y;
    else return (*_addition_point)(x, y);
}

/**
 *	@brief  Type�� �ۼӺ����������ڿ�����
 *	@param	Type tmp        : ����
 *	@param  unsigned num    : �ۼӴ���
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
 *	@brief  SegTreeNode�� ���캯��
 *	@param	index l             : ������߽�
 *	@param  index r             : �����ұ߽�
 *	@param  SegmentTree *tree   : // �����߶������ָ��
**/
template<typename Type>
SegmentTree<Type>::SegTreeNode::SegTreeNode(index l, index r, Type* dataList, SegmentTree *tree)
: _l(l), _r(r), _isLazyWork(false), _lazyType(addend), _tree(tree) {

	// ��ʼ��
    // memset(&_lazyTag, 0, sizeof(_lazyTag));

    // Ҷ�ӽڵ�
    if(l == r) {
        _lChld = _rChld = nullptr;
        if(dataList != nullptr) _data = dataList[l];
        else memset(&_data, 0, sizeof(_data));
    }

    // ��Ҷ�ӽڵ�
    else {
        index mid = (l + r) >> 1;
        _lChld = new SegmentTree<Type>::SegTreeNode(l, mid, dataList, tree);
        _rChld = new SegmentTree<Type>::SegTreeNode(mid + 1, r, dataList, tree);
        _data = _tree->addition(_lChld->_data, _rChld->_data);
    }
}

/**
 *	@brief  SegTreeNode�� ��������
**/
template<typename Type>
SegmentTree<Type>::SegTreeNode::~SegTreeNode() {
    debug.print("�ڲ��� SegTreeNode ���� ����");
    if(_lChld) {
        delete _lChld;
        delete _rChld;
    }
}

/**
 *	@brief  ��������[l, r]���������ݵ�ֵ
 *	@param	index l         : ������߽�
 *	@param  index r         : �����ұ߽�
 *	@param  Type val        : �仯�� ������
 *	@param  updateType type : ���·�ʽ �仯�򸲸�
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::updateNode(index l, index r, Type val, updateType type) {

    // ������ȫ����
	if(l <= _l && _r <= r) {
        updateData(val, type);
		return;
	}

    // ������·�
	if(_lazyType) lazyTag_down();
                    
    // ���������ӽڵ�
    index mid = (_l + _r) >> 1;
    if(l <= mid) _lChld->updateNode(l, r, val, type);
    if(mid < r) _rChld->updateNode(l, r, val, type);

    // ���µ�ǰ�ڵ�
    _data = _tree->addition(_lChld->_data, _rChld->_data);
}
// template<typename Type>
// void SegmentTree<Type>::SegTreeNode::updateNode(index pos, Type val, updateType type) {

// }

/**
 *	@brief  ��ȡ����[l, r]���������ݵļӺ�
 *	@param  index l  : ������߽�
 *	@param	index r  : �����ұ߽�     
 *	@return �������������ݵļӺ�
**/
template<typename Type>
Type SegmentTree<Type>::SegTreeNode::queryNode(index l, index r) {

    // ������ȫ����
	if(l <= _l && _r <= r) return _data;

    // ������·�
	if(_isLazyWork) lazyTag_down();

    // �ϲ������ӽڵ��ֵ
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
 *	@brief  �·������
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::lazyTag_down() {
    
    // ��Ҷ�ӽڵ� �·������
    if(_lChld) {
        _lChld->updateData(_lazyTag, _lazyType);
        _rChld->updateData(_lazyTag, _lazyType);
    }

    // ��������
    // memset(&_lazyTag, 0, sizeof(_lazyTag));
    _isLazyWork = false;
}

/**
 *	@brief  �ڵ�����ݸ��� data �� lazyTag
 *	@param  Type val        : �仯�� ������
 *	@param  updateType type : ���·�ʽ �仯�򸲸�
 *	@return null
**/
template<typename Type>
void SegmentTree<Type>::SegTreeNode::updateData(Type val, updateType type) {

    // �������ڵ�ֵ ���ϱ仯��
    if(type == addend) {
        _data = _tree->addition(_data, _tree->multiplyBasedAdd(val, _r - _l + 1));
        if(_isLazyWork) _lazyTag = _tree->addition(_lazyTag, val);
        else _lazyTag = val;
    }

    // �������ڵ�ֵ ���踲����
    else {
        _data = _tree->multiplyBasedAdd(val, _r - _l + 1);
        _lazyTag = val;
    }

    _isLazyWork = true;
    _lazyType = type;
}
