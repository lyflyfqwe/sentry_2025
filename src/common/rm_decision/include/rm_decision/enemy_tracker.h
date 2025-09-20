#ifndef RM_DECISION_ENEMY_TRACKER_H
#define RM_DECISION_ENEMY_TRACKER_H

#include <vector>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <memory>

namespace rm_decision{

struct EnemyPoint {
    double x;
    double y;
    double z;
    double v;  // 新增权值
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    double density = 0.0;

    EnemyPoint(double x_, double y_, double z_, double v_,
              std::chrono::time_point<std::chrono::system_clock> ts)
        : x(x_), y(y_), z(z_), v(v_), timestamp(ts) {}
};

class EnemyTracker{
private:
    std::vector<EnemyPoint> points_;
    double lambda_t_ = 0.5;       // 时间衰减系数
    double lambda_s_ = 0.2;       // 空间密度系数
    double lambda_v_ = 0.3;       // 权值影响系数
    double cluster_radius_ = 2.0;
    double max_age_seconds_ = 5.0;

    // 三因素注意力计算
    double attentionScore(const EnemyPoint& p)const {
        auto now = std::chrono::system_clock::now();
        double age = std::chrono::duration<double>(now - p.timestamp).count();
        
        // 时间衰减 + 空间密度 + 权值影响
        return p.v * p.density * std::exp(-(
            lambda_t_ * age + 
            lambda_s_ / p.density +  // 密度倒置处理
            lambda_v_ * (1 - p.v)    // 权值补偿
        ));
    }

    void calculateDensity(){
        const double radius_sq = cluster_radius_ * cluster_radius_;
        
        for(auto& p : points_){
            p.density = std::accumulate(points_.begin(), points_.end(), 0.0,
                [&](double sum, const EnemyPoint& other) {
                    double dx = p.x - other.x;
                    double dy = p.y - other.y;
                    double dz = p.z - other.z;
                    return (dx*dx + dy*dy + dz*dz) <= radius_sq ? sum + other.v : sum;
                });
        }
    }

    bool getWeightedClusterCenter(double& cx, double& cy, double& cz) const{
        std::vector<std::vector<EnemyPoint>> clusters;
        std::vector<bool> processed(points_.size(), false);
        const double radius_sq = cluster_radius_ * cluster_radius_;

        // 带权值的DBSCAN聚类
        for(size_t i{0}; i<points_.size(); ++i){
            if(processed[i]) continue;
            
            std::vector<EnemyPoint> cluster;
            std::vector<size_t> neighbors;
            
            // 寻找邻近点（考虑权值阈值）
            for(size_t j{0}; j<points_.size(); ++j){
                if(points_[j].v < 0.2) continue;  // 过滤低权值点
                double dx = points_[i].x - points_[j].x;
                double dy = points_[i].y - points_[j].y;
                double dz = points_[i].z - points_[j].z;
                if (dx*dx + dy*dy + dz*dz <= radius_sq) neighbors.push_back(j);
            }

            // 构成有效集群（加权密度）
            double cluster_weight = std::accumulate(neighbors.begin(), neighbors.end(), 0.0,
                [&](double sum, size_t idx) { return sum + points_[idx].v; });
            
            if(cluster_weight >= 1.5) {  // 权值总和阈值
                for (auto idx : neighbors) {
                    if (!processed[idx]) {
                        cluster.push_back(points_[idx]);
                        processed[idx] = true;
                    }
                }
                clusters.push_back(cluster);
            }
        }
    
        if(!clusters.empty()){
            // 选择最高权值集群
            auto max_cluster = *std::max_element(clusters.begin(), clusters.end(),
                [](const auto& a, const auto& b) {
                    double wa = std::accumulate(a.begin(), a.end(), 0.0,
                        [](double s, const EnemyPoint& p) { return s + p.v; });
                    double wb = std::accumulate(b.begin(), b.end(), 0.0,
                        [](double s, const EnemyPoint& p) { return s + p.v; });
                    return wa < wb;
                });

            // 加权中心计算
            double sum_x{0}, sum_y{0}, sum_z{0}, total_v{0};
            for(const auto& p : max_cluster){
                sum_x += p.x * p.v;
                sum_y += p.y * p.v;
                sum_z += p.z * p.v;
                total_v += p.v;
            }
            cx = sum_x / total_v;
            cy = sum_y / total_v;
            cz = sum_z / total_v;
            return true;
        }
        return false;
    }

public:
    typedef std::shared_ptr<EnemyTracker> Ptr;

    /**
     * @param decay_t // 时间系数：1.0=快速遗忘旧点，0.1=保持长期记忆
     * @param decay_s // 空间系数：值越大越关注密集区域
     * @param decay_v // 权值系数：值越大越突出高权值点
     * @param radius // 聚类半径：根据战场规模调整
     * @param max_age // 最大时效：快速移动场景建议较小值
     */
    EnemyTracker(double decay_t=0.5, double decay_s=0.2, double decay_v=0.3,
                double radius=2.0, double max_age=5.0)
        : lambda_t_(decay_t), lambda_s_(decay_s), lambda_v_(decay_v),
          cluster_radius_(radius), max_age_seconds_(max_age) {}

    /**
     * @param vaild 是否有效，无效将不会添加
     */
    void updatePosition(double x, double y, double z, double v, bool vaild){
        auto now = std::chrono::system_clock::now();
        if(vaild) points_.emplace_back(x, y, z, v, now);
        
        auto cutoff = now - std::chrono::duration<double>(max_age_seconds_);
        points_.erase(
            std::remove_if(points_.begin(), points_.end(),
                [cutoff](const EnemyPoint& p) {return p.timestamp < cutoff;}),
            points_.end());
        
        calculateDensity();
    }

    bool selectTarget(double& target_x, double& target_y, double& target_z){
        if (points_.empty()) return false;
        // 优先选择加权集群中心
        double cluster_x, cluster_y, cluster_z;
        if(getWeightedClusterCenter(cluster_x, cluster_y, cluster_z)){
            target_x = cluster_x;
            target_y = cluster_y;
            target_z = cluster_z;
            return true;
        }

        // 次优选择：三因素注意力加权
        double total_weight = 0;
        double sum_x{0}, sum_y{0}, sum_z{0};
        for(const auto& p : points_){
            double w = attentionScore(p);
            sum_x += p.x * w;
            sum_y += p.y * w;
            sum_z += p.z * w;
            total_weight += w;
        }

        if(total_weight > 1e-6){
            target_x = sum_x / total_weight;
            target_y = sum_y / total_weight;
            target_z = sum_z / total_weight;
            return true;
        }
        return false;
    }
};
} //namespace rm_decision

#endif