#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

unsigned long code[256];
unsigned char crc[8];

void dopisanie(unsigned char *kod);

int getNum(char ch)
{
    int num = 0;
    if (ch >= '0' && ch <= '9') {
        num = ch - 0x30;
    }
    else {
        switch (ch) {
            case 'a':
                num = 10;
                break;
            case 'b':
                num = 11;
                break;
            case 'c':
                num = 12;
                break;
            case 'd':
                num = 13;
                break;
            case 'e':
                num = 14;
                break;
            case 'f':
                num = 15;
                break;
            default:
                num = 0;
        }
    }
    return num;
}

unsigned int hex2int(unsigned char hex[])
{
    unsigned int x = 0;
    x = (getNum(hex[0])) * 16 + (getNum(hex[1]));
}

unsigned long move_bits(unsigned long number, char count_bits) {
    unsigned long wynik=0;
    for(int i = 1; i < (count_bits + 1); i++) {
        if(number & 1) wynik |= 1 << (count_bits - i);
        number >>= 1;
    }
    return wynik;
}

void creates_codes() {
    for (int i=0; i < 256; i++) {
        code[i]= move_bits(i, 8) << 24;
        for (int j = 0; j < 8; j++) {
            if ((code[i] & (1 << 31)) != 0) code[i] = (code[i] << 1) ^ 0x04c11db7; // 0x04c11db7 - wielomian dla CRC32
            else code[i] = (code[i] << 1);
        }
        code[i] = move_bits(code[i], 32);
    }
}

unsigned char * CRC32(unsigned char *txt) {
    unsigned long  crc32=0xffffffff;        //0xffffffff - wartosc poczatkowa 2^32-1
    int  wynik;
    char kode[20];
    int dlugosc;
    dlugosc = strlen(txt);
    while(dlugosc--) crc32= (crc32>>8) ^ code[(crc32 & 255) ^ *txt++];
    wynik=crc32^0xffffffff;
    itoa(wynik,kode,16);
    int k;
for (k=0; k<8; k++) {crc[k]=kode[k];};
    return *kode;
}

void dopisanie(unsigned char *kod) {
    int i=0, j=0, wartosc;
    FILE *inputFileHandle;
    if((inputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\CRC-32\\untitled1\\test.txt", "awb")) == NULL) {
        printf("Nie powiodlo sie otworzenie pliku %s do zapisu \n", "test.txt");
    }
    else {
        for(i=0; i<8; i+=2) {
            wartosc = hex2int(kod);
            for(j=0; j<6-i; j++) {
                kod[j]=kod[j+2];
            }
            fprintf(inputFileHandle, "%c", (char)wartosc);
        }
    }
    fclose(inputFileHandle);
}

void sprCRC() {

    int i=0, readCount = 0, readBytesLength = 1;
    unsigned char buffer[1], kod[8], tekst[10000], crcodczyt[4];
    FILE *inputFileHandle;
    if((inputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\CRC-32\\untitled1\\test.txt", "rb")) == NULL) {
        printf("Nie powiodlo sie otworzenie pliku %s do odczytu \n", "test.txt");
    }
    else {
        printf("\nInput file:\n");
        while (readCount = fread(buffer, sizeof(unsigned char), readBytesLength, inputFileHandle)) {
            tekst[i]=buffer[0];
            printf("%c", tekst[i]);
            itoa(buffer[0],kod,16);
            printf("    0x%s \n",kod);
            i++;
        }
        printf("\n");
        crcodczyt[0]=tekst[i-4];
        crcodczyt[1]=tekst[i-3];
        crcodczyt[2]=tekst[i-2];
        crcodczyt[3]=tekst[i-1];
        tekst[i-4]=0;
    }
    fclose(inputFileHandle);
    int k;
    int czy=1;
    CRC32(tekst);
    for(k=0; k<4; k++) {
        itoa(crcodczyt[k],kod,16);
        if(kod[1]==NULL) {
            kod[1]=kod[0];
            kod[0]='0';
        }
        if(kod[0]!=crc[2*k] || kod[1]!=crc[2*k+1]) {
            czy=0;
            break;
        }
    }
    if(czy) printf("CRC32 poprawny :)\n");
    else printf("CRC32 niepoprawny :(\n");
}


int main() {

    int i=0, readCount = 0, readBytesLength = 1;
    int wybor;
    unsigned char buffer[1], kod[20], tab[10000];
    creates_codes();
    FILE *inputFileHandle;

while(1) {
    printf("\n1. Calculate CRC32\n");
    printf("2. Check CRC32\n");
    printf("3. Exit\n-->");
    scanf("%i", &wybor);
    switch(wybor) {
        case 1:
            if((inputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\CRC-32\\untitled1\\test.txt", "rb")) == NULL) {
                printf("Nie powiodlo sie otworzenie pliku %s do odczytu \n", "test.txt");
            }
            else {
                printf("\nInput file:\n");
                while (readCount = fread(buffer, sizeof(unsigned char), readBytesLength, inputFileHandle)) {
                    printf("%c", buffer[0]);
                    tab[i]=buffer[0];
                    i++;
                }
            }
            fclose(inputFileHandle);
            unsigned char *pol = CRC32(tab);
            printf("\nCalculated CRC32: %s\n", crc);
            dopisanie(crc);
            printf("Dopisano CRC do pliku\n");
            break;
        case 2:
            sprCRC();
            break;
        case 3:
            return 0;
        default:
            printf("\nNiepoprawna komenda.\n");
            break;
    }
}
}
