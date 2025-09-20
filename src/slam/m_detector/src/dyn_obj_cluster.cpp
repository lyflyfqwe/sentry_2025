#include "m_detector/dyn_obj_cluster.h"

namespace m_detector{
void DynObjCluster::init(){
    origin_xyz_ << -100., -100., -20.;
    range_map_ << 200., 200., 40.;
    gridmap_size_edge_xy_ = static_cast<int>(std::ceil(range_map_(0) / voxel_resolution_));
    gridmap_size_edge_z_ = static_cast<int>(std::ceil(range_map_(2) / voxel_resolution_));
    gridmap_size_ = gridmap_size_edge_xy_ * gridmap_size_edge_xy_ * gridmap_size_edge_z_;
    umap_.reserve(gridmap_size_);
    umap_.resize(gridmap_size_);
    umap_ground_.reserve(gridmap_size_);
    umap_ground_.resize(gridmap_size_);
    umap_inside_box_.reserve(gridmap_size_);
    umap_inside_box_.resize(gridmap_size_);
}

void DynObjCluster::clusterProcess(std::vector<int>& dyns_index, pcl::PointCloud<pcl::PointXYZINormal> points_event, 
        const pcl::PointCloud<pcl::PointXYZINormal>& points_raw, const Eigen::Matrix3d odom_rot, const Eigen::Vector3d odom_pos){
    odom_pos_ = odom_pos;
    odom_rot_ = odom_rot;
    pcl::PointCloud<pcl::PointXYZINormal>::Ptr points_event_shared(new pcl::PointCloud<pcl::PointXYZINormal>());
    points_event_shared = points_event.makeShared();
    BBox bbox;
    clusterAndTrack(dyns_index, points_event_shared, points_raw, bbox);
    cur_frame_++;
}

void DynObjCluster::extractXYZ(const int& position, Eigen::Vector3f& pos){
    int left = position;
    pos(0) = origin_xyz_(0) + static_cast<float>(std::floor(position / (gridmap_size_edge_xy_ * gridmap_size_edge_z_))) * voxel_resolution_;
    auto temp = 
        static_cast<float>(std::floor(position / (gridmap_size_edge_xy_ * gridmap_size_edge_z_))) * (gridmap_size_edge_xy_ * gridmap_size_edge_z_);
    left = static_cast<int>(left - temp);
    pos(1) = origin_xyz_(1) + static_cast<float>(left / gridmap_size_edge_z_) * voxel_resolution_;
    temp = static_cast<float>(std::floor(left / gridmap_size_edge_z_) * gridmap_size_edge_z_);
    left = static_cast<int>(left - temp);
    pos(2) = origin_xyz_(2) + left * voxel_resolution_;
}

void DynObjCluster::clusterAndTrack(std::vector<int>& dyns_index, pcl::PointCloud<pcl::PointXYZINormal>::Ptr& points_event, 
        const pcl::PointCloud<pcl::PointXYZINormal>& points_raw, BBox& bbox){
    std::vector<std::vector<int>> voxel_clusters;
    std::unordered_set<int> used_map_set;
    getResultClusterVoxel(points_event, umap_, voxel_clusters, used_map_set);
    pubResultClusterVoxel(dyns_index, bbox, voxel_clusters, points_raw, used_map_set);
}

void DynObjCluster::getResultClusterVoxel(pcl::PointCloud<pcl::PointXYZINormal>::Ptr points_event_in, std::vector<cluster_predict::Point_Cloud>& umap_in,
        std::vector<std::vector<int>>& voxel_clusters_out, std::unordered_set<int>& used_map_set_out){
    cluster_predict::VOXEL_CLUSTER cluster;
    cluster.setInputCloud(*points_event_in);
    cluster.setVoxelResolution(voxel_resolution_, gridmap_size_edge_xy_, gridmap_size_edge_z_, origin_xyz_);
    cluster.setExtendRange(cluster_extend_pixel_);
    cluster.setMinClusterSize(cluster_min_pixel_number_);
    cluster.createVoxelMap(umap_in, used_map_set_out);
    cluster.extract(voxel_clusters_out);
}

bool DynObjCluster::planeEstimate(Eigen::Vector4f& result, const pcl::PointCloud<pcl::PointXYZINormal>& points){
    const float threshold{0.1};
    int points_size = static_cast<int>(points.size());
    Eigen::Matrix<float, Eigen::Dynamic, 3> A;
    Eigen::Matrix<float, Eigen::Dynamic, 1> b;
    A.resize(points_size, 3);
    b.resize(points_size, 1);
    b.setOnes();
    b *= -1.0f;
    for (int j{0}; j < points_size; j++){
        A(j, 0) = points[j].x;
        A(j, 1) = points[j].y;
        A(j, 2) = points[j].z;
    }
    Eigen::Vector3f normvec = A.colPivHouseholderQr().solve(b);
    float norm = normvec.norm(), average_dis{0.0};
    for(int j{0}; j <  points_size; j++){
        float tmp = fabs(normvec.dot(A.row(j)) + 1.0);
        average_dis += tmp;
        if(tmp > threshold) return false;
    }
    average_dis = std::max(0.01f, average_dis / points_size / norm);
    result(0) = normvec(0);
    result(1) = normvec(1);
    result(2) = normvec(2);
    result(3) = average_dis;
    return true;
}

bool DynObjCluster::groundEstimate(const pcl::PointCloud<pcl::PointXYZINormal>& ground_pcl, const Eigen::Vector3f& world_z, Eigen::Vector3f& ground_norm, 
        Eigen::Vector4f& ground_plane, pcl::PointCloud<pcl::PointXYZINormal>& true_ground, std::unordered_set<int>& extend_pixels){
    if(ground_pcl.size() > 0) return false;
    size_t BNUM = std::max(4, (int)ground_pcl.size() / 100);
    const float threshold {0.10f};
    const float max_angle_from_body {30.0f / 57.3f};
    pcl::PointCloud<pcl::PointXYZINormal> split_pcl, max_points;
    size_t max_count{0};
    Eigen::Vector3f max_normvec(0, 0, 0);
    for(size_t i{0}; i < ground_pcl.size(); i++){
        split_pcl.push_back(ground_pcl[i]);
        if(split_pcl.size() == BNUM){
            Eigen::Vector4f plane;
            if(planeEstimate(plane, split_pcl) && plane[3] > threshold){
                Eigen::Vector3f normvec = plane.head(3).normalized();
                if(normvec.cross(world_z).norm() < std::sin(max_angle_from_body)){ 
                    size_t count{0};
                    pcl::PointCloud<pcl::PointXYZINormal> tmp_points;
                    for(size_t j{0}; j < ground_pcl.size(); j++){
                        Eigen::Vector3f point;
                        point[0] = ground_pcl[j].x;
                        point[1] = ground_pcl[j].y;
                        point[2] = ground_pcl[j].z;
                        float dis = fabs(point.dot(plane.head(3)) + 1.0f) / plane.head(3).norm();
                        if(dis < threshold){
                            tmp_points.push_back(ground_pcl[j]);
                            count++;
                        }
                    }
                    if(count > max_count){
                        max_count = count;
                        max_normvec = normvec;
                        ground_plane = plane;
                        max_points = tmp_points;
                        if (max_count > 0.6f * ground_pcl.size()) break;
                    }
                }   
            }
            split_pcl.clear();
        }
    }
    if(ground_pcl.size() > 0 && (max_count > 0.2f * ground_pcl.size() || max_count > 500)){
        Eigen::Vector4f plane;
        if(planeEstimate(plane, split_pcl) && plane[3] > threshold){
            Eigen::Vector3f normvec = plane.head(3).normalized();
            if(normvec.cross(world_z).norm() < std::sin(max_angle_from_body)){
                max_normvec = normvec;
                ground_plane = plane;
            }
        }
        for(size_t j{0}; j < ground_pcl.size(); j++){
            Eigen::Vector3f point;
            point[0] = ground_pcl[j].x;
            point[1] = ground_pcl[j].y;
            point[2] = ground_pcl[j].z;
            float dis = fabs(point.dot(ground_plane.head(3)) + 1.0f) / ground_plane.head(3).norm();
            if(dis < threshold){
                true_ground.push_back(ground_pcl[j]);
                int voxel = std::floor((ground_pcl[j].x - origin_xyz_(0)) / voxel_resolution_) * gridmap_size_edge_xy_ * gridmap_size_edge_z_
                     + std::floor((ground_pcl[j].y - origin_xyz_(1)) / voxel_resolution_) * gridmap_size_edge_z_
                        + std::floor((ground_pcl[j].z - origin_xyz_(2)) / voxel_resolution_);
                extend_pixels.erase(voxel);
            }
        }
        if (max_normvec[2] < 0) max_normvec *= -1;
        ground_norm = max_normvec;
    }
    return std::abs(ground_norm.norm() - 1.0f) < 0.1f;
}

void DynObjCluster::eventExtend(const Eigen::Matrix3f& R, bool ground_detect, BBox& bbox, std::vector<int>& dyn_index, const int& bbox_index){
    for(size_t i{0}; i < bbox.ground_voxels_vec_[bbox_index].size(); i++){
        int voxel_cur = bbox.ground_voxels_vec_[bbox_index][i];
        if(bbox.ground_voxels_set_[bbox_index].count(voxel_cur) && umap_ground_[voxel_cur].point_num_ > 2){
            int x_ind[6] = {1, -1, 0, 0, 0, 0};
            int y_ind[6] = {0, 0, 1, -1, 0, 0};
            int z_ind[6] = {0, 0, 0, 0, 1, -1};
            for (int ind{0}; ind < 6; ind++){
                int voxel_neighbor = voxel_cur + x_ind[ind] * gridmap_size_edge_xy_ * gridmap_size_edge_z_
                     + y_ind[ind] * gridmap_size_edge_z_ + z_ind[ind];
                if(voxel_neighbor < 0 || voxel_neighbor > gridmap_size_) continue;
                if ((umap_inside_box_[voxel_neighbor].bbox_index_ > -1 && umap_inside_box_[voxel_neighbor].point_num_ > 0 
                        && umap_inside_box_[voxel_neighbor].bbox_index_ == bbox_index) || (umap_[voxel_neighbor].point_num_ > 0 
                        && umap_[voxel_neighbor].cloud_->size() > 0 && umap_[voxel_neighbor].bbox_index_ == bbox_index)){
                    pcl::PointCloud<pcl::PointXYZINormal> points = *(umap_ground_[voxel_cur].cloud_);
                    Eigen::Vector4f plane;
                    if(ground_detect){
                        if(planeEstimate(plane, points) && std::abs(R.col(0).dot(plane.head(3).normalized())) < 0.8f){
                            umap_[voxel_cur].bbox_index_ = bbox_index;
                            umap_[voxel_cur].cloud_ = umap_ground_[voxel_cur].cloud_;
                            bbox.point_clouds_[bbox_index] += points;
                            for (size_t j{0}; j < umap_ground_[voxel_cur].cloud_index_->size(); j++){
                                dyn_index[umap_ground_[voxel_cur].cloud_index_->at(j)] = 1;
                                bbox.point_indices_[bbox_index].push_back(umap_ground_[voxel_cur].cloud_index_->at(j));
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
}

void DynObjCluster::groundRemove(const Eigen::Vector4f &ground_plane, pcl::PointCloud<pcl::PointXYZINormal> &cluster_pcl, std::vector<int> &cluster_pcl_index, 
        std::vector<int> &dyn_index, pcl::PointCloud<pcl::PointXYZINormal> &true_ground){
    const float threshold = {0.10f};
    pcl::PointCloud<pcl::PointXYZINormal> new_clustet_pcl;
    std::vector<int> new_cluster_pcl_index;
    for(size_t i{0}; i < cluster_pcl.size(); i++){
        Eigen::Vector3f point;
        point[0] = cluster_pcl[i].x;
        point[1] = cluster_pcl[i].y;
        point[2] = cluster_pcl[i].z;
        float dis = std::fabs(point.dot(ground_plane.head(3)) + 1.0f) / ground_plane.head(3).norm();
        if(dis > threshold){
            new_clustet_pcl.push_back(cluster_pcl[i]);
            new_cluster_pcl_index.push_back(cluster_pcl_index[i]);
        }
        else{
            dyn_index[new_cluster_pcl_index[i]] = 0;
            true_ground.push_back(cluster_pcl[i]);
        }
    }
    cluster_pcl = new_clustet_pcl;
    cluster_pcl_index = new_cluster_pcl_index;
}

void DynObjCluster::isolateRemove(pcl::PointCloud<pcl::PointXYZINormal>& cluster_pcl, std::vector<int>& cluster_pcl_ind, std::vector<int>& dyn_tag){
    if(cluster_pcl.size() < 2) return;
    pcl::PointCloud<pcl::PointXYZINormal> new_cluster_pcl;
    std::vector<int> new_cluster_pcl_ind;
    cluster_predict::VOXEL_CLUSTER cluster;
    std::unordered_map<int, cluster_predict::Point_Cloud::Ptr> umap_cluster;
    std::vector<std::vector<int>> voxel_cluster;
    cluster.setInputCloud(cluster_pcl);
    cluster.setVoxelResolution(voxel_resolution_, gridmap_size_edge_xy_, gridmap_size_edge_z_, origin_xyz_);
    cluster.setExtendRange(cluster_extend_pixel_);
    cluster.setMinClusterSize(cluster_min_pixel_number_);
    cluster.createVoxelMap(umap_cluster);
    cluster.extract(voxel_cluster);
    size_t max_cluster_ind{0}, max_voxel_num{0};
    for(size_t i{0}; i < voxel_cluster.size(); i++){
        if (voxel_cluster[i].size() > max_voxel_num){
            max_cluster_ind = i;
            max_voxel_num = voxel_cluster[i].size();
        }
    }
    std::unordered_set<int> dyn_index;
    for (size_t i{0}; i < max_voxel_num; i++){
        int voxel = voxel_cluster[max_cluster_ind][i];
        for (size_t j{0}; j < umap_cluster[voxel]->cloud_->size(); j++){
            new_cluster_pcl.push_back(umap_cluster[voxel]->cloud_->points[j]);
            new_cluster_pcl_ind.push_back(cluster_pcl_ind[umap_cluster[voxel]->cloud_index_->at(j)]);
            dyn_index.insert(cluster_pcl_ind[umap_cluster[voxel]->cloud_index_->at(j)]);
        }
    }
    for(size_t i{0}; i < cluster_pcl_ind.size(); i++){
        if(!dyn_index.count(cluster_pcl_ind[i])) dyn_tag[cluster_pcl_ind[i]] = 0;
    }
    std::unordered_map<int, cluster_predict::Point_Cloud::Ptr>().swap(umap_cluster);
    cluster_pcl = new_cluster_pcl;
    cluster_pcl_ind = new_cluster_pcl_ind;
}

void DynObjCluster::pubResultClusterVoxel(std::vector<int>& dyns_index, BBox& bbox, std::vector<std::vector<int>>& voxel_clusters,
        const pcl::PointCloud<pcl::PointXYZINormal>& points_raw, std::unordered_set<int>& used_map_set){
    pcl::PointCloud<pcl::PointXYZINormal> cluster_points, true_ground;
    cluster_points.reserve(points_raw.size());
    true_ground.reserve(points_raw.size());
    Eigen::Vector3f world_z = odom_rot_.cast<float>().col(2);

    int j{0};
    for(auto it = voxel_clusters.begin(); it != voxel_clusters.end(); it++, j++){
        Eigen::Vector3f xyz;
        extractXYZ(*(it->begin()), xyz);
        float x_min = xyz(0), x_max = xyz(0);
        float y_min = xyz(1), y_max = xyz(1);
        float z_min = xyz(2), z_max = xyz(2);
        int n{0};
        for(auto pit = it->begin(); pit != it->end(); ++pit){
            int voxel = *pit;
            umap_[voxel].bbox_index_ = j;
            n = n + umap_[voxel].point_num_;
            extractXYZ(voxel, xyz);
            if(xyz(0) < x_min) x_min = xyz(0);
            if(xyz(1) < y_min) y_min = xyz(1);
            if(xyz(2) < z_min) z_min = xyz(2);
            if((xyz(0) + voxel_resolution_) > x_max) x_max = xyz(0) + voxel_resolution_;
            if((xyz(1) + voxel_resolution_) > y_max) y_max = xyz(1) + voxel_resolution_;
            if((xyz(2) + voxel_resolution_) > z_max) z_max = xyz(2) + voxel_resolution_;
        }
        float x_size = x_max - x_min;
        float y_size = y_max - y_min;
        float z_size = z_max - z_min;
        if(cluster_min_pixel_number_ == 1 || (x_size > voxel_resolution_+0.001f && y_size > voxel_resolution_+0.001f)
                || (x_size > voxel_resolution_+0.001f && z_size > voxel_resolution_+0.001f) 
                || (y_size > voxel_resolution_+0.001f && z_size > voxel_resolution_+0.001f)){
            pcl::PointCloud<pcl::PointXYZINormal> pcl_temp;
            bbox.point_clouds_.push_back(pcl_temp);
            std::vector<int> indices_temp;
            bbox.point_indices_.push_back(indices_temp);
            PoseWithCovariance pose_with_cov;
            pose_with_cov.pose.position[0] = (x_max + x_min) / 2;
            pose_with_cov.pose.position[1] = (y_max + y_min) / 2;
            pose_with_cov.pose.position[2] = (z_max + z_min) / 2;
            pose_with_cov.pose.orientation[0] = 0;
            pose_with_cov.pose.orientation[1] = 0;
            pose_with_cov.pose.orientation[2] = 0;
            pose_with_cov.pose.orientation[3] = 1;
            pose_with_cov.covariance[0 * 6 + 0] = x_size / 2;
            pose_with_cov.covariance[1 * 6 + 1] = y_size / 2;
            pose_with_cov.covariance[2 * 6 + 2] = z_size / 2;
            pose_with_cov.covariance[3 * 6 + 3] = x_size;
            pose_with_cov.covariance[4 * 6 + 4] = y_size;
            pose_with_cov.covariance[5 * 6 + 5] = z_size;
            pose_with_cov.covariance[2 * 6 + 3] = x_max;
            pose_with_cov.covariance[3 * 6 + 4] = y_max;
            pose_with_cov.covariance[4 * 6 + 5] = z_max;
            pose_with_cov.covariance[3 * 6 + 2] = x_min;
            pose_with_cov.covariance[4 * 6 + 3] = y_min;
            pose_with_cov.covariance[5 * 6 + 4] = z_min;
            bbox.centers_.push_back(pose_with_cov);
            pcl::PointCloud<pcl::PointXYZINormal> pcl_temp_2;
            bbox.ground_points_.push_back(pcl_temp_2);
            bbox.true_ground_.push_back(pcl_temp_2);
            std::unordered_set<int> set_temp;
            bbox.ground_voxels_set_.push_back(set_temp);
            std::vector<int> vec_temp;
            bbox.ground_voxels_vec_.push_back(vec_temp);
            bbox.umap_points_num_.push_back(n);
        }
        else{
            j--;
            for(auto v = it->begin(); v != it->end(); ++v)
                umap_[*v].reset();
        }
    }

    std::vector<int> bbox_index_vec(bbox.centers_.size());
    for(size_t i{0}; i < bbox.centers_.size(); i++)
        bbox_index_vec[i] = i;
    std::vector<std::unordered_set<int>> used_map_set_vec(bbox.centers_.size());
    std::for_each(std::execution::par, bbox_index_vec.begin(), bbox_index_vec.end(), [&](const int& bbox_iter){
        pcl::PointXYZINormal point_center, point_min, point_max;

        point_center.x = bbox.centers_[bbox_iter].pose.position[0];
        point_center.y = bbox.centers_[bbox_iter].pose.position[1];
        point_center.z = bbox.centers_[bbox_iter].pose.position[2];

        point_min.x = bbox.centers_[bbox_iter].covariance[3*6+2];
        point_min.y = bbox.centers_[bbox_iter].covariance[4*6+3];
        point_min.z = bbox.centers_[bbox_iter].covariance[5*6+4];

        point_max.x = bbox.centers_[bbox_iter].covariance[2*6+3];
        point_max.y = bbox.centers_[bbox_iter].covariance[3*6+4];
        point_max.z = bbox.centers_[bbox_iter].covariance[4*6+5];

        float x_size = bbox.centers_[bbox_iter].covariance[3*6+3];
        float y_size = bbox.centers_[bbox_iter].covariance[4*6+4];
        float z_size = bbox.centers_[bbox_iter].covariance[5*6+5]; 

        int n_x = std::max(1.0f, 1.0f * x_size) / voxel_resolution_;
        int n_y = std::max(1.0f, 1.0f * y_size) / voxel_resolution_;
        int n_z = std::max(1.0f, 1.0f * z_size) / voxel_resolution_;

        int voxel_center = 
            std::floor((point_center.x - origin_xyz_(0)) / voxel_resolution_) * gridmap_size_edge_xy_ * gridmap_size_edge_z_
            + std::floor((point_center.y - origin_xyz_(1)) / voxel_resolution_) * gridmap_size_edge_z_
            + std::floor((point_center.z - origin_xyz_(2)) / voxel_resolution_);
        int ii{0};
        Eigen::Vector3f xyz;
        for(int i{0}; i <= 2*n_x+1; i++){
            ii += (i%2? 1 : -1) * i;
            int jj{0};
            for(int j{0}; j <= 2*n_y+1; j++){
                jj += (j%2? 1 : -1) * j;
                int kk{0};
                for(int k{0}; k <= 2*n_z+1; k++){
                    kk += (k%2? 1 : -1) * k;
                    int voxel = voxel_center + ii * gridmap_size_edge_xy_ * gridmap_size_edge_z_ + jj * gridmap_size_edge_z_ + kk;
                    if(voxel < 0 || voxel > gridmap_size_) continue;
                    extractXYZ(voxel, xyz);
                    Eigen::Vector3f voxel_loc(
                        xyz(0) + 0.5f * voxel_resolution_, 
                        xyz(1) + 0.5f * voxel_resolution_, 
                        xyz(2) + 0.5f * voxel_resolution_
                    );
                    if(umap_[voxel].point_num_ == 0 && 
                        !((voxel_loc(0) > point_min.x && voxel_loc(0) < point_max.x) && 
                            (voxel_loc(1) > point_min.y && voxel_loc(1) < point_max.y) && 
                            (voxel_loc(2) > point_min.z && voxel_loc(2) < point_max.z))){
                        umap_ground_[voxel].bbox_index_ = bbox_iter;
                        used_map_set_vec[bbox_iter].insert(voxel);
                        bbox.ground_voxels_set_[bbox_iter].emplace(voxel);
                        bbox.ground_voxels_vec_[bbox_iter].push_back(voxel);
                    }
                    else if(umap_[voxel].point_num_ == 0 && 
                            (voxel_loc(0) > point_min.x && voxel_loc(0) < point_max.x) && 
                            (voxel_loc(1) > point_min.y && voxel_loc(1) < point_max.y) && 
                            (voxel_loc(2) > point_min.z && voxel_loc(2) < point_max.z)){
                        umap_inside_box_[voxel].bbox_index_ = bbox_iter;
                        used_map_set_vec[bbox_iter].insert(voxel);
                    }
                }
            }
        }
    });

    for(size_t bbox_iter{0}; bbox_iter < bbox.centers_.size(); bbox_iter++)
        used_map_set.merge(used_map_set_vec[bbox_iter]);
    
    for(size_t ite{0}; ite < points_raw.size(); ite++){
        if(dyns_index[ite] == -1) continue;
        int voxel = std::floor((points_raw[ite].x - origin_xyz_(0)) / voxel_resolution_) * gridmap_size_edge_xy_ * gridmap_size_edge_z_ 
            + std::floor((points_raw[ite].y - origin_xyz_(1)) / voxel_resolution_) * gridmap_size_edge_z_ 
            + std::floor((points_raw[ite].z - origin_xyz_(2)) / voxel_resolution_);
        if(voxel < 0 || voxel > gridmap_size_) continue;
        if(umap_ground_[voxel].bbox_index_ > -1){
            bbox.ground_points_[umap_ground_[voxel].bbox_index_].push_back(points_raw[ite]);
            if(umap_ground_[voxel].point_num_ == 0){
                umap_ground_[voxel].cloud_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
                umap_ground_[voxel].cloud_->reserve(5);
                umap_ground_[voxel].cloud_index_.reset(new std::vector<int>());
                umap_ground_[voxel].cloud_index_->reserve(5);
                umap_ground_[voxel].cloud_->push_back(points_raw[ite]);
                umap_ground_[voxel].cloud_index_->push_back(ite);
            }
            else{
                umap_ground_[voxel].cloud_->push_back(points_raw[ite]);
                umap_ground_[voxel].cloud_index_->push_back(ite);
            }
            umap_ground_[voxel].point_num_++;
            dyns_index[ite] = 0;
        }
        else if(umap_[voxel].point_num_ > 0 && umap_[voxel].bbox_index_ > -1){
            auto tmp = points_raw[ite];
            tmp.curvature = ite;
            bbox.point_clouds_[umap_[voxel].bbox_index_].push_back(tmp);
            bbox.point_indices_[umap_[voxel].bbox_index_].push_back(ite);
            umap_[voxel].cloud_->push_back(tmp);
            dyns_index[ite] = 1;
        }
        else if(umap_inside_box_[voxel].bbox_index_ > -1){
            auto tmp = points_raw[ite];
            tmp.curvature = ite;
            bbox.point_clouds_[umap_inside_box_[voxel].bbox_index_].push_back(tmp);
            bbox.point_indices_[umap_inside_box_[voxel].bbox_index_].push_back(ite);
            if(umap_inside_box_[voxel].point_num_ == 0){
                umap_inside_box_[voxel].cloud_.reset(new pcl::PointCloud<pcl::PointXYZINormal>());
                umap_inside_box_[voxel].cloud_->reserve(5);
                umap_inside_box_[voxel].cloud_->push_back(tmp);
            }
            else{
                umap_inside_box_[voxel].cloud_->push_back(tmp);
            }
            umap_inside_box_[voxel].point_num_++;
            dyns_index[ite] = 1;
        }
        else dyns_index[ite] = 0;
    }

    std::for_each(std::execution::par, bbox_index_vec.begin(), bbox_index_vec.end(), [&](const int& bbox_iter){
        //只用于可视化
        // float x_size = bbox.centers_[bbox_iter].covariance[3*6+3];
        // float y_size = bbox.centers_[bbox_iter].covariance[4*6+4];
        // float z_size = bbox.centers_[bbox_iter].covariance[5*6+5];
        // float x_min = bbox.centers_[bbox_iter].covariance[3*6+2];
        // float y_min = bbox.centers_[bbox_iter].covariance[4*6+3];
        // float z_min = bbox.centers_[bbox_iter].covariance[5*6+4];
        // float x_max = bbox.centers_[bbox_iter].covariance[2*6+3];
        // float y_max = bbox.centers_[bbox_iter].covariance[3*6+4];
        // float z_max = bbox.centers_[bbox_iter].covariance[4*6+5];

        Eigen::Vector3f ground_norm(0.0, 0.0, 0.0);
        Eigen::Vector4f ground_plane;
        bool is_ground_detect = groundEstimate(bbox.ground_points_[bbox_iter], world_z, ground_norm, ground_plane, 
                bbox.true_ground_[bbox_iter], bbox.ground_voxels_set_[bbox_iter]);
        Eigen::Matrix3f R;
        R.col(0) = ground_norm;
        if(is_ground_detect){
            eventExtend(R, is_ground_detect, bbox, dyns_index, bbox_iter);
            groundRemove(ground_plane, bbox.point_clouds_[bbox_iter], bbox.point_indices_[bbox_iter], dyns_index, bbox.true_ground_[bbox_iter]);
        }
        isolateRemove(bbox.point_clouds_[bbox_iter], bbox.point_indices_[bbox_iter], dyns_index);
        if(static_cast<float>(bbox.umap_points_num_[bbox_iter]) / static_cast<float>(bbox.point_clouds_[bbox_iter].size()) < thrustable_thresold_){
            for(size_t i{0}; i < bbox.point_indices_[bbox_iter].size(); i++)
                dyns_index[bbox.point_indices_[bbox_iter][i]] = 0;
        }
        else{
            cluster_points += bbox.point_clouds_[bbox_iter];
            true_ground += bbox.true_ground_[bbox_iter];
        }
    });
    
    //for reset
    for(auto ite = used_map_set.begin(); ite != used_map_set.end(); ite++){
        umap_[*ite].reset();
        umap_ground_[*ite].reset();
        umap_inside_box_[*ite].reset();
    }
}

// void DynObjCluster::getResultCluster(pcl::PointCloud<pcl::PointXYZINormal>::Ptr points_in, std::vector<pcl::PointIndices>& cluster_indices){
//     if(points_in->size() < 2) return;
//     pcl::search::KdTree<pcl::PointXYZINormal>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZINormal>());
//     tree->setInputCloud(points_in);
//     cluster_predict::DBSCANKdtreeCluster<pcl::PointXYZINormal> ec;
//     ec.setCorePointsMinPts(nn_points_size_);
//     ec.setClusterTolerance(nn_points_radius_);
//     ec.setMinClusterSize(min_cluster_size_);
//     ec.setMaxClusterSize(max_cluster_size_);
//     ec.setSearchMethod(tree);
//     ec.setInputCloud(points_in);
//     ec.extract(cluster_indices);
// }
}
