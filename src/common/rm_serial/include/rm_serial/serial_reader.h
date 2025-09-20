#ifndef SERIAL_READER_H
#define SERIAL_READER_H

#include "rm_serial/serial_port.h"

namespace rm_serial{
class SerialReader{
protected: 
    std::deque<uint8_t> data_queue_; // 数据队列  
    std::mutex mutex_queue_;          // 队列互斥锁  
    std::condition_variable data_available_; // 条件变量，用于通知数据可用  
    std::thread thread_process_;       // 数据处理线程  
    bool is_running_;                  // 线程运行标志
    int count_print_; //  添加计数器 一行打印10个
    uint8_t buffer_size_;
    bool is_exit_{false};

    void printData(){  
        while (is_running_){  
            std::unique_lock<std::mutex> lock(mutex_queue_);  
            data_available_.wait(lock, [this] { return !data_queue_.empty() || !is_running_; });  

            if (!is_running_ && data_queue_.empty())  break; // 退出线程  

            uint8_t data = data_queue_.front();  
            data_queue_.pop_front();  
            lock.unlock();  

            // 打印数据  
            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << std::endl;  
            if(count_print_++ % 10 == 0) std::cout << std::endl;
        }  
    }

public:
    typedef std::shared_ptr<SerialReader> Ptr;
    SerialReader(size_t buffer_size=100, bool is_print_log=false): count_print_(0){
        buffer_size_ = buffer_size; 
        is_running_ = is_print_log;
        thread_process_ = std::thread(&SerialReader::printData, this); 
    }

    void exit(){
        is_exit_ = true;
        data_available_.notify_one(); // 唤醒线程，让其退出  
    }

    ~SerialReader(){
        // 停止数据处理线程  
        is_running_ = false;  
        data_available_.notify_one(); // 唤醒线程，让其退出  
        thread_process_.join(); 
    }

    // 接收串口数据的回调函数  
    void receiveData(uint8_t data){  
        {
            std::lock_guard<std::mutex> lock(mutex_queue_);  
            if(data_queue_.size() > buffer_size_) data_queue_.pop_front();
            data_queue_.push_back(data);  
        }  
        data_available_.notify_one(); // 通知处理线程有数据可用  
    }

    uint8_t getData(){
        std::unique_lock<std::mutex> lock(mutex_queue_);  
        data_available_.wait(lock, [this] { return !data_queue_.empty() || is_exit_; });  
        if(is_exit_) return -1;
        uint8_t data = data_queue_.front();  
        data_queue_.pop_front();  
        lock.unlock();
        return data;
    }
};
} // namespace rm_serial


#endif