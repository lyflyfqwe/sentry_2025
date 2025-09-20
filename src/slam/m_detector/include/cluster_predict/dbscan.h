#ifndef DBSCAN_H
#define DBSCAN_H

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/passthrough.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/progressive_morphological_filter.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/common/common.h>

#include <pcl/search/impl/search.hpp>
#include <pcl/features/normal_3d.h>

inline bool comparedPointIndices(const pcl::PointIndices& a, const pcl::PointIndices& b){
    return (static_cast<int>(a.indices.size()) < static_cast<int>(b.indices.size()));
}

namespace m_detector{
namespace cluster_predict{
enum dbcsan_state{
    UN_PROCESSED, PROCESSING, PROCESSED
};

template<typename PointT> class DBSCANSimpleCluster{
protected:

    typename pcl::PointCloud<PointT>::Ptr input_cloud_;
    typename pcl::search::KdTree<PointT>::Ptr search_method_;
    double eps_{0.0};
    size_t min_pts_{0};
    size_t min_pts_per_cluster_{1};
    size_t max_pts_per_cluster_{std::numeric_limits<size_t>::max()};

    virtual int radiusSearch(int index, double radius, std::vector<int>& k_indices,
            std::vector<float>& k_sqr_distance) const{
        k_indices.clear();
        k_sqr_distance.clear();
        k_indices.push_back(index);
        k_sqr_distance.push_back(0);
        int size = input_cloud_->points.size();
        double radius_sqr = radius*radius;
        for(int i{0}; i < size; i++){
            if(i == index) continue;
            double distance_x = input_cloud_->points[i].x - input_cloud_->points[index].x;
            double distance_y = input_cloud_->points[i].y - input_cloud_->points[index].y;
            double distance_z = input_cloud_->points[i].z - input_cloud_->points[index].z;
            double distance_square = distance_x * distance_x + distance_y * distance_y + distance_z * distance_z;
            if(distance_square <= radius_sqr){
                k_indices.push_back(i);
                k_sqr_distance.push_back(distance_square);
            }
        }
        return k_indices.size();
    }

public:
    virtual void setInputCloud(typename pcl::PointCloud<PointT>::Ptr point_cloud){
        input_cloud_ = point_cloud;
    }

    void setSearchMethod(typename pcl::search::KdTree<PointT>::Ptr method){
        search_method_ = method;
    }

    void extract(std::vector<pcl::PointIndices>& cluster_indices){
        std::vector<int> nn_indices;
        std::vector<float> nn_distances;
        std::vector<bool> is_noise(input_cloud_->points.size(), false);
        std::vector<int> types(input_cloud_->points.size(), dbcsan_state::UN_PROCESSED);
        for(int i{0}; i < input_cloud_->points.size(); i++){
            if(types[i] == dbcsan_state::PROCESSED)
                continue;
            int nn_size = radiusSearch(i, eps_, nn_indices, nn_distances);
            if(nn_size < min_pts_){
                is_noise[i] = true;
                continue;
            }

            std::vector<int> seed_queue;
            seed_queue.push_back(i);
            types[i] = dbcsan_state::PROCESSED;
            
            for(int j{0}; j < nn_size; j++){
                if(nn_indices[j] != i){
                    seed_queue.push_back(nn_indices[j]);
                    types[nn_indices[j]] = dbcsan_state::PROCESSING;
                }
            }
            int sq_index{1};
            while(sq_index < seed_queue.size()){
                int cloud_index = seed_queue.at(sq_index);
                if(is_noise[cloud_index] || types[cloud_index] == dbcsan_state::PROCESSED){
                    types[cloud_index] = dbcsan_state::PROCESSED;
                    sq_index++;
                    continue;
                }
                nn_size = radiusSearch(cloud_index, eps_, nn_indices, nn_distances);
                if(nn_size >= min_pts_){
                    for(int j{0}; j < nn_size; j++){
                        if(types[nn_indices[j]] == dbcsan_state::UN_PROCESSED){
                            seed_queue.push_back(nn_indices[j]);
                            types[nn_indices[j]] = dbcsan_state::PROCESSING;
                        }
                    }
                }

                types[cloud_index] = dbcsan_state::PROCESSED;
                sq_index++;
            }
            if(seed_queue.size() >= min_pts_per_cluster_ && seed_queue.size() <= max_pts_per_cluster_){
                pcl::PointIndices r;
                r.indices.resize(seed_queue.size());
                for(int j{0}; j < seed_queue.size(); ++j)
                    r.indices[j] = seed_queue[j];
                std::sort(r.indices.begin(), r.indices.end());
                r.indices.erase(
                    std::unique(r.indices.begin(), r.indices.end(), r.indices.end())
                );  //去重
                r.header = input_cloud_->header;
                cluster_indices.push_back(r);
            }
        }
        std::sort(cluster_indices.cbegin(), cluster_indices.cend(), comparedPointIndices);
    }

    void setClusterTolerance(double tolerance){
        eps_ = tolerance;
    }

    void setMinClusterSize(int min_size){
        min_pts_per_cluster_ = min_size;
    }

    void setMaxClusterSize(int max_size){
        max_pts_per_cluster_ = max_size;
    }

    void setCorePointsMinPts(int core_min_pts){
        min_pts_ = core_min_pts;
    }
    
}; // class DBSCANSimpleCluster

template<typename PointT> 
class DBSCANKdtreeCluster: public DBSCANSimpleCluster<PointT>{
protected:
    virtual int 
    radiusSearch(int index, double radius, std::vector<int>& k_indices, 
            std::vector<float>& k_sqr_distances) const{
        return this->search_method_->radiusSearch(index, radius, k_indices, k_sqr_distances);
    }
};
} // namespace cluster_predict
} // namespace m_detector

#endif