/*
 * @Author: NZB 
 * @Date: 2022-04-26 23:28:17 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-04-30 17:34:20
 */

#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

enum uptType { addend/*变化量*/, cover/*覆盖量*/ }; // 定义更新类型

/* 线段树类SegmentNode */
template<typename Type>
class SegmentTree {

public:
    typedef const unsigned int index;   // 线段树上的位置信息
    typedef const Type c_Type;          // 常量Type数据类型
    typedef const uptType c_uptType;    // 常量uptType枚举类型
    
    SegmentTree(unsigned dataSize = 0, c_Type* const dataList = nullptr, Type (*func)(c_Type&, c_Type&) = nullptr);
    ~SegmentTree();

    void update(index l, index r, c_Type& val, c_uptType type);     // 区间更新
    void update(index pos, c_Type& val, c_uptType type);            // 单点更新
    Type query(index l, index r);                                   // 区间查询
    Type query(index pos);                                          // 单点查询

private:
    class SegTreeNode;          // 内部节点类
    SegTreeNode* _root;         // 头节点
    unsigned _dataSize;         // 数据量
    Type (*_addition_point)(c_Type&, c_Type&);          // 函数指针，指向Type类加法函数的指针

    Type addition(c_Type& x, c_Type& y);                // Type类 加法函数
    Type multiplyBasedAdd(Type tmp, unsigned num);      // Type类 累加函数
};

/* SegmentNode的内部类SegTreeNode */
template<typename Type>
class SegmentTree<Type>::SegTreeNode {

public:
    SegTreeNode(index l, index r, c_Type* const dataList, SegmentTree* tree);
    ~SegTreeNode();

    void updateNode(index l, index r, c_Type& val, c_uptType type);     // 区间更新
    Type queryNode(index l, index r);                                   // 区间查询

private:
    index _l, _r;               // 该节点的区间范围
    SegTreeNode* _lChld;        // 左子节点
    SegTreeNode* _rChld;        // 右子节点
    SegmentTree* _tree;         // 所属线段树类的指针

    bool _isLazyWork;           // 懒标记是否可用
    uptType _lazyType;          // 懒标记类型
    Type _data;                 // 数据
    Type _lazyTag;              // 懒标记

    void lazyTag_down();                            // 懒标记下发
    void updateData(c_Type& val, c_uptType type);   // 节点的数据更新 data 和 lazyTag
};

#endif
