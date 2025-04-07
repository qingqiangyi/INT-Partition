
#ifndef CMAP_H_


#define CMAP_H_

//����vector��ģ�壬����ĺ���breadthFirstTraverseImpl(vector<int> preVec)���õ�

#include <vector>


#include "Node.h"

using namespace std;
class CMap
{
public:
    CMap(int capacity);           //���캯��������ͼ
    ~CMap();                      //��������������ͼ

    bool addNode(Node* pNode);    //���ӽڵ�
    void resetMap();             //�����������ӹ�ϵ
    //������ͼ���ڽӾ����ex);
    vector<Node> DFS1(int nodeIndex);
    void dfs_for_error_unit();
    void getPath(vector<Node>vector_sw, int index);
    void getPath1(vector<Node>noDegree, vector<Node>vector_sw, int index);
    bool setValueToMatrixForDirectedGraph(int row, int col, int val = 1);
    //������ͼ���ڽӾ����Ԫ������ֵ
    bool setValueToMatrixForUndirectedGraph(int row, int col, int val = 1);

    void printMartrix();          //��ӡ�ڽӾ���
    void print_DFS_res();
    //void printNode();
    void depthFirstTraverse(int nodeIndex);//������ȱ���
    vector<Node> DFS(int nodeIndndex);
    void breadthFirstTraverse(int nodeIndex);            //������ȱ���
    void emit_a_link(int a, int b);
    void emit_a_link1(int a, int b);
    int find1(int nodeData);
    void updateDegree();
    void copyMap();
    Node find(int nodeData);
    bool port_for_errorfinding;//���ϼ��˿�ģ��
private:
    bool getValueFromMatrix(int row, int col, int& val);   //��ȡ�ڽӾ�����Ԫ�ص�ֵ
    //������ȱ����Ĺ��ߺ����������洢ĳһ���Ԫ��
    void breadthFirstTraverseImpl(vector<int> preVec);


public:
    int m_iNodeCount;              //ͼ��ǰ�Ľڵ������
    int m_iCapacity;               //ͼ�Ľڵ������                                
    Node* m_pNodeArray;            //ָ��ͼ���׽ڵ��ָ��

    int* m_pMatrix;                //ָ���ڽӾ�����Ԫ�ص�ָ��

    vector<Node> noDegree;
    vector<vector<Node>> noDegree_for_error_unit;
    vector<int> nodegree_id;
    bool been_dfs;//�Ƿ��Ѿ�����·���滮��Ĭ��Ϊfalse
};

#endif
#pragma once
#pragma once
