#ifndef DEPTH_MAP_H
#define DEPTH_MAP_H

#include <math.h>
#include <algorithm>
#include <memory>
#include <execution>
#include <omp.h>  //omp_get_wtime()

#include <Eigen/Dense>

namespace m_detector{
const int MAP_NUM = 17;
const int HASH_PRIM = 19;
const int MAX_1D_HALF = 499; //3142() //158 31416 pi / ver_resolution
const int MAX_1D = 1257; //(2095) //(317)  // 2*pi/ hor_resolution
const int MAX_2D_N = 564393;  //1317755(1317755) //(50086) 39489912 // MAX_1D * MAX_1D_HALF
const int MAX_N = 100000;
const int DEPTH_WIDTH = (80 * 10);

enum DynObjFlag{ STATIC, CASE1, CASE2, CASE3, SELF, UNCERTAIN, INVALID };

//深度图单个像素
struct PointSoph{
    typedef std::shared_ptr<PointSoph> Ptr;

    int                 hor_ind_;  //depthmap 横坐标
    Eigen::Vector3f     vec_;   //depthmap 【0】水平角度   【1】垂直角度  【2】深度 弧度制
    int                 ver_ind_; //depthmap 纵坐标
    int                 position_;  //纵*横长 + 横
    int                 occu_times_;
    int                 is_occu_times_;
    Eigen::Vector3i     occu_index_;
    Eigen::Vector3i     is_occu_index_;
    double              time_;
    Eigen::Vector3f     occ_vec_;
    Eigen::Vector3f     is_occ_vec_;
    Eigen::Matrix3d     rot_;
    Eigen::Vector3d     transl_;
    DynObjFlag          dyn_flag_;
    Eigen::Vector3d     glob_;
    Eigen::Vector3d     local_;
    Eigen::Vector3f     cur_vec_;
    float               intensity_; //强度
    bool                is_distort_;
    Eigen::Vector3d     last_closest_;

    std::array<float, MAP_NUM> last_depth_interps_ = {};  //仅在 depthMapInterpolationStatic 出现
    std::array<Eigen::Vector3f, HASH_PRIM> last_vecs_ = {};     //仅在 sphericalProjection 出现过
    std::array<Eigen::Vector3i, HASH_PRIM> last_positions_ = {};

    PointSoph(Eigen::Vector3d& point, float& hor_resolution_max, float& ver_resolution_max){
        vec_(2)     = float(point.norm());
        vec_(0)     = atan2f(float(point(1)), float(point(0)));
        vec_(1)     = atan2f(float(point(2)), sqrt(pow(float(point(0)), 2) + std::pow(float(point(1)), 2)));
        hor_ind_    = std::floor((vec_(0) + M_PI) / hor_resolution_max);
        ver_ind_    = std::floor((vec_(1) + 0.5 * M_PI) / ver_resolution_max);
        position_   = hor_ind_ * MAX_1D_HALF + ver_ind_;
        time_       = -1;
        occu_times_ = is_occu_times_ = 0;
        occu_index_ = -1 * Eigen::Vector3i::Ones();
        is_occu_index_ = -1 * Eigen::Vector3i::Ones();
        occ_vec_.setZero();
        is_occ_vec_.setZero();
        transl_.setZero();
        glob_.setZero();
        rot_.setOnes();
        last_depth_interps_.fill(0.0);
        last_vecs_.fill(Eigen::Vector3f::Zero());
        last_positions_.fill(Eigen::Vector3i::Zero());
        is_distort_ = false;
        cur_vec_.setZero();
        local_.setZero();
        last_closest_.setZero();
    }

    PointSoph(Eigen::Vector3f s, int index_1, int index_2, int pos){
        vec_ = s;
        hor_ind_ = index_1;
        ver_ind_ = index_2;
        position_ = pos;
        occu_times_ = is_occu_times_ = 0;
        time_ = -1;
        occu_index_ = -1 * Eigen::Vector3i::Ones();
        is_occu_index_ = -1 * Eigen::Vector3i::Ones();
        occ_vec_.setZero();
        is_occ_vec_.setZero();
        transl_.setZero();
        glob_.setZero();
        rot_.setOnes();
        last_depth_interps_.fill(0.0);
        last_vecs_.fill(Eigen::Vector3f::Zero());
        last_positions_.fill(Eigen::Vector3i::Zero());
        is_distort_ = false;
        cur_vec_.setZero();
        local_.setZero();
        last_closest_.setZero();
    }

    PointSoph(const PointSoph& other){
        vec_ = other.vec_; 
        hor_ind_  = other.hor_ind_;
        ver_ind_  = other.ver_ind_;
        position_  = other.position_;
        time_ = other.time_;
        occu_times_ = other.occu_times_;
        is_occu_times_ = other.is_occu_times_;
        occu_index_ = other.occu_index_;
        is_occu_index_ = other.is_occu_index_;
        occ_vec_ = other.occ_vec_;
        is_occ_vec_ = other.is_occ_vec_;
        transl_ = other.transl_;
        glob_ = other.glob_;
        rot_ = other.rot_;
        dyn_flag_ = other.dyn_flag_;
        last_depth_interps_ = other.last_depth_interps_;
        last_vecs_ = other.last_vecs_;
        last_positions_ = other.last_positions_;
        local_ = other.local_;
        is_distort_ = other.is_distort_;
        cur_vec_ = other.cur_vec_;
        last_closest_ = other.last_closest_;
    }

    void operator=(const PointSoph& other){
        vec_ = other.vec_;
        hor_ind_  = other.hor_ind_;
        ver_ind_  = other.ver_ind_;
        position_  = other.position_;
        time_ = other.time_;
        occu_times_ = other.occu_times_;
        is_occu_times_ = other.is_occu_times_;
        occu_index_ = other.occu_index_;
        is_occu_index_ = other.is_occu_index_;
        occ_vec_ = other.occ_vec_;
        is_occ_vec_ = other.is_occ_vec_;
        transl_ = other.transl_;
        glob_ = other.glob_;
        rot_ = other.rot_;
        dyn_flag_ = other.dyn_flag_;
        last_depth_interps_ = other.last_depth_interps_;
        last_vecs_ = other.last_vecs_;
        last_positions_ = other.last_positions_;
        local_ = other.local_;
        is_distort_ = other.is_distort_;
        cur_vec_ = other.cur_vec_;
        last_closest_ = other.last_closest_;
    }

    PointSoph(){
        vec_.setZero();
        hor_ind_  = ver_ind_ = position_ = occu_times_ = is_occu_times_ = 0;
        time_       = -1;
        occu_index_ = -1 * Eigen::Vector3i::Ones();
        is_occu_index_ = -1 * Eigen::Vector3i::Ones();
        occ_vec_.setZero();
        is_occ_vec_.setZero();
        transl_.setZero();
        glob_.setZero();
        rot_.setOnes();
        last_depth_interps_.fill(0.0);
        last_vecs_.fill(Eigen::Vector3f::Zero());
        last_positions_.fill(Eigen::Vector3i::Zero());
        is_distort_ = false;
        cur_vec_.setZero();
        local_.setZero();
        last_closest_.setZero();
    }

    ~PointSoph(){}

    //只更新 vec_ hor_ind_ ver_ind_ position_
    void getVec(Eigen::Vector3d & point, float & hor_resolution_max, float & ver_resolution_max){
        vec_(2)    = float(point.norm());
        vec_(0)    = atan2f(float(point(1)), float(point(0)));
        vec_(1)    = atan2f(float(point(2)), std::sqrt(std::pow(float(point(0)), 2) + std::pow(float(point(1)), 2)));
        hor_ind_   = std::floor((vec_(0) + M_PI) / hor_resolution_max);
        ver_ind_   = std::floor((vec_(1) + 0.5 * M_PI) / ver_resolution_max);
        position_  = hor_ind_ * MAX_1D_HALF + ver_ind_;
    }

    void reset(){
        occu_times_ = is_occu_times_ = 0;
        occu_index_ = -1 * Eigen::Vector3i::Ones();
        is_occu_index_ = -1 * Eigen::Vector3i::Ones();
        occ_vec_.setZero();
        is_occ_vec_.setZero();
        last_closest_.setZero();
        last_depth_interps_.fill(0.0);
        last_vecs_.fill(Eigen::Vector3f::Zero());
        last_positions_.fill(Eigen::Vector3i::Zero());
        is_distort_ = false;
    }
};

// typedef std::vector<std::vector<PointSoph*>> DepthMap2D;

struct DepthMap{
    typedef std::shared_ptr<DepthMap> Ptr;

    std::vector<std::vector<PointSoph*>> depth_map_;
    double           time_;
    int              map_index_;
    Eigen::Matrix3d  project_R_;
    Eigen::Vector3d  project_T_; 
    std::vector<PointSoph::Ptr> point_sopth_pointer_;
    int              point_sopth_pointer_count_ = 0;
    float*           min_depth_static_ = nullptr;
    float*           max_depth_static_ = nullptr;
    float*           min_depth_all_ = nullptr;
    float*           max_depth_all_ = nullptr; //每一列的最大深度
    int*             max_depth_index_all_ = nullptr; //每一列的最大深度的索引
    int*             min_depth_index_all_ = nullptr;
    std::vector<int> index_vector_;

    DepthMap(){
        depth_map_.assign(MAX_2D_N, std::vector<PointSoph*>());
        time_ = 0.0;
        project_R_.setIdentity(3, 3);
        project_T_.setZero(3, 1);

        min_depth_static_ = new float[MAX_2D_N];
        min_depth_all_ = new float[MAX_2D_N];
        max_depth_all_ = new float[MAX_2D_N];
        max_depth_static_ = new float[MAX_2D_N];
        std::fill_n(min_depth_static_, MAX_2D_N, 0.0);
        std::fill_n(min_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_static_, MAX_2D_N, 0.0);
        max_depth_index_all_ = new int[MAX_2D_N];
        min_depth_index_all_ = new int[MAX_2D_N];
        std::fill_n(min_depth_index_all_, MAX_2D_N, -1);
        std::fill_n(max_depth_index_all_, MAX_2D_N, -1);
        map_index_ = -1;
        index_vector_.assign(MAX_2D_N, 0);
        for(int i{0}; i < MAX_2D_N; i++) index_vector_[i] = i;
    }

    DepthMap(Eigen::Matrix3d rot, Eigen::Vector3d trans, double cur_time, int frame){
        depth_map_.assign(MAX_2D_N, std::vector<PointSoph*>());
        time_ = cur_time;
        project_R_ = rot;
        project_T_ = trans;
        min_depth_static_ = new float[MAX_2D_N];
        min_depth_all_ = new float[MAX_2D_N];
        max_depth_all_ = new float[MAX_2D_N];
        max_depth_static_ = new float[MAX_2D_N];
        std::fill_n(min_depth_static_, MAX_2D_N, 0.0);
        std::fill_n(min_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_static_, MAX_2D_N, 0.0);
        max_depth_index_all_ = new int[MAX_2D_N];
        min_depth_index_all_ = new int[MAX_2D_N];
        std::fill_n(min_depth_index_all_, MAX_2D_N, -1);
        std::fill_n(max_depth_index_all_, MAX_2D_N, -1);
        map_index_ = frame;
        index_vector_.assign(MAX_2D_N, 0);
        for(int i{0}; i < MAX_2D_N; i++) index_vector_[i] = i;
    }

    DepthMap(const DepthMap& other){
        depth_map_ = other.depth_map_;       
        time_ = other.time_;
        project_R_ = other.project_R_;
        project_T_ = other.project_T_;
        point_sopth_pointer_ = other.point_sopth_pointer_;
        min_depth_static_ = new float[MAX_2D_N];
        min_depth_all_ = new float[MAX_2D_N];
        max_depth_all_ = new float[MAX_2D_N];
        max_depth_static_ = new float[MAX_2D_N];   
        std::fill_n(min_depth_static_, MAX_2D_N, 0.0);
        std::fill_n(min_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_static_, MAX_2D_N, 0.0);
        max_depth_index_all_ = new int[MAX_2D_N];       
        min_depth_index_all_ = new int[MAX_2D_N];
        map_index_ = other.map_index_;      
        for(int i = 0; i < MAX_2D_N; i++)
        {
            min_depth_static_[i] = other.min_depth_static_[i];
            max_depth_static_[i] = other.max_depth_static_[i];
            min_depth_all_[i] = other.min_depth_all_[i];
            max_depth_all_[i] = other.max_depth_all_[i];
            max_depth_index_all_[i] = other.max_depth_index_all_[i];
            min_depth_index_all_[i] = other.min_depth_index_all_[i];
        }
        index_vector_.assign(MAX_2D_N, 0);
        for(int i{0}; i < MAX_2D_N; i++) index_vector_[i] = i;
    }

    ~DepthMap(){
        if(min_depth_static_ != nullptr) delete[] min_depth_static_;
        if(min_depth_all_  != nullptr) delete[] min_depth_all_;
        if(max_depth_all_  != nullptr) delete[] max_depth_all_;
        if(max_depth_static_  != nullptr) delete[] max_depth_static_;
        if(max_depth_index_all_  != nullptr) delete[] max_depth_index_all_;
        if(min_depth_index_all_  != nullptr) delete[] min_depth_index_all_;
    }

    void reSet(Eigen::Matrix3d rot, Eigen::Vector3d transl, double cur_time, int frame){   
        time_ = cur_time;
        project_R_ = rot;
        project_T_ = transl;
        map_index_ = frame;
        // double t = omp_get_wtime();
        std::for_each(std::execution::par, index_vector_.begin(), index_vector_.end(), [&](const int &i){
            depth_map_[i].clear();
        });
        std::fill_n(min_depth_static_, MAX_2D_N, 0.0);
        std::fill_n(min_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_all_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_static_, MAX_2D_N, 0.0);
        std::fill_n(max_depth_index_all_, MAX_2D_N, -1);
        std::fill_n(min_depth_index_all_, MAX_2D_N, -1);
    }
};

}

#endif