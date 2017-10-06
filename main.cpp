#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>


#include "hash.h"
#include "Block.h"
#include "common.h"
#include "BlockChain.h"


using namespace std;
/*
Hash header: index + prevHash + merkleRoot(data) + nonce
*/





int main() {
    printf("Welcome! To quit-> Control c \n");
    char tmp[201];
    int ch;
    auto bc = BlockChain();
    for ( int i = 0; i < 10; i++ ) {
        vector<string> v;
        int temp;
        printf("(1) Look at Blocks \n(2) Add block\n");
        scanf("%d",&temp);
        if (temp == 1){
            printf("What Block do you want to look at? ");
            scanf("%d",&temp);
            try {
                bc.getBlock(temp).toString();
            }
            catch (const exception& e){
                cout << e.what() << "\n";
            }
        }
        else{
            printf("\nADDING BLOCKS!\nEnter your message: ");
            scanf("%200s",tmp);
            string str = tmp;
            printf("Entered '%s' into block\n",str.c_str());
            v.push_back(str);
        
            printf("Press any number to add block to blockchain:");
            scanf("%d",&ch);
        
            auto pair = findHash(bc.getNumOfBlocks(),bc.getLatestBlockHash(),v);
            bc.addBlock(bc.getNumOfBlocks(),bc.getLatestBlockHash(),pair.first,pair.second,v );
        }
    }
        
    

    // bc.addBlock(0,string("00000000000000"),string("003d9dc40cad6b414d45555e4b83045cfde74bcee6b09fb42536ca2500087fd9"),string("46"),v);
    printf("\n");
    return 0;
} 

