//pMap->printMartrix();
/*
pMap->printMartrix();
pMap->setValueToMatrixForUndirectedGraph(0, k*k/4,0);
vector<CMap*> maps;
maps.push_back(pMap);
//maps[0]->printMartrix();
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // ��ʼ����ʱ�����
	for (int j = 0; j < maps.size(); j++) {
		//maps[j]->noDegree.clear();
		//maps[j]->resetMap();
		//maps[j]->updateDegree();
		//maps[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps[j]->depthFirstTraverse1(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // �ۼ�ʱ��
	}
	cout << total_diff.count() << endl;
}
*/
std::chrono::duration<double> planning_time(0);
auto plan1 = std::chrono::high_resolution_clock::now();

//��һ�η������õ������������磩
int num_of_layer2 = layer2area(nodeList, sizeof(nodeList) / sizeof(nodeList[0]), k);
//print(devide1_1);cout << endl;print(devide1_2);//��ӡ������������

//�ڶ��η������õ�������������Ķ�����ͨ������
vector<vector<Node>> sets = foundSets(devide1_1, sizeof(nodeList) / sizeof(nodeList[0]), k);//�ϲ�����
//print(sets);//��ӡ�ϲ�����Ķ�����ͨ������
vector<vector<Node>> pods = foundPods1(devide1_2, sizeof(nodeList) / sizeof(nodeList[0]), k);//�²�����
//print(pods);//��ӡ�²�����Ķ�����ͨ������

//�����η���
vector<vector<Node>> smallest_units = smallest_devision1(sets, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k, 0, 0);
//print(smallest_units); cout << smallest_units.size() << endl;//��ӡ�ϲ����绮�ֵ���С��Ԫ
vector<vector<Node>> smallest_units2 = smallest_devision2(pods, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k);
//print(smallest_units2); cout << smallest_units2.size() << endl;//��ӡ�²����绮�ֵ���С��Ԫ
smallest_units.insert(smallest_units.end(), smallest_units2.begin(), smallest_units2.end());//�������������绮�ֵ���С�������һ��
//print(smallest_units); 
//cout << smallest_units.size() << endl;

const int map_num = smallest_units.size();
//std::vector<std::shared_ptr<Map>> maps;
vector<CMap*> maps;//************************************************************
for (int i = 0; i < map_num; i++) {
	CMap* smallmap = build_small_units_link(smallest_units[i]);
	maps.push_back(smallmap);
}
auto plan2 = std::chrono::high_resolution_clock::now();
planning_time = plan2 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;
/*
//����·���ع滮
for (int i = 0; i < maps[80]->m_iNodeCount; i++)
	cout << maps[80]->m_pNodeArray[i].m_cData << ",";
cout << endl;
//���ù��ϵ�Ԫ����
maps[80]->port_for_errorfinding = true;
maps[80]->setValueToMatrixForUndirectedGraph(0, 10, 0);


//����Ѱ�ҹ��ϵ�Ԫ��ʱ��
auto beg_t2 = chrono::system_clock::now();
vector<CMap*> error_maps = find_error_units(maps);


//������ϵ�Ԫ·���滮��ʱ��
//cout << error_maps.size() << endl;
auto beg_t1 = chrono::system_clock::now();
for (int i = 0; i < error_maps.size(); i++) {
	cout << "Processing error map " << i << endl;
	error_maps[i]->depthFirstTraverse(0); // ������0��ʼDFS
}
auto end_t1 = chrono::system_clock::now();
chrono::duration<double> diff1 = end_t1 - beg_t1;
cout << "diff1:" << diff1.count() << endl;
*/

/*
//ɾ����������
vector<CMap*> maps_without_error;
vector<vector<Node>> units_without_error;
for (int i = 0; i < maps.size(); i++) {
	CMap* tp_maps;
	vector<Node>tp_units;
	if (maps[i]->port_for_errorfinding == false) {
		maps_without_error.push_back(maps[i]);
		units_without_error.push_back(smallest_units[i]);
		//tp_maps = maps[i];
		//tp_units = smallest_units[i];
	}
	//maps_without_error.push_back(tp_maps);
	//units_without_error.push_back(tp_units);
	//tp_maps.clear();
	//tp_units.clear();
}
cout << maps_without_error.size() << endl;
*/
/*
//ʣ�����˹��Ϲ滮��ʱ��
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // ��ʼ����ʱ�����
	for (int j = 0; j < maps_without_error.size(); j++) {
		maps_without_error[j]->noDegree.clear();
		//maps_without_error[j]->resetMap();
		//maps_without_error[j]->updateDegree();
		maps_without_error[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps_without_error[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // �ۼ�ʱ��
	}
	cout << total_diff.count() << endl;
}*/

/*
auto end_t2 = chrono::system_clock::now();
chrono::duration<double> diff2 = end_t2 - beg_t2;
cout << "diff2:" << diff2.count() << endl;
*/


//���߳�
/*
for (int pp = 0; pp < 6; pp++) {
	chrono::duration<double> total_diff(0); // ��ʼ����ʱ�����
	for (int j = 0; j < maps.size(); j++) {
		maps[j]->noDegree.clear();
		maps[j]->resetMap();
		maps[j]->updateDegree();
		maps[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // �ۼ�ʱ��
	}
	cout << total_diff.count() << endl;
}
*/
//���߳��޸�
/*
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // ��ʼ����ʱ�����
	for (int j = 0; j < maps.size(); j++) {
		auto beg_t = chrono::system_clock::now();
		maps[j]->noDegree.clear();
		maps[j]->resetMap();
		maps[j]->updateDegree();
		maps[j]->been_dfs = false;
		maps[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		//cout << diff.count() << endl;
		total_diff += diff; // �ۼ�ʱ��
	}
	//cout << total_diff.count() << endl;
}

auto plan2 = std::chrono::high_resolution_clock::now();
planning_time = plan2 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;
*/

/*
//�Ե��߳̽����˷��飬ÿ��50������ÿ���ʱ10^��-7���룬���������ʱҲΪ10^(-7)�룬�������ַ�ʽ�ܺ�ʱ��һ����
for (int pp = 0; pp < 6; pp++) {
	std::chrono::duration<double> diff(0);
	chrono::duration<double> total_diff(0); // ��ʼ����ʱ�����
	for (int j = 0; j < maps.size(); j += 50) {
		for (int k = 0; k < 50 && (j + k) < maps.size(); ++k) {
			auto beg_t = std::chrono::system_clock::now();
			maps[j + k]->noDegree.clear();
			maps[j + k]->resetMap();
			maps[j + k]->updateDegree();
			maps[j + k]->been_dfs = false;
			//auto beg_t = std::chrono::system_clock::now();
			maps[j + k]->depthFirstTraverse(0);
			auto end_t = std::chrono::system_clock::now();
			diff = (end_t - beg_t);
			total_diff += diff;
		}
		std::cout << "Task " << j << " completed in " << diff.count() << " seconds" << std::endl;
	}
	cout << total_diff.count() << endl;
}
*/



//�Զ��߳̽����˷��飬����ÿ��������Խ�࣬��ʱԽ��
std::chrono::duration<double> diff(0);
auto beg_t = std::chrono::system_clock::now();
//���߳���Դ��
ThreadPool pool(12);
auto end_t = std::chrono::system_clock::now();
diff += end_t - beg_t;
std::cout << "diff: " << diff.count() << " seconds" << std::endl;

// ����һ���洢������������
std::vector<std::future<std::chrono::duration<double>>> results;

// ����һ���洢ÿ���߳���ִ��ʱ��Ĺ�ϣ��
std::unordered_map<std::thread::id, ThreadContext> threadContexts;

// ����һ�����������������
std::mutex output_mutex;
std::chrono::duration<double> total_diff(0);
/*
for (int i = 0; i < 12; ++i) {
	pool.enqueue([] {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		});
}*/
// ����һ���������洢�ܵĴ����ִ��ʱ��
std::chrono::duration<double> total_task_time(0);

//auto end_t = std::chrono::system_clock::now();
//diff += end_t - beg_t;
//std::cout << "diff: " << diff.count() << " seconds" << std::endl;
for (int j = 0; j < maps.size(); j += 60) {
	//auto beg_t222 = std::chrono::high_resolution_clock::now();
	results.emplace_back(
		pool.enqueue([j, &maps, &output_mutex, &threadContexts, &total_task_time] {
			std::chrono::duration<double> diff(0);
			//auto thread_id = std::this_thread::get_id();

			// ��¼�߳̿�ʼʱ��
			//{
			//	std::lock_guard<std::mutex> lock(threadContexts[thread_id].mutex);
			//	threadContexts[thread_id].start_time = std::chrono::high_resolution_clock::now();
			//}

			// ����һ������
			for (int k = 0; k < 60 && (j + k) < maps.size(); ++k) {

				maps[j + k]->noDegree.clear();
				maps[j + k]->resetMap();
				maps[j + k]->updateDegree();
				maps[j + k]->been_dfs = false;
				maps[j + k]->depthFirstTraverse(0);


			}

			//{
			//	std::lock_guard<std::mutex> lock(threadContexts[thread_id].mutex);
			//	threadContexts[thread_id].end_time = std::chrono::high_resolution_clock::now();
			//	threadContexts[thread_id].executionTime += diff;
			//}
			return diff;
			})
	);
	//auto end_t222 = std::chrono::high_resolution_clock::now();
	//total_diff = end_t222 - beg_t222;
	//std::cout << "Total: " << total_diff.count() << " seconds" << std::endl;
}
auto beg_t111 = std::chrono::high_resolution_clock::now();
//��ȡ���������ִ��ʱ�䲢�ۼ�
for (auto&& result : results) {
	result.get(); // ��Ҫ�ȴ������������
}
auto plan3 = std::chrono::high_resolution_clock::now();
planning_time = plan3 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;

//��ȡ���������ִ��ʱ�䲢�ۼ�
//for (auto&& result : results) {
//	total_diff += result.get();
//}
auto end_t111 = std::chrono::high_resolution_clock::now();
total_diff = end_t111 - beg_t111;
std::cout << "Total execution time: " << total_diff.count() << " seconds" << std::endl;
// ���ÿ���̵߳���ִ��ʱ��
//{
//	std::lock_guard<std::mutex> lock(output_mutex);
//	for (const auto& pair : threadContexts) {
//		std::cout << "Thread " << pair.first << " total execution time: " << pair.second.executionTime.count() << " seconds" << std::endl;
//	}
//}
{
	std::lock_guard<std::mutex> lock(output_mutex);
	for (const auto& pair : threadContexts) {
		auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(pair.second.end_time - pair.second.start_time);
		std::cout << "Thread " << pair.first << " total execution time: " << duration.count() << " seconds (Actual task time: " << pair.second.executionTime.count() << " seconds)" << std::endl;
	}
}

//std::cout << "Total execution time: " << total_diff.count() << " seconds" << std::endl;

std::cout << "Total task execution time: " << total_task_time.count() << " seconds" << std::endl;


/*
ThreadPool pool(12);

// ����һ���洢������������
std::vector<std::future<std::chrono::duration<double>>> results;

// ����һ���洢ÿ���߳���ִ��ʱ��Ĺ�ϣ��
std::unordered_map<std::thread::id, ThreadContext> threadContexts;

// ����һ�����������������
std::mutex output_mutex;

// Ԥ�����̳߳�
for (int i = 0; i < 12; ++i) {
	pool.enqueue([] {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	});
}

// ��¼�������߳�����Ŀ�ʼʱ��
//auto total_start = std::chrono::system_clock::now();

// Ϊÿ����������߳�
for (int j = 0; j < maps.size(); j += 8) {
	results.emplace_back(
		pool.enqueue([j, &maps, &output_mutex, &threadContexts] {
			std::chrono::duration<double> diff(0);

			// ����һ������
			for (int k = 0; k < 8 && (j + k) < maps.size(); ++k) {
				auto beg_t = std::chrono::system_clock::now();
				maps[j + k]->noDegree.clear();
				maps[j + k]->resetMap();
				maps[j + k]->updateDegree();
				maps[j + k]->been_dfs = false;
				maps[j + k]->depthFirstTraverse(0);
				auto end_t = std::chrono::system_clock::now();
				diff += end_t - beg_t;
			}

			// ��ȡ�߳�ID���ۼ�����ִ��ʱ��
			std::thread::id thread_id = std::this_thread::get_id();
			{
				std::lock_guard<std::mutex> lock(threadContexts[thread_id].mutex);
				threadContexts[thread_id].executionTime += diff;
			}

			// ʹ��������ʱ�����
			//{
			 //   std::lock_guard<std::mutex> lock(output_mutex);
			 //   std::cout << "Task " << j << " completed in " << diff.count() << " seconds by thread " << thread_id << std::endl;
		   // }

			return diff;
		})
	);
}
auto total_start = std::chrono::system_clock::now();

// ��ȡ���������ִ��ʱ�䲢�ۼ�
std::chrono::duration<double> total_task_time(0);
for (auto&& result : results) {
	total_task_time += result.get();
}

// ��¼�������߳�����Ľ���ʱ��
auto total_end = std::chrono::system_clock::now();
std::chrono::duration<double> total_duration = total_end - total_start;

// ���ÿ���̵߳���ִ��ʱ��
{
	std::lock_guard<std::mutex> lock(output_mutex);
	for (const auto& pair : threadContexts) {
		std::cout << "Thread " << pair.first << " total execution time: " << pair.second.executionTime.count() << " seconds" << std::endl;
	}
}



// ����ܵ�����ִ��ʱ�䣨��������ִ��ʱ����ܺͣ�
// std::cout << "Total task execution time (sum of all tasks): " << total_task_time.count() << " seconds" << std::endl;

 // ������̲߳���ִ��ʱ�䣨������ʼ��������ʱ�䣩
 std::cout << "Total parallel execution time: " << total_duration.count() << " seconds" << std::endl;
 */





 /*
 //��������߳�
 //���߳���Դ��
 ThreadPool pool(10);

 // ����һ���洢������������
 std::vector<std::future<std::chrono::duration<double>>> results;

 // ����һ�����������������
 std::mutex output_mutex;
 std::chrono::duration<double> total_diff(0);

 for (int i = 0; i < 10; ++i) {
	 pool.enqueue([] {
		 std::this_thread::sleep_for(std::chrono::milliseconds(1));
		 });
 }

 for (int j = 0; j < maps.size(); ++j) {
	 results.emplace_back(
		 pool.enqueue([j, &maps, &output_mutex] {
			 std::chrono::duration<double> diff(0);

			 // ʹ������������������̵����
			 {
				 //std::lock_guard<std::mutex> lock(output_mutex);
				 auto beg_t = std::chrono::system_clock::now();
				 maps[j]->noDegree.clear();
				 maps[j]->resetMap();
				 maps[j]->updateDegree();
				 maps[j]->been_dfs = false;
				 maps[j]->depthFirstTraverse(0);
				 auto end_t = std::chrono::system_clock::now();
				 diff = end_t - beg_t;
			 }



			 // ʹ��������ʱ�����
			 {
				 std::lock_guard<std::mutex> lock(output_mutex);
				 std::cout << "Task " << j << " completed in " << diff.count() << " seconds" << std::endl;
			 }

			 return diff;
			 })
	 );
 }


 // ��ȡ���������ִ��ʱ�䲢�ۼ�
 for (auto&& result : results) {
	 total_diff += result.get();
 }

 std::cout << "Total execution time: " << total_diff.count() << " seconds" << std::endl;
 */





//=============================================
//=============================================
//=============================================







	//����·���ع滮(δ��������)

	//��n����Ԫ�Ľڵ���
	//for (int i = 0; i < pMap[]->m_iNodeCount; i++)
	//	cout << maps[k * k * k / 100]->m_pNodeArray[i].m_cData << ",";
	//cout << endl;

	//���ù��ϵ�Ԫ����
	//��n����Ԫ��ĳ����·��������

vector<Node> nodeVector;

for (int i = 0; i < switch_sum; i++) {
	nodeVector.push_back(pMap->m_pNodeArray[i]);
}
CMap* smallmap1 = build_small_units_link(nodeVector);


auto replan_t1 = chrono::system_clock::now();

smallmap1->DFS1(0);
//smallmap1->print_DFS_res();
//CMap* smallmap = build_small_units_link(pMap);
//smallmap1->printMartrix();

auto replan_t2 = chrono::system_clock::now();
chrono::duration<double> diff_replan = replan_t2 - replan_t1;
cout << "replanning dfs time:" << diff_replan.count() << endl;











	//spine-leaf����
	/*const int num = 232;//�ܵĽ���������
	Node nodeList1[num];
	for (int i = 0; i < num; i++) {
		Node* pNode = new Node(i + 1, '3');
		nodeList1[i] = *pNode;
	}

	vector<vector<Node>> part = devision(nodeList1, 115, 117, 120, 0, 0, num,115,117);
	print(part); cout << "*****************************" << endl; cout << part.size() << endl;
	*/


std::chrono::duration<double> planning_time(0);
auto plan1 = std::chrono::high_resolution_clock::now();


//��һ�η������õ������������磩
int num_of_layer2 = layer2area(nodeList, sizeof(nodeList) / sizeof(nodeList[0]), k);
/*print(devide1_1);//���Եõ��ϲ�������²�����Ľڵ�
cout << endl;
print(devide1_2);*/
//�ڶ��η������õ�������������Ķ�����ͨ������
/*vector<vector<Node>> sets = foundSets(nodeList, sizeof(nodeList) / sizeof(nodeList[0]), k);//�ϲ�����*/

vector<vector<Node>> sets = foundSets(devide1_1, sizeof(nodeList) / sizeof(nodeList[0]), k);//�ϲ�����
//print(sets);//��ӡ�ϲ�����Ķ�����ͨ������
vector<vector<Node>> pods = foundPods1(devide1_2, sizeof(nodeList) / sizeof(nodeList[0]), k);//�²�����
//print(pods);//��ӡ�²�����Ķ�����ͨ������
//vector<vector<Node>> pods = foundPods1(nodeList, sizeof(nodeList) / sizeof(nodeList[0]));//�²�����


//�����η���
vector<vector<Node>> smallest_units = smallest_devision1(sets, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k, 0, 0);
//print(smallest_units); cout << smallest_units.size() << endl;
vector<vector<Node>> smallest_units2 = smallest_devision2(pods, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k);
//print(smallest_units2); cout << smallest_units2.size() << endl;
//vector<vector<Node>> smallest_units = smallest_devision(pods, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k);

smallest_units.insert(smallest_units.end(), smallest_units2.begin(), smallest_units2.end());//�������������绮�ֵ��������һ��
//print(smallest_units); cout << smallest_units.size() << endl;

auto plan2 = std::chrono::high_resolution_clock::now();
planning_time = plan2 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;


//-------------------------------------------------------
// ------------------------------------------------------

//DFS·���滮(���̣߳�

//cout << smallest_units.size() << endl;
auto beg_t = chrono::system_clock::now(); //��ʼʱ��

for (int i = 0; i < smallest_units.size(); i++) {
	CMap* smallmap = build_small_units_link(smallest_units[i]);
	smallmap->updateDegree();
	//vector<Node> res = smallmap->DFS1(smallmap->m_pNodeArray[0].m_cData);
	vector<Node> res = smallmap->DFS1(0);
	//cout << "mcdata:" << smallmap->m_pNodeArray[0].m_cData << endl;//�����һ���ڵ�ֵ
	//smallmap->printMartrix();
	//smallmap->print_DFS_res(); //��ӡ·��
}

auto end_t = chrono::system_clock::now();
chrono::duration<double> diff = end_t - beg_t;

printf("performTest total time: ");
cout << diff.count() << endl;

const int map_num = smallest_units.size();

vector<CMap*> maps;
for (int i = 0; i < map_num; i++) {
	CMap* smallmap = build_small_units_link(smallest_units[i]);
	maps.push_back(smallmap);
}

cout << "����" << maps.size() << "����С��Ԫ��Ҫ�滮·��" << endl;




//-------------------------------------------------------
// ------------------------------------------------------


//����·���ع滮

//��n����Ԫ�Ľڵ���
for (int i = 0; i < maps[k * k * k / 100]->m_iNodeCount; i++)
	cout << maps[k * k * k / 100]->m_pNodeArray[i].m_cData << ",";
cout << endl;

//���ù��ϵ�Ԫ����
//��n����Ԫ��ĳ����·��������
maps[k * k * k / 100]->port_for_errorfinding = true;
maps[k * k * k / 100]->setValueToMatrixForUndirectedGraph(0, 8, 0);


//����Ѱ�ҹ��ϵ�Ԫ��ʱ��
auto beg_t2 = chrono::system_clock::now();
vector<CMap*> error_maps = find_error_units(maps);


//������ϵ�Ԫ·���滮��ʱ��
//cout << error_maps.size() << endl;
auto beg_t1 = chrono::system_clock::now();
for (int i = 0; i < error_maps.size(); i++) {
	//cout << "Processing error map " << i << endl;
	error_maps[i]->DFS1(0);
	//error_maps[i]->depthFirstTraverse(0); // ������0��ʼDFS
}
auto end_t1 = chrono::system_clock::now();
chrono::duration<double> diff1 = end_t1 - beg_t1;
cout << "replanning dfs time:" << diff1.count() << endl;


/*
//ɾ����������
vector<CMap*> maps_without_error;
vector<vector<Node>> units_without_error;
for (int i = 0; i < maps.size(); i++) {
	CMap* tp_maps;
	vector<Node>tp_units;
	if (maps[i]->port_for_errorfinding == false) {
		maps_without_error.push_back(maps[i]);
		units_without_error.push_back(smallest_units[i]);
		//tp_maps = maps[i];
		//tp_units = smallest_units[i];
	}
	//maps_without_error.push_back(tp_maps);
	//units_without_error.push_back(tp_units);
	//tp_maps.clear();
	//tp_units.clear();
}
cout << maps_without_error.size() << endl;
*/
/*
//ʣ�����˹��Ϲ滮��ʱ��
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // ��ʼ����ʱ�����
	for (int j = 0; j < maps_without_error.size(); j++) {
		maps_without_error[j]->noDegree.clear();
		//maps_without_error[j]->resetMap();
		//maps_without_error[j]->updateDegree();
		maps_without_error[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps_without_error[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // �ۼ�ʱ��
	}
	cout << total_diff.count() << endl;
}*/

auto end_t2 = chrono::system_clock::now();
chrono::duration<double> diff2 = end_t2 - beg_t2;
cout << "replanning time:" << diff2.count() << endl;

