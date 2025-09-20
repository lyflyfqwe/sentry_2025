#ifndef EA_DISK_H
#define EA_DISK_H

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>

namespace m_detector{
namespace cluster_predict{
class EA_Disk{
public:
    int n_, size_;
    std::vector<float> radius_;

    EA_Disk(int num){
        n_ = num;
        size_ = std::pow(2*n_+1, 2);
        for(int i{0}; i < n_+1; i++){
            float r = std::sqrt(2) * (2*i+1) * (2*n_+1);
            radius_.push_back(r);
        }
    }

    int indexFind(Eigen::Vector2f r_theta){
        float r = r_theta[0];
        float theta = r_theta[1];
        if(r <= radius_[0]) return 0;
        for(int i{0}; i < n_; i++){
            if(r > radius_[i] && r <= radius_[i+1]){
                int offset = std::pow(2*i+1, 2) - 1;
                int num_of_this_ring = 8 * (i+1);
                for(int j{0}; j < num_of_this_ring; j++){
                    if(theta >= j * 1.0f / num_of_this_ring * 2 * M_PI && theta < (j+1) * 1.0f / num_of_this_ring * 2 * M_PI)
                        return offset + j;
                }
            }
        }
    }

    //直角坐标->球坐标
    void catesianToSphere(const Eigen::Vector3f xyz, Eigen::Vector2f &phitheta){
        phitheta[0] = std::acos(xyz[2]);
        phitheta[1] = std::atan2(xyz[1], xyz[0]);
        if (phitheta[1] < 0) phitheta[1] += 2 * M_PI;
    }

    void sphereToDisk(const Eigen::Vector2f phitheta, Eigen::Vector2f &Rtheta){
        Rtheta[0] = 2 * std::sin(0.5 * phitheta[0]);
        Rtheta[1] = phitheta[1];
    }
};
} // namespace cluster_predict
} // namespace m_detector

#endif