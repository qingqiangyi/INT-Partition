
#ifndef NODE_H_


#define NODE_H_

class Node
{
public:
	Node(int data = 0, int nodekind = 0); //�ڵ�Ĺ��캯��
	int m_cData;        //�ڵ��������
	bool m_bIsVisited;   //��Ǹýڵ��Ƿ񱻷��ʹ�
	char node_kind;      //�ڵ����ͣ���ԽС�㼶Խ�ͣ�0��host ��1��leaf��2 ��spine;����������1tor��2leaf��3spine��
	int degree;//�ڵ�Ķȣ���ʼΪ0��·���滮ʱ�ٱ������˼���
};

#endif
#pragma once
#pragma once
