#!/bin/bash

PYTHON_DIR="Python-3.8.5"

# 编译加解密工具
cd crypt;sh build.sh;cd -
cp -r crypt/build/unis_crypt_dist ${PYTHON_DIR}/

# 编译python文件
cd ${PYTHON_DIR};sh build.sh;cd -

