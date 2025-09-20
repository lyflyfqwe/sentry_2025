#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include <iostream>
#include <chrono>
#include <memory>

namespace debug{
class Debug{
private:
    // unpacker 函数，用于打印单个参数  
    template <class T> T 
    unpacker(const T& t) {  
        std::cout << t << " "; // 输出参数  
        return t; // 返回参数（可选）  
    }

    // 针对 const char* 的重载，支持 C 字符串打印  
    void unpacker(const char* t) {  
        std::cout << t << " "; // 直接输出 C 风格字符串  
    }

public:
    // print 函数，用于打印名称和任意数量的参数  
    template <typename T, typename... Args> void 
    print(const std::string& name, const T& t, const Args&... data) {  
        std::cout << name << ": " << t << " "; // 打印名称和第一个参数  
        (unpacker(data), ...); // 展开参数包并调用 unpacker  
        std::cout << "\n"; // 换行  
    } 
};

class Timer{
private:
    std::chrono::_V2::system_clock::time_point time_init_;
    int64_t duration_{0};

public:
    typedef std::shared_ptr<Timer> Ptr;

    Timer(){
        time_init_ = std::chrono::system_clock::now();
    }

    Timer(Timer& timer){
        time_init_ = std::chrono::system_clock::now();
        duration_ = std::chrono::duration_cast<std::chrono::milliseconds>(time_init_ - timer.time_init_).count();
    }

    void durLog(std::string log_a = "", std::string log_b = ""){
        std::cout << log_a << ": " << duration_ << log_b << "\n";
    }
};
}

#endif