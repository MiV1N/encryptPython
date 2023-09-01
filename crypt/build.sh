#! /bin/bash
set -e

CSD=$(dirname "$(readlink -f "$0")") #current script dir

cd ${CSD} && python3 ${CSD}/create_key.py #生成secret_key.h
mv ${CSD}/secret_key.h ${CSD}/include/

BUILD_DIR="${CSD}/build"

# 检查 build 目录是否存在
if [ -d "$BUILD_DIR" ]; then
  # 如果存在，则删除 build 目录
  rm -rf "$BUILD_DIR"
fi

# 创建新的 build 目录
mkdir -p "$BUILD_DIR"
cd $BUILD_DIR
cmake .. && make -j && make install
cp ${CSD}/test/py2pye.sh ${BUILD_DIR}/unis_crypt_dist/bin/

echo "======================================================"
echo "all build files you can find in $BUILD_DIR/unis_crypt_dist."
echo "======================================================"
