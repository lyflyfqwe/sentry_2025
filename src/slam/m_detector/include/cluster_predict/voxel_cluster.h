#ifndef VOXEL_CLUSTER_H
#define VOXEL_CLUSTER_H

#include <unordered_map>
#include <unordered_set>
#include <boost/shared_ptr.hpp>

#include <Eigen/Dense>
#include <Eigen/Core>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>

namespace m_detector{
namespace cluster_predict{

const int HASH_LENGTH = 10000;

class VOXEL{
public:
    int64_t x_, y_, z_;
    VOXEL(int64_t vx=0, int64_t vy=0, int64_t vz=0): x_(vx), y_(vy), z_(vz){}
    bool operator==(const VOXEL& other) const{
        return x_==other.x_ && y_==other.y_ && z_==other.z_;
    }
};

class Point_Cloud{
public:
    int bbox_index_{-1}, point_num_{0};
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr cloud_;
    boost::shared_ptr<std::vector<int>> cloud_index_;
    
    typedef boost::shared_ptr<Point_Cloud> Ptr;

    Point_Cloud(pcl::PointXYZINormal point, int index){
        cloud_->points.push_back(point);
        bbox_index_ = index;
    }

    Point_Cloud(pcl::PointXYZINormal point){
        cloud_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
        cloud_->reserve(5);
        cloud_->points.push_back(point);
    }

    Point_Cloud(int index){
        bbox_index_ = index;
    }

    Point_Cloud(){}

    ~Point_Cloud(){}

    void reset(){
        point_num_ = 0;
        bbox_index_ = -1;
    }
};

class VOXEL_CLUSTER{
protected:
    pcl::PointCloud<pcl::PointXYZINormal> points_;
    float voxel_resolution_, grid_edge_size_xy_, grid_edge_size_z_;
    Eigen::Vector3f origin_xyz_;
    std::vector<pcl::PointXYZINormal> umap_;
    int max_range_, min_cluster_voxels_;

public:
    std::vector<int> voxel_list_;
    std::unordered_set<int> voxel_set_;
    
    void setInputCloud(const pcl::PointCloud<pcl::PointXYZINormal>& points){
        points_ = points;
    }

    void setVoxelResolution(float voxel_length, float edge_size_xy, float edge_size_z, Eigen::Vector3f& origin_xyz){
        voxel_resolution_ = voxel_length;
        grid_edge_size_xy_ = edge_size_xy;
        grid_edge_size_z_ = edge_size_z;
        origin_xyz_ = origin_xyz;
    }

    void setExtendRange(int range){
        max_range_ = range;
    }

    void setMinClusterSize(int min_size){
        min_cluster_voxels_ = min_size;
    }

    void createVoxelMap(std::vector<Point_Cloud>& map_in, std::unordered_set<int>& map_set){
        for(size_t i{0}; i < points_.size(); i++){
            pcl::PointXYZINormal point_temp;
            point_temp.x = points_.points[i].x;
            point_temp.y = points_.points[i].y;
            point_temp.z = points_.points[i].z;
            point_temp.intensity = 0;

            float position_f = std::floor((point_temp.x-origin_xyz_(0))/voxel_resolution_) * grid_edge_size_xy_ * grid_edge_size_z_
                    + std::floor((point_temp.y-origin_xyz_(1))/voxel_resolution_) * grid_edge_size_z_
                            + std::floor((point_temp.z-origin_xyz_(2))/voxel_resolution_);
            int position = static_cast<int>(position_f);
            if(position < 0 || position > grid_edge_size_xy_ * grid_edge_size_xy_ * grid_edge_size_z_)
                continue;
            if(map_in[position].point_num_ > 0){
                map_in[position].point_num_ = map_in[position].point_num_+1;
                continue;
            }
            else{
                map_set.insert(position);
                voxel_list_.push_back(position);
                voxel_set_.insert(position);
                map_in[position].cloud_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
                map_in[position].cloud_index_.reset(new std::vector<int>());
                map_in[position].point_num_ = 1;
            }
        }
    }

    void createVoxelMap(std::vector<Point_Cloud>& map_in){
        for(size_t i{0}; i < points_.size(); i++){
            pcl::PointXYZINormal point_temp;
            point_temp.x = points_.points[i].x;
            point_temp.y = points_.points[i].y;
            point_temp.z = points_.points[i].z;
            point_temp.intensity = 0;

            float position_f = std::floor((point_temp.x-origin_xyz_(0))/voxel_resolution_) * grid_edge_size_xy_ * grid_edge_size_z_
                    + std::floor((point_temp.y-origin_xyz_(1))/voxel_resolution_) * grid_edge_size_z_
                            + std::floor((point_temp.z-origin_xyz_(2))/voxel_resolution_);
            int position = static_cast<int>(position_f);
            if(position < 0 || position > grid_edge_size_xy_ * grid_edge_size_xy_ * grid_edge_size_z_)
                continue;
            if(map_in[position].point_num_ > 0){
                map_in[position].cloud_->push_back(point_temp);
                map_in[position].point_num_ = map_in[position].point_num_ + 1;
                map_in[position].cloud_index_->push_back(static_cast<int>(i));
                continue;
            }
            else{
                voxel_list_.push_back(position);
                voxel_set_.insert(position);
                map_in[position].cloud_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
                map_in[position].cloud_->reserve(5);
                map_in[position].cloud_->push_back(point_temp);
                map_in[position].point_num_ = 1;
                map_in[position].cloud_index_.reset(new std::vector<int>());
                map_in[position].cloud_index_->reserve(5);
                map_in[position].cloud_index_->push_back(static_cast<int>(i));
            }
        }
    }

    void createVoxelMap(std::unordered_map<int, Point_Cloud::Ptr>& map){
        for(size_t i{0}; i < points_.size(); i++){
            pcl::PointXYZINormal point_temp;
            point_temp.x = points_.points[i].x;
            point_temp.y = points_.points[i].y;
            point_temp.z = points_.points[i].z;

            float position_f = std::floor((point_temp.x-origin_xyz_(0))/voxel_resolution_) * grid_edge_size_xy_ * grid_edge_size_z_
                    + std::floor((point_temp.y-origin_xyz_(1))/voxel_resolution_) * grid_edge_size_z_
                            + std::floor((point_temp.z-origin_xyz_(2))/voxel_resolution_);
            int position = static_cast<int>(position_f);
            if(position < 0 || position > grid_edge_size_xy_ * grid_edge_size_xy_ * grid_edge_size_z_)
                continue;
            if(map.count(position)){
                map[position]->cloud_->push_back(point_temp);
                map[position]->point_num_ = map[position]->point_num_ + 1;
                map[position]->cloud_index_->push_back(static_cast<int>(i));
                continue;
            }
            else{
                voxel_list_.push_back(position);
                voxel_set_.insert(position);
                map[position].reset(new Point_Cloud(point_temp));
                map[position]->point_num_ = 1;
                map[position]->cloud_index_.reset(new std::vector<int>());
                map[position]->cloud_index_->reserve(5);
                map[position]->cloud_index_->push_back(static_cast<int>(i));
            }
        }
    }

    void extendVoxelNeighbor(int voxel, std::unordered_set<int>& voxel_added){
        for(int x_neighbor{-max_range_}; x_neighbor <= max_range_; x_neighbor++){
            for(int y_neighbor{-max_range_}; y_neighbor <= max_range_; y_neighbor++){
                for(int z_neighbor{-max_range_}; z_neighbor <= max_range_; z_neighbor++){
                    float distance = std::sqrt(x_neighbor*x_neighbor + y_neighbor*y_neighbor + z_neighbor*z_neighbor);
                    if(distance - static_cast<float>(max_range_) < 0.001f){
                        int voxel_neighbor = voxel + x_neighbor*grid_edge_size_xy_*grid_edge_size_z_
                                + y_neighbor*grid_edge_size_z_ + z_neighbor;
                        if(voxel_neighbor < 0 || voxel_neighbor > grid_edge_size_xy_*grid_edge_size_xy_*grid_edge_size_z_)
                            continue;
                        if(voxel_set_.count(voxel_neighbor) && !voxel_added.count(voxel_neighbor)){
                            voxel_added.insert(voxel_neighbor);
                            extendVoxelNeighbor(voxel_neighbor, voxel_added);
                        }
                    }
                }
            }
        }
    }

    void extract(std::vector<std::vector<int>>& voxel_clusters){
        for(size_t index{0}; index < voxel_list_.size(); index++){
            int voxel_cur = voxel_list_[index];
            if(voxel_set_.count(voxel_cur)){
                std::unordered_set<int> voxel_added;
                voxel_added.emplace(voxel_cur);
                extendVoxelNeighbor(voxel_cur, voxel_added);

                size_t size{0};
                std::vector<int> voxel_candidate_vec;
                for(auto iter = voxel_added.begin(); iter != voxel_added.end(); ++iter){
                    voxel_set_.erase(*iter); //移除
                    voxel_candidate_vec.push_back(*iter);
                    size++;
                }
                if(static_cast<int>(size) >= min_cluster_voxels_)
                    voxel_clusters.push_back(voxel_candidate_vec);
            }
            else continue;
        }
    }
};

} // namespace cluster_predict
} // namespace m_detector

namespace std{
template<> struct hash<m_detector::cluster_predict::VOXEL>{
    int64_t operator()(const m_detector::cluster_predict::VOXEL& s) const{
        using std::hash;
        using std::size_t;
        using namespace m_detector::cluster_predict;
        return s.z_*HASH_LENGTH*HASH_LENGTH + s.y_*HASH_LENGTH + s.x_; 
    }
};
}

#endif