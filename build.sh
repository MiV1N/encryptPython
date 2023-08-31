#!/bin/bash

set -e
CSD=$(dirname "$(readlink -f "$0")") #current script dir
PYTHON_DIR="${CSD}/Python-3.8.5"

# 编译加解密工具 ################################################
CRYPT_TOOL_PATH="${CSD}/crypt/"
sh ${CRYPT_TOOL_PATH}/build.sh
cp -r ${CRYPT_TOOL_PATH}/build/unis_crypt_dist  ${PYTHON_DIR}/

# 编译python文件 ################################################
PYTHON_INSTALL_PATH="${CSD}/dist/"
sh ${PYTHON_DIR}/build.sh ${PYTHON_INSTALL_PATH}

# 编译完成删除所有的secret_key.h
find ./ -name "secret_key.h" |xargs -I {} rm -f {}


# 在crypt/test/dist 下生成测试用的pye文件 #########################
bash ${CRYPT_TOOL_PATH}/test/test.sh  ${PYTHON_INSTALL_PATH}/bin/python3
echo "#############################################"
echo "可以执行以下命令测试是否可以成功加载pye文件"
echo "${PYTHON_INSTALL_PATH}/bin/python3  ${CRYPT_TOOL_PATH}/test/dist/test.pye"
echo "#############################################"