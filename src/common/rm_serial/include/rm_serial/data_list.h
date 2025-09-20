#ifndef DATA_LIST_H
#define DATA_LIST_H

#include <iostream>
#include <vector>
#include <variant>
#include <stdexcept>
#include <cstring>
#include <typeindex>
#include <any>
#include <memory>

namespace rm_serial{

struct BIG {};  //大端序解析标签
struct SMALL {}; //小端序解析标签

template <typename EndianPolicy> class DataList{
private:
    using SupportedTypes = std::variant<uint8_t, uint16_t, int32_t, float, double>;  //目前支持的数据类型

    std::vector<std::type_index> type_sequence_;
    size_t uint8Size_{0}; //一个包的大小 即包含uint8_t的数量
    std::vector<SupportedTypes> data_;

    // 根据策略解析字节流
    template <typename T> T 
    parse(const std::vector<uint8_t>& vec, size_t& index, size_t size){
        if (index + size > vec.size()) 
            throw std::out_of_range("Insufficient data for type " + std::string(typeid(T).name()));

        T value = 0;
        if constexpr (std::is_same_v<T, uint8_t>) value = vec[index++];
        else {  // 根据字节序策略解析多字节类型
            if constexpr (std::is_same_v<EndianPolicy, BIG>){ // 大端序：高位在前
                if constexpr (std::is_same_v<T, uint16_t>)
                    value = static_cast<uint16_t>(vec[index]) << 8 | vec[index + 1];
                else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, float>){
                    uint32_t bits = (vec[index] << 24) | (vec[index + 1] << 16) | (vec[index + 2] << 8) | vec[index + 3];
                    std::memcpy(&value, &bits, sizeof(T));
                } 
                else if constexpr (std::is_same_v<T, double>){
                    uint64_t bits = (static_cast<uint64_t>(vec[index]) << 56) |
                                   (static_cast<uint64_t>(vec[index + 1]) << 48) |
                                   (static_cast<uint64_t>(vec[index + 2]) << 40) |
                                   (static_cast<uint64_t>(vec[index + 3]) << 32) |
                                   (static_cast<uint64_t>(vec[index + 4]) << 24) |
                                   (static_cast<uint64_t>(vec[index + 5]) << 16) |
                                   (static_cast<uint64_t>(vec[index + 6]) << 8) |
                                   static_cast<uint64_t>(vec[index + 7]);
                    std::memcpy(&value, &bits, sizeof(double));
                }
            } 
            else{  // 小端序：低位在前              
                if constexpr (std::is_same_v<T, uint16_t>)
                    value = vec[index] | (static_cast<uint16_t>(vec[index + 1]) << 8);
                else if constexpr (std::is_same_v<T, int32_t> || std::is_same_v<T, float>){
                    uint32_t bits = vec[index] | (vec[index + 1] << 8) | (vec[index + 2] << 16) | (vec[index + 3] << 24);
                    std::memcpy(&value, &bits, sizeof(T));
                }
                else if constexpr (std::is_same_v<T, double>){
                    uint64_t bits = static_cast<uint64_t>(vec[index]) |
                                   (static_cast<uint64_t>(vec[index + 1]) << 8) |
                                   (static_cast<uint64_t>(vec[index + 2]) << 16) |
                                   (static_cast<uint64_t>(vec[index + 3]) << 24) |
                                   (static_cast<uint64_t>(vec[index + 4]) << 32) |
                                   (static_cast<uint64_t>(vec[index + 5]) << 40) |
                                   (static_cast<uint64_t>(vec[index + 6]) << 48) |
                                   (static_cast<uint64_t>(vec[index + 7]) << 56);
                    std::memcpy(&value, &bits, sizeof(double));
                }
            }
            index += size;
        }
        return value;
    }

public:
typedef std::shared_ptr<DataList<EndianPolicy>> Ptr;
    // 动态添加类型到解析序列
    template <typename T> void addType(){
        static_assert(
            std::is_same_v<T, uint8_t> ||
            std::is_same_v<T, uint16_t> ||
            std::is_same_v<T, int32_t> ||
            std::is_same_v<T, float> ||
            std::is_same_v<T, double>,
            "Unsupported type"
        );
        type_sequence_.push_back(typeid(T));
        uint8Size_ += sizeof(T);
    }

    size_t getUint8Size(){
        return uint8Size_;
    }

    // 从字节流填充数据
    void fillFromVector(const std::vector<uint8_t>& vec){
        size_t index{0};
        data_.clear();
        for(const auto& type : type_sequence_){
            if(type == typeid(uint8_t)) data_.push_back(parse<uint8_t>(vec, index, 1));
            else if(type == typeid(uint16_t)) data_.push_back(parse<uint16_t>(vec, index, 2));
            else if(type == typeid(int32_t)) data_.push_back(parse<int32_t>(vec, index, 4));    
            else if(type == typeid(float)) data_.push_back(parse<float>(vec, index, 4));    
            else if(type == typeid(double))  data_.push_back(parse<double>(vec, index, 8)); 
            else throw std::invalid_argument("Unsupported type in sequence");
        }
    }

    // 获取值
    template <typename T> T getValue(size_t index) const{
        if(index >= data_.size()){
            throw std::out_of_range("Index out of range");
        }
        return std::get<T>(data_[index]);
    }
};
} //namespace rm_serial

#endif