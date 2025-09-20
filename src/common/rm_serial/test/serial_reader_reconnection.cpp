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

namespace rm_serial{
class SerialPort{
private:  
    std::string port_;  
    uint32_t baudrate_;  
    serial::Serial serial_;  
    std::function<void(uint8_t)> dataCallback_;  
    std::thread thread_read_;
    std::atomic<bool> running_; // 原子布尔变量，用于控制线程 

    std::chrono::steady_clock::time_point last_reconnection_attempt_; // 上次尝试重连的时间  
    long reconnect_interval_; // 重连间隔时间，毫秒 

    void openSerialPort(){
        try{  
            serial_.setPort(port_);  
            serial_.setBaudrate(baudrate_);  
            serial::Timeout timeout = serial::Timeout::simpleTimeout(1000); // 设置超时  
            serial_.setTimeout(timeout);  
            serial_.open();  

            if(serial_.isOpen()){
                std::cout << "Serial port " << port_ << " opened at " << baudrate_ << std::endl; 
                running_ = true; // 串口打开成功后，设置 running_ 为 true  
                last_reconnection_attempt_ = std::chrono::steady_clock::now(); //  成功打开串口后，更新时间
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

    void closeSerialPort(){  
        if(serial_.isOpen()){  
            serial_.close();  
            std::cout << "串口已关闭" << std::endl;  
            running_ = false;  
        }  
    }

public:  
    SerialPort(const std::string& port, uint32_t baudrate)
    :port_(port), baudrate_(baudrate), running_(false), reconnect_interval_(5000){  
        openSerialPort();
    }  

    ~SerialPort(){  
        stopReadThread();
        closeSerialPort();
    }

    // 从串口读取数据  
    bool readData(uint8_t& data){  
        if(!serial_.isOpen()){  
            if(std::chrono::steady_clock::now() - last_reconnection_attempt_ > std::chrono::milliseconds(reconnect_interval_)){  
                std::cerr << "readData 串口未连接,尝试重连... 重连时间间隔： " << reconnect_interval_ << " ms" << std::endl;  
                openSerialPort();  
                last_reconnection_attempt_ = std::chrono::steady_clock::now(); // 更新上次尝试重连的时间  
            }    
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
            closeSerialPort(); // 出现异常时关闭串口，触发重连
            return false;  
        }catch(const std::exception& e){  
            std::cerr << "Exception: " << e.what() << std::endl;  
            closeSerialPort(); // 出现异常时关闭串口，触发重连
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
        if(thread_read_.joinable()) thread_read_.join(); 
    }

    //  新增的 writeData 函数  
    bool writeData(uint8_t data){
        if(!serial_.isOpen()) {  
            std::cerr << "writeData 串口未连接" << std::endl;  
            return false;  
        }  

        try{  
            serial_.write(&data, 1);  
            return true;  
        }catch(const serial::IOException& e){  
            std::cerr << "IOException: " << e.what() << std::endl;  
            closeSerialPort(); // 出现异常时关闭串口，触发重连  
            return false;  
        }catch(const std::exception& e){  
            std::cerr << "Exception: " << e.what() << std::endl;  
            closeSerialPort(); // 出现异常时关闭串口，触发重连  
            return false;  
        }  
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

class SerialSender{
private:  
    SerialPort& serial_port_;  
    std::deque<uint8_t> data_queue_;  
    std::mutex mutex_queue_;  
    std::condition_variable data_available_;  
    std::thread thread_send_;  
    std::atomic<bool> is_running_;

    void sendData(){  
        while(is_running_){
            std::unique_lock<std::mutex> lock(mutex_queue_);  
            data_available_.wait(lock, [this] { return !data_queue_.empty() || !is_running_; });  

            if(!is_running_ && data_queue_.empty()) break;

            uint8_t data = data_queue_.front();  
            data_queue_.pop_front();  
            lock.unlock();  

            if(!serial_port_.writeData(data)) {  
                std::cerr << "发送失败: 0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << std::endl;  
            }
        }  
    }

    void stop(){  
        is_running_ = false;  
        data_available_.notify_one();  
        if(thread_send_.joinable()) thread_send_.join();  
    }

public:
    SerialSender(SerialPort& serialPort) : serial_port_(serialPort), is_running_(true){ 
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
};
}

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
    rm_serial::SerialSender serial_sender(serial_port);

    serial_port.setDataCallback([&](uint8_t data) {  
        serial_reader.receiveData(data);  
    });  

    serial_port.startReadThread();  

    //  持续向发送队列中添加数据  
    std::thread sendThread([&]() {  
        std::vector<uint8_t> datas;
        for(int x{0}; x < 10; x++) datas.push_back(static_cast<uint8_t>(x));
        int index{0};
        while(running) {  
            serial_sender.enqueueData(datas.at(index++ % 10));  
            std::this_thread::sleep_for(std::chrono::milliseconds(50)); //  控制发送频率  
        }  
    });  

    // 保持程序运行，直到接收到中断信号  
    while(running){  
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); //  防止 CPU 占用过高  
    } 
    serial_port.stopReadThread(); //  显式调用 stopReadThread  
    return 0; 
}