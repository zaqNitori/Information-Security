#include <iostream>
using namespace std;
#include "expandKey.h"

#define KEY 42811 //1010 0111 0011 1011
#define RCON1 128 //1000 0000
#define RCON2 48 //0011 0000

int keySBOX[4][4] =
{
    {9,4,10,11},
    {13,1,8,5},
    {6,2,0,3},
    {12,14,15,7}
};

ExpandKey::ExpandKey()
{

}

int ExpandKey::getLeft(int n,int shift) { return n>>shift; }

int ExpandKey::getRight(int n,int shift)
{
    int tmp = n>>shift;
    return ((tmp<<shift) ^ n);
}

int ExpandKey::subNib(int n)
{
    int ll = n>>6;
    int lr = (n>>4) ^ (ll<<2);
    int rl = (n ^ (ll<<6) ^ (lr<<4))>>2;
    int rr = (n ^ (ll<<6) ^ (lr<<4) ^ (rl<<2));
    return (keySBOX[ll][lr]<<4 | keySBOX[rl][rr]);
}

void ExpandKey::expandKey(vector<int> vkey)
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
