# encryptPython
python解释器加密


# 实际应用
## 需要删除的文件
1. 操作码替换后的文件

   ![图片](https://github.com/MiV1N/encryptPython/assets/111258029/00410040-15e0-435b-840a-f130df79e635)

3. 加密so的头文件
   编译后的include目录下的unis_encrypt.h，定义了so的加解密接口

4. importlib文件夹
   定义了import pye文件的解密方法，做编译后的lib/python3.8/importlib

