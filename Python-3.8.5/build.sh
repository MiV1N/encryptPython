#!/bin/bash
set -e

CSD=$(dirname "$(readlink -f "$0")") #current script dir

# 设置LD_LIBRARY_PATH环境变量
export LD_LIBRARY_PATH="${CSD}/unis_crypt_dist/lib"
# 打印设置后的LD_LIBRARY_PATH环境变量值
echo "LD_LIBRARY_PATH设置为: $LD_LIBRARY_PATH"

# 编译
PYTHON_INSTALL_PATH=$1

cd ${CSD} 
rm  configure   #删除一下configure，避免configure.ac的改动没有更新到configure里面
autoconf 
./configure --prefix="${PYTHON_INSTALL_PATH}"

make regen-importlib  #重新生成 _frozen的importlib (操作码的替换+import解密流程)，这里需要有可以执行的python命令
make -j8 
make install
# # 检查退出状态码
# if [ $? -ne 0 ]; then
#   echo "上一个命令执行失败，请检查错误信息。"
#   exit 1
# fi

# 拷贝共享库到发布目录
cp -r ${CSD}/unis_crypt_dist/lib  ${PYTHON_INSTALL_PATH}
cp -r ${CSD}/unis_crypt_dist/bin  ${PYTHON_INSTALL_PATH}


echo "===================================================="
echo "编译结果在${PYTHON_INSTALL_PATH}路径下"
echo "注意:python执行文件需要依赖动态库${PYTHON_INSTALL_PATH}/lib/libunis_encrypt.so"
echo "===================================================="
