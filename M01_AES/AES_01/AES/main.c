#include <stdio.h>
#include <stdlib.h>

void statePrint(unsigned char state[0x04][0x04]){
    int i,j;

    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            printf("%#04x ", state[i][j]);
        }
    }
    printf("\n");
}

void shiftRows(unsigned char state[0x04][0x04]){
    int i,j,k,tmp;

    printf("ShiftRows \n");
    for(i=0; i<4; i++){
        for(j=4-i; j<4; j++){
            tmp = state[i][0];
            for(k=0; k<4-1; k++){
                state[i][k] = state[i][k+1];
            }
            state[i][3] = tmp;
        }
    }
    statePrint(state);
}

void subBytes(unsigned char state[0x04][0x04], unsigned char S_BOX[0x16][0x16]){
    int i,j,x,y;

    printf("SubBytes \n");
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            if(i != 1 && i != 3){ //2,4열 항등함수
                x = state[i][j] >> 4;
                y = state[i][j] - (x << 4);
                state[i][j] = S_BOX[x][y];
            }
        }
    }
    statePrint(state);
}

void MixColumns(unsigned char state[0x04][0x04]){
    unsigned char a[0x04], b[0x04], h;
    int i,j;

    printf("MixColumns \n");
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            a[j] = state[j][i];
            h = (unsigned char)((signed char)state[j][i] >> 7);
            b[j] = state[j][i] << 1;
            b[j] ^= 0x1b & h;
        }
        state[0][i] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
        state[1][i] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
        state[2][i] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
        state[3][i] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
    }

    statePrint(state);
}

void addRoundKey(unsigned char state[0x04][0x04]){
    int i,j;
    FILE *pFile = NULL;
	FILE *output_txt = NULL;

	unsigned char buffer2[0x16];
	int a=0;

	pFile = fopen("C:\\Users\\kyu\\Desktop\\NSR_professional-manpower\\M01_AES\\AES_01\\AES\\PT01.dat","rb");
	output_txt = fopen("C:\\Users\\kyu\\Desktop\\NSR_professional-manpower\\M01_AES\\AES_01\\AES\\PT01_txt.dat","wb");

    unsigned char RoundKey [0x04][0x04] = {
        {0xEA, 0x47, 0x3C, 0x1D},
        {0x5F, 0x36, 0x1B, 0x34},
        {0xAF, 0x2A, 0x25, 0x2A},
        {0xE7, 0x40, 0xC5, 0x58}
    }; //key

    unsigned char S_BOX[0x16][0x16] = {
        {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76},
        {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0},
        {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15},
        {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75},
        {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84},
        {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf},
        {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8},
        {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2},
        {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73},
        {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb},
        {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79},
        {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08},
        {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a},
        {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e},
        {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf},
        {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}
    };

	if( pFile == NULL ){
		printf("error");
	}else{
		while( !feof( pFile ) ){

            fread(buffer2, sizeof(unsigned char), 16, pFile);

            state[0][0] = buffer2[0];
            state[0][1] = buffer2[1];
            state[0][2] = buffer2[2];
            state[0][3] = buffer2[3];
            state[1][0] = buffer2[4];
            state[1][1] = buffer2[5];
            state[1][2] = buffer2[6];
            state[1][3] = buffer2[7];
            state[2][0] = buffer2[8];
            state[2][1] = buffer2[9];
            state[2][2] = buffer2[10];
            state[2][3] = buffer2[11];
            state[3][0] = buffer2[12];
            state[3][1] = buffer2[13];
            state[3][2] = buffer2[14];
            state[3][3] = buffer2[15];

            printf("Key : %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x %#04x \n",
                   buffer2[0],buffer2[1],buffer2[2],buffer2[3],
                   buffer2[4],buffer2[5],buffer2[6],buffer2[7],
                   buffer2[8],buffer2[9],buffer2[10],buffer2[11],
                   buffer2[12],buffer2[13],buffer2[14],buffer2[15]);

            printf("AddRoundKey \n");
            for(i=0; i<4; i++){
                for(j=0; j<4; j++){
                    state[i][j] ^= RoundKey[i][j];
                }
            }
            statePrint(state);
            shiftRows(state);
            subBytes(state, S_BOX);
            MixColumns(state);

            printf("AddRoundKey \n");
            for(i=0; i<4; i++){
                for(j=0; j<4; j++){
                    state[i][j] ^= RoundKey[i][j];
                }
            }
            statePrint(state);
            shiftRows(state);
            subBytes(state, S_BOX);
            MixColumns(state);

            printf("AddRoundKey \n");
            for(i=0; i<4; i++){
                for(j=0; j<4; j++){
                    state[i][j] ^= RoundKey[i][j];
                }
            }
            statePrint(state);
            shiftRows(state);
            subBytes(state, S_BOX);
            MixColumns(state);

            for(i=0; i<4; i++){
                for(j=0; j<4; j++){
                    state[i][j] ^= RoundKey[i][j];
                }
            }

            printf("Secret key : ");
            statePrint(state);

            if(a == 5){
                break;
            }

            fwrite(state, sizeof(unsigned char), 16, output_txt);

            a++;
		}

		fclose(pFile);
		fclose(output_txt);
	}
}

int main(){
    unsigned char state[0x04][0x04] = {
        {0xad, 0x1a, 0xc1, 0x5c},
        {0xd8, 0xcd, 0xdc, 0xed},
        {0x92, 0xd9, 0xcd, 0x1c},
        {0xad, 0x4a, 0x54, 0x35}
    }; //Plain Text

    printf("PlanText \n");
    statePrint(state);
    addRoundKey(state);

    return 0;
}
