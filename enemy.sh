DATA="[1.0, 0.0, 0.0, 10]"  
ros2 topic pub "/rm_enemy" std_msgs/msg/Float32MultiArray "{data: ${DATA}}"  --rate 20