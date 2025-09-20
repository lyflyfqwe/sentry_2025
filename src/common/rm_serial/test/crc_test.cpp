#include <vector>  
#include <cstring>  // for memcpy  
#include <serial/serial.h>  

#include "debug_tools/debug_tools.h"

struct DataPacket {  
    uint8_t id;  
    uint8_t type;  
    float value1;  
    float value2;  
    // 假设数据包结构固定，可以根据需要修改  
};  

uint8_t calculateCRC(const std::vector<uint8_t>& data) {  
    uint8_t crc = 0xFF;  // 初始化 CRC 值  
    for (auto byte : data) {  
        crc ^= byte;  // 执行异或操作  
        for (int i = 0; i < 8; i++) {
            if (crc & 0x01) {  
                crc = (crc >> 1) ^ 0x8C;  // 多项式 0x8C  
            } else {
                crc >>= 1;  
            }  
        }  
    }  
    return crc;  
}  

void parseData(std::vector<uint8_t>& buffer) {  
    while (buffer.size() >= sizeof(DataPacket) + 1) {  // 至少要有一个数据包和 CRC 字节  
        DataPacket packet;  
        std::memcpy(&packet, buffer.data(), sizeof(DataPacket));  
        
        uint8_t receivedCRC = buffer[sizeof(DataPacket)];  
        std::vector<uint8_t> payload(buffer.begin(), buffer.begin() + sizeof(DataPacket) + 1);  
        uint8_t calculatedCRC = calculateCRC(payload);  

        if (receivedCRC == calculatedCRC) {  
            // 数据解析成功  
            std::cout << "数据解析成功！" << std::endl;  
            std::cout << "ID: " << static_cast<int>(packet.id) << std::endl;  
            std::cout << "Type: " << static_cast<int>(packet.type) << std::endl;  
            std::cout << "Value1: " << packet.value1 << std::endl;  
            std::cout << "Value2: " << packet.value2 << std::endl;  

            // 移除已解析数据包  
            buffer.erase(buffer.begin(), buffer.begin() + sizeof(DataPacket) + 1);  
        } else {  
            // CRC 校验失败，可能表示数据包不完整或数据损坏  
            std::cerr << "CRC 校验失败！" << std::endl;  

            // 移动到下一个字节开始再次尝试解析  
            buffer.erase(buffer.begin());  // 删除第一个字节，继续尝试  
        }  
    }  
}

void sendData(serial::Serial& mySerial, const DataPacket& packet) {  
    std::vector<uint8_t> data(sizeof(DataPacket) + 1);  // +1 用于 CRC  
    std::memcpy(data.data(), &packet, sizeof(DataPacket));  
    
    // 计算并添加 CRC  
    uint8_t crc = calculateCRC(data);  
    data.push_back(crc);  

    // 发送数据  
    mySerial.write(data);  
    std::cout << "数据发送成功！ ID: " << static_cast<int>(packet.id) << std::endl;  
}

int main() {  
    serial::Serial serial_temp;

    try{
        serial_temp.setPort("/dev/ttyUSB0");
        serial_temp.setBaudrate(115200);
        serial::Timeout time_out = serial::Timeout::simpleTimeout(1000);
        serial_temp.setTimeout(time_out);
        serial_temp.open();
    }catch (serial::IOException& e){
        debug_tools::Debug().print("Unable to open port: ", e.what());
    }
    if (!serial_temp.isOpen()) return 1;

    // 示例: 发送一个数据包  
    DataPacket packetToSend = {1, 2, 3.14f, 2.71f};  // 初始化数据包  
    sendData(serial_temp, packetToSend);  // 发送数据包  

    std::vector<uint8_t> buffer;  // 用于存储接收到的数据  
    while (true) {  
        std::vector<uint8_t> tempBuffer(100);  
        size_t bytesRead = serial_temp.read(tempBuffer.data(), tempBuffer.size());  
        
        // 将接收到的数据添加到缓冲区  
        buffer.insert(buffer.end(), tempBuffer.begin(), tempBuffer.begin() + bytesRead);  
        
        // 解析缓冲区中的数据  
        parseData(buffer);  
    }  

    return 0;  
}