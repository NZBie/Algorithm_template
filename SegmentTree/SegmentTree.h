/*
 * @Author: NZB 
 * @Date: 2022-04-26 23:28:17 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-04-29 22:29:04
 */

#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

enum updateType { addend/*变化量*/, cover/*覆盖量*/ }; // 定义更新类型

/* 线段树类SegmentNode */
template<typename Type>
class SegmentTree {

public:
    typedef const unsigned int index; // 线段树上的位置信息
    
    SegmentTree(unsigned dataSize = 0, Type* dataList = nullptr, Type (*func)(Type, Type) = nullptr);
    ~SegmentTree();

    void update(index l, index r, Type val, updateType type);   // 区间更新
    void update(index pos, Type val, updateType type);          // 单点更新
    Type query(index l, index r);                               // 区间查询
    Type query(index pos);                                      // 单点查询

private:
    class SegTreeNode;          // 内部节点类
    SegTreeNode* _root;         // 头节点
    unsigned _dataSize;         // 数据量
    Type (*_addition_point)(Type, Type);                // 函数指针，指向Type类加法函数的指针

    Type addition(Type x, Type y);                      // Type类 加法函数
    Type multiplyBasedAdd(Type tmp, unsigned num);      // Type类 累加函数
};

/* SegmentNode的内部类SegTreeNode */
template<typename Type>
class SegmentTree<Type>::SegTreeNode {

public:
    SegTreeNode(index l, index r, Type* dataList, SegmentTree* tree);
    ~SegTreeNode();

    void updateNode(index l, index r, Type val, updateType type);   // 区间更新
    Type queryNode(index l, index r);                               // 区间查询

private:
    index _l, _r;               // 该节点的区间范围
    bool _isLazyWork;           // 懒标记是否可用
    updateType _lazyType;       // 懒标记类型
    Type _data;                 // 数据
    Type _lazyTag;              // 懒标记
    SegTreeNode* _lChld;        // 左子节点
    SegTreeNode* _rChld;        // 右子节点
    SegmentTree* _tree;         // 所属线段树类的指针

    void lazyTag_down();                            // 懒标记下发
    void updateData(Type val, updateType type);     // 节点的数据更新 data 和 lazyTag
};

#endif
