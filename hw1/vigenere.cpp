#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

float p[] = {0.82, 0.15, 0.28, 0.43, 1.27
, 0.22, 0.2, 0.61, 0.7, 0.02, 0.08
, 0.4, 0.24, 0.67, 0.75, 0.19, 0.01
, 0.6, 0.63, 0.91, 0.28, 0.1, 0.23
, 0.01, 0.2, 0.01};

vector<pair<float,string>> vfs;

string caesar()
{
    int shift;
    string s,out = "";
    cout<<"Encrypting text\n";
    cout<<"Enter shift key: ";
    cin>>shift;
    shift%=26;
    cin.get();
    cout<<"Enter PlainText: ";
    getline(cin,s);

    for(char c:s)
    {
        if(c == ' ') continue;
        if(c>='A' && c<='Z')
        {
            c+=shift;
            c = c>'Z'?c-'Z'+'A'-1:c;
            c+=32;
        }
        else if(c>='a' && c<='z')
        {
            c+=shift;
            c = c>'z'?c-'z'+'a'-1:c;
        }
        out += c;
    }
    return out;
}

int main(void)
{
    string s,tmp;
    float weight;
    s = caesar();
    for(int i=0,j;i<26;i++)
    {
        weight = 1e5;
        tmp="";
        j=0;
        for(char c:s)
        {
            c -= i;
            c = c<'a'?c+26:c;
            weight *= p[c-'a'];
            tmp+=c;
            if(j++>=7)
            {
                weight *= 1e4;
                j=0;
            }
        }
        vfs.push_back(make_pair(weight,tmp));
    }
    sort(vfs.begin(),vfs.end());
    for(int i=25;i>=16;i--)
        cout<<25-i<<". weight -> "<<vfs[i].first
        <<endl<<vfs[i].second<<endl;
    return 0;
}


