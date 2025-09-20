ros2 topic pub /rm_sentry_info std_msgs/msg/Int32MultiArray "{data: [400, 400, 110, 0, 0, 0,100]}" --rate 100
# ros2 topic pub /rm_sentry std_msgs/msg/Int32MultiArray "{data: [400]}" --once
# ros2 topic pub /nav_core_odom nav_msgs/msg/Odometry "{pose: {pose: {position: {x: 0.0, y: 0.0, z: 0.0},orientation: {x: 0.0, y: 0.0, z: 0.0, w: 1.0}}}}" --once
#ros2 topic pub /rm_enemy std_msgs/msg/Float32MultiArray "{data: [5.0,5.0,0.0,1.0]}" --rate 1

    # info_.ref.hp = info.at(0);
    # info_.ref.bollet = info.at(1);
   
    # info_.ref.hp_tower_e = info.at(3);
    # info_.ref.control = info.at(4);
    # info_.ref.target_x = info.at(5);
    # info_.ref.target_y = info.at(6);
 # info_.ref.energy = info.at(2);
waits
