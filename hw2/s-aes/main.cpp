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

int main(void)
{
    int plaintext = PLAINTEXT;
    int ciphertext;

    ExpandKey exKey;
    Encryption encrypt;
    Decryption decrypt;

    exKey.expandKey(vkey);

    cout<<"Original plaintext: "<<plaintext<<endl;
    ciphertext = encrypt.encrypt(plaintext,vkey);
    cout<<"After encryption: "<<ciphertext<<endl;
    plaintext = decrypt.decrypt(ciphertext,vkey);
    cout<<"After decryption: "<<plaintext<<endl;
    return 0;
}