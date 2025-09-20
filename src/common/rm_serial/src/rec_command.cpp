#include "rclcpp/rclcpp.hpp"
#include "rm_interface/msg/command.hpp"
#include "rm_serial/serial_reader.h"
#include "rm_serial/data_list.h"
#include "rm_serial/serial_sender.h"
#include "debug_tools/debug_tools.h"

#include <std_msgs/msg/int32_multi_array.hpp>

namespace rm_serial{

class RecCommand: public rclcpp::Node{
private:
    SerialPort::Ptr serial_port_;
    SerialReader::Ptr serial_reader_;
    std::vector<uint8_t> datas_;  //读取串口的缓冲 
    bool right_flag_{false};  //数据头正确标志
    SerialSender::Ptr serial_sender_;
    DataList<SMALL>::Ptr data_list_;
    size_t data_uint8_size_;
    rclcpp::Subscription<rm_interface::msg::Command>::SharedPtr sub_cmd_;
    rclcpp::Publisher<std_msgs::msg::Int32MultiArray>::SharedPtr pub_sentry_info_;

    std::thread thread_read_;

    template<typename T>
    void structToVector(const T& data_struct, std::vector<uint8_t>& data_vec) {  
        // 计算结构体的大小  
        size_t struct_size = sizeof(data_struct);  
        
        // 清空 vector  
        data_vec.clear();  
        
        // 将结构体的数据复制到 vector 中  
        data_vec.resize(struct_size); // 调整 vector 大小以适应结构体大小  
        std::memcpy(data_vec.data(), &data_struct, struct_size); // 逐个字节复制  
    }

    template <typename T> std::vector<uint8_t> toVecUint8(const T& data){  
        static_assert(std::is_trivially_copyable<T>::value, "T must be trivially copyable");  
        std::vector<uint8_t> buffer(sizeof(T));  
        std::memcpy(buffer.data(), &data, sizeof(T));  
        return buffer;  
    } 

    void callBackCmd(const rm_interface::msg::Command::ConstSharedPtr& msg){
        debug_tools::Debug().print("callBackCmd");
        serial_sender_->enqueueDatas(toVecUint8(static_cast<uint8_t>(0x5A)));   //uint8_t
        serial_sender_->enqueueDatas(toVecUint8(msg->x));  //float
        serial_sender_->enqueueDatas(toVecUint8(msg->y));  //float
        serial_sender_->enqueueDatas(toVecUint8(msg->r));  //float
        serial_sender_->enqueueDatas(toVecUint8(static_cast<uint8_t>(msg->robot_mode)));  //uint8_t  0:分离 1:跟随 2:小陀螺
        serial_sender_->enqueueDatas(toVecUint8(static_cast<uint8_t>(msg->ahead))); //uint8_t 1:开启前哨站模式
        serial_sender_->enqueueDatas(toVecUint8(static_cast<uint8_t>(0xA5)));  //uint8_t
    }

    void readDataFromQueue(){
        while(rclcpp::ok()){
            auto data = serial_reader_->getData();
            // std::cout << "[0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << "]";
            if(datas_.empty() && data == 0xA5){ //第一个且数据头正确
                right_flag_ = true;
                std::cout << "head:[0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << "] ";
                continue;
            }
            if(datas_.size() == data_uint8_size_){  //容器里的已经达到除了头和尾的大小
                for(const auto& d : datas_) 
                    std::cout << "{0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(d) << "} ";
                debug_tools::Debug().print("size:", static_cast<int>(datas_.size()));

                if(data == 0x5A){
                    std::cout << "end:[0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << "] \n";
                    data_list_->fillFromVector(datas_); //从容器std::vector<uint8_t>内解析数据,容器内不含头和尾
                    auto hp = data_list_->getValue<uint16_t>(0);
                    auto bollet = data_list_->getValue<uint16_t>(1);
                    auto hp_tower_e = data_list_->getValue<uint16_t>(2);
                    auto control = data_list_->getValue<uint16_t>(3);
                    auto target_x = data_list_->getValue<uint16_t>(4);
                    auto target_y = data_list_->getValue<uint16_t>(5);
                    auto energy = data_list_->getValue<uint8_t>(6);
                    debug_tools::Debug().print("[RecCommand] rec data: ", hp, target_x, target_y, hp_tower_e, control,
                        target_x, static_cast<int>(target_y));

                    std::cout << std::dec << hp << " " << target_x << " " << target_y << " " << hp_tower_e << " "  << control << " " << target_x << "\n";

                    std_msgs::msg::Int32MultiArray msg_sentry_info;
                    msg_sentry_info.data.push_back(static_cast<int>(hp));
                    msg_sentry_info.data.push_back(static_cast<int>(bollet));
                    msg_sentry_info.data.push_back(static_cast<int>(hp_tower_e));
                    msg_sentry_info.data.push_back(static_cast<int>(control));
                    msg_sentry_info.data.push_back(static_cast<int>(target_x));
                    msg_sentry_info.data.push_back(static_cast<int>(target_y));
                    msg_sentry_info.data.push_back(static_cast<int>(energy));
                    pub_sentry_info_->publish(msg_sentry_info);
                }

                datas_.clear();
                right_flag_ = false;
            }
            if(right_flag_) datas_.push_back(data);
        }
    }

public: 
    RecCommand(): Node("rm_serial_rev_command_node", rclcpp::NodeOptions()){
        std::string port = "/dev/ttyUSB0";  
        uint32_t baudrate = 115200;  
        serial_port_ = std::make_shared<SerialPort>(port, baudrate);

        data_list_ = std::make_shared<DataList<SMALL>>();

        data_list_->addType<uint16_t>();
        data_list_->addType<uint16_t>();
        data_list_->addType<uint16_t>();
        data_list_->addType<uint16_t>();
        data_list_->addType<uint16_t>();
        data_list_->addType<uint16_t>();
        data_list_->addType<uint8_t>();
        data_uint8_size_ = data_list_->getUint8Size();

        serial_reader_ = std::make_shared<SerialReader>(100, false);
        serial_port_->setDataCallback([&](uint8_t data){
            // std::cout << " [0x" << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data) << "] ";
            serial_reader_->receiveData(data);
        });
        serial_port_->startReadThread();

        serial_sender_ = std::make_shared<SerialSender>(serial_port_);
        sub_cmd_ = this->create_subscription<rm_interface::msg::Command>(
                "/motion_cmd", 10, std::bind(&RecCommand::callBackCmd, this, std::placeholders::_1));

        pub_sentry_info_ = this->create_publisher<std_msgs::msg::Int32MultiArray>("/rm_sentry_info", 10);

        thread_read_ = std::thread(&RecCommand::readDataFromQueue, this);
    }

    ~RecCommand(){
        serial_port_->stopReadThread();
        serial_reader_->exit();
        if(thread_read_.joinable()) thread_read_.join();
        debug_tools::Debug().print("~RecCommand");
    }
};
}//namespace rm_serial

int main(int argc, char** argv){
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rm_serial::RecCommand>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}

