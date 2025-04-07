
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "CMap.h"
#include <map>
#include<thread>
#include<mutex>
#include <random>
#include <winternl.h>
#include <Windows.h>
#include<chrono>
#include"ExcelFormat.h"
#include <cmath>
#include "ThreadPool.h"
#include <future>
#include "ThreadStats.h"

//30x30单pod
using namespace std;
using namespace ExcelFormat;
using task_return_type = std::chrono::system_clock::duration;

std::random_device rd; //真实随机数产生器

std::mt19937 mt(rd()); //生成计算随机数mt;

std::uniform_int_distribution<int> dist(-1000, 1000);//生成-1000到1000之间的离散均匀分部数

auto rnd = std::bind(dist, mt);
mutex m;     //互斥
mutex m1;
mutex m2;
mutex m3;
mutex temp;


vector<vector<Node>> devide1_1;
vector<vector<Node>> devide1_2;
vector<vector<Node>> foundSets(vector<vector<Node>> nodeList, int length,int k);
vector<vector<Node>> foundPods(CMap* mapp, Node nodeList[], int length);
vector<vector<Node>> foundPods1(vector<vector<Node>> nodeList, int length, int k);
vector<vector<vector<Node>>> foundPods2(CMap* mapp, Node nodeList[], int length);//for 多pods
vector<vector<Node>> smallest_devision1(vector<vector<Node>> nodeList, int length, int Lmax, int k, int k1, int k2);
vector<vector<Node>> smallest_devision2(vector<vector<Node>> nodeList, int length, int Lmax, int k);
void print(vector<vector<Node>>nodeList);
void print_r(vector<Node>nodeList);
int layer2area(Node nodeList[], int length,int k);
vector<vector<Node>> devision(Node nodeList[], int k1, int k2, int Lmax, int add1, int add2, const int k, int k1_init, int k2_init);
std::vector<std::vector<Node>> devision1(std::vector<Node> nodeList, int k1, int k2, int Lmax, int add1, int add2, const int k, int k1_init, int k2_init);

void layer2Sub_area(CMap* mapp, Node* nodeList);
vector<vector<Node>> smallest_devision(vector<vector<Node>> nodeList, int length, int a, int b);
CMap* build_small_units_link(vector<Node> nodeList);
vector<Node> DepthFirstTraverse(CMap* map, int nodeIndex);
vector<Node> DepthFirstTraverse1(CMap* map, int nodeIndex, int i);
vector<CMap*> find_error_units(vector<CMap*>maps);
vector<Node> test(CMap* map);
void simulate_hard_computation();
void multithreads(int thread_num, vector<CMap*>maps_without_error, vector<vector<Node>>smallest_units);
/*
vector<vector<Node>> foundSets(Node nodeList[], int length,int k) {
	vector<Node> a_set;
	vector<vector<Node>> sets;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < 40; j++) {//************************************************
			if (nodeList[i + j].node_kind == '3') {
				a_set.push_back(nodeList[i + j]);
			}
		}
		sets.push_back(a_set);
		i += 40;//************************************************
		a_set.clear();

	}
	//pod.push_back(a_pod);
	if (sets.size() == 0) {
		cout << "**********there's no set***********" << endl;
		return {};
	}
	else
	{

		return sets;

	}
}
*/
//上层网络划分得到set
vector<vector<Node>> foundSets(vector<vector<Node>> nodeList, int length, int k) {
	vector<Node> a_set;
	vector<vector<Node>> sets;
	for (int n = 1; n < k / 2 + 1; n++) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < nodeList[i].size(); j++) {
				if (nodeList[i][j].node_kind == '3' && j > (n - 1) * k / 2 - 1 && j < n * k / 2) {
					a_set.push_back(nodeList[i][j]);
				}
				if (nodeList[i][j].node_kind == '2' && j % (k/2) == n-1) {
					a_set.push_back(nodeList[i][j]);
				}
				//cout << i <<"-" << j << endl;
			}
			

		}
		sets.push_back(a_set);
		a_set.clear();
	}
	
	//pod.push_back(a_pod);
	if (sets.size() == 0) {
		cout << "**********there's no set***********" << endl;
		return {};
	}
	else
	{

		return sets;

	}
}

vector<vector<Node>> foundPods(CMap* mapp, Node nodeList[], int length) {
	vector<Node> a_pod_1;
	vector<Node> a_pod_2;
	vector<vector<Node>> pod;
	map<int, char> pod_member;
	int last_switches_sum = length;
	int k;
	if (nodeList[0].node_kind == '3')  k = length / 3;
	if (nodeList[0].node_kind == '2')  k = length / 2;
	for (int i = 0; i < length; i++) {
		if (nodeList[i].node_kind != '3') {
			pod_member.insert(make_pair(nodeList[i].m_cData, nodeList[i].node_kind));

			for (int j = i; j < length; j++) {
				if (mapp->m_pMatrix[i * mapp->m_iCapacity + j] == 1) {
					pod_member.insert(make_pair(nodeList[j].m_cData, nodeList[j].node_kind));
				}
			}
		}
	}
	map<int, char>::iterator t;
	for (int i = 0; i < length; i += 2 * k) {
		for (int j = 0; j < 2 * k; j++) {
			t = pod_member.find(i * 2 * k + j);
			if (t != pod_member.end()) {
				if (t->second == '1') {
					Node* p = new Node(t->first, t->second);
					a_pod_1.push_back(*p);
					delete p;
				}
				if (t->second == '2') {
					Node* p = new Node(t->first, t->second);
					a_pod_2.push_back(*p);
					delete p;
				}
			}

		}
	}

	pod.push_back(a_pod_1);
	pod.push_back(a_pod_2);
	return pod;

}
//下层网络划分得到Pod
vector<vector<Node>> foundPods1(vector<vector<Node>> nodeList, int length, int k) {
	vector<vector<Node>> pod;
	//vector<Node> first_edge;
	//vector<Node> second_edge;
	vector<Node> a_pod;
	for (int n = 1; n < k + 1; n++) {
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < k * k / 2; j++) {
				if (nodeList[i][j].node_kind == '2' && j > (n - 1) * k / 2 - 1 && j < n * k / 2) {
					//first_edge.push_back(nodeList[i][j]);
					a_pod.push_back(nodeList[i][j]);
				}
				if (nodeList[i][j].node_kind == '1' && j > (n - 1) * k / 2 - 1 && j < n * k / 2) {
					//second_edge.push_back(nodeList[i][j]);
					a_pod.push_back(nodeList[i][j]);
				}
				//cout << i << "-" << j << endl;
			}
		}
		pod.push_back(a_pod);
		a_pod.clear();
	}
	
	//pod.push_back(first_edge);
	//pod.push_back(second_edge);
	return pod;
}
vector<vector<vector<Node>>> foundPods2(CMap* mapp, Node nodeList[], int length) {
	vector<Node>first_edge;
	vector<Node>second_edge;
	vector<vector<Node>>a_pod;
	vector<vector<vector<Node>>>pods;
	for (int i = 0; i < length; i++) {//找到第一个汇聚层sw开始找到pod，按照连接规律
		if (nodeList[i].node_kind == '2') {
			for (int j = 0; j < 40; j++) {//************************************************
				first_edge.push_back(nodeList[i + j]);
			}
			for (int r = 0; r < length; r++) {
				if (nodeList[r].node_kind == '1' && mapp->m_pMatrix[i * mapp->m_iCapacity + r] == 1) {
					second_edge.push_back(nodeList[r]);
				}
			}
			a_pod.push_back(first_edge);
			a_pod.push_back(second_edge);
			pods.push_back(a_pod);
			i += 39;//过渡到下一个pod的第一个汇聚层sw//************************************************
			first_edge.clear();
			second_edge.clear();
			a_pod.clear();
		}
		else continue;

	}

	return pods;
}
void print(vector<vector<Node>>nodeList) {
	for (int i = 0; i < nodeList.size(); i++) {
		for (int j = 0; j < nodeList[i].size(); j++) {
			cout << "s" << nodeList[i][j].m_cData << ",";
		}
		cout << endl;
	}
}
void print_r(vector<Node>nodeList) {
	//cout << "size=" << nodeList.size() << endl;
	for (int i = nodeList.size() - 1; i >= 0; i--) {
		if (i == 0) cout << "s" << nodeList[i].m_cData << endl;
		else cout << "s" << nodeList[i].m_cData << "->";
	}
}
//划分得到两层网络
int layer2area(Node nodeList[], int length,int k) {
	int num_flag = 0;
	vector<Node> first_edge;
	vector<Node> second_edge;


	if (nodeList[k * k / 4].node_kind == '2')
		num_flag = 3;
	if (nodeList[k * k / 4].node_kind == '1')
		num_flag = 2;
	//cout << length - 1 << ' ' << nodeList[k + 1].node_kind << endl;
	//cout << num_flag << endl;
	if (num_flag == 2) {

		for (int i = 0; i < length; i++) {

			if (nodeList[i].node_kind == '2')
				first_edge.push_back(nodeList[i]);

			if (nodeList[i].node_kind == '1')
				second_edge.push_back(nodeList[i]);
		}
		/*for (int i = 0; i < first_edge.size(); i++){
			cout << "*****************first edge***************" << endl;
			cout << first_edge[i].m_cData << " " << first_edge[i].node_kind << endl;
		}

		for (int i = 0; i < second_edge.size(); i++) {
			cout << "******************second edge****************" << endl;
			cout << second_edge[i].m_cData << " " << second_edge[i].node_kind << endl;
		}*/

		devide1_1.push_back(first_edge);
		devide1_1.push_back(second_edge);
		first_edge.clear();
		second_edge.clear();
	}
	if (num_flag == 3) {
		for (int i = 0; i < length; i++) {
			if (nodeList[i].node_kind == '3')
				first_edge.push_back(nodeList[i]);
			if (nodeList[i].node_kind == '2')
				second_edge.push_back(nodeList[i]);
		}
		devide1_1.push_back(first_edge);
		devide1_1.push_back(second_edge);
		first_edge.clear();
		second_edge.clear();
		for (int i = 0; i < length; i++) {
			if (nodeList[i].node_kind == '2')
				first_edge.push_back(nodeList[i]);
			if (nodeList[i].node_kind == '1')
				second_edge.push_back(nodeList[i]);
		}
		devide1_2.push_back(first_edge);
		devide1_2.push_back(second_edge);
		first_edge.clear();
		second_edge.clear();
		
	}
	return(num_flag);
}
void layer2Sub_area(CMap* mapp, Node* nodeList) {

}

//判断是否能够将拓扑划分为Nmin个单元
bool isValidDivision(int k1, int k2, int L, int Nmin, int a, int b) {
	return b % 2 == 0 && b < k2 + 1 && k1 % a == 0 && k2 % b == 0 && a < k1 + 1;
}
//将set数组中的每一行进行分割
vector<Node> createPart(vector<Node>& set, int start, int end) {
	vector<Node> part;
	for (int j = start; j < end; j++) {
		part.push_back(set[j]);
	}
	return part;
}
//将set数组分割好后存入deParts
vector<vector<Node>> createParts(vector<vector<Node>>& sets, int a, int b, int k,int na,int nb,int j1,int j2) {
	vector<vector<Node>> devParts;
	for (int i = 0; i < k / 2; i++) {
		vector<vector<Node>> sPart;
		for (int n = 0; n < na; n++) {
			sPart.push_back(createPart(sets[i], j1+n * a, j1+(n + 1) * a));
		}
		for (int n = 0; n < nb; n++) {
			sPart.push_back(createPart(sets[i], j2+n * b + k / 2, j2+(n + 1) * b + k / 2));
		}
		for (int m = 0; m < na; m++) {
			for (int n = 0; n < nb; n++) {
				vector<Node> Part = sPart[m];
				Part.insert(Part.end(), sPart[na + n].begin(), sPart[na + n].end());
				devParts.push_back(Part);
			}
		}
	}
	return devParts;
}
//第二次划分
vector<vector<Node>> secondDivision(vector<vector<Node>>& sets, int k1, int k2, int k,int Nmin, int L, int j1, int j2) {
	vector<vector<Node>> devParts;
	if (k1 * k2 % Nmin == 0 && k1 * k2 / Nmin % 4 == 0) {//剩余拓扑能够划分为Nmin个区域
		L = k1 * k2 / Nmin;
		for (int a1 = 2; a1 < k1 * k2 / Nmin + 1; a1 += 2) {
			int b1 = k1 * k2 / Nmin / a1;
			if (isValidDivision(k1, k2, L, Nmin, a1, b1)) {
				int na1 = k1 / a1, nb1 = k2 / b1;
				vector<vector<Node>> newParts = createParts(sets, a1, b1, k, na1, nb1, j1, j2);
				devParts.insert(devParts.end(), newParts.begin(), newParts.end());
				break;
			}
		}
	}
	else {//剩余拓扑不能划分为Nmin个区域
		bool found = false;
		int rest1 = 0, rest2 = 0;
		for (; L > 0; L -= 4) {
			for (int a1 = 2; a1 < k1 + 1; a1 += 2) {
				if (L % a1 == 0 && L / a1 % 2 == 0 && L / a1 < k2 + 1) {
					int b1 = L / a1;
					int na1 = k1 / a1, nb1 = k2 / b1;
					vector<vector<Node>> newParts = createParts(sets, a1, b1, k, na1, nb1, j1, j2);
					devParts.insert(devParts.end(), newParts.begin(), newParts.end());
					rest1 = k1 % a1; rest2 = k2 % b1; Nmin = Nmin - newParts.size() * 2 / k;
					found = true;
					break;
				}
			}
			if (found) break;
		}
		if (rest1 == 0 && rest2 == 0) { cout << "完成划分" << endl; }
		if (rest1 != 0 && rest2 == 0) {
			k1 = rest1; k2 = k2 - rest2;
			vector<vector<Node>> newParts1 = secondDivision(sets, k1, k2, k, Nmin, L, k1-rest1, k-k2);
			devParts.insert(devParts.end(), newParts1.begin(), newParts1.end());
		}
		/******尚未验证******/
		if (rest1 == 0 && rest2 != 0) {
			k1 = k1 - rest1; k2 = rest2;
			vector<vector<Node>> newParts1 = secondDivision(sets, k1, k2, k, Nmin, L, k1 - rest1, k - k2);
			devParts.insert(devParts.end(), newParts1.begin(), newParts1.end());
		}
		if (rest1 != 0 && rest2 != 0) {
			k1 = rest1; k2 = k2 - rest2;
			vector<vector<Node>> newParts1 = secondDivision(sets, k1, k2, k, Nmin, L, k1 - rest1, k - k2);
			devParts.insert(devParts.end(), newParts1.begin(), newParts1.end());
		}
		//cout << rest1 << endl; cout << rest2 << endl;
	}
	return devParts;
}

//上层网络最小单元划分
vector<vector<Node>> smallest_devision1(vector<vector<Node>> nodeList, int length, int Lmax, int k, int k1 = 0, int k2 = 0) {
	vector<Node> a_set;
	vector<vector<Node>> sets;
	vector<vector<Node>> devParts;
	vector<Node> sPart1;
	vector<vector<Node>> sPart;

	int L = Lmax / 4 * 4;
	//将两层网络的交换机数量变为偶数
	if (k / 2 % 2 != 0) {
		for (int i = 0; i < k / 2; i++) {
			a_set.push_back(nodeList[i][0]);
			for (int j = 0; j < 3 * k / 2; j++) {
				a_set.push_back(nodeList[i][j]);
			}
			sets.push_back(a_set);
			a_set.clear();
		}
		for (int i = 0; i < k / 2; i++) {
			vector<vector<Node>> part= devision1(sets[i], k / 2+1, k, L, 0, 0, k * 3 / 2+1, k / 2+1, k);
			devParts.insert(devParts.end(), part.begin(), part.end());
			part.clear();
			
		}
	}
	else {
		for (int i = 0; i < k / 2; i++) {
			for (int j = 0; j < 3 * k / 2; j++) {
				a_set.push_back(nodeList[i][j]);
				//cout << i << "-" << j<<endl;
			}
			sets.push_back(a_set);
			a_set.clear();
		}
		
		//判断拓扑是否能被划分为Nmin个区域
		int Nmin = std::ceil(static_cast<double>(k) * k / 2 / L);
		//cout << Nmin << endl;

		bool found1 = false;
		for (; L > 0; L -= 4) {  //当当前的L不符合条件时，寻找L=L-4，继续对新的L进行划分
			Nmin = std::ceil(static_cast<double>(k) * k / 2 / L);
			for (int a = 2; a < L / 2 + 1; a += 2) {  //第一次划分
				if (k * k / 2 % Nmin == 0 && k * k / 2 / Nmin % 4 == 0)  //可以被划分为Nmin个区域
				{
					L = k * k / 2 / Nmin;
					int b = L / a;
					if (isValidDivision(k / 2, k, L, Nmin, a, b)) {
						devParts = createParts(sets, a, b, k, k / 2 / a, k / b, 0, 0);
						found1 = true;
						break;
					}
				}
				if (found1) break;
				else {  //不可以被划分为Nmin个区域
					for (int a = 2; a < k * k / 4 / Nmin && a < k / 2 + 1; a += 2) {
						if (L % a == 0)
						{
							int b = L / a;
							if (b % 2 == 0 && b < k + 1) {
								int b = L / a;
								int na = k / 2 / a, nb = k / b, rest1 = k / 2 % a, rest2 = k % b;
								//将拓扑上层交换机分成na*a组，下层交换机分成nb*b组，将这几组交换机互相组合，之后对剩余的交换机重新划分
								devParts = createParts(sets, a, b, k, na, nb, 0, 0);
								/******************上面这部分实现了首先将na*nb*a*b这几个区域划分出来*********************/
								if (rest1 == 0 && rest2 == 0) {  //第二次划分
									cout << "完成划分" << endl;
								}
								if (rest1 != 0 && rest2 == 0) {
									k1 = rest1;
									k2 = k - rest2;
									Nmin = Nmin - devParts.size() * 2 / k;//判断是否能够直接划分为Nmin个单元
									vector<vector<Node>> newParts1 = secondDivision(sets, k1, k2, k, Nmin, L, na * a, 0);
									devParts.insert(devParts.end(), newParts1.begin(), newParts1.end());	//完成第二次划分，还需要对剩余拓扑继续划分
									/***********只能在secondDivision函数中继续划分？？？因为要定义rest3需要a1，但是a1在这里未定义*************/
								}
								if (rest1 == 0 && rest2 != 0) {
									k1 = k / 2 - rest1;
									k2 = rest2;
									Nmin = Nmin - devParts.size() * 2 / k;//判断是否能够直接划分为Nmin个单元
									vector<vector<Node>> newParts1 = secondDivision(sets, k1, k2, k, Nmin, L, 0, nb * b);
									devParts.insert(devParts.end(), newParts1.begin(), newParts1.end());
									/*************继续划分*************/
								}
								if (rest1 != 0 && rest2 != 0) {
									//处理剩余的第一个拓扑
									k1 = k / 2 - rest1;
									k2 = rest2;
									//Nmin = Nmin - devParts.size() * 2 / k;//判断是否能够直接划分为Nmin个单元
									Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
									vector<vector<Node>> newParts1 = secondDivision(sets, k1, k2, k, Nmin, L, 0, nb * b);
									devParts.insert(devParts.end(), newParts1.begin(), newParts1.end());

									//处理剩余的第二个拓扑
									k1 = rest1;
									k2 = k;
									//Nmin = Nmin - devParts.size() * 2 / k;//判断是否能够直接划分为Nmin个单元
									Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
									vector<vector<Node>> newParts2 = secondDivision(sets, k1, k2, k, Nmin, L, na * a, 0);
									devParts.insert(devParts.end(), newParts2.begin(), newParts2.end());
								} 
								found1 = true;
								break;
							}
						}
					}if (found1) break;
				}
			}
			if (found1) break;
		}
	}	
	return devParts;
}

std::vector<std::vector<Node>> devision1(std::vector<Node> nodeList, int k1, int k2, int Lmax, int add1, int add2, const int k, int k1_init, int k2_init) {
	vector<vector<Node>> devParts;
	vector<vector<Node>> sPart;
	int L = Lmax / 4 * 4;
	std::vector<Node> newNodeList;
	//const int k = k1 + k2;
	//将上下两层交换机的数量1均变为偶数
	if (k1 % 2 != 0 && k2 % 2 == 0) {
		newNodeList.resize(k1 + k2 + 1);
		newNodeList[0] = nodeList[0];
		std::copy(nodeList.begin(), nodeList.begin() + k1 + k2, newNodeList.begin() + 1);
		k1 = k1 + 1;
	}
	if (k2 % 2 != 0 && k1 % 2 == 0) {
		newNodeList.resize(k1 + k2 + 1);
		std::copy(nodeList.begin(), nodeList.begin() + k1 + k2, newNodeList.begin());
		newNodeList[k1 + k2] = nodeList[k1 + k2 - 1];
		k2 = k2 + 1;
	}
	if (k1 % 2 != 0 && k2 % 2 != 0) {
		newNodeList.resize(k1 + k2 + 2);
		newNodeList[0] = nodeList[0];
		std::copy(nodeList.begin(), nodeList.begin() + k1 + k2, newNodeList.begin() + 1);
		newNodeList[k1 + k2 + 1] = nodeList[k1 + k2 - 1];
		k1 = k1 + 1; k2 = k2 + 1;
	}
	if (k1 % 2 == 0 && k2 % 2 == 0) { newNodeList.resize(k); std::copy(nodeList.begin(), nodeList.begin() + k, newNodeList.begin()); }

	int Nmin;
	bool found1 = false;
	for (; L > 0; L -= 4) {  //当当前的L不符合条件时，寻找L=L-4，继续对新的L进行划分
		Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
		if (k1 * k2 % Nmin == 0 && k1 * k2 / Nmin % 4 == 0)  //可以被划分为Nmin个区域
		{
			for (int a = 2; a < L / 2 + 1; a += 2) {
				L = k1 * k2 / Nmin;
				int b = L / a;
				if (isValidDivision(k1, k2, L, Nmin, a, b)) {
					for (int n = 0; n < k1 / a; n++) {
						sPart.push_back(createPart(newNodeList, add1 + n * a, add1 + (n + 1) * a));
					}
					for (int n = 0; n < k2 / b; n++) {
						sPart.push_back(createPart(newNodeList, add2 + n * b + k1_init, add2 + (n + 1) * b + k1_init));
					}
					for (int m = 0; m < k1 / a; m++) {
						for (int n = 0; n < k2 / b; n++) {
							vector<Node> Part = sPart[m];
							Part.insert(Part.end(), sPart[k1 / a + n].begin(), sPart[k1 / a + n].end());
							devParts.push_back(Part);
						}
					}
					found1 = true;
					break;
				}
			}
		}
		if (found1) break;
		else {  //不可以被划分为Nmin个区域
			for (int a = 2; a < L + 1 && a < k1 + 1; a += 2) {
				if (L % a == 0)
				{
					int b = L / a;
					if (b % 2 == 0 && b < k2 + 1) {
						int b = L / a;
						int na = k1 / a, nb = k2 / b, rest1 = k1 % a, rest2 = k2 % b;
						//将拓扑上层交换机分成na*a组，下层交换机分成nb*b组，将这几组交换机互相组合，之后对剩余的交换机重新划分
						for (int n = 0; n < k1 / a; n++) {
							sPart.push_back(createPart(newNodeList, add1 + n * a, add1 + (n + 1) * a));
						}
						for (int n = 0; n < k2 / b; n++) {
							sPart.push_back(createPart(newNodeList, add2 + n * b + k1_init, add2 + (n + 1) * b + k1_init));
						}
						for (int m = 0; m < k1 / a; m++) {
							for (int n = 0; n < k2 / b; n++) {
								vector<Node> Part = sPart[m];
								Part.insert(Part.end(), sPart[k1 / a + n].begin(), sPart[k1 / a + n].end());
								devParts.push_back(Part);
							}
						}
						/******************上面这部分实现了首先将na*nb*a*b这几个区域划分出来*********************/
						if (rest1 == 0 && rest2 == 0) {  //第二次划分
							cout << "完成划分" << endl;
						}
						if (rest1 != 0 && rest2 == 0) {
							add1 = k1 / a * a; add2 = 0;
							int k1_1 = rest1, k2_1 = k2 - rest2;
							Nmin = Nmin - devParts.size();//判断是否能够直接划分为Nmin个单元
							vector<vector<Node>> newParts = devision1(nodeList, k1_1, k2_1, L, add1, add2, k, k1_init, k2_init);
							devParts.insert(devParts.end(), newParts.begin(), newParts.end());	//完成第二次划分，还需要对剩余拓扑继续划分
							/***********只能在secondDivision函数中继续划分？？？因为要定义rest3需要a1，但是a1在这里未定义*************/
						}
						if (rest1 == 0 && rest2 != 0) {
							add1 = 0; add2 = k2 / b * b;
							int k1_1 = k1 - rest1, k2_1 = rest2;
							Nmin = Nmin - devParts.size();//判断是否能够直接划分为Nmin个单元
							vector<vector<Node>> newParts = devision1(nodeList, k1_1, k2_1, L, add1, add2, k, k1_init, k2_init);
							devParts.insert(devParts.end(), newParts.begin(), newParts.end());
							/*************继续划分*************/
						}
						if (rest1 != 0 && rest2 != 0) {
							//处理剩余的第一个拓扑
							add1 = 0; add2 = k2 / b * b;
							int k1_1 = k1 - rest1, k1_2 = rest2;
							//Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
							vector<vector<Node>> newParts = devision1(nodeList, k1_1, k1_2, L, add1, add2, k, k1_init, k2_init);
							devParts.insert(devParts.end(), newParts.begin(), newParts.end());

							//处理剩余的第二个拓扑
							add1 = k1 / a * a; add2 = 0;
							int k2_1 = rest1, k2_2 = k2;
							//Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
							vector<vector<Node>> newParts2 = devision1(nodeList, k2_1, k2_2, L, add1, add2, k, k1_init, k2_init);
							devParts.insert(devParts.end(), newParts2.begin(), newParts2.end()); 
						}
						found1 = true;
						break;
					}
				}
			}if (found1) break;
		}


		if (found1) break;
	}

	return devParts;
}

vector<vector<Node>> devision(Node nodeList[], int k1, int k2,int Lmax,int add1, int add2,const int k,int k1_init,int k2_init) {
	vector<vector<Node>> devParts;
	vector<vector<Node>> sPart;
	int L = Lmax / 4 * 4; 
	std::vector<Node> newNodeList;
	//const int k = k1 + k2;
	//将上下两层交换机的数量1均变为偶数
	if (k1 % 2 != 0&& k2 % 2 == 0) {
		newNodeList.resize(k1+k2 + 1);
		newNodeList[0] = nodeList[0];
		std::copy(nodeList, nodeList + k1+k2, newNodeList.begin() + 1);
		k1 = k1 + 1; 
	}
	if (k2 % 2 != 0&& k1 % 2 == 0) {
		newNodeList.resize(k1+k2 + 1);
		std::copy(nodeList, nodeList + k1+k2, newNodeList.begin());
		newNodeList[k1+k2] = nodeList[k1+k2 - 1];
		k2 = k2 + 1;
	}
	if (k1 % 2 != 0 && k2 % 2 != 0) {
		newNodeList.resize(k1+k2 + 2);
		newNodeList[0] = nodeList[0];
		std::copy(nodeList, nodeList + k1+k2, newNodeList.begin()+1);
		newNodeList[k1+k2+1] = nodeList[k1+k2-1];
		k1 = k1 + 1; k2 = k2 + 1; 
	}
	if (k1 % 2 == 0 && k2 % 2 == 0) { newNodeList.resize(k); std::copy(nodeList, nodeList + k, newNodeList.begin()); }
	
	int Nmin;
	bool found1 = false;
	for (; L > 0; L -= 4) {  //当当前的L不符合条件时，寻找L=L-4，继续对新的L进行划分
		Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
		if (k1 * k2 % Nmin == 0 && k1 * k2 / Nmin % 4 == 0)  //可以被划分为Nmin个区域
		{
			for (int a = 2; a < L / 2 + 1; a += 2) {
				L = k1 * k2 / Nmin;
				int b = L / a; 
				if (isValidDivision(k1, k2, L, Nmin, a, b)) {
					for (int n = 0; n < k1 / a; n++) {
						sPart.push_back(createPart(newNodeList, add1 + n * a, add1 + (n + 1) * a));
					}
					for (int n = 0; n < k2 / b; n++) {
						sPart.push_back(createPart(newNodeList, add2 + n * b + k1_init, add2 + (n + 1) * b + k1_init));
					}
					for (int m = 0; m < k1 / a; m++) {
						for (int n = 0; n < k2 / b; n++) {
							vector<Node> Part = sPart[m];
							Part.insert(Part.end(), sPart[k1 / a + n].begin(), sPart[k1 / a + n].end());
							devParts.push_back(Part);
						}
					}
					found1 = true;
					break;
				}
			}			
		}
		if (found1) break;
		else {  //不可以被划分为Nmin个区域
			for (int a = 2; a < L + 1 && a < k1 + 1; a += 2) {
				if (L % a == 0)
				{
					int b = L / a;
					if (b % 2 == 0 && b < k2 + 1) {
						int b = L / a;
						int na = k1 / a, nb = k2 / b, rest1 = k1 % a, rest2 = k2 % b;
						//将拓扑上层交换机分成na*a组，下层交换机分成nb*b组，将这几组交换机互相组合，之后对剩余的交换机重新划分
						for (int n = 0; n < k1 / a; n++) {
							sPart.push_back(createPart(newNodeList, add1 + n * a, add1 + (n + 1) * a));
						}
						for (int n = 0; n < k2 / b; n++) {
							sPart.push_back(createPart(newNodeList, add2 + n * b + k1_init, add2 + (n + 1) * b + k1_init));
						}
						for (int m = 0; m < k1 / a; m++) {
							for (int n = 0; n < k2 / b; n++) {
								vector<Node> Part = sPart[m];
								Part.insert(Part.end(), sPart[k1 / a + n].begin(), sPart[k1 / a + n].end());
								devParts.push_back(Part);
							}
						}
						/******************上面这部分实现了首先将na*nb*a*b这几个区域划分出来*********************/
						if (rest1 == 0 && rest2 == 0) {  //第二次划分
							cout << "完成划分" << endl;
						}
						if (rest1 != 0 && rest2 == 0) {
							add1 = k1 / a * a; add2 = 0; 
							int k1_1 = rest1, k2_1 = k2 - rest2;
							Nmin = Nmin - devParts.size();//判断是否能够直接划分为Nmin个单元
							vector<vector<Node>> newParts = devision(nodeList, k1_1, k2_1, L, add1, add2,k,k1,k2);
							devParts.insert(devParts.end(), newParts.begin(), newParts.end());	//完成第二次划分，还需要对剩余拓扑继续划分
							/***********只能在secondDivision函数中继续划分？？？因为要定义rest3需要a1，但是a1在这里未定义*************/
						}
						if (rest1 == 0 && rest2 != 0) {
							add1 = 0; add2 = k2 / b * b;
							int k1_1 = k1 - rest1, k2_1 = rest2;
							Nmin = Nmin - devParts.size();//判断是否能够直接划分为Nmin个单元
							vector<vector<Node>> newParts = devision(nodeList, k1_1, k2_1, L, add1, add2,k,k1,k2);
							devParts.insert(devParts.end(), newParts.begin(), newParts.end());
							/*************继续划分*************/
						}
						if (rest1 != 0 && rest2 != 0) {
							//处理剩余的第一个拓扑
							add1 = 0; add2 = k2 / b * b; 
							int k1_1 = k1 - rest1, k1_2 = rest2;
							//Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
							vector<vector<Node>> newParts = devision(nodeList, k1_1, k1_2, L, add1, add2,k,k1,k2);
							devParts.insert(devParts.end(), newParts.begin(), newParts.end());

							//处理剩余的第二个拓扑
							add1 = k1 / a * a; add2 = 0; 
							int k2_1 = rest1, k2_2 = k2;
							//Nmin = std::ceil(static_cast<double>(k1) * k2 / L);
							vector<vector<Node>> newParts2 = devision(nodeList, k2_1, k2_2, L, add1, add2,k,k1,k2);
							devParts.insert(devParts.end(), newParts2.begin(), newParts2.end());
						}
						found1 = true;
						break;
					}
				}
			}if (found1) break;
		}
		
		
		for (int a = 2; a < L / 2 + 1; a += 2) {  //第一次划分
			
		}
		if (found1) break;
	}
	
	return devParts;
}
//下层网络最小单元划分
vector<vector<Node>> smallest_devision2(vector<vector<Node>> nodeList, int length, int Lmax, int k) {
	vector<Node> a_set;
	vector<vector<Node>> sets;
	vector<vector<Node>> devParts;
	vector<Node> sPart1;
	vector<vector<Node>> sPart;

	int L = Lmax / 4 * 4;
	//将两层网络的交换机数量变为偶数
	if (k / 2 % 2 != 0) {
		for (int i = 0; i < k; i++) {
			a_set.push_back(nodeList[i][0]);
			for (int j = 0; j < k; j++) {
				a_set.push_back(nodeList[i][j]);
			}
			a_set.push_back(nodeList[i][k-1]);
			sets.push_back(a_set);
			a_set.clear();
		}
		for (int i = 0; i < k; i++) {
			vector<vector<Node>> part = devision1(sets[i], k / 2 + 1, k / 2 + 1, L, 0, 0, k + 2, k / 2 + 1, k / 2 + 1);
			devParts.insert(devParts.end(), part.begin(), part.end());
			part.clear();

		}
	}
	else {
		for (int i = 0; i < k; i++) {
			for (int j = 0; j < k; j++) {
				a_set.push_back(nodeList[i][j]);
				//cout << i << "-" << j<<endl;
			}
			sets.push_back(a_set);
			a_set.clear();
		}
		for (int i = 0; i < k; i++) {
			vector<vector<Node>> part = devision1(sets[i], k / 2, k / 2, L, 0, 0, k, k / 2, k / 2);
			devParts.insert(devParts.end(), part.begin(), part.end());
			part.clear();
			
		}
	}
	
	return devParts;
}
vector<vector<Node>> smallest_devision(vector<vector<Node>> nodeList, int length, int a, int b) {
	int N = a * b;
	int i = (length / 2) % a;
	int j = (length / 2) % b;
	vector<vector<Node>> devParts;

	if (N > length * length) cout << "*********************not case 1*****************" << endl;
	else {
		if (i == 0 && j == 0) {
			for (int r1 = 0; r1 < nodeList[0].size(); r1 += a) {
				for (int r3 = 0; r3 < nodeList[1].size(); r3 += b) {
					vector<Node> sPart;
					for (int r2 = 0; r2 < a; r2++)  sPart.push_back(nodeList[0][r1 + r2]);
					for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
					devParts.push_back(sPart);
				}
			}
		}
		else if (i != 0 && j == 0) {
			if (i % 2 == 0) {
				for (int r1 = 0; r1 < nodeList[0].size() - i; r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart;
						for (int r2 = 0; r2 < a; r2++)  sPart.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart);

					}
				}
				for (int r1 = nodeList[0].size() - i; r1 < nodeList[0].size(); r1 += i) {
					for (int r3 = nodeList[1].size() - b; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart1;
						for (int r2 = 0; r2 < i; r2++)  sPart1.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart1.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart1);
					}
				}
			}
			else
			{
				for (int r1 = 0; r1 < nodeList[0].size() - i; r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart;
						for (int r2 = 0; r2 < a; r2++)  sPart.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart);

					}
				}
				for (int r1 = nodeList[0].size() - a; r1 < nodeList[0].size(); r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart1;
						for (int r2 = 0; r2 < a; r2++)  sPart1.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart1.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart1);

					}
				}
			}
		}
		else if (i == 0 && j != 0) {
			if (j % 2 == 0) {
				for (int r1 = 0; r1 < nodeList[0].size(); r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size() - j; r3 += b) {
						vector<Node> sPart;
						for (int r2 = 0; r2 < a; r2++)  sPart.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart);
					}

				}
				for (int r1 = nodeList[0].size() - a; r1 < nodeList[0].size(); r1 += a) {
					for (int r3 = nodeList[1].size() - j; r3 < nodeList[1].size(); r3 += j) {
						vector<Node> sPart1;
						for (int r2 = 0; r2 < a; r2++)  sPart1.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < j; r4++)  sPart1.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart1);
					}
				}
			}
			else
			{
				for (int r1 = 0; r1 < nodeList[0].size(); r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size() - j; r3 += b) {
						vector<Node> sPart;
						for (int r2 = 0; r2 < a; r2++)  sPart.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart);
					}

				}
				for (int r1 = 0; r1 < nodeList[0].size(); r1 += a) {
					for (int r3 = nodeList[1].size() - b; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart1;
						for (int r2 = 0; r2 < a; r2++)  sPart1.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart1.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart1);
					}
				}
			}
		}
		else if (i != 0 && j != 0) {
			if (i % 2 == 0 && j % 2 == 0) {
				for (int r1 = 0; r1 < nodeList[0].size() - i; r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size() - j; r3 += b) {
						vector<Node> sPart;
						for (int r2 = 0; r2 < a; r2++) sPart.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart);
					}
				}
				for (int r1 = 0; r1 < nodeList[0].size() - i; r1 += a) {

					for (int r3 = nodeList[1].size() - j; r3 < nodeList[1].size(); r3 += j) {

						vector<Node> sPart1;
						for (int r2 = 0; r2 < a; r2++) sPart1.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < j; r4++)  sPart1.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart1);

					}

				}
				for (int r1 = nodeList[0].size() - i; r1 < nodeList[0].size(); r1 += i) {
					cout << "do3" << endl;
					for (int r3 = 0; r3 < nodeList[1].size() - j; r3 += b) {
						vector<Node> sPart2;
						for (int r2 = 0; r2 < i; r2++) sPart2.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart2.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart2);
					}
				}
			}
			else
			{
				for (int r1 = 0; r1 < nodeList[0].size() - i; r1 += a) {
					for (int r3 = 0; r3 < nodeList[1].size() - j; r3 += b) {
						vector<Node> sPart;
						for (int r2 = 0; r2 < a; r2++) sPart.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart);
					}
				}
				for (int r1 = 0; r1 < nodeList[0].size() - i; r1 += a) {
					for (int r3 = nodeList[1].size() - b; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart1;
						for (int r2 = 0; r2 < a; r2++) sPart1.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart1.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart1);
					}
				}
				for (int r3 = 0; r3 < nodeList[1].size() - j; r3 += b) {
					for (int r1 = nodeList[0].size() - a; r1 < nodeList[0].size(); r1 += a) {
						vector<Node> sPart2;
						for (int r2 = 0; r2 < a; r2++) sPart2.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart2.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart2);
					}
				}
				for (int r1 = nodeList[0].size() - a; r1 < nodeList[0].size(); r1 += a) {
					for (int r3 = nodeList[1].size() - b; r3 < nodeList[1].size(); r3 += b) {
						vector<Node> sPart3;
						for (int r2 = 0; r2 < a; r2++) sPart3.push_back(nodeList[0][r1 + r2]);
						for (int r4 = 0; r4 < b; r4++)  sPart3.push_back(nodeList[1][r3 + r4]);
						devParts.push_back(sPart3);
					}
				}

			}

		}
	}
	return devParts;
}
CMap* build_small_units_link(vector<Node> nodeList) {
	int size = nodeList.size();
	//cout <<"mapnodesize="<< size<<endl;
	CMap* pMap = new CMap(200);

	vector<Node> first_edge;
	vector<Node> second_edge;
	vector<Node> third_edge;

	for (int i = 0; i < size; i++) {
		Node* pNode = new Node(nodeList[i].m_cData, nodeList[i].node_kind);
		//cout << nodeList[i].m_cData; cout << endl; 
		pMap->addNode(pNode);
		//cout << nodeList[i].node_kind<<endl;
		if (nodeList[i].node_kind == '3') third_edge.push_back(nodeList[i]);
		if (nodeList[i].node_kind == '2') first_edge.push_back(nodeList[i]); 
		if (nodeList[i].node_kind == '1') { second_edge.push_back(nodeList[i]);  }
	}
	/*for (int i = 0; i < first_edge.size(); i++)
		cout << "s" << first_edge[i].node_kind << ",";
	cout << endl;
	for (int i = 0; i < second_edge.size(); i++)
		cout << "s" << second_edge[i].node_kind << ",";
	cout << endl;*/
	if (nodeList[0].node_kind == '3') {
		for (int i = 0; i < third_edge.size(); i++) {
			for (int j = 0; j < first_edge.size(); j++) {
				//cout << "third_edge.size():" << first_edge.size()<<endl;
				//pMap->setValueToMatrixForUndirectedGraph(third_edge[i].m_cData - 1, first_edge[j].m_cData - 1);
				pMap->setValueToMatrixForUndirectedGraph(i, third_edge.size() +j);
				//cout << "third:" << third_edge[i].m_cData - 1 << endl;
			}
		}
	}
	else {
		for (int i = 0; i < first_edge.size(); i++) {
			for (int j = 0; j < second_edge.size(); j++) {
				//pMap->setValueToMatrixForUndirectedGraph(first_edge[i].m_cData - 1, second_edge[j].m_cData - 1);
				pMap->setValueToMatrixForUndirectedGraph(i, first_edge.size() + j);
			}
		}
	}

	
	//pMap->printMartrix();
	//pMap->depthFirstTraverse(second_edge[0].m_cData);

	cout << endl;
	return pMap;
}

vector<Node> DepthFirstTraverse(CMap* map, int nodeIndex) {
	vector<Node> vector_sw;
	int index = 0;
	map->updateDegree();
	vector_sw.push_back(map->find(nodeIndex));
	//cout << "起始点为s" << nodeIndex << endl;
	vector<Node> sum_list;
	map->getPath(vector_sw, index);
	return map->noDegree;
}

vector<Node> DepthFirstTraverse1(CMap* map, int nodeIndex, int i) {
	//m.lock();
	//lock_guard<mutex> l(m);
	//simulate_hard_computation();
	map->DFS1(nodeIndex);
	//m.unlock();
	//temp.lock();
	//cout << "topo" << i << " done!" << endl;
	//temp.unlock();
	return map->noDegree;

}

vector<CMap*> find_error_units(vector<CMap*>maps) {
	vector<CMap*>error_maps;
	for (int i = 0; i < maps.size(); i++) {
		if (maps[i]->port_for_errorfinding == true)	error_maps.push_back(maps[i]);
	}
	return error_maps;
}

vector<Node> test(CMap* map) {
	vector<Node> list;
	for (int i = 0; i < map->m_iNodeCount; i++)
		list.push_back(map->m_pNodeArray[i]);
	for (int i = 0; i < list.size(); i++)
		cout << list[i].m_cData << ",";
	cout << endl;
	return list;
}
void simulate_hard_computation() {
	std::this_thread::sleep_for(std::chrono::milliseconds(2000 + rnd()));
}
void multithreads(int thread_num, vector<CMap*>maps_without_error, vector<vector<Node>>smallest_units) {
	int maps_waiting = maps_without_error.size();
	while (maps_waiting != 0) {
		if (maps_waiting >= thread_num) {
			//m3.lock();
			//cout << "*********************************有" << maps_waiting << "个未处理拓扑***" << endl;
			int counter = thread_num;
			vector<thread> threads;
			while (counter != 0) {
				m2.lock();
				for (int i = 0; i < maps_without_error.size(); i++) {
					if (!maps_without_error[i]->been_dfs) {
						//m1.lock();
						//temp.lock();
						//cout << "处理拓扑" << i << endl;
						//temp.unlock();
						maps_without_error[i]->been_dfs = true;
						threads.push_back(thread(DepthFirstTraverse1, ref(maps_without_error[i]), smallest_units[i][0].m_cData, i));
						//m1.unlock();
						break;
					}
					else continue;
				}
				//temp.lock();
				//cout << "counter=" << counter << endl;
				//temp.unlock();
				counter--;
				m2.unlock();
			}
			for (int i = 0; i < threads.size(); i++) {
				threads[i].join();
				maps_waiting--;
			}
			//m3.unlock();
		}
		else {
			//m3.lock();
			//cout << "***有" << maps_waiting << "个未处理拓扑**************************" << endl;
			int counter = maps_waiting;
			vector<thread> threads;
			while (counter != 0) {
				m2.lock();
				for (int i = 0; i < maps_without_error.size(); i++) {
					if (!maps_without_error[i]->been_dfs) {
						//m1.lock();
						//temp.lock();
						//cout << "处理拓扑" << i << endl;
						//temp.unlock();
						maps_without_error[i]->been_dfs = true;
						threads.push_back(thread(DepthFirstTraverse1, ref(maps_without_error[i]), smallest_units[i][0].m_cData, i));
						//m1.unlock();
						break;
					}
					else continue;
				}
				//temp.lock();
				//cout << "counter=" << counter << endl;
				//temp.unlock();
				counter--;
				m2.unlock();
			}
			for (int i = 0; i < threads.size(); i++) {
				threads[i].join();
				maps_waiting--;
			}
			//m3.unlock();
		}
	}

}
int main(void)
{
	// 将数据保存到选定位置的excel文件中
	BasicExcel xls;

	BasicExcelWorksheet* sheet = NULL;
	BasicExcelCell* cell = NULL;

	int RowNum = 1;    // 标题在第一行，数据从第二行开始保存，所以这里value为1而不是0

	// 创建工作表1并获取关联的BasicExcelWorksheet指针
	xls.New(1);
	sheet = xls.GetWorksheet(0);
	XLSFormatManager fmt_mgr(xls);
	// 设置Excel表格中的字体
	ExcelFont font_bold;
	font_bold._weight = FW_BOLD; // 700

	CellFormat fmt_bold(fmt_mgr);
	fmt_bold.set_font(font_bold);

	int col = 0, row = 0;
	//写入第一行，也就是表头
	cell = sheet->Cell(row, col); cell->Set("Number Of Threads");      cell->SetFormat(fmt_bold); ++col;
	cell = sheet->Cell(row, col); cell->Set("RUN TIME");  cell->SetFormat(fmt_bold); col = 0;
	/*
	//构建30x30的单pod拓扑
	CMap* pMap = new CMap(80);
	const int switch_sum = 80;
	Node nodeList[switch_sum];
	//30x30节点添加
	for (int i = 0; i < switch_sum; i++) {
		if (i < 40) {
			Node* pNode = new Node(i + 1, '2');
			nodeList[i] = *pNode;
			pMap->addNode(pNode);
			//cout << "add a node" << i << endl;
			//out << nodeList[i].m_cData << " " << nodeList[i].node_kind << endl;

		}
		else {
			Node* pNode = new Node(i + 1, '1');
			nodeList[i] = *pNode;
			pMap->addNode(pNode);
			//cout << "add a node" << i << endl;
			//cout << nodeList[i].m_cData << " " << nodeList[i].node_kind << endl;;

		}

	}

	//30x30全连接添加
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			pMap->setValueToMatrixForUndirectedGraph(i, 40 + j);
		}
	}
	//pMap->printMartrix();[当k=80时，打印出来的确实是40*40的全连接拓扑]
	//cout << "size of nodelist is:" << sizeof(nodeList) / sizeof(nodeList[0]) << endl;
	*/


	//构建k元胖树拓扑
	const int k = 32;//*************k为胖树元数**************
	const int Lmax = 40;//**************Lmax为最大路径长度*****************
	CMap* pMap = new CMap(k * k * 5 / 4);
	const int switch_sum = k * k * 5 / 4;
	Node nodeList[switch_sum];
	//节点添加
	for (int i = 0; i < switch_sum; i++) {
		if (i < k * k / 4) {
			Node* pNode = new Node(i + 1, '3');
			nodeList[i] = *pNode;
			pMap->addNode(pNode);
			//cout << "add a node" << i << endl;
			//cout << nodeList[i].m_cData << " " << nodeList[i].node_kind << endl;

		}
		else if (i < k * k * 3 / 4) {
			Node* pNode = new Node(i + 1, '2');
			nodeList[i] = *pNode;
			pMap->addNode(pNode);
			//cout << "add a node" << i << endl;
			//cout << nodeList[i].m_cData << " " << nodeList[i].node_kind << endl;;

		}
		else {
			Node* pNode = new Node(i + 1, '1');
			nodeList[i] = *pNode;
			pMap->addNode(pNode);
			//cout << "add a node" << i << endl;
			//cout << nodeList[i].m_cData << " " << nodeList[i].node_kind << endl;;
		}

	}




	//第一次分区（得到上下两层网络）
	int num_of_layer2 = layer2area(nodeList, sizeof(nodeList) / sizeof(nodeList[0]), k);
	/*print(devide1_1);//可以得到上层网络和下层网络的节点
	cout << endl;
	print(devide1_2);*/
	//第二次分区（得到上下两层网络的二层连通子区域）
	/*vector<vector<Node>> sets = foundSets(nodeList, sizeof(nodeList) / sizeof(nodeList[0]), k);//上层网络*/

	vector<vector<Node>> sets = foundSets(devide1_1, sizeof(nodeList) / sizeof(nodeList[0]), k);//上层网络
	//print(sets);//打印上层网络的二层连通子区域
	vector<vector<Node>> pods = foundPods1(devide1_2, sizeof(nodeList) / sizeof(nodeList[0]), k);//下层网络
	//print(pods);//打印下层网络的二层连通子区域
	//vector<vector<Node>> pods = foundPods1(nodeList, sizeof(nodeList) / sizeof(nodeList[0]));//下层网络


	//第三次分区
	vector<vector<Node>> smallest_units = smallest_devision1(sets, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k, 0, 0);
	//print(smallest_units); cout << smallest_units.size() << endl;
	vector<vector<Node>> smallest_units2 = smallest_devision2(pods, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k);
	//print(smallest_units2); cout << smallest_units2.size() << endl;
	//vector<vector<Node>> smallest_units = smallest_devision(pods, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k);

	smallest_units.insert(smallest_units.end(), smallest_units2.begin(), smallest_units2.end());//将上下两层网络划分的区域合在一起
	print(smallest_units); 
	cout << smallest_units.size() << endl;



	//DFS路径规划(单线程）
	std::vector<CMap*> maps1;  // 存储所有 smallmap 指针

	for (int i = 0; i < smallest_units.size(); i++) {
		CMap* smallmap = build_small_units_link(smallest_units[i]);
		smallmap->updateDegree();
		maps1.push_back(smallmap);  // 存储指针
	}

//cout << smallest_units.size() << endl;
	auto beg_t = chrono::system_clock::now(); //开始时间

	for (CMap* map : maps1) {
		vector<Node> res = map->DFS1(0);
	}

	auto end_t = chrono::system_clock::now();
	chrono::duration<double> diff = end_t - beg_t;

	printf("performTest total time: ");
	cout << diff.count() << endl;







	/*

	const int map_num = smallest_units.size();

	 //不分组多线程
     //多线程资源池

	vector<CMap*> maps;
	for (int i = 0; i < map_num; i++) {
		CMap* smallmap = build_small_units_link(smallest_units[i]);
		maps.push_back(smallmap);
	}
	auto beg_t111 = std::chrono::system_clock::now();
	ThreadPool pool(10);
	std::vector<std::future<task_return_type>> results;
	std::chrono::duration<double> total_diff(0);
	std::mutex output_mutex;
	
	//auto beg_t111 = std::chrono::system_clock::now();
	// 修改后的任务提交循环
	for (int j = 0; j < maps.size(); ++j) {
		results.emplace_back(
			pool.enqueue([map_idx = j, &maps, &output_mutex] {  // 使用初始化捕获保证索引正确
				try {
					auto beg_t = std::chrono::system_clock::now();

					// 操作特定map对象（确保每个map只被一个线程访问）
					CMap* current_map = maps[map_idx];
					current_map->noDegree.clear();
					//current_map->resetMap();
					current_map->updateDegree();
					current_map->been_dfs = false;
					current_map->DFS1(0);
					//current_map->printMartrix();

					auto end_t = std::chrono::system_clock::now();
					auto diff = end_t - beg_t;


					// 获取当前线程ID
					auto thread_id = std::this_thread::get_id();

					// 更新线程统计信息（加锁保护）
					{
						std::lock_guard<std::mutex> lock(stats_mutex);
						auto& stats = thread_stats[thread_id];
						stats.task_count++;
						stats.total_time += diff;
						if (diff < stats.min_time) stats.min_time = diff;
						if (diff > stats.max_time) stats.max_time = diff;
					}

					// 输出任务日志（可选）
					//{
					//	std::lock_guard<std::mutex> lock(output_mutex);
					//	std::cout << "Task " << map_idx
					//		<< " executed by thread " << thread_id
					//		<< " in " << std::chrono::duration<double>(diff).count()
					//		<< " seconds\n";
					//}




					// 输出时加锁
					{
						//std::lock_guard<std::mutex> lock(output_mutex);
						//std::cout << "Task " << map_idx
						//	<< " completed in " << std::chrono::duration<double>(diff).count()
						//	<< " seconds\n";
					}

					return diff;
				}
				catch (const std::exception& e) {
					std::lock_guard<std::mutex> lock(output_mutex);
					std::cerr << "Error in task " << map_idx
						<< ": " << e.what() << "\n";
					return task_return_type::zero();
				}
				})
		);
	}

	auto end_t111 = std::chrono::system_clock::now();
	cout << "111:" << std::chrono::duration<double>(end_t111 - beg_t111).count() << endl;
	
	

	// 等待所有任务完成并处理异常
	for (auto& future : results) {
		try {
			total_diff += future.get();  // 阻塞直到任务完成
		}
		catch (const std::exception& e) {
			std::lock_guard<std::mutex> lock(output_mutex);
			std::cerr << "Unhandled exception: " << e.what() << "\n";
		}
	}

	std::cout << "Total execution time: " << total_diff.count() << " seconds\n";

	// 打印线程统计
	std::lock_guard<std::mutex> lock(stats_mutex);
	for (const auto& entry : thread_stats) {  // entry 是键值对
		const std::thread::id& thread_id = entry.first;  // 直接获取线程ID
		const ThreadStats& stats = entry.second;

		std::cout << "\nThread " << thread_id << ":\n"
			<< "  Tasks executed: " << stats.task_count << "\n"
			<< "  Total time: " << stats.total_time.count() << " s\n"
			<< "  Average time: " << stats.total_time.count() / stats.task_count << " s\n"
			<< "  Min time: " << stats.min_time.count() << " s\n"
			<< "  Max time: " << stats.max_time.count() << " s\n";
	}


	*/





	




	














	
	return 0;
}
