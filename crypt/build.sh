#! /bin/bash
set -e
python3 create_key.py
mv secret_key.h inc/

build_dir="build"

# 检查 build 目录是否存在
if [ -d "$build_dir" ]; then
  # 如果存在，则删除 build 目录
  rm -rf "$build_dir"
fi

# 创建新的 build 目录
mkdir "$build_dir"
cd build
cmake ..
make 
make install
# find ./ -name "secret_key.h" |xargs -I {} rm -f {}

echo "======================================================"
echo "all build files you can find in build/unis_crypt_dist."
echo "======================================================"
