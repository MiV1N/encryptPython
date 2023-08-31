
def decrypt(content):
    import ctypes
    # 定义 AES_ENCODE_DATA 结构体
    class AES_ENCODE_DATA(ctypes.Structure):
        _fields_ = [  #顺序不能错
            ('textBuffer', ctypes.POINTER(ctypes.c_char)),
            ('enBuffer', ctypes.POINTER(ctypes.c_char)),
            ('bufferSize', ctypes.c_long),
        ]

    # 加载 C 动态库
    lib = ctypes.CDLL('libunis_encrypt.so')

    # 创建 AES_ENCODE_DATA 实例
    aes_encode_data = AES_ENCODE_DATA()
    aes_encode_data.enBuffer = ctypes.cast(content, ctypes.POINTER(ctypes.c_char))
    aes_encode_data.bufferSize = len(content)

    # 声明函数原型
    unis_aes_decrypt = lib.unis_aes_decrypt
    unis_aes_decrypt.argtypes = [ctypes.POINTER(AES_ENCODE_DATA)]
    unis_aes_decrypt.restype = ctypes.c_int

    # 声明函数原型
    free_endecbuf = lib.free_endecbuf
    free_endecbuf.argtypes = [ctypes.POINTER(ctypes.c_char)]
    free_endecbuf.restype = ctypes.c_int

    # 调用 C 函数
    ret = unis_aes_decrypt(ctypes.byref(aes_encode_data))

    if ret == 0 and aes_encode_data.textBuffer is not None:
        # print(unis_aes_decrypt.textBuffer.decode())
        decodeBuffer = ctypes.string_at(aes_encode_data.textBuffer, aes_encode_data.bufferSize)
        if 0 != free_endecbuf(aes_encode_data.textBuffer):
            print(f"error occur when free decode buffer")
        return decodeBuffer
    else:
        print(f"decrypt error,error code :{ret}")
        return None


path = "/home/ubuntu/test/python3.8.5/crypt/test/dist/test.pye"

if path.endswith(".pye"):
    with open(path,"rb") as f:
        enc_data = f.read()

    decrypted_data = decrypt(enc_data[20:])
    print(f"{decrypted_data[-10:]}")

