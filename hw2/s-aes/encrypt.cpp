#include <iostream>
using namespace std;

#include "encrypt.h"

int nowKey[4];

int keySBOX[4][4] =
{
    {9,4,10,11},
    {13,1,8,5},
    {6,2,0,3},
    {12,14,15,7}
};

int mixCol[2][2] =
{
    { 1 , 4 },
    { 4 , 1 }
};

int mixColTable[3][16] =
{
    { 0,2,4,6,8,10,12,14,3,1,7,5,11,9,15,13 },//2
    { 0,4,8,12,3,7,11,15,6,2,14,10,5,1,13,9 },//4
    { 0,9,1,8,2,11,3,10,4,13,5,12,6,15,7,14}  //9
};

Encryption::Encryption()
{

}

void Encryption::updateNowKey(int n)
{
    nowKey[0] = n>>12;
    nowKey[1] = (n^(nowKey[0]<<12))>>8;
    nowKey[2] = (n^(nowKey[0]<<12)^(nowKey[1]<<8))>>4;
    nowKey[3] = (n^(nowKey[0]<<12)^(nowKey[1]<<8)^(nowKey[2]<<4));
}

int Encryption::addRoundKey(int text,int k1,int k2)
{
    int tmp = (k1<<8) | k2;
    return (text ^ tmp);
}

int Encryption::shiftRow(int n)
{
    int tmp;
    updateNowKey(n);
    tmp = nowKey[1];
    nowKey[1] = nowKey[3];
    nowKey[3] = tmp;
    return (nowKey[0]<<12 | nowKey[1]<<8 | nowKey[2]<<4 | nowKey[3]);
}

int Encryption::mixColumns(int n)
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

int Encryption::getLeft(int n,int shift) { return n>>shift; }

int Encryption::getRight(int n,int shift)
{
    int tmp = n>>shift;
    return ((tmp<<shift) ^ n);
}

int Encryption::subNib(int n)
{
    int ll = n>>6;
    int lr = (n>>4) ^ (ll<<2);
    int rl = (n ^ (ll<<6) ^ (lr<<4))>>2;
    int rr = (n ^ (ll<<6) ^ (lr<<4) ^ (rl<<2));
    return (keySBOX[ll][lr]<<4 | keySBOX[rl][rr]);
}

int Encryption::encrypt(int plaintext, vector<int> vkey)
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
