#ifndef DECRYPT_H
#define DECRYPT_H

#include <vector>
using std::vector;

class Decryption
{
public:
    Decryption();
    int decrypt(int,vector<int>);

private:
    void updateNowKey(int);
    int addRoundKey(int,int,int);
    int shiftRow(int);
    int mixColumnsINV(int);
    int subNibINV(int);
    int getLeft(int,int);
    int getRight(int,int);
};

#endif // DECRYPT_H
