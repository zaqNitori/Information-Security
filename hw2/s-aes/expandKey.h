#ifndef EXPANDKEY_H
#define EXPANDKEY_H

#include <vector>
using std::vector;

class ExpandKey
{
public:
    ExpandKey();
    int subNib(int);
    void expandKey(vector<int>);

private:
    int getLeft(int,int);
    int getRight(int,int);
};

#endif // EXPANDKEY_H
