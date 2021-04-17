#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "encrypt.h"
#include "decrypt.h"
#include "expandKey.h"

#define PLAINTEXT 28523 //0110 1111 0110 1011
#define CIPHERTEXT 1 //0000 0111 0011 1000
#define KEY 42811 //1010 0111 0011 1011
#define RCON1 128 //1000 0000
#define RCON2 48 //0011 0000

vector<int> vkey;
int nowKey[4];

int keySBOX[4][4] =
{
    {9,4,10,11},
    {13,1,8,5},
    {6,2,0,3},
    {12,14,15,7}
};

int keyINVSBOX[4][4] =
{
    {10,5,9,11},
    {1,7,8,15},
    {6,0,2,3},
    {12,4,13,14}
};

int mixCol[2][2] =
{
    { 1 , 4 },
    { 4 , 1 }
};

int mixColINV[2][2] =
{
    { 9 , 2 },
    { 2 , 9 }
};

int mixColTable[3][16] =
{
    { 0,2,4,6,8,10,12,14,3,1,7,5,11,9,15,13 },//2
    { 0,4,8,12,3,7,11,15,6,2,14,10,5,1,13,9 },//4
    { 0,9,1,8,2,11,3,10,4,13,5,12,6,15,7,14}  //9
};

void updateNowKey(int n)
{
    nowKey[0] = n>>12;
    nowKey[1] = (n^(nowKey[0]<<12))>>8;
    nowKey[2] = (n^(nowKey[0]<<12)^(nowKey[1]<<8))>>4;
    nowKey[3] = (n^(nowKey[0]<<12)^(nowKey[1]<<8)^(nowKey[2]<<4));
}

int addRoundKey(int text,int k1,int k2)
{
    int tmp = (k1<<8) | k2;
    return (text ^ tmp);
}

int shiftRow(int n)
{
    int tmp;
    updateNowKey(n);
    tmp = nowKey[1];
    nowKey[1] = nowKey[3];
    nowKey[3] = tmp;
    return (nowKey[0]<<12 | nowKey[1]<<8 | nowKey[2]<<4 | nowKey[3]);
}

int mixColumns(int n)
{
    int tmp[4];
    updateNowKey(n);
    tmp[0] = nowKey[0] ^ mixColTable[1][nowKey[1]];
    tmp[1] = mixColTable[1][nowKey[0]] ^ nowKey[1];
    tmp[2] = nowKey[2] ^ mixColTable[1][nowKey[3]];
    tmp[3] = mixColTable[1][nowKey[2]] ^ nowKey[3];
    for(int i=0;i<4;i++)
        nowKey[i] = tmp[i];
    return (nowKey[0]<<12 | nowKey[1]<<8 | nowKey[2]<<4 | nowKey[3]);
}

int mixColumnsINV(int n)
{
    int tmp[4];
    updateNowKey(n);
    tmp[0] = mixColTable[2][nowKey[0]] ^ mixColTable[0][nowKey[1]];
    tmp[1] = mixColTable[0][nowKey[0]] ^ mixColTable[2][nowKey[1]];
    tmp[2] = mixColTable[2][nowKey[2]] ^ mixColTable[0][nowKey[3]];
    tmp[3] = mixColTable[0][nowKey[2]] ^ mixColTable[2][nowKey[3]];
    for(int i=0;i<4;i++)
        nowKey[i] = tmp[i];
    return (nowKey[0]<<12 | nowKey[1]<<8 | nowKey[2]<<4 | nowKey[3]);
}

int getLeft(int n,int shift) { return n>>shift; }

int getRight(int n,int shift)
{
    int tmp = n>>shift;
    return ((tmp<<shift) ^ n);
}

int subNib(int n)
{
    int ll = n>>6;
    int lr = (n>>4) ^ (ll<<2);
    int rl = (n ^ (ll<<6) ^ (lr<<4))>>2;
    int rr = (n ^ (ll<<6) ^ (lr<<4) ^ (rl<<2));
    return (keySBOX[ll][lr]<<4 | keySBOX[rl][rr]);
}

int subNibINV(int n)
{
    int ll = n>>6;
    int lr = (n>>4) ^ (ll<<2);
    int rl = (n ^ (ll<<6) ^ (lr<<4))>>2;
    int rr = (n ^ (ll<<6) ^ (lr<<4) ^ (rl<<2));
    return (keyINVSBOX[ll][lr]<<4 | keyINVSBOX[rl][rr]);
}

void expandKey()
{
    cout<<"Start Key\n";
    int tmp,rotW,subW;
    vkey.push_back(getLeft(KEY,8));
    vkey.push_back(getRight(KEY,8));

    rotW = getLeft(vkey[1],4) | (getRight(vkey[1],4)<<4);
    subW = subNib(rotW);
    tmp = subW ^ RCON1;
    vkey.push_back((tmp ^ vkey[0]));
    vkey.push_back((vkey[2] ^ vkey[1]));

    rotW = tmp = 0;
    rotW = getLeft(vkey[3],4) | (getRight(vkey[3],4)<<4);
    subW = subNib(rotW);
    tmp = subW ^ RCON2;
    vkey.push_back((tmp ^ vkey[2]));
    vkey.push_back((vkey[4] ^ vkey[3]));
    cout<<"End Key\n";
    return;
}

int encrypt(int plaintext)
{
    cout<<"Encrypt Start\n";
    int ciphertext;

    ciphertext = addRoundKey(plaintext,vkey[0],vkey[1]);

    ciphertext = (subNib(getLeft(ciphertext,8))<<8) | subNib(getRight(ciphertext,8));
    ciphertext = shiftRow(ciphertext);
    ciphertext = mixColumns(ciphertext);
    ciphertext = addRoundKey(ciphertext,vkey[2],vkey[3]);

    ciphertext = (subNib(getLeft(ciphertext,8))<<8) | subNib(getRight(ciphertext,8));
    ciphertext = shiftRow(ciphertext);

    ciphertext = addRoundKey(ciphertext,vkey[4],vkey[5]);
    cout<<"Encrypt End\n";
    return ciphertext;
}

int decrypt(int ciphertext)
{
    cout<<"Decrypt Start\n";
    int plaintext;
    plaintext = addRoundKey(ciphertext,vkey[4],vkey[5]);

    plaintext = shiftRow(plaintext);
    plaintext = (subNibINV(getLeft(plaintext,8))<<8) | subNibINV(getRight(plaintext,8));
    plaintext = addRoundKey(plaintext,vkey[2],vkey[3]);
    plaintext = mixColumnsINV(plaintext);

    plaintext = shiftRow(plaintext);
    plaintext = (subNibINV(getLeft(plaintext,8))<<8) | subNibINV(getRight(plaintext,8));
    plaintext = addRoundKey(plaintext,vkey[0],vkey[1]);

    cout<<"Decrypt End\n";
    return plaintext;
}

int main(void)
{
    int plaintext = PLAINTEXT;
    int ciphertext;

    expandKey();

    cout<<"Original plaintext: "<<plaintext<<endl;
    ciphertext = encrypt(plaintext);
    cout<<"After encryption: "<<ciphertext<<endl;
    plaintext = decrypt(ciphertext);
    cout<<"After decryption: "<<plaintext<<endl;

    return 0;
}
