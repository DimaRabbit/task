#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip> 

void sum_range(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = vec1[i] + vec2[i];
    }
}


double parallel_sum(const std::vector<int>& vec1, const std::vector<int>& vec2, std::vector<int>& result, int num_threads) {
    std::vector<std::thread> threads;
    int size = vec1.size();
    int block_size = size / num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();  

    for (int i = 0; i < num_threads; ++i) {
        int start = i * block_size;
        int end = (i == num_threads - 1) ? size : (i + 1) * block_size;
        threads.push_back(std::thread(sum_range, std::cref(vec1), std::cref(vec2), std::ref(result), start, end));
    }

    for (auto& t : threads) {
        t.join();  
    }

    auto end_time = std::chrono::high_resolution_clock::now();  
    std::chrono::duration<double> elapsed = end_time - start_time;
    return elapsed.count();  
}

int main() {
   
    unsigned int num_cores = std::thread::hardware_concurrency();
    std::cout << "Количество доступных аппаратных ядер: " << num_cores << std::endl;

    std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };
   
    std::vector<int> threads_count = { 2, 4, 8, 16 };

    
    std::cout << std::setw(10) << "Elements" << std::setw(10) << "Threads" << std::setw(20) << "Time (seconds)" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;

   
    for (int size : sizes) {
       
        std::vector<int> vec1(size, 1);  
        std::vector<int> vec2(size, 2); 
        std::vector<int> result(size, 0);  

        for (int num_threads : threads_count) {
            
            double elapsed_time = parallel_sum(vec1, vec2, result, num_threads);

            
            std::cout << std::setw(10) << size << std::setw(10) << num_threads << std::setw(20) << elapsed_time << std::endl;
        }
    }

    return 0;
}