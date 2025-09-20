#ifndef SERIAL_SENDER_H
#define SERIAL_SENDER_H

#include "rm_serial/serial_port.h"
#include "debug_tools/debug_tools.h"

namespace rm_serial{
class SerialSender{
private:  
    SerialPort::Ptr serial_port_;  
    std::deque<uint8_t> data_queue_;  
    std::mutex mutex_queue_;  
    std::condition_variable data_available_;  
    std::thread thread_send_;  
    std::atomic<bool> is_running_;

    void sendData(){  
        while(is_running_){
            std::unique_lock<std::mutex> lock(mutex_queue_);  
            data_available_.wait(lock, [this] { return !data_queue_.empty() || !is_running_; });  

            debug_tools::Debug().print("queue size:", static_cast<int>(data_queue_.size()));

            if(!is_running_ && data_queue_.empty()) break;

            uint8_t data = data_queue_.front();  
            data_queue_.pop_front();  
            lock.unlock();  

            if(!serial_port_->writeData(data)) {  
                std::cerr << "发送失败: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << " ";  
            }
        }  
    }

    void stop(){  
        is_running_ = false;  
        data_available_.notify_one();  
        if(thread_send_.joinable()) thread_send_.join();  
    }

public:
    typedef std::shared_ptr<SerialSender> Ptr;
    SerialSender(SerialPort::Ptr serialPort) : serial_port_(serialPort), is_running_(true){ 
        thread_send_ = std::thread(&SerialSender::sendData, this);  
    }

    ~SerialSender(){  
        stop();  
    }

    void enqueueData(uint8_t data){  
        {  
            std::lock_guard<std::mutex> lock(mutex_queue_);  
            data_queue_.push_back(data);  
        }  
        data_available_.notify_one();  
    }

    void enqueueDatas(const std::vector<uint8_t>& datas){
        {  
            std::lock_guard<std::mutex> lock(mutex_queue_);  
            for(const auto& d : datas) data_queue_.push_back(d); 
        }  
        data_available_.notify_one();  
    }
};
}//namespace rm_serial

#endif