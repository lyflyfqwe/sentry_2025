gnome-terminal -- bash -c"
ros2 launch livox_ros_driver2 msg_MID360_launch.py; \
exec bash"&

sleep 1

gnome-terminal -- bash -c"
ros2 launch auto_start mapping_ikd.launch.py; /
exec bash"&

sleep 1

gnome-terminal -- bash -c"
ros2 run motion_planning motion_planning_ros_node; /
exec"&

sleep 1

gnome-terminal -- bash -c"
ros2 launch auto_start planning.launch.py; /
exec bash"

