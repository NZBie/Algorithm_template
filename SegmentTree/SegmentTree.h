/*
 * @Author: NZB 
 * @Date: 2022-04-26 23:28:17 
 * @Last Modified by: NZB
 * @Last Modified time: 2022-04-30 17:34:20
 */

#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

enum uptType { addend/*�仯��*/, cover/*������*/ }; // �����������

/* �߶�����SegmentNode */
template<typename Type>
class SegmentTree {

public:
    typedef const unsigned int index;   // �߶����ϵ�λ����Ϣ
    typedef const Type c_Type;          // ����Type��������
    typedef const uptType c_uptType;    // ����uptTypeö������
    
    SegmentTree(unsigned dataSize = 0, c_Type* const dataList = nullptr, Type (*func)(c_Type&, c_Type&) = nullptr);
    ~SegmentTree();

    void update(index l, index r, c_Type& val, c_uptType type);     // �������
    void update(index pos, c_Type& val, c_uptType type);            // �������
    Type query(index l, index r);                                   // �����ѯ
    Type query(index pos);                                          // �����ѯ

private:
    class SegTreeNode;          // �ڲ��ڵ���
    SegTreeNode* _root;         // ͷ�ڵ�
    unsigned _dataSize;         // ������
    Type (*_addition_point)(c_Type&, c_Type&);          // ����ָ�룬ָ��Type��ӷ�������ָ��

    Type addition(c_Type& x, c_Type& y);                // Type�� �ӷ�����
    Type multiplyBasedAdd(Type tmp, unsigned num);      // Type�� �ۼӺ���
};

/* SegmentNode���ڲ���SegTreeNode */
template<typename Type>
class SegmentTree<Type>::SegTreeNode {

public:
    SegTreeNode(index l, index r, c_Type* const dataList, SegmentTree* tree);
    ~SegTreeNode();

    void updateNode(index l, index r, c_Type& val, c_uptType type);     // �������
    Type queryNode(index l, index r);                                   // �����ѯ

private:
    index _l, _r;               // �ýڵ�����䷶Χ
    SegTreeNode* _lChld;        // ���ӽڵ�
    SegTreeNode* _rChld;        // ���ӽڵ�
    SegmentTree* _tree;         // �����߶������ָ��

    bool _isLazyWork;           // ������Ƿ����
    uptType _lazyType;          // ���������
    Type _data;                 // ����
    Type _lazyTag;              // �����

    void lazyTag_down();                            // ������·�
    void updateData(c_Type& val, c_uptType type);   // �ڵ�����ݸ��� data �� lazyTag
};

#endif
