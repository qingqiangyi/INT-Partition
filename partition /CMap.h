
#ifndef CMAP_H_


#define CMAP_H_

//包含vector类模板，后面的函数breadthFirstTraverseImpl(vector<int> preVec)会用到

#include <vector>


#include "Node.h"

using namespace std;
class CMap
{
public:
    CMap(int capacity);           //构造函数，创建图
    ~CMap();                      //析构函数，销毁图

    bool addNode(Node* pNode);    //增加节点
    void resetMap();             //重置拓扑连接关系
    //给有向图的邻接矩阵的ex);
    vector<Node> DFS1(int nodeIndex);
    void dfs_for_error_unit();
    void getPath(vector<Node>vector_sw, int index);
    void getPath1(vector<Node>noDegree, vector<Node>vector_sw, int index);
    bool setValueToMatrixForDirectedGraph(int row, int col, int val = 1);
    //给无向图的邻接矩阵的元素设置值
    bool setValueToMatrixForUndirectedGraph(int row, int col, int val = 1);

    void printMartrix();          //打印邻接矩阵
    void print_DFS_res();
    //void printNode();
    void depthFirstTraverse(int nodeIndex);//深度优先遍历
    vector<Node> DFS(int nodeIndndex);
    void breadthFirstTraverse(int nodeIndex);            //广度优先遍历
    void emit_a_link(int a, int b);
    void emit_a_link1(int a, int b);
    int find1(int nodeData);
    void updateDegree();
    void copyMap();
    Node find(int nodeData);
    bool port_for_errorfinding;//故障检测端口模拟
private:
    bool getValueFromMatrix(int row, int col, int& val);   //获取邻接矩阵中元素的值
    //广度优先遍历的工具函数，用来存储某一层的元素
    void breadthFirstTraverseImpl(vector<int> preVec);


public:
    int m_iNodeCount;              //图当前的节点的数量
    int m_iCapacity;               //图的节点的容量                                
    Node* m_pNodeArray;            //指向图中首节点的指针

    int* m_pMatrix;                //指向邻接矩阵首元素的指针

    vector<Node> noDegree;
    vector<vector<Node>> noDegree_for_error_unit;
    vector<int> nodegree_id;
    bool been_dfs;//是否已经进行路径规划，默认为false
};

#endif
#pragma once
#pragma once
