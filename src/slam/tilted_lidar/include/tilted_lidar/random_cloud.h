#ifndef RANDOM_CLOUD_H
#define RANDOM_CLOUD_H

#include <pcl/point_cloud.h>  
#include <pcl/point_types.h>

namespace tilted_lidar{

void generateDenseTetrahedronCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr cloud, double resolution){
    // 正四面体的顶点  
    pcl::PointXYZ vertexA(1.0, 1.0, 1.0);  
    pcl::PointXYZ vertexB(1.0, -1.0, -1.0);  
    pcl::PointXYZ vertexC(-1.0, 1.0, -1.0);  
    pcl::PointXYZ vertexD(-1.0, -1.0, 1.0);  

    // 面的顶点索引  
    std::vector<std::vector<pcl::PointXYZ>> faces = {  
        { vertexA, vertexB, vertexC },  
        { vertexA, vertexB, vertexD },  
        { vertexA, vertexC, vertexD },  
        { vertexB, vertexC, vertexD }  
    };  

    resolution = 1/resolution;

    // 生成每个面的密集点云  
    for(const auto &face : faces){  
        pcl::PointXYZ p1 = face[0];  
        pcl::PointXYZ p2 = face[1];  
        pcl::PointXYZ p3 = face[2];  

        // 生成三角形内的点  
        for(int i{0}; i <= resolution; ++i){
            for(int j{0}; j <= resolution - i; ++j){
                float u = static_cast<float>(i) / resolution;  
                float v = static_cast<float>(j) / resolution;  
                // 计算第三个点在三角形内的补充  
                float w = 1.0 - u - v;  
                pcl::PointXYZ point;  
                point.x = u * p1.x + v * p2.x + w * p3.x;  
                point.y = u * p1.y + v * p2.y + w * p3.y;  
                point.z = u * p1.z + v * p2.z + w * p3.z;  

                cloud->points.push_back(point);  
            }  
        }
    }  

    // 设置点云的宽高  
    cloud->width = cloud->points.size();  
    cloud->height = 1; // 单行  
}  

}

#endif