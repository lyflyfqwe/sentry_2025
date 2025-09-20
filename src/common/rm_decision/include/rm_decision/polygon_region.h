#ifndef RM_DECISION_POLYGON_REGION_H
#define RM_DECISION_POLYGON_REGION_H

#include <vector>
#include <memory>
#include <random>
#include <Eigen/Eigen>
#include "debug_tools/debug_tools.h"

namespace rm_decision{
class PolygonRegion{
private:
    std::vector<Eigen::Vector2d> points_;
    double avg_z_{0};

    inline int orientation(const Eigen::Vector2d& p, const Eigen::Vector2d& q, const Eigen::Vector2d& r){
        double val = (q.y() - p.y()) * (r.x() - q.x()) - (q.x() - p.x()) * (r.y() - q.y());
        if(std::fabs(val) < 1e-9) return 0; // collinear
        return val>0? 1 : 2; // clock or counterclock wise
    }

    inline bool onSegment(const Eigen::Vector2d& p, const Eigen::Vector2d& q, const Eigen::Vector2d& r){
        return q.x() <= std::max(p.x(), r.x()) && q.x() >= std::min(p.x(), r.x()) 
                && q.y() <= std::max(p.y(), r.y()) && q.y() >= std::min(p.y(), r.y());
    }

    inline bool doIntersect(const Eigen::Vector2d& p1, const Eigen::Vector2d& q1, 
            const Eigen::Vector2d& p2, const Eigen::Vector2d& q2){
        int o1 = orientation(p1, q1, p2);
        int o2 = orientation(p1, q1, q2);
        int o3 = orientation(p2, q2, p1);
        int o4 = orientation(p2, q2, q1);
        if(o1 != o2 && o3 != o4) return true;
        if(o1 == 0 && onSegment(p1, p2, q1)) return false;
        if (o2 == 0 && onSegment(p1, q2, q1)) return false;
        if (o3 == 0 && onSegment(p2, p1, q2)) return false;
        if (o4 == 0 && onSegment(p2, q1, q2)) return false;
        return false;
    }

    inline bool isPointInsidePolygon(std::vector<Eigen::Vector2d>& polygon, const Eigen::Vector2d& p){
        auto n = static_cast<int>(polygon.size());
        if(n < 3) return false; // A polygon must have at least 3 vertices

        auto extreme = Eigen::Vector2d{1e9, p.y()}; 
        int count{0}, i{0};
        do{
            int next = (i + 1) % n;

            if(doIntersect(polygon[i], polygon[next], p, extreme)){
                if(orientation(polygon[i], p, polygon[next]) == 0) return false; // Point is on boundary
                count++;
            }
            i = next;
        }while(i != 0);
        return count%2 == 1; // Inside if odd, outside if even
    }

    bool inside(const Eigen::Vector2d& p){
        return isPointInsidePolygon(points_, p); 
    }

public:
    typedef std::shared_ptr<PolygonRegion> Ptr;

    PolygonRegion(){};

    PolygonRegion(const std::vector<Eigen::Vector3d>& vec){ 
        double num{0};
        for(const auto& p : vec){
            num += p.z();
            points_.push_back(Eigen::Vector2d(p.x(), p.y()));
        } 
        avg_z_ = num / static_cast<double>(points_.size());
    }

    size_t size(){ return points_.size(); }
    
    /**
     * @brief 给构成多边形的点集添加一个点，使多边形增加一条边
     */
    void push_back(const Eigen::Vector3d& p){ 
        double num = avg_z_ * static_cast<double>(points_.size());
        points_.push_back(Eigen::Vector2d(p.x(), p.y())); 
        avg_z_ = (num + p.z()) / static_cast<double>(points_.size());
    }

    /**
     * @brief 判断点的 x,y是否位于多边形的二维平面内，且点的z与多边形点集的平均z的误差值小于miss_z
     * @param p 输入的三维点坐标
     * @param miss_z z轴的最大误差
     * @return 是否位于多边形内
     */
    bool inside(const Eigen::Vector3d& p, double miss_z){
        return std::abs(p.z()-avg_z_) < miss_z 
            && isPointInsidePolygon(points_, Eigen::Vector2d(p.x(), p.y())); 
    }

    std::vector<Eigen::Vector2d> getPoints(){ return points_; }

    double getAvgZ(){ return avg_z_; }

    /**
     * @brief 获取多边形内的随机点
     */
    bool randomPoint(Eigen::Vector3d& pos){
        if(points_.size() <= 2) return false;
        // 计算边界框  
        double min_x = std::numeric_limits<double>::max();  
        double max_x = std::numeric_limits<double>::lowest();  
        double min_y = std::numeric_limits<double>::max();  
        double max_y = std::numeric_limits<double>::lowest();  

        int num{0};
        for(const auto& point : points_){
            num++;  
            min_x = std::min(min_x, point.x());  
            max_x = std::max(max_x, point.x());  
            min_y = std::min(min_y, point.y());  
            max_y = std::max(max_y, point.y());  
        }

        std::random_device rd;  // 获取随机数种子  
        std::mt19937 gen(rd()); // 初始化随机数生成器  
        std::uniform_real_distribution<double> disX(min_x, max_x); // X坐标的均匀分布  
        std::uniform_real_distribution<double> disY(min_y, max_y); // Y坐标的均匀分布  

        Eigen::Vector2d random_p;  
        do{
            random_p.x() = disX(gen);
            random_p.y() = disY(gen);  
        }while(!inside(random_p)); // 重复直到找到一个在多边形内的点  

        pos = Eigen::Vector3d(random_p.x(), random_p.y(), avg_z_);
        return true;
    }  
};
} //namespace rm_decision

#endif