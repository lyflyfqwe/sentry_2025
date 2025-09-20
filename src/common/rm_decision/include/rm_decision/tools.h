#ifndef RM_DECISION_TOOLS_H
#define RM_DECISION_TOOLS_H

#include <Eigen/Eigen>
#include <vector>

namespace rm_decision{
class Tools{
public:
    template<typename T, int S> bool
    vecToEigen(const std::vector<T, std::allocator<T>>& vec, Eigen::Vector<T, S>& eigen){
        if(static_cast<int>(vec.size()) < S) return false;
        for(int i{0}; i < S; i++) eigen(i) = vec.at(i);
        return true;
    }

    inline double getYaw(const Eigen::Matrix3d& mat){
        return std::atan2(mat(1, 0), mat(0, 0));
    }

    inline double getYaw(const Eigen::Vector3d& v){
        return std::atan2(v.y(), v.x());
    }

    /**
     * @brief 输入弧度制,输出四元数
     */
    Eigen::Quaterniond eulerToQuaternion(double roll, double pitch, double yaw){
        Eigen::AngleAxisd rollAngle(roll, Eigen::Vector3d::UnitX());  
        Eigen::AngleAxisd pitchAngle(pitch, Eigen::Vector3d::UnitY());  
        Eigen::AngleAxisd yawAngle(yaw, Eigen::Vector3d::UnitZ());  
    
        // 合成四元数，顺序为先 yaw、后 pitch、再 roll  
        Eigen::Quaterniond q = yawAngle * pitchAngle * rollAngle;  
        return q;  
    }

    // inline void vecToRegion(const std::vector<double>& vec, PolygonRegion& region){
    //     for(const auto& p : vec) region.push_back();
    // }
}; //class Tools
} //namespace rm_decision

#endif

