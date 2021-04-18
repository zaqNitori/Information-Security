資料夾s-aes存放S-AES的實現code

S-aes.cpp 是本次作業
ExpandKey: 擴增原有的Key

encrypt: 加密function
addRoundKey: 跟算出來的Key做Xor
shiftRow: 移位運算
subNib: 利用S-BOX做位元替換
mixColumns: 矩陣運算查表後後互相xor

decrypt: 解密function
subNibINV: 用INV-SBOX把被替換的文字給換回來
mixColumnsINV: 用加密矩陣的反矩陣做運算，然後查表後互相xor

資料夾s-deswithCounterMode
s-des.cpp 簡易DES加解密演算法with counter mode

hw1的s-des有部分寫錯，雖然能夠正確將密文解回原文，但是加密過程出錯。
問題出在getColumn的部分寫錯，hw2有修改了。
這次不需要使用decrypt function所以就刪掉了

splitPlaintext	將資料量很大的原文，切成許多個chunk並儲存
doPermutation	進行位置變換
keyShift	做左移動作
getRow		取得第4跟第1個bit並且轉換成row
getColumn	取得第2跟第3個bit並且轉換成column
funcF		演算法的critical function
funcFK		跟funcF做Xor

encrypt		加密演算法

Line 6		要被加密的明文
Line 7		題目給的密文
Line 8		運算用的Key
Line 9		counter起始值

t107590026@ntut.org.tw