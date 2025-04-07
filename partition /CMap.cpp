#include "CMap.h"
#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

// Constructor to create the graph
CMap::CMap(int capacity) {
    m_iCapacity = capacity;
    m_iNodeCount = 0;
    m_pNodeArray = new Node[capacity];
    m_pMatrix = new int[capacity * capacity];
    for (int i = 0; i < capacity * capacity; ++i) {
        m_pMatrix[i] = 0;
    }
    been_dfs = false;
}

// Destructor to destroy the graph
CMap::~CMap() {
    delete[] m_pNodeArray;
    delete[] m_pMatrix;
}

// Add a node
bool CMap::addNode(Node* pNode) {
    if (m_iNodeCount >= m_iCapacity) {
        return false;
    }
    m_pNodeArray[m_iNodeCount++] = *pNode;
    return true;
}

// Reset the connection relationships
void CMap::resetMap() {
    for (int i = 0; i < m_iCapacity * m_iCapacity; ++i) {
        m_pMatrix[i] = 0;
    }
}

// Depth-first search from a specific node
vector<Node> CMap::DFS1(int startIndex) {
    vector<Node> result;
    if (startIndex < 0 || startIndex >= m_iNodeCount) return result;

    stack<int> s;
    vector<vector<bool>> visitedEdges(m_iNodeCount, vector<bool>(m_iNodeCount, false));
    s.push(startIndex);

    while (!s.empty()) {
        int current = s.top();
        bool hasUnvisitedEdge = false;

        // 遍历所有邻接节点
        for (int i = 0; i < m_iNodeCount; ++i) {
            if (m_pMatrix[current * m_iCapacity + i] != 0 && !visitedEdges[current][i]) {
                // 标记无向图的边为已访问
                visitedEdges[current][i] = true;
                visitedEdges[i][current] = true;
                s.push(i);
                hasUnvisitedEdge = true;
                break; // 继续深入
            }
        }

        // 无未访问的边，回溯并记录节点
        if (!hasUnvisitedEdge) {
            result.push_back(m_pNodeArray[current]);
            s.pop();
        }
    }

    // 反转结果以得到正确顺序
    //reverse(result.begin(), result.end());
    return result;
}



//m_pNodeArray[i].degree
/*
vector<Node> CMap::DFS1(int nodeIndex) {
    vector<Node> result;
    vector<bool> visited(m_iNodeCount, false);
    stack<int> s;
    s.push(nodeIndex);
    int index = s.top();
    //s.pop();
    while (!s.empty()) {
        //int index = s.top();
        //s.pop();
        //cout << "degree:" << m_pNodeArray[index].degree;
        if (m_pNodeArray[index].degree>0) {
            visited[index] = false; 
            result.push_back(m_pNodeArray[index]);
            for (int i = m_iNodeCount - 1; i >= 0; --i) {
                if (m_pMatrix[index * m_iCapacity + i] != 0 && !visited[i] && m_pNodeArray[index].degree > 0) {
                    s.pop();
                    s.push(i); 
                    m_pNodeArray[index].degree--; 
                    m_pNodeArray[i].degree--; 
                    m_pMatrix[index * m_iCapacity + i] = 0; 
                    m_pMatrix[i * m_iCapacity + index] = 0; 
                    //cout<<"i"<<i<<":"<<m_pMatrix[i * m_iCapacity + index]<<endl;
                    index = i;
                    break;
                }
            }
        }
        else {
            visited[index] = true;
            s.pop();
        }
    }
    return result;
}*/

/*
vector<Node> CMap::DFS1(int nodeIndex) {
    vector<Node> result;
    vector<bool> visited(m_iNodeCount, false);
    stack<int> s;
    s.push(nodeIndex);
    while (!s.empty()) {
        int index = s.top();
        s.pop();
        if (!visited[index]) {
            visited[index] = true;
            result.push_back(m_pNodeArray[index]);
            for (int i = m_iNodeCount - 1; i >= 0; --i) {
                if (m_pMatrix[index * m_iCapacity + i] != 0 && !visited[i]) {
                    s.push(i);
                }
            }
        }
    }
    return result;
}*/

// Depth-first search for error unit
void CMap::dfs_for_error_unit() {
    for (int i = 0; i < m_iNodeCount; ++i) {
        if (!m_pNodeArray[i].m_bIsVisited) {
            DFS1(i);
            noDegree_for_error_unit.push_back(noDegree);
        }
    }
}

// Get path based on vector of nodes and index
void CMap::getPath(vector<Node> vector_sw, int index) {
    for (int i = 0; i < vector_sw.size(); ++i) {
        if (vector_sw[i].m_cData == m_pNodeArray[index].m_cData) {
            cout << vector_sw[i].m_cData << " ";
        }
    }
    cout << endl;
}

// Get path based on nodes with no degree and vector of nodes and index
void CMap::getPath1(vector<Node> noDegree, vector<Node> vector_sw, int index) {
    for (int i = 0; i < noDegree.size(); ++i) {
        if (noDegree[i].m_cData == vector_sw[index].m_cData) {
            cout << noDegree[i].m_cData << " ";
        }
    }
    cout << endl;
}

// Set value to matrix for directed graph
bool CMap::setValueToMatrixForDirectedGraph(int row, int col, int val) {
    if (row >= m_iNodeCount || col >= m_iNodeCount || row < 0 || col < 0) {
        return false;
    }
    m_pMatrix[row * m_iCapacity + col] = val;
    return true;
}

// Set value to matrix for undirected graph
bool CMap::setValueToMatrixForUndirectedGraph(int row, int col, int val) {
    if (row >= m_iNodeCount || col >= m_iNodeCount || row < 0 || col < 0) {
        return false;
    }
    m_pMatrix[row * m_iCapacity + col] = val;
    m_pMatrix[col * m_iCapacity + row] = val;
    return true;
}

// Print the adjacency matrix
void CMap::printMartrix() {
    for (int i = 0; i < m_iNodeCount; ++i) {
        for (int j = 0; j < m_iNodeCount; ++j) {
            cout << m_pMatrix[i * m_iCapacity + j] << " ";
        }
        cout << endl;
    }
}

// Print depth-first search result
void CMap::print_DFS_res() {
    vector<Node> dfsResult = DFS1(0);
    for (const auto& node : dfsResult) {
        cout << node.m_cData << " ";
    }
    cout << endl;
}

// Depth-first traversal from a specific node
void CMap::depthFirstTraverse(int nodeIndex) {
    vector<bool> visited(m_iNodeCount, false);
    stack<int> s;
    s.push(nodeIndex);
    while (!s.empty()) {
        int index = s.top();
        s.pop();
        if (!visited[index]) {
            visited[index] = true;
            cout << m_pNodeArray[index].m_cData << " ";
            for (int i = m_iNodeCount - 1; i >= 0; --i) {
                if (m_pMatrix[index * m_iCapacity + i] != 0 && !visited[i]) {
                    s.push(i);
                }
            }
        }
    }
    cout << endl;
}

// Depth-first search from a specific node
vector<Node> CMap::DFS(int nodeIndex) {
    return DFS1(nodeIndex);
}

// Breadth-first traversal from a specific node
void CMap::breadthFirstTraverse(int nodeIndex) {
    vector<bool> visited(m_iNodeCount, false);
    queue<int> q;
    q.push(nodeIndex);
    visited[nodeIndex] = true;
    while (!q.empty()) {
        int index = q.front();
        q.pop();
        cout << m_pNodeArray[index].m_cData << " ";
        for (int i = 0; i < m_iNodeCount; ++i) {
            if (m_pMatrix[index * m_iCapacity + i] != 0 && !visited[i]) {
                q.push(i);
                visited[i] = true;
            }
        }
    }
    cout << endl;
}

// Emit a link between two nodes
void CMap::emit_a_link(int a, int b) {
    setValueToMatrixForDirectedGraph(a, b);
}

// Emit a link between two nodes (alternative method)
void CMap::emit_a_link1(int a, int b) {
    setValueToMatrixForUndirectedGraph(a, b);
}

// Find a node by its data
int CMap::find1(int nodeData) {
    for (int i = 0; i < m_iNodeCount; ++i) {
        if (m_pNodeArray[i].m_cData == nodeData) {
            return i;
        }
    }
    return -1;
}

// Update node degrees
void CMap::updateDegree() {
    for (int i = 0; i < m_iNodeCount; ++i) {
        int degree = 0;
        for (int j = 0; j < m_iNodeCount; ++j) {
            if (m_pMatrix[i * m_iCapacity + j] != 0) {
                ++degree;
            }
        }
        m_pNodeArray[i].degree = degree;
    }
}

// Copy the map
void CMap::copyMap() {
    CMap* newMap = new CMap(m_iCapacity);
    newMap->m_iNodeCount = m_iNodeCount;
    for (int i = 0; i < m_iNodeCount; ++i) {
        newMap->m_pNodeArray[i] = m_pNodeArray[i];
    }
    for (int i = 0; i < m_iNodeCount * m_iNodeCount; ++i) {
        newMap->m_pMatrix[i] = m_pMatrix[i];
    }
}

// Find a node by its data
Node CMap::find(int nodeData) {
    for (int i = 0; i < m_iNodeCount; ++i) {
        if (m_pNodeArray[i].m_cData == nodeData) {
            return m_pNodeArray[i];
        }
    }
    // Return an invalid Node if not found
    return Node();
}

// Get the value from the adjacency matrix
bool CMap::getValueFromMatrix(int row, int col, int& val) {
    if (row >= m_iNodeCount || col >= m_iNodeCount || row < 0 || col < 0) {
        return false;
    }
    val = m_pMatrix[row * m_iCapacity + col];
    return true;
}

// Breadth-first traversal implementation
void CMap::breadthFirstTraverseImpl(vector<int> preVec) {
    for (int nodeIndex : preVec) {
        breadthFirstTraverse(nodeIndex);
    }
}
