#include <iostream>
#include <string>

using namespace std;

#define PLAINTEXT 190
#define CIPHERTEXT 70 /*"01000110"*/
#define KEY 642 /*"1010000010"*/

int keyP10[] = {3,5,2,7,4,10,1,9,8,6};
int keyP8[] = {6,3,7,4,8,5,10,9};
int keyP4[] = {2,4,3,1};
int ep[] = {4,1,2,3,2,3,4,1};
int initialPer[] = {2,6,3,1,4,8,5,7};
int reinitialPer[] = {4,1,3,5,7,2,8,6};

int sbox0[4][4] =
{
    {1,0,3,2},
    {3,2,1,0},
    {0,2,1,3},
    {3,1,3,2}
};

int sbox1[4][4] =
{
    {0,1,2,3},
    {2,0,1,3},
    {3,0,1,0},
    {2,1,0,3}
};

int doPermutation(int n,int per[],int len,int outlen)
{
    int tp[len]={0},i=len-1,ans=0;
    while(n>0)
    {
        tp[i--] = n%2;
        n = n>>1;
    }
    for(i=0;i<outlen;i++)
    {
        ans = ans<<1;
        ans += tp[per[i]-1];
    }
    return ans;
}

int keyShift(int k,int shift,int len)
{
    int tp;
    int tplen = len/2;
    int l = k>>tplen;
    int r = k&((1<<tplen)-1);
    shift %= tplen;
    l = l<<shift;
    r = r<<shift;
    for(int i=tplen;i<tplen+shift;i++)
    {
        tp = 1<<i;
        if(l&tp)
        {
            l-=tp;
            l+=tp>>tplen;
        }
        if(r&tp)
        {
            r-=tp;
            r+=tp>>tplen;
        }
    }
    return (l<<tplen|r);
}

int getRow(int n)
{
    return (((n>>3)<<1) | (n&1));
}

int getColumn(int n)
{
    return (((n&(1<<2))<<1) | (n&(1<<1)));
}

int funcF(int rp,int key)
{
    int tp,l,r;
    tp = doPermutation(rp,ep,4,8);
    tp = tp^key;
    l = tp>>4;
    r = tp&((1<<4)-1);
    tp = sbox0[getRow(l)][getColumn(l)]<<2;
    tp = tp | sbox1[getRow(r)][getColumn(r)];
    tp = doPermutation(tp,keyP4,4,4);
    return tp;
}

int funcFK(int lp,int rp,int key)
{
    return (lp^funcF(rp,key));
}

int encrypt(int plainText,int key)
{
    int cipherText;
    int lp,rp,tp;
    int key1,key2;

    cout<<"Encrypt Start\n";
    cipherText = doPermutation(plainText,initialPer,8,8);
    lp = cipherText>>4;
    rp = cipherText&((1<<4)-1);
    key = doPermutation(key,keyP10,10,10);

    key = keyShift(key,1,10);
    key1 = doPermutation(key,keyP8,10,8);
    tp = funcFK(lp,rp,key1);

    lp = rp;
    rp = tp;

    key = keyShift(key,2,10);
    key2 = doPermutation(key,keyP8,10,8);
    tp = funcFK(lp,rp,key2);
    cipherText = (tp<<4) | rp;
    cipherText = doPermutation(cipherText,reinitialPer,8,8);

    cout<<"Encrypt End\n";
    return cipherText;
}

int decrypt(int cipherText,int key)
{
    int plainText;
    int lp,rp,tp;
    int key1,key2;

    cout<<"Decrypt Start\n";
    plainText = doPermutation(cipherText,initialPer,8,8);

    key = doPermutation(key,keyP10,10,10);
    key = keyShift(key,1,10);
    key1 = doPermutation(key,keyP8,10,8);
    key = keyShift(key,2,10);
    key2 = doPermutation(key,keyP8,10,8);

    lp = plainText>>4;
    rp = plainText&((1<<4)-1);
    tp = funcFK(lp,rp,key2);

    lp = rp;
    rp = tp;

    tp = funcFK(lp,rp,key1);
    plainText = (tp<<4) | rp;
    plainText = doPermutation(plainText,reinitialPer,8,8);

    cout<<"Decrypt End\n";
    return plainText;
}

int main(void)
{
    int cipher,plain,key;
    plain = PLAINTEXT;
    key = KEY;
    cout<<"PlainText going to encrypt: "<<PLAINTEXT<<endl;
    cipher = encrypt(plain,key);
    cout<<"cipher Text after encrypt: "<<cipher<<endl;
    plain = decrypt(cipher,key);
    cout<<"plain Text after decrypt: "<<plain<<endl;
    return 0;
}
