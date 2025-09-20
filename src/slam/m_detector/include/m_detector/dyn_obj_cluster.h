#ifndef DYN_OBJ_CLUSTER_H
#define DYN_OBJ_CLUSTER_H

#include <unordered_map>
#include <unordered_set>
#include <execution>

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/Eigenvalues>

#include "cluster_predict/voxel_cluster.h"
#include "cluster_predict/dbscan.h"
#include "cluster_predict/ea_disk.h"

namespace m_detector{
struct PoseWithCovariance{
    union{
        float *position;   //[x, y, z]
        float *orientation; //[x, y, z, w]
    }pose;
    float *covariance;
    PoseWithCovariance(){
        this->pose.position = new float[3];
        this->pose.orientation = new float[4];
        this->covariance = new float[36];
    }
    ~PoseWithCovariance(){
        if(this->pose.position != nullptr) delete[] this->pose.position;
        if(this->pose.orientation != nullptr) delete[] this->pose.orientation;
        if(this->covariance != nullptr) delete[] this->covariance;
    }
};

struct BBox{

    std::vector<std::vector<int>> point_indices_;
    std::vector<pcl::PointCloud<pcl::PointXYZINormal>> point_clouds_;
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr points_;
    //缺少Center
    // std::vector<geometry_msgs::PoseWithCovarianceStamped> center_;
    std::vector<PoseWithCovariance> centers_;

    std::vector<pcl::PointCloud<pcl::PointXYZINormal>> ground_points_, true_ground_;

    std::vector<std::unordered_set<int>> ground_voxels_set_;
    std::vector<std::vector<int>> ground_voxels_vec_;

    std::vector<Eigen::Vector3f> OOBB_min_point_, OOBB_max_point_;
    std::vector<Eigen::Matrix3f> OOBB_R_;
    std::vector<int> umap_points_num_;

    void reset(){
        point_indices_.clear();
        point_clouds_.clear();
        centers_.clear();
        ground_points_.clear();
        true_ground_.clear();
        ground_voxels_set_.clear();
        ground_voxels_vec_.clear();
        OOBB_min_point_.clear();
        OOBB_max_point_.clear();
        OOBB_R_.clear();
    }
};

class DynObjCluster{
private:
    Eigen::Vector3f origin_xyz_, range_map_;
    float voxel_resolution_{0.5};
    int gridmap_size_, gridmap_size_edge_xy_, gridmap_size_edge_z_;
    std::vector<cluster_predict::Point_Cloud> umap_, umap_ground_, umap_inside_box_;
    Eigen::Matrix3d odom_rot_;
    Eigen::Vector3d odom_pos_;
    int cluster_extend_pixel_{2}, cluster_min_pixel_number_{4}; //聚类拓展像素
    int cur_frame_{0};
    float thrustable_thresold_{0.3f};

    int nn_points_size_{3};
    float nn_points_radius_{0.6f};
    int min_cluster_size_{8}, max_cluster_size_{25000};

public:
    void init();

    void clusterProcess(std::vector<int>& dyns_index, pcl::PointCloud<pcl::PointXYZINormal> points_event, 
            const pcl::PointCloud<pcl::PointXYZINormal>& points_raw, const Eigen::Matrix3d odom_rot, const Eigen::Vector3d odom_pos);

protected:
    void extractXYZ(const int& position, Eigen::Vector3f& pos);

    void clusterAndTrack(std::vector<int>& dyns_index, pcl::PointCloud<pcl::PointXYZINormal>::Ptr& points_event, 
            const pcl::PointCloud<pcl::PointXYZINormal>& points_raw, BBox& bbox);

    void getResultClusterVoxel(pcl::PointCloud<pcl::PointXYZINormal>::Ptr points_event_in, std::vector<cluster_predict::Point_Cloud>& umap_in,
            std::vector<std::vector<int>>& voxel_clusters_out, std::unordered_set<int>& used_map_set_out);

    bool planeEstimate(Eigen::Vector4f& result, const pcl::PointCloud<pcl::PointXYZINormal>& points);

    bool groundEstimate(const pcl::PointCloud<pcl::PointXYZINormal>& ground_pcl, const Eigen::Vector3f& world_z, Eigen::Vector3f& ground_norm, 
            Eigen::Vector4f& ground_plane, pcl::PointCloud<pcl::PointXYZINormal>& true_ground, std::unordered_set<int>& extend_pixels);

    void eventExtend(const Eigen::Matrix3f& R, bool ground_detect, BBox& bbox, std::vector<int>& dyn_index, const int& bbox_index);

    void groundRemove(const Eigen::Vector4f &ground_plane, pcl::PointCloud<pcl::PointXYZINormal> &cluster_pcl, std::vector<int> &cluster_pcl_ind, 
            std::vector<int> &dyn_index, pcl::PointCloud<pcl::PointXYZINormal> &true_ground);
    
    void isolateRemove(pcl::PointCloud<pcl::PointXYZINormal>& cluster_pcl, std::vector<int>& cluster_pcl_ind, std::vector<int>& dyn_tag);

    void pubResultClusterVoxel(std::vector<int>& dyns_index, BBox& bbox, std::vector<std::vector<int>>& voxel_clusters,
            const pcl::PointCloud<pcl::PointXYZINormal>& points_raw, std::unordered_set<int>& used_map_set);

    //未使用, 有问题
    // void getResultCluster(pcl::PointCloud<pcl::PointXYZINormal>::Ptr points_in, std::vector<pcl::PointIndices> &cluster_indices);
};

}

#endif