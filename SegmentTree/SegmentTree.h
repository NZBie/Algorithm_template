#ifndef SEGMENTTREE_H
#define SEGMENTTREE_H

enum updateType { addend/*�仯��*/, cover/*������*/ }; // �����������

/* �߶�����SegmentNode */
template<typename Type>
class SegmentTree {

public:
    typedef const unsigned int index;
    
    SegmentTree(unsigned dataSize = 0, Type* dataList = nullptr, Type (*func)(Type, Type) = nullptr);
    ~SegmentTree();

    void update(index l, index r, Type val, updateType type);   // �������
    void update(index pos, Type val, updateType type);          // �������
    Type query(index l, index r);                               // �����ѯ
    Type query(index pos);                                      // �����ѯ

private:
    class SegTreeNode;          // �ڲ��ڵ���
    SegTreeNode* _root;         // ͷ�ڵ�
    unsigned _dataSize;         // ������
    Type (*_addition_point)(Type, Type);                // ����ָ�룬ָ��Type��ӷ�������ָ��

    Type addition(Type x, Type y);                      // Type�� �ӷ�����
    Type multiplyBasedAdd(Type tmp, unsigned num);      // Type�� �ۼӺ���
};

/* SegmentNode���ڲ���SegTreeNode */
template<typename Type>
class SegmentTree<Type>::SegTreeNode {

public:
    SegTreeNode(index l, index r, Type* dataList, SegmentTree* tree);
    ~SegTreeNode();

    void updateNode(index l, index r, Type val, updateType type);   // �������
    // void updateNode(index pos, Type val, updateType type);          // �������
    Type queryNode(index l, index r);                               // �����ѯ
    // Type queryNode(index pos);                                      // �����ѯ

private:
    index _l, _r;               // �ýڵ�����䷶Χ
    bool _isLazyWork;           // ������Ƿ����
    updateType _lazyType;       // ���������
    Type _data;                 // ����
    Type _lazyTag;              // �����
    SegTreeNode* _lChld;        // ���ӽڵ�
    SegTreeNode* _rChld;        // ���ӽڵ�
    SegmentTree* _tree;         // �����߶������ָ��

    void lazyTag_down();                            // ������·�
    void updateData(Type val, updateType type);     // �ڵ�����ݸ��� data �� lazyTag
};

#endif