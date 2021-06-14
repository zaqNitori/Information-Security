from Crypto.PublicKey import RSA

key = RSA.generate(1024)
pvk = key.exportKey()
pbk = key.publickey().exportKey()
  
i = 12

k = RSA.importKey(pvk)
print('e = ',k.e)
print('n = ',k.n)
print('d = ',k.d)

sig = pow(12,k.d,k.n)
sig = sig % k.n
print(sig)

k = RSA.importKey(pbk)
print('e = ',k.e)
print('n = ',k.n)

ans = pow(sig,k.e,k.n)
ans = ans % k.n
print(ans)
