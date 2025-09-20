from launch import LaunchDescription  
from launch_ros.actions import Node  
from launch_ros.substitutions import FindPackageShare
import os

def generate_launch_description(): 
    pkg_share = FindPackageShare('tilted_lidar').find('tilted_lidar')  
    rviz_config_file = os.path.join(pkg_share, 'config', 'tilted_lidar.rviz')
    
    return LaunchDescription([  
        Node(  
            package='tilted_lidar',  
            executable='tilted_lidar_node',  
            name='tilted_lidar_node',  
            output='screen',  
        ),  
        Node(  
            package='tilted_lidar', 
            executable='publish_body_cloud_node',  
            name='publish_body_cloud_node',  
            output='screen',  
        ), 
        Node(  
            package='rviz2',  
            executable='rviz2',  
            name='rviz',  
            output='screen',  
            arguments=['-d', rviz_config_file],  
        ),  
    ])