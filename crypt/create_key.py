import hashlib 
import sys
from os.path import abspath, dirname, join, pardir
import os

PY2 = sys.version_info.major < 3


PASSPHRASE_ENV = 'PYCONCRETE_PASSPHRASE'
ROOT_DIR = abspath(dirname(__file__))
SECRET_HEADER_PATH = join(ROOT_DIR, 'secret_key.h')
secret_key_header_created = False
print(SECRET_HEADER_PATH)

def hash_key(key):
    if PY2:
        factor = sum([ord(s) for s in key])
    else:
        factor = sum([s for s in key])
    factor %= 128
    if factor < 16:
        factor += 16

    m = hashlib.md5()
    m.update(key)
    k = m.digest()

    return k, factor


def create_secret_key_header(key, factor):
    # reference from - http://stackoverflow.com/questions/1356896/how-to-hide-a-string-in-binary-code
    # encrypt the secret key in binary code
    # avoid to easy read from HEX view
    global secret_key_header_created
    if secret_key_header_created:
        return

    key_val_lst = []
    for i, k in enumerate(key):
        n = ord(k) if PY2 else k
        key_val_lst.append("(0x%X ^ (0x%X - %d))" % (n, factor, i))
    key_val_code = ", ".join(key_val_lst)

    code = """
/* Auto-generated by create_key.py */
#define SECRET_NUM 0x%X
#define SECRET_KEY_LEN %d
static const unsigned char* GetSecretKey()
{
    unsigned int i = 0;
    static unsigned char key[] = {%s, 0/* terminal char */};
    static int is_encrypt = 1/*true*/;
    if( is_encrypt )
    {
        for(i = 0 ; i < SECRET_KEY_LEN ; ++i)
        {
            key[i] = key[i] ^ (SECRET_NUM - i);
        }
        is_encrypt = 0/*false*/;
    }
    return key;
}
    """ % (
        factor,
        len(key),
        key_val_code,
    )


    with open(SECRET_HEADER_PATH, 'w') as f:
        f.write(code)

    secret_key_header_created = True


def remove_secret_key_header():
    if os.path.exists(SECRET_HEADER_PATH):
        os.remove(SECRET_HEADER_PATH)



def pre_process():
    if secret_key_header_created:
        return


    passphrase = os.getenv(PASSPHRASE_ENV)

    if not passphrase:
        passphrase = input(
            "please input the passphrase \nfor encrypt your python script (enter for default) : \n"
        )
        if len(passphrase) == 0:
            raise Exception("empty passphrase is not allow")
        else:
            passphrase2 = input("please input again to confirm\n")
            if passphrase != passphrase2:
                raise Exception("Passphrase is different")

    k, f = hash_key(passphrase.encode('utf8'))
    create_secret_key_header(k, f)


if __name__ == '__main__':
    pre_process()