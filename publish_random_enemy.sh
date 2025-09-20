TOPIC="/rm_enemy"
MSG_TYPE="std_msgs/msg/Float32MultiArray"
ENEMY_ID=1.0
MIN_X=-3.0
MAX_X=3.0
MIN_Y=-3.0
MAX_Y=3.0
MIN_Z=0.0
MAX_Z=0.0
FREQUENCY=1 #每 5 秒发布一次 ww
DELAY=$(awk "BEGIN { print 1.0 / ${FREQUENCY} }")
echo "敌人位置范围: X [${MIN_X}, ${MAX_X}], Y [${MIN_Y}, ${MAX_Y}], Z [${MIN_Z}, ${MAX_Z}]"
while true; do
    RAND_0_1_X=$(awk "BEGIN { printf \"%.10f\", ${RANDOM}/32767.0 }")
    RAND_X=$(echo ${RAND_0_1_X} | awk "{ print ${MIN_X} + \$1 * (${MAX_X} - ${MIN_X}) }")
    RAND_0_1_Y=$(awk "BEGIN { printf \"%.10f\", ${RANDOM}/32767.0 }")
    RAND_Y=$(echo ${RAND_0_1_Y} | awk "{ print ${MIN_Y} + \$1 * (${MAX_Y} - ${MIN_Y}) }")
    RAND_0_1_Z=$(awk "BEGIN { printf \"%.10f\", ${RANDOM}/32767.0 }")
    RAND_Z=$(echo ${RAND_0_1_Z} | awk "{ print ${MIN_Z} + \$1 * (${MAX_Z} - ${MIN_Z}) }")
    MSG_DATA="{data: [${RAND_X}, ${RAND_Y}, ${RAND_Z}, ${ENEMY_ID}]}"
    ros2 topic pub ${TOPIC} ${MSG_TYPE} "${MSG_DATA}" --once
    sleep ${DELAY}
done