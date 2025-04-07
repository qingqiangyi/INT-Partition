
#ifndef NODE_H_


#define NODE_H_

class Node
{
public:
	Node(int data = 0, int nodekind = 0); //节点的构造函数
	int m_cData;        //节点的数据域
	bool m_bIsVisited;   //标记该节点是否被访问过
	char node_kind;      //节点类型，数越小层级越低，0是host ，1是leaf，2 是spine;如果三层就是1tor、2leaf、3spine；
	int degree;//节点的度，初始为0，路径规划时再遍历拓扑计入
};

#endif
#pragma once
#pragma once
