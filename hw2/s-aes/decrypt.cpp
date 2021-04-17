#include <iostream>
using namespace std;

#include "decrypt.h"

int nowKey[4];

int keyINVSBOX[4][4] =
{
    {10,5,9,11},
    {1,7,8,15},
    {6,0,2,3},
    {12,4,13,14}
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

Decryption::Decryption()
{

}

void Decryption::updateNowKey(int n)
{
    nowKey[0] = n>>12;
    nowKey[1] = (n^(nowKey[0]<<12))>>8;
    nowKey[2] = (n^(nowKey[0]<<12)^(nowKey[1]<<8))>>4;
    nowKey[3] = (n^(nowKey[0]<<12)^(nowKey[1]<<8)^(nowKey[2]<<4));
}

int Decryption::addRoundKey(int text,int k1,int k2)
{
    int tmp = (k1<<8) | k2;
    return (text ^ tmp);
}

int Decryption::shiftRow(int n)
{
    int tmp;
    updateNowKey(n);
    tmp = nowKey[1];
    nowKey[1] = nowKey[3];
    nowKey[3] = tmp;
    return (nowKey[0]<<12 | nowKey[1]<<8 | nowKey[2]<<4 | nowKey[3]);
}

int Decryption::mixColumnsINV(int n)
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

int Decryption::getLeft(int n,int shift) { return n>>shift; }

int Decryption::getRight(int n,int shift)
{
    int tmp = n>>shift;
    return ((tmp<<shift) ^ n);
}

int Decryption::subNibINV(int n)
{
    int ll = n>>6;
    int lr = (n>>4) ^ (ll<<2);
    int rl = (n ^ (ll<<6) ^ (lr<<4))>>2;
    int rr = (n ^ (ll<<6) ^ (lr<<4) ^ (rl<<2));
    return (keyINVSBOX[ll][lr]<<4 | keyINVSBOX[rl][rr]);
}

int Decryption::decrypt(int ciphertext, vector<int> vkey)
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
