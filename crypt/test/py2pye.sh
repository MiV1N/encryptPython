#!/bin/bash

############################################################################
#                用途
# 生成测试用的test.pye文件，生成路径在dist目录下
# 使用方式 sh test.sh <编译的python解释器install目录下的/bin/python3的路径>
# 比如： sh test.sh ~/test/python3.8.5/Python-3.8.5/dist/bin/python3 
##########################################################################

set -e

CSD=$(dirname "$(readlink -f "$0")") #current script dir

# 检查参数是否为空
if [ -z "$1" ]; then
    echo "请提供Python解释器的路径作为参数"
    exit 1
fi

# 获取Python解释器路径
PATHON_PATH="$1"


# 提取给定路径的父目录
PYTHON_PARENT_DIR="$(dirname "$PATHON_PATH")"
# 修改上层目录为"lib"
PYTHON_LIB_PATH="${PYTHON_PARENT_DIR%/*}/lib"
# 设置LD_LIBRARY_PATH环境变量
export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${PYTHON_LIB_PATH}"
# 打印设置后的LD_LIBRARY_PATH环境变量值
echo "LD_LIBRARY_PATH设置为: $LD_LIBRARY_PATH"


# 定义函数递归处理目录
compile_files() {
    local source_dir="$1"
    local dist_dir="$2"

    # 遍历源目录中的文件
    for file in "$source_dir"/*; do
        if [ -d "$file" ]; then
            # 如果是目录，则递归处理子目录
            local subdir="$(basename "$file")"

            compile_files "$source_dir/$subdir" "$dist_dir"
        elif [ -f "$file" ] && [ "${file##*.}" = "py" ]; then
            # 如果是.py文件，则编译为.pyc文件并保存到dist目录中
            local filename="$(basename "$file")"
            local dist_subdir="${dist_dir}/${file%/*}"
            # echo "filename:${filename}"
            # echo "dist_subdir:${dist_subdir}"
            mkdir -p "$dist_subdir"
            "$PATHON_PATH" -m compileall -b -f -q "$file" -d "$dist_subdir"
            mv "${file}c" "$dist_subdir/${filename}c"
        fi
    done
}



cd $CSD
# 源目录和dist目录的路径，请根据实际情况修改
source_directory="./"
dist_directory="./dist"

rm -rf $dist_directory
# 调用函数处理源目录
compile_files "$source_directory" "$dist_directory"

#转换为pye
${PYTHON_PARENT_DIR}/scan_encrypt $dist_directory
find $dist_directory -name "*.pyc" -delete