#ifndef ENCRYPT_H
#define ENCRYPT_H

#include <vector>
using std::vector;

class Encryption
{
public:
    Encryption();
    int encrypt(int,vector<int>);

private:
    void updateNowKey(int);
    int addRoundKey(int,int,int);
    int shiftRow(int);
    int mixColumns(int);
    int subNib(int);
    int getLeft(int,int);
    int getRight(int,int);
};

#endif // ENCRYPT_H


