#include "rm_serial/data_list.h"

int main() {
    rm_serial::DataList<rm_serial::BIG> data_list_big;   //大端序解析

    data_list_big.addType<uint8_t>();    // 索引 0
    data_list_big.addType<uint16_t>();   // 索引 1
    data_list_big.addType<float>();      // 索引 2
    data_list_big.addType<int32_t>();    // 索引 3
    data_list_big.addType<double>();     // 索引 4
    data_list_big.addType<uint8_t>();    // 索引 5

    // 输入数据（大端序）
    std::vector<uint8_t> vec_big = {
        0x11,                     // uint8_t: 17
        0x12, 0x34,               // uint16_t: 0x1234 = 4660
        0x3f, 0x8c, 0xcc, 0xcd,   // float: 1.1
        0x00, 0x00, 0x00, 0x05,   // int32_t: 5
        0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // double: 2.5
        0x01                       // uint8_t: 1
    };

    try {
        data_list_big.fillFromVector(vec_big);

        // 访问数据
        std::cout << "BIG Endian:" << std::endl;
        std::cout << "--- uint8_t[0]: " << static_cast<int>(data_list_big.getValue<uint8_t>(0)) << std::endl;
        std::cout << "--- uint16_t[1]: " << data_list_big.getValue<uint16_t>(1) << std::endl;
        std::cout << "--- float[2]: " << data_list_big.getValue<float>(2) << std::endl;
        std::cout << "--- int32_t[3]: " << data_list_big.getValue<int32_t>(3) << std::endl;
        std::cout << "--- double[4]: " << data_list_big.getValue<double>(4) << std::endl;
        std::cout << "--- uint8_t[5]: " << static_cast<int>(data_list_big.getValue<uint8_t>(5)) << std::endl;

    }catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }

    rm_serial::DataList<rm_serial::SMALL> data_list_small;  //小端序解析

    data_list_small.addType<uint16_t>();
    data_list_small.addType<uint16_t>();

    // 输入数据（小端序）
    std::vector<uint8_t> vec_small = {0x12, 0x34, 0x56, 0x78};

    try{
        data_list_small.fillFromVector(vec_small);

        std::cout << "SMALL Endian:" << std::endl;
        std::cout << "--- uint16_t[0]: " << data_list_small.getValue<uint16_t>(0) << std::endl; // 输出 0x3412 = 13330
        std::cout << "--- uint16_t[1]: " << data_list_small.getValue<uint16_t>(1) << std::endl; // 输出 0x7856 = 30806

    }catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}