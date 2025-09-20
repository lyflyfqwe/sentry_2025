#include <deque>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip> // 用于格式化输出
#include <iostream>
#include <functional>
#include <atomic>
#include <serial/serial.h>
#include <csignal>

#include <simple_serial/serial_port.h>

namespace rm_serial{
class SerialPort{
private:  
    std::string port_;  
    uint32_t baudrate_;  
    serial::Serial serial_;  
    std::function<void(uint8_t)> dataCallback_;  
    std::thread thread_read_;
    std::atomic<bool> running_; // 原子布尔变量，用于控制线程 

public:  
    SerialPort(const std::string& port, uint32_t baudrate) : port_(port), baudrate_(baudrate), running_(false){  
        try{  
            serial_.setPort(port_);  
            serial_.setBaudrate(baudrate_);  
            serial::Timeout timeout = serial::Timeout::simpleTimeout(1000); // 设置超时  
            serial_.setTimeout(timeout);  
            serial_.open();  

            if(serial_.isOpen()){
                std::cout << "Serial port " << port_ << " opened at " << baudrate_ << std::endl; 
                running_ = true; // 串口打开成功后，设置 running_ 为 true  
            } 
            else std::cerr << "打开串口失败" << port_ << std::endl;  
        } 
        catch (const serial::IOException& e){  
            std::cerr << "IOException: " << e.what() << std::endl;  
        } 
        catch(const std::exception& e){  
            std::cerr << "Exception: " << e.what() << std::endl;  
        }  
    }  

    ~SerialPort(){  
        if (serial_.isOpen()) {  
            serial_.close();  
            std::cout << "串口已关闭" << std::endl;  
        }
    }

    // 从串口读取数据  
    bool readData(uint8_t& data){  
        if(!serial_.isOpen()){  
            std::cerr << "串口未连接" << std::endl;  
            return false;  
        }

        try{  
            if(serial_.available()){  
                data = serial_.read(1)[0]; // 读取一个字节  
                return true;  
            }
            else return false; // 没有数据可读   
        } 
        catch(const serial::IOException& e){  
            std::cerr << "IOException: " << e.what() << std::endl;  
            return false;  
        }catch(const std::exception& e){  
            std::cerr << "Exception: " << e.what() << std::endl;  
            return false;  
        } 
    }  

    // 设置数据接收回调函数  
    void setDataCallback(std::function<void(uint8_t)> callback) {  
        dataCallback_ = callback;  
    }

    // 启动串口读取线程  
    void startReadThread(){
        running_ = true; // 启动线程前，设置 running_ 为 true 
        thread_read_ = std::thread([this](){  
            uint8_t data;  
            while(running_){  // 使用 running_ 控制循环  
                if(readData(data)) {  
                    if(dataCallback_) dataCallback_(data);  
                } 
                // 没有数据时休眠一段时间，避免 CPU 占用过高
                else std::this_thread::sleep_for(std::chrono::milliseconds(10));  
            }  
        });  
    }

     // 停止串口读取线程  
    void stopReadThread(){  
        running_ = false; // 设置 running_ 为 false，停止线程
        // 这里只是简单地detach，可能会导致资源泄漏  
        thread_read_.detach();  
    }
};

class SerialReader{
private:  
    std::deque<uint8_t> data_queue_; // 数据队列  
    std::mutex mutex_queue_;          // 队列互斥锁  
    std::condition_variable data_available_; // 条件变量，用于通知数据可用  
    std::thread thread_process_;       // 数据处理线程  
    bool is_running_;                  // 线程运行标志
    int count_print_; //  添加计数器 一行打印10个

    void processData() {  
        while (is_running_){  
            std::unique_lock<std::mutex> lock(mutex_queue_);  
            data_available_.wait(lock, [this] { return !data_queue_.empty() || !is_running_; });  

            if (!is_running_ && data_queue_.empty())  break; // 退出线程  

            uint8_t data = data_queue_.front();  
            data_queue_.pop_front();  
            lock.unlock();  

            // 打印数据  
            std::cout << "0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << std::endl;  
            count_print_++;
            if(count_print_ % 10 == 0) std::cout << std::endl;
        }  
    }

public:
    SerialReader(): is_running_(true), count_print_(0){
        thread_process_ = std::thread(&SerialReader::processData, this); 
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
            data_queue_.push_back(data);  
        }  
        data_available_.notify_one(); // 通知处理线程有数据可用  
    }
};
}

simple_serial::SerialPort::Ptr port_;

// 定义一个全局变量，用于控制程序的运行  
std::atomic<bool> running(true);  

// 信号处理函数，用于捕获 Ctrl+C 信号  
void signalHandler(int signum){  
    std::cout << "Interrupt signal (" << signum << ") received.\n";  
    running = false;  
}

int main() {  
    std::string port = "/dev/ttyUSB0";  
    uint32_t baudrate = 115200;  

    rm_serial::SerialPort serial_port(port, baudrate);  
    rm_serial::SerialReader serial_reader;  

    port_ = std::make_shared<simple_serial::SerialPort>("/dev/ttyUSB1", 115200, 2000);

    serial_port.setDataCallback([&](uint8_t data) {  
        serial_reader.receiveData(data);  
    });  

    serial_port.startReadThread();  

    // 保持程序运行，直到接收到中断信号  
    while(running){  
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //  防止 CPU 占用过高  
    } 

    serial_port.stopReadThread(); //  显式调用 stopReadThread  

    // SerialParser 会在析构函数中停止线程  

    return 0; 
}