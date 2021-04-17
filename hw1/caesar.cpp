#include <iostream>
#include <string.h>

using namespace std;

int main(void)
{
    int shift;
    string s;
    cout<<"Enter shift key: ";
    cin>>shift;
    shift%=26;
    cin.get();
    cout<<"Enter PlainText: ";
    getline(cin,s);

    for(int i=0;i<s.size();i++)
    {
        if(s[i]>='A' && s[i]<='Z')
        {
            s[i]+=shift;
            s[i] = s[i]>'Z'?s[i]-'Z'+'A'-1:s[i];
        }
        else if(s[i]>='a' && s[i]<='z')
        {
            s[i]+=shift;
            s[i] = s[i]>'z'?s[i]-'z'+'a'-1:s[i];
        }
    }
    cout<<"CipherText: "<<s<<endl;

    for(int i=0;i<s.size();i++)
    {
        if(s[i]>='A' && s[i]<='Z')
        {
            s[i]-=shift;
            s[i] = s[i]<'A'?s[i]+26:s[i];
        }
        else if(s[i]>='a' && s[i]<='z')
        {
            s[i]-=shift;
            s[i] = s[i]<'a'?s[i]+26:s[i];
        }
    }
    cout<<"Original PlainText: "<<s<<endl;
}
