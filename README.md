# 创建编译脚本
- cd 到你的工作目录 与README.md同路径
```
touch c_build.bash
echo "python3 src/auto_start/script/colcon_build.py" > c_build.bash
sudo chmod 777 -R ./c_build.bash
```
# 编译命令 
```
./c_build.bash
```
# 更改编译的包
- 修改 "src/auto_start/script/colcon_build.py" 文件的pkgs列表