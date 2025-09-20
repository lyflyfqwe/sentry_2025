#include "rm_decision/polygon_region.h"
#include "debug_tools/debug_tools.h"
#include "rm_decision/rm_decision.h"

void listToVec(const std::vector<double>& list, std::vector<Eigen::Vector3d>& vec){
    int num_p = static_cast<int>(list.size())/3;
    vec.clear();
    for(int i{0}; i < num_p; i++) 
        vec.push_back(Eigen::Vector3d(list.at(3*i), list.at(3*i+1), list.at(3*i+2)));
}

int main(){
    auto list = std::vector<double>{
        11.7762, 6.99451, 1.0,
        11.7355, 2.82593, 2.0,
        10.4638, 1.96768, 1.0,
        11.232, 0.166951, 2.0,
        10.515, -1.55228, 1.0,
        4.82919, 2.74793, 2.0,
        7.4812, 6.88453, 1
    };
    std::vector<Eigen::Vector3d> points;
    listToVec(list, points);

    auto polygon = std::make_shared<rm_decision::PolygonRegion>();
    for(const auto& p : points){
        debug_tools::Debug().print(p.x(), p.y(), p.z());
        polygon->push_back(p);
    } 
    
    Eigen::Vector3d pos;
    for(int i{0}; i < 5; i++)
        if(polygon->randomPoint(pos)) debug_tools::Debug().print("random:", pos.x(), pos.y(), pos.z());
    return 0;
}