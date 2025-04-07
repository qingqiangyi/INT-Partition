
#include "Node.h"

Node::Node(int data, int nodekind)
{
	m_cData = data;
	m_bIsVisited = false;//默认为该节点没有被访问过
	node_kind = nodekind;
	degree = 0;
}
