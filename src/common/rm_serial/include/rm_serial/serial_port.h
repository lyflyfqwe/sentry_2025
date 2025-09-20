#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

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

    // 从串口读取数据  
    bool readData(uint8_t& data){  
        if(!serial_.isOpen()){  
            if(std::chrono::steady_clock::now() - last_reconnection_attempt_ > std::chrono::milliseconds(reconnect_interval_)){  
                std::cerr << "readData 串口未连接,尝试重连... 重连时间间隔： " << static_cast<int>(reconnect_interval_) << " ms" << std::endl;  
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
        }
        catch(const std::exception& e){  
            std::cerr << "Exception: " << e.what() << std::endl;  
            closeSerialPort(); // 出现异常时关闭串口，触发重连
            return false;  
        } 
    } 
        
public:  
    typedef std::shared_ptr<SerialPort> Ptr;

    SerialPort(const std::string& port, uint32_t baudrate)
            :port_(port), baudrate_(baudrate), running_(false), reconnect_interval_(1000){  
        openSerialPort();
    }  

    ~SerialPort(){  
        stopReadThread();
        closeSerialPort();
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
        }
        catch(const serial::IOException& e){  
            std::cerr << "IOException: " << e.what() << std::endl;  
            closeSerialPort(); // 出现异常时关闭串口，触发重连  
            return false;  
        }
        catch(const std::exception& e){  
            std::cerr << "Exception: " << e.what() << std::endl;  
            closeSerialPort(); // 出现异常时关闭串口，触发重连  
            return false;  
        }  
    }
};
}//namespace rm_serial

#endif