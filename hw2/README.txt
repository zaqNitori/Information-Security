資料夾s-aes存放S-AES的實現code

ExpandKey: 擴增原有的Key

encrypt: 加密function
addRoundKey: 跟算出來的Key做Xor
shiftRow: 移位運算
subNib: 利用S-BOX做位元替換
mixColumns: 矩陣運算查表後後互相xor

decrypt: 解密function
subNibINV: 用INV-SBOX把被替換的文字給換回來
mixColumnsINV: 用加密矩陣的反矩陣做運算，然後查表後互相xor

