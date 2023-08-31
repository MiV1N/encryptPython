#!/bin/bash

# 设置LD_LIBRARY_PATH环境变量
export LD_LIBRARY_PATH="${PWD}/unis_crypt_dist/lib"
# 打印设置后的LD_LIBRARY_PATH环境变量值
echo "LD_LIBRARY_PATH设置为: $LD_LIBRARY_PATH"

# 编译
DIST_PATH="${PWD}/dist"

autoconf
./configure --prefix=${DIST_PATH} 

make -j8 & make install  #编译python解释器
PATH_BACK=${PATH}
export PATH=${DIST_PATH}/bin/python3:${PATH}  #将python解释器添加到路径中，由于我们可能有操作码的替换，需要放在前面

make regen-importlib  #重新生成 _frozen的importlib
make -j8 & make install

export PATH=${PATH_BACK} #恢复环境变量

# 检查退出状态码
if [ $? -ne 0 ]; then
  echo "上一个命令执行失败，请检查错误信息。"
  exit 1
fi

# 拷贝共享库到发布目录
cp -r unis_crypt_dist/*  ${DIST_PATH}


echo "===================================================="
echo "编译结果在${DIST_PATH}路径下"
echo "注意:python执行文件需要依赖动态库lib/libunis_encrypt.so"
echo "===================================================="