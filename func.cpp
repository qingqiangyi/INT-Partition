//pMap->printMartrix();
/*
pMap->printMartrix();
pMap->setValueToMatrixForUndirectedGraph(0, k*k/4,0);
vector<CMap*> maps;
maps.push_back(pMap);
//maps[0]->printMartrix();
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // 初始化总时间变量
	for (int j = 0; j < maps.size(); j++) {
		//maps[j]->noDegree.clear();
		//maps[j]->resetMap();
		//maps[j]->updateDegree();
		//maps[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps[j]->depthFirstTraverse1(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // 累计时间
	}
	cout << total_diff.count() << endl;
}
*/
std::chrono::duration<double> planning_time(0);
auto plan1 = std::chrono::high_resolution_clock::now();

//第一次分区（得到上下两层网络）
int num_of_layer2 = layer2area(nodeList, sizeof(nodeList) / sizeof(nodeList[0]), k);
//print(devide1_1);cout << endl;print(devide1_2);//打印上下两层网络

//第二次分区（得到上下两层网络的二层连通子区域）
vector<vector<Node>> sets = foundSets(devide1_1, sizeof(nodeList) / sizeof(nodeList[0]), k);//上层网络
//print(sets);//打印上层网络的二层连通子区域
vector<vector<Node>> pods = foundPods1(devide1_2, sizeof(nodeList) / sizeof(nodeList[0]), k);//下层网络
//print(pods);//打印下层网络的二层连通子区域

//第三次分区
vector<vector<Node>> smallest_units = smallest_devision1(sets, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k, 0, 0);
//print(smallest_units); cout << smallest_units.size() << endl;//打印上层网络划分的最小单元
vector<vector<Node>> smallest_units2 = smallest_devision2(pods, sizeof(nodeList) / sizeof(nodeList[0]), Lmax, k);
//print(smallest_units2); cout << smallest_units2.size() << endl;//打印下层网络划分的最小单元
smallest_units.insert(smallest_units.end(), smallest_units2.begin(), smallest_units2.end());//将上下两层网络划分的最小区域合在一起
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
//故障路径重规划
for (int i = 0; i < maps[80]->m_iNodeCount; i++)
	cout << maps[80]->m_pNodeArray[i].m_cData << ",";
cout << endl;
//设置故障单元数量
maps[80]->port_for_errorfinding = true;
maps[80]->setValueToMatrixForUndirectedGraph(0, 10, 0);


//计算寻找故障单元的时间
auto beg_t2 = chrono::system_clock::now();
vector<CMap*> error_maps = find_error_units(maps);


//计算故障单元路径规划的时间
//cout << error_maps.size() << endl;
auto beg_t1 = chrono::system_clock::now();
for (int i = 0; i < error_maps.size(); i++) {
	cout << "Processing error map " << i << endl;
	error_maps[i]->depthFirstTraverse(0); // 从索引0开始DFS
}
auto end_t1 = chrono::system_clock::now();
chrono::duration<double> diff1 = end_t1 - beg_t1;
cout << "diff1:" << diff1.count() << endl;
*/

/*
//删除故障拓扑
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
//剩余拓扑故障规划的时间
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // 初始化总时间变量
	for (int j = 0; j < maps_without_error.size(); j++) {
		maps_without_error[j]->noDegree.clear();
		//maps_without_error[j]->resetMap();
		//maps_without_error[j]->updateDegree();
		maps_without_error[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps_without_error[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // 累计时间
	}
	cout << total_diff.count() << endl;
}*/

/*
auto end_t2 = chrono::system_clock::now();
chrono::duration<double> diff2 = end_t2 - beg_t2;
cout << "diff2:" << diff2.count() << endl;
*/


//单线程
/*
for (int pp = 0; pp < 6; pp++) {
	chrono::duration<double> total_diff(0); // 初始化总时间变量
	for (int j = 0; j < maps.size(); j++) {
		maps[j]->noDegree.clear();
		maps[j]->resetMap();
		maps[j]->updateDegree();
		maps[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // 累计时间
	}
	cout << total_diff.count() << endl;
}
*/
//单线程修改
/*
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // 初始化总时间变量
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
		total_diff += diff; // 累计时间
	}
	//cout << total_diff.count() << endl;
}

auto plan2 = std::chrono::high_resolution_clock::now();
planning_time = plan2 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;
*/

/*
//对单线程进行了分组，每组50个任务，每组耗时10^（-7）秒，单个任务耗时也为10^(-7)秒，但是两种方式总耗时是一样的
for (int pp = 0; pp < 6; pp++) {
	std::chrono::duration<double> diff(0);
	chrono::duration<double> total_diff(0); // 初始化总时间变量
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



//对多线程进行了分组，并且每组任务数越多，耗时越短
std::chrono::duration<double> diff(0);
auto beg_t = std::chrono::system_clock::now();
//多线程资源池
ThreadPool pool(12);
auto end_t = std::chrono::system_clock::now();
diff += end_t - beg_t;
std::cout << "diff: " << diff.count() << " seconds" << std::endl;

// 创建一个存储任务结果的向量
std::vector<std::future<std::chrono::duration<double>>> results;

// 创建一个存储每个线程总执行时间的哈希表
std::unordered_map<std::thread::id, ThreadContext> threadContexts;

// 创建一个互斥锁来保护输出
std::mutex output_mutex;
std::chrono::duration<double> total_diff(0);
/*
for (int i = 0; i < 12; ++i) {
	pool.enqueue([] {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		});
}*/
// 创建一个变量来存储总的代码段执行时间
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

			// 记录线程开始时间
			//{
			//	std::lock_guard<std::mutex> lock(threadContexts[thread_id].mutex);
			//	threadContexts[thread_id].start_time = std::chrono::high_resolution_clock::now();
			//}

			// 处理一组任务
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
//获取所有任务的执行时间并累加
for (auto&& result : results) {
	result.get(); // 需要等待所有任务完成
}
auto plan3 = std::chrono::high_resolution_clock::now();
planning_time = plan3 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;

//获取所有任务的执行时间并累加
//for (auto&& result : results) {
//	total_diff += result.get();
//}
auto end_t111 = std::chrono::high_resolution_clock::now();
total_diff = end_t111 - beg_t111;
std::cout << "Total execution time: " << total_diff.count() << " seconds" << std::endl;
// 输出每个线程的总执行时间
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

// 创建一个存储任务结果的向量
std::vector<std::future<std::chrono::duration<double>>> results;

// 创建一个存储每个线程总执行时间的哈希表
std::unordered_map<std::thread::id, ThreadContext> threadContexts;

// 创建一个互斥锁来保护输出
std::mutex output_mutex;

// 预热线线程池
for (int i = 0; i < 12; ++i) {
	pool.enqueue([] {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	});
}

// 记录整个多线程任务的开始时间
//auto total_start = std::chrono::system_clock::now();

// 为每组任务分配线程
for (int j = 0; j < maps.size(); j += 8) {
	results.emplace_back(
		pool.enqueue([j, &maps, &output_mutex, &threadContexts] {
			std::chrono::duration<double> diff(0);

			// 处理一组任务
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

			// 获取线程ID并累加任务执行时间
			std::thread::id thread_id = std::this_thread::get_id();
			{
				std::lock_guard<std::mutex> lock(threadContexts[thread_id].mutex);
				threadContexts[thread_id].executionTime += diff;
			}

			// 使用锁保护时间输出
			//{
			 //   std::lock_guard<std::mutex> lock(output_mutex);
			 //   std::cout << "Task " << j << " completed in " << diff.count() << " seconds by thread " << thread_id << std::endl;
		   // }

			return diff;
		})
	);
}
auto total_start = std::chrono::system_clock::now();

// 获取所有任务的执行时间并累加
std::chrono::duration<double> total_task_time(0);
for (auto&& result : results) {
	total_task_time += result.get();
}

// 记录整个多线程任务的结束时间
auto total_end = std::chrono::system_clock::now();
std::chrono::duration<double> total_duration = total_end - total_start;

// 输出每个线程的总执行时间
{
	std::lock_guard<std::mutex> lock(output_mutex);
	for (const auto& pair : threadContexts) {
		std::cout << "Thread " << pair.first << " total execution time: " << pair.second.executionTime.count() << " seconds" << std::endl;
	}
}



// 输出总的任务执行时间（所有任务执行时间的总和）
// std::cout << "Total task execution time (sum of all tasks): " << total_task_time.count() << " seconds" << std::endl;

 // 输出多线程并行执行时间（从任务开始到结束的时间）
 std::cout << "Total parallel execution time: " << total_duration.count() << " seconds" << std::endl;
 */





 /*
 //不分组多线程
 //多线程资源池
 ThreadPool pool(10);

 // 创建一个存储任务结果的向量
 std::vector<std::future<std::chrono::duration<double>>> results;

 // 创建一个互斥锁来保护输出
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

			 // 使用锁保护整个任务过程的输出
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



			 // 使用锁保护时间输出
			 {
				 std::lock_guard<std::mutex> lock(output_mutex);
				 std::cout << "Task " << j << " completed in " << diff.count() << " seconds" << std::endl;
			 }

			 return diff;
			 })
	 );
 }


 // 获取所有任务的执行时间并累加
 for (auto&& result : results) {
	 total_diff += result.get();
 }

 std::cout << "Total execution time: " << total_diff.count() << " seconds" << std::endl;
 */





//=============================================
//=============================================
//=============================================







	//故障路径重规划(未划分网络)

	//第n个单元的节点数
	//for (int i = 0; i < pMap[]->m_iNodeCount; i++)
	//	cout << maps[k * k * k / 100]->m_pNodeArray[i].m_cData << ",";
	//cout << endl;

	//设置故障单元数量
	//第n个单元的某条链路发生故障

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











	//spine-leaf拓扑
	/*const int num = 232;//总的交换机数量
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
//print(smallest_units); cout << smallest_units.size() << endl;

auto plan2 = std::chrono::high_resolution_clock::now();
planning_time = plan2 - plan1;
std::cout << "Total_plan: " << planning_time.count() << " seconds" << std::endl;


//-------------------------------------------------------
// ------------------------------------------------------

//DFS路径规划(单线程）

//cout << smallest_units.size() << endl;
auto beg_t = chrono::system_clock::now(); //开始时间

for (int i = 0; i < smallest_units.size(); i++) {
	CMap* smallmap = build_small_units_link(smallest_units[i]);
	smallmap->updateDegree();
	//vector<Node> res = smallmap->DFS1(smallmap->m_pNodeArray[0].m_cData);
	vector<Node> res = smallmap->DFS1(0);
	//cout << "mcdata:" << smallmap->m_pNodeArray[0].m_cData << endl;//输出第一个节点值
	//smallmap->printMartrix();
	//smallmap->print_DFS_res(); //打印路径
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

cout << "共有" << maps.size() << "个最小单元需要规划路径" << endl;




//-------------------------------------------------------
// ------------------------------------------------------


//故障路径重规划

//第n个单元的节点数
for (int i = 0; i < maps[k * k * k / 100]->m_iNodeCount; i++)
	cout << maps[k * k * k / 100]->m_pNodeArray[i].m_cData << ",";
cout << endl;

//设置故障单元数量
//第n个单元的某条链路发生故障
maps[k * k * k / 100]->port_for_errorfinding = true;
maps[k * k * k / 100]->setValueToMatrixForUndirectedGraph(0, 8, 0);


//计算寻找故障单元的时间
auto beg_t2 = chrono::system_clock::now();
vector<CMap*> error_maps = find_error_units(maps);


//计算故障单元路径规划的时间
//cout << error_maps.size() << endl;
auto beg_t1 = chrono::system_clock::now();
for (int i = 0; i < error_maps.size(); i++) {
	//cout << "Processing error map " << i << endl;
	error_maps[i]->DFS1(0);
	//error_maps[i]->depthFirstTraverse(0); // 从索引0开始DFS
}
auto end_t1 = chrono::system_clock::now();
chrono::duration<double> diff1 = end_t1 - beg_t1;
cout << "replanning dfs time:" << diff1.count() << endl;


/*
//删除故障拓扑
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
//剩余拓扑故障规划的时间
for (int pp = 0; pp < 1; pp++) {
	chrono::duration<double> total_diff(0); // 初始化总时间变量
	for (int j = 0; j < maps_without_error.size(); j++) {
		maps_without_error[j]->noDegree.clear();
		//maps_without_error[j]->resetMap();
		//maps_without_error[j]->updateDegree();
		maps_without_error[j]->been_dfs = false;
		auto beg_t = chrono::system_clock::now();
		maps_without_error[j]->depthFirstTraverse(0);
		auto end_t = chrono::system_clock::now();
		chrono::duration<double> diff = end_t - beg_t;
		total_diff += diff; // 累计时间
	}
	cout << total_diff.count() << endl;
}*/

auto end_t2 = chrono::system_clock::now();
chrono::duration<double> diff2 = end_t2 - beg_t2;
cout << "replanning time:" << diff2.count() << endl;

