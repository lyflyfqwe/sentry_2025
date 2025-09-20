#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <deque>
#include <cstring>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iomanip>
#include <iostream>
#include <functional>
#include <atomic>
#include <serial/serial.h>
#include <csignal>
#include <chrono>

namespace rm_serial {
class SerialPort {
private:
    std::string port_;
    uint32_t baudrate_;
    serial::Serial serial_;
    std::function<void(uint8_t)> dataCallback_;
    std::thread thread_read_;
    std::atomic<bool> running_;
    std::chrono::steady_clock::time_point last_reconnection_attempt_;
    long reconnect_interval_;
    mutable std::mutex serial_mutex_;  // 保护串口操作的互斥锁

    void openSerialPort() {
        std::lock_guard<std::mutex> lock(serial_mutex_);
        try {
            serial_.setPort(port_);
            serial_.setBaudrate(baudrate_);
            serial::Timeout timeout = serial::Timeout::simpleTimeout(1000);
            serial_.setTimeout(timeout);
            serial_.open();

            if (serial_.isOpen()) {
                std::cout << "Serial port " << port_ << " opened at " << baudrate_ << std::endl;
                running_ = true;
                last_reconnection_attempt_ = std::chrono::steady_clock::now();
            } else {
                std::cerr << "Failed to open serial port " << port_ << std::endl;
            }
        } catch (const serial::IOException& e) {
            std::cerr << "IOException: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    }

    void closeSerialPortImpl() {
        if (serial_.isOpen()) {
            serial_.close();
            std::cout << "Serial port closed." << std::endl;
            running_ = false;
        }
    }

public:
    typedef std::shared_ptr<SerialPort> Ptr;

    SerialPort(const std::string& port, uint32_t baudrate)
        : port_(port), baudrate_(baudrate), running_(false), reconnect_interval_(1000) {
        openSerialPort();
    }

    ~SerialPort() {
        stopReadThread();
        closeSerialPort();
    }

    void closeSerialPort() {
        std::lock_guard<std::mutex> lock(serial_mutex_);
        closeSerialPortImpl();
    }

    bool readData(uint8_t& data) {
        std::unique_lock<std::mutex> lock(serial_mutex_);

        if (!serial_.isOpen()) {
            auto now = std::chrono::steady_clock::now();
            if (now - last_reconnection_attempt_ > std::chrono::milliseconds(reconnect_interval_)) {
                std::cerr << "Attempting to reconnect..." << std::endl;
                lock.unlock();
                openSerialPort();
                lock.lock();
                last_reconnection_attempt_ = now;  // 更新重连时间，无论成功与否
            }
            return false;
        }

        try {
            if (serial_.available()) {
                data = serial_.read(1)[0];
                return true;
            } else {
                return false;
            }
        } catch (const std::exception& e) {
            std::cerr << "Read error: " << e.what() << std::endl;
            closeSerialPortImpl();
            return false;
        }
    }

    void setDataCallback(std::function<void(uint8_t)> callback) {
        dataCallback_ = callback;
    }

    void startReadThread() {
        running_ = true;
        thread_read_ = std::thread([this]() {
            uint8_t data;
            while (running_) {
                if (readData(data)) {
                    if (dataCallback_) dataCallback_(data);
                } else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));
                }
            }
        });
    }

    void stopReadThread() {
        running_ = false;
        if (thread_read_.joinable()) {
            thread_read_.join();
        }
    }

    bool writeData(uint8_t data) {
        std::lock_guard<std::mutex> lock(serial_mutex_);

        if (!serial_.isOpen()) {
            std::cerr << "Serial port not connected for writing." << std::endl;
            return false;
        }

        try {
            serial_.write(&data, 1);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Write error: " << e.what() << std::endl;
            closeSerialPortImpl();
            return false;
        }
    }

    void setReconnectInterval(long interval_ms) {
        reconnect_interval_ = interval_ms;
    }
};
} // namespace rm_serial

// rm_serial::SerialPort serial("/dev/ttyUSB0", 115200);
// serial.setDataCallback([](uint8_t data) {
//     std::cout << "Received: " << static_cast<int>(data) << std::endl;
// });
// serial.startReadThread();

// // 主线程中发送数据
// serial.writeData(0xAA);

// // 设置重连间隔为2秒
// serial.setReconnectInterval(2000);

#endif