
#include "Node.h"

Node::Node(int data, int nodekind)
{
	m_cData = data;
	m_bIsVisited = false;//Ĭ��Ϊ�ýڵ�û�б����ʹ�
	node_kind = nodekind;
	degree = 0;
}