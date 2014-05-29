#include <stdio.h>
#include <stdlib.h>
#include <openssl/aes.h>
#include <openssl/rand.h>

int encrypt(char * input,int data_len,char * output,int * en_len)
{
    AES_KEY aes;
    unsigned char key[32];
    unsigned int len = 0,input_len;
    unsigned char rand_str[128];
    unsigned char iv[32];
    int i,ret;
    unsigned char str1[32];
    unsigned char str2[32];
    unsigned char * myinput;


    str1[0]=99;
    str1[1]=104;
    str1[2]=117;
    str1[3]=97;
    str1[4]=110;
    str1[5]=103;
    str1[6]=32;
    str1[7]=32;
    str1[8]=113;
    str1[9]=105;
    str1[10]=97;
    str1[11]=110;
    str1[12]=32;
    str1[13]=32;
    str1[14]=109;
    str1[15]=105;
    
    str2[0]=32;
    str2[1]=119;
    str2[2]=111;
    str2[3]=32;
    str2[4]=32;
    str2[5]=106;
    str2[6]=105;
    str2[7]=97;
    str2[8]=111;
    str2[9]=32;
    str2[10]=32;
    str2[11]=32;
    str2[12]=103;
    str2[13]=117;
    str2[14]=111;
    str2[15]=32;
    
    str1[16]=110;
    str1[17]=103;
    str1[18]=32;
    str1[19]=32;
    str1[20]=121;
    str1[21]=117;
    str1[22]=101;
    str1[23]=32;
    str1[24]=32;
    str1[25]=103;
    str1[26]=117;
    str1[27]=97;
    str1[28]=110;
    str1[29]=103;
    str1[30]=35;
    str1[31]=33;
    
    str2[16]=32;
    str2[17]=32;
    str2[18]=32;
    str2[19]=100;
    str2[20]=101;
    str2[21]=32;
    str2[22]=32;
    str2[23]=32;
    str2[24]=32;
    str2[25]=32;
    str2[26]=103;
    str2[27]=97;
    str2[28]=110;
    str2[29]=103;
    str2[30]=33;
    str2[31]=35;
    
    sprintf(rand_str,"%d%d",&aes+(unsigned int)input,&iv+(unsigned)output);

    RAND_seed(rand_str,32);
    ret = RAND_bytes(rand_str+32,32);
    if(ret != 1) {
        return 1; //get random error
    }

    RAND_seed(rand_str+32,32);
    ret = RAND_bytes(rand_str,32);
    if(ret != 1) {
        return 1; //get random error
    }
    
    input_len = data_len + 4;
    myinput = malloc(input_len);
    if(myinput == NULL) {
        return 3;
    }

    sprintf(myinput,"%d",data_len);
    memcpy(myinput+4,input,data_len);

    if ((input_len + 1) % AES_BLOCK_SIZE == 0) {
        len = input_len + 1;
    } else {
        len = ((input_len + 1) / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
    }
    //printf("data_len=%d,input_len=%d,len=%d\n",data_len,input_len,len);

    for (i=0; i<32; ++i) {
        key[i] = rand_str[i];
    }
    

    for (i=0; i<32; ++i) {
        iv[i] = str1[i];
    }
    if (AES_set_encrypt_key(key, 256, &aes) < 0) {
        free(myinput);
        return 2; //set encrypt key error
    }
    
    AES_cbc_encrypt(myinput, output, len, &aes, iv, AES_ENCRYPT);
    
    for (i=0; i<32; ++i) {
        iv[i] = str2[i];
        key[i] = str2[i];
    }
    if (AES_set_encrypt_key(key, 256, &aes) < 0) {
        free(myinput);
        return 2; //set encrypt key error
    }
    AES_cbc_encrypt(rand_str, &rand_str[32], 32, &aes, iv, AES_ENCRYPT);
    
    for(i=0;i<32;i++)
        output[len+i] = rand_str[i+32];
    
    *en_len = len + 32;
    free(myinput);
    return 0;
}

int decrypt(char * input,int en_len,char * output,int * de_len)
{
    AES_KEY aes;
    unsigned char key[32];
    unsigned char iv[32];
    unsigned int len = 0;
    int i;
    unsigned char str1[32];
    unsigned char str2[32];
    unsigned char data_len[5];
    unsigned char * myoutput;

    str1[0]=99;
    str1[1]=104;
    str1[2]=117;
    str1[3]=97;
    str1[4]=110;
    str1[5]=103;
    str1[6]=32;
    str1[7]=32;
    str1[8]=113;
    str1[9]=105;
    str1[10]=97;
    str1[11]=110;
    str1[12]=32;
    str1[13]=32;
    str1[14]=109;
    str1[15]=105;
    
    str2[0]=32;
    str2[1]=119;
    str2[2]=111;
    str2[3]=32;
    str2[4]=32;
    str2[5]=106;
    str2[6]=105;
    str2[7]=97;
    str2[8]=111;
    str2[9]=32;
    str2[10]=32;
    str2[11]=32;
    str2[12]=103;
    str2[13]=117;
    str2[14]=111;
    str2[15]=32;
    
    len = en_len - 32;
    
    str1[16]=110;
    str1[17]=103;
    str1[18]=32;
    str1[19]=32;
    str1[20]=121;
    str1[21]=117;
    str1[22]=101;
    str1[23]=32;
    str1[24]=32;
    str1[25]=103;
    str1[26]=117;
    str1[27]=97;
    str1[28]=110;
    str1[29]=103;
    str1[30]=35;
    str1[31]=33;
    
    str2[16]=32;
    str2[17]=32;
    str2[18]=32;
    str2[19]=100;
    str2[20]=101;
    str2[21]=32;
    str2[22]=32;
    str2[23]=32;
    str2[24]=32;
    str2[25]=32;
    str2[26]=103;
    str2[27]=97;
    str2[28]=110;
    str2[29]=103;
    str2[30]=33;
    str2[31]=35;

    for (i=0; i<32; ++i) {
        key[i] = str2[i];
    }
    
    
    for (i=0; i<32; ++i) {
        iv[i] = str2[i];
    }
    
    if (AES_set_decrypt_key(key, 256, &aes) < 0) {
        return 2; //set encrypt key error
    }
    
    AES_cbc_encrypt(&input[en_len - 32], key, 32, &aes, iv,
                    AES_DECRYPT);
    
    for (i=0; i<32; ++i) {
        iv[i] = str1[i];
    }
    
    if (AES_set_decrypt_key(key, 256, &aes) < 0) {
        return 2; //set encrypt key error
    }
    myoutput = malloc(en_len);
    if(myoutput == NULL){
        return 3;
    }
    AES_cbc_encrypt(input, myoutput, len, &aes, iv,
                    AES_DECRYPT);
    
    memcpy(data_len,myoutput,4);
    data_len[4] = '\0';
    *de_len = strtoul(data_len,NULL,0);
    //printf("in decrypt data_len=%s\n",data_len);
    //printf("in decrypt data_len=%d\n",*de_len);
    
    memcpy(output,myoutput + 4,*de_len);
    //printf("in decrypt data=%s\n",output);
    free(myoutput);
    return 0;
}

