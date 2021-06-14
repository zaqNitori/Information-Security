from Crypto.Cipher import PKCS1_v1_5
from Crypto.PublicKey import RSA
import random
import base64

# 公鑰加密
def rsa_encode(msg, public_key):
    msg = msg.encode()
    length = len(msg)
    default_length = 117
    
    pbk = RSA.importKey(public_key)
    pb = PKCS1_v1_5.new(pbk)
    
    if length < default_length:
        cipher_text = base64.b64encode(
            pb.encrypt(msg))
        return cipher_text
    
    offset = 0
    res = []
    while length - offset > 0:
        if length - offset > default_length:
            res.append(pb.encrypt(
                msg[offset:offset+default_length]))
        else:
            res.append(pb.encrypt(msg[offset:]))
        offset += default_length
    byte_data = b''.join(res)

    return base64.b64encode(byte_data)

# 公鑰解密
def rsa_decode(msg, private_key):
    msg = base64.b64decode(msg)
    length = len(msg)
    default_length = 128
    
    pvk = RSA.importKey(private_key)
    pv = PKCS1_v1_5.new(pvk)
    
    if length < default_length:
        return b''.join(pv.decrypt(msg, b'xyz'))
    
    offset = 0
    res = []
    while length - offset > 0:
        if length - offset > default_length:
            res.append(pv.decrypt(
                msg[offset:offset+default_length], b'xyz'))
        else:
            res.append(pv.decrypt(
                msg[offset:], b'xyz'))
        offset += default_length

    return b''.join(res)

def addMAC(msg, r):
    s = "\nYour MAC is\n" + str(r)
    return (msg + s)

def encrypt(msg,pbk,pvk,mac):
    cipher = rsa_encode(msg, pbk)
    k = RSA.importKey(pvk)
    sig = pow(mac, k.d, k.n)
    sig = sig % k.n
    msg = addMAC(cipher.decode(), sig)
    return msg.encode()

def decrypt(cipher, key):
    msg = rsa_decode(cipher, key)
    return msg

def signature(mac, key):
    k = RSA.importKey(key)
    sig = pow(mac, k.e, k.n)
    sig = sig % k.n
    return sig
