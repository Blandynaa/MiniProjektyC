#include <stdio.h>
#include <stdlib.h>
#define MODEL_ARR_LEN 256

struct HuffmanNode {
	int symbol;
	int frequency;
	int level;
	struct HuffmanNode * left;
	struct HuffmanNode * right;
	struct HuffmanNode * parent;
};

typedef struct HuffmanNode HuffmanNode;

HuffmanNode huffmanModelArray[MODEL_ARR_LEN];
int lastnode, longest=0, longestv=-1;
unsigned char tabcode[MODEL_ARR_LEN][15]={0};

struct HuffmanNode* para(HuffmanNode *nodeostatni, HuffmanNode *nodeprzedostatni) {
	static int a=-1;
	struct HuffmanNode *nowy = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
	nowy->right = nodeostatni;
	nowy->left = nodeprzedostatni;
	nowy->symbol = a;
	nowy->level=0;
	nowy->frequency = nodeostatni->frequency + nodeprzedostatni->frequency;
	nodeostatni->parent = nowy;
	nodeprzedostatni->parent = nowy;
	lastnode=a; a--;
	return nowy;
}

int CompareHuffmanNodes(const void *item1, const void *item2) {
	HuffmanNode *node1 = (HuffmanNode *)item1;
	HuffmanNode *node2 = (HuffmanNode *)item2;
	int compareResult = (node1->frequency - node2->frequency);
	if(compareResult == 0) compareResult = (node1->symbol - node2->symbol);
	return -compareResult;
}

void SortHuffmanModel(HuffmanNode *huffmanModelArray, int modelArrayLength) {
	qsort(huffmanModelArray, modelArrayLength, sizeof(HuffmanNode), CompareHuffmanNodes);
}

void HuffmanTree(HuffmanNode *huffmanModelArray, int modelArrayLength) {
	int i=0;
	struct HuffmanNode *pom1, *pom2, *pusty = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
	pusty->symbol = 0;
	pusty->frequency = 0;
	pusty->left = NULL;
	pusty->right = NULL;
	while (modelArrayLength>1) {
		struct HuffmanNode *pom1 = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
		struct HuffmanNode *pom2 = (struct HuffmanNode *)malloc(sizeof(struct HuffmanNode));
			pom1->symbol = huffmanModelArray[modelArrayLength-1].symbol;
			pom1->frequency = huffmanModelArray[modelArrayLength-1].frequency;
			pom1->left = huffmanModelArray[modelArrayLength-1].left;
			pom1->right = huffmanModelArray[modelArrayLength-1].right;
			pom2->symbol = huffmanModelArray[modelArrayLength-2].symbol;
			pom2->frequency = huffmanModelArray[modelArrayLength-2].frequency;
			pom2->left = huffmanModelArray[modelArrayLength-2].left;
			pom2->right = huffmanModelArray[modelArrayLength-2].right;
		huffmanModelArray[modelArrayLength-2] = *para(pom1, pom2);
		SortHuffmanModel(huffmanModelArray, modelArrayLength);
		modelArrayLength--;
	}
}

void setlevel(HuffmanNode * v) {
	if(v) {
        if(v->left) { v->left->level=v->level+1; v->right->level=v->level+1; }
	    setlevel ( v->left);
        setlevel ( v->right);
	}
}

void PrintTree(HuffmanNode * v, FILE *outputFileHandle) {
	if(v) {
  		if(v->right!=NULL) fprintf(outputFileHandle, "%3i :%3d   childleft: %4d   childright: %4d  ", v->symbol, v->frequency, v->left->symbol, v->right->symbol);
		if(v->right==NULL) fprintf(outputFileHandle, "%3i :%3d   childleft: NULL   childright: NULL  ", v->symbol, v->frequency);
		if(v->symbol!=lastnode) fprintf(outputFileHandle, " rodzic: %4d\n", v->parent->symbol);
		else fprintf(outputFileHandle, " rodzic: NULL\n");
  	  	PrintTree ( v->left, outputFileHandle);
  	  	PrintTree ( v->right, outputFileHandle);
  	}
}

void PrintCode( struct HuffmanNode * v , int b, FILE *outputFileHandle) {
	static int tabb[256]={0}, k;
	if(v) {
  		tabb[v->level] = b;
  		if(v->symbol>0) {
  			fprintf(outputFileHandle, "%3d ", v->symbol);
  			for(k=1; k<=v->level; k++) fprintf(outputFileHandle, "%d", tabb[k]);
            fprintf(outputFileHandle, "\n");
            if(v->level>longestv) { longestv=v->level; longest=v->symbol; }
        }
  	  	PrintCode ( v->left, 0, outputFileHandle);
  	  	PrintCode ( v->right, 1, outputFileHandle);
  	}
}

void zapisKodowania() {
    int a,i;
    unsigned char b[15];
    FILE* intputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\untitled\\cmake-build-debug\\tree.code.txt", "r");
    while(fscanf(intputFileHandle,"%d %s",&a,b)!=EOF) { for(i=0;i<15;i++) tabcode[a][i]=b[i]; }
    fclose(intputFileHandle);
}

void kodowaniepliku() {
    FILE *inputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\untitled\\smalltest.txt", "rb");
    FILE * outputFileHandle2 = fopen("compressed.txt", "wb");
    static int i=0;
    int j=0, k=0, readCount = 0, readBytesLength = 1;
    unsigned char buffer[1], bajt = 0;
    while (readCount = fread(buffer, sizeof(unsigned char), readBytesLength, inputFileHandle)) {
        for(j=0; j<15; j++) {
            if (tabcode[buffer[0]][j]=='1') {bajt |= 1 << i; i++;}
            else if (tabcode[buffer[0]][j]=='0') {bajt &= ~(1 << i); i++;}
            else break;
            if(i==8) {
                fprintf(outputFileHandle2, "%c", bajt);
                bajt=0; i=0;
            }
        }
    }
    if(i!=0) {
        for(k=0; k<longestv; k++) {
            if(tabcode[longest][k]=='1') {
                if(k+i<8) bajt |= 1 << (k+i);
                else break;
            }
        }
        fprintf(outputFileHandle2, "%c", bajt);
    }
    fclose(inputFileHandle);
    fclose(outputFileHandle2);
}

void odkodowaniepliku() {
    zapisKodowania();
    FILE *inputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\untitled\\cmake-build-debug\\compressed.txt", "rb");
    FILE *outputFileHandle2 = fopen("decompressed.txt", "wb");
    printf("\nPliki wejsciowe:\n   compressed.txt - poprawnie pobrany :D\n   tree.code      - poprawnie pobrany :3\n");
    static int i=0, d=0;
    unsigned char a, buffer[1], kod[15]={0};
    int j=0, k=0, l=0, readCount = 0, getbit = 0, readBytesLength = 1;
    while (readCount = fread(buffer, sizeof(unsigned char), readBytesLength, inputFileHandle)) {
        for(j=0; j<8; j++) {
            getbit=(buffer[0] >> j) & 1;
            (getbit==1) ? (kod[i]='1') : (kod[i]='0');
            for (k=0; k<MODEL_ARR_LEN; k++) {
                if(tabcode[k][0]!=0) { l=0;
                    while(tabcode[k][l]!=0 && kod[l]!=0) {
                        if(kod[l]==tabcode[k][l]) l++;
                        else { l--; break; }
                    }
                    if (tabcode[k][l]==0 && kod[l]==0) l--;
                    if(l==i) {
                        d++;
                        a=(char)k;
                        if(a=='d'&& d==41);
                        else fprintf(outputFileHandle2, "%c", (char)k);
                        for(i; i>=0; i--) kod[i]=0;
                        i=-1;
                        break;
                    }
                }
            }
            i++;
        }
    }
    fclose(inputFileHandle);
    fclose(outputFileHandle2);
}

void kompresja() {
    FILE *inputFileHandle = fopen("C:\\Users\\win10\\Desktop\\Semestr 4\\TIIK cw\\untitled\\smalltest.txt", "rb");
    int i=0, readCount = 0, readBytesLength = 1;
    printf("\nPlik wejsciowy:\n   plik tekstowy - poprawnie pobrany :)\n");
    unsigned char buffer[1];
    while (readCount = fread(buffer, sizeof(unsigned char), readBytesLength, inputFileHandle)) {
        for(i=0; i<MODEL_ARR_LEN; i++) {
            if(huffmanModelArray[i].symbol == buffer[0]) {
                huffmanModelArray[i].frequency++;
                break;
            }
            if(huffmanModelArray[i].symbol == 0) {
                huffmanModelArray[i].symbol = buffer[0];
                huffmanModelArray[i].frequency = 1;
                huffmanModelArray[i].right = NULL;
                huffmanModelArray[i].left = NULL;
                huffmanModelArray[i].parent = NULL;
                huffmanModelArray[i].level = 0;
                break;
            }
        }
    }
    fclose(inputFileHandle);
    for (i=0; i<MODEL_ARR_LEN; i++) { if(huffmanModelArray[i].frequency==0) break; }
    SortHuffmanModel(huffmanModelArray, i);
    FILE *outputFileHandle;
    outputFileHandle = fopen("tree.model.txt", "w");
    for (i=0; i<MODEL_ARR_LEN; i++) {
        if(huffmanModelArray[i].frequency==0) break;
        fprintf(outputFileHandle, "%3i :%3i \n", huffmanModelArray[i].symbol, huffmanModelArray[i].frequency);
    }
    fclose(outputFileHandle);
    HuffmanTree(huffmanModelArray, i);
    HuffmanNode *root = huffmanModelArray;
    setlevel(root);
    outputFileHandle = fopen("tree.graf.txt", "w");
    PrintTree (root, outputFileHandle);
    fclose(outputFileHandle);
    outputFileHandle = fopen("tree.code.txt", "w");
    PrintCode (root, 0, outputFileHandle);
    fclose(outputFileHandle);
    zapisKodowania();
    kodowaniepliku();
}

int main () {
    int wybor;
    while(1) {
        printf("\nTiK Laboratorium nr 3\n");
        printf("1. Kompresja\n");
        printf("2. Dekompresja\n");
        printf("3. Wyjscie\n");
        printf("-->");
        scanf("%d", &wybor);
        switch (wybor) {
            case 1:
                kompresja();
                printf("\nPliki wyjsciowe:\n   tree.model\n   tree.graf\n   tree.code\n   compressed.txt\n");
                break;
            case 2:
                odkodowaniepliku();
                printf("\nPlik wyjsciowy:\n   decompressed.txt\n");
                break;
            case 3:
                return 0;
            default:
                printf("Niepoprawna komenda!\n");
                break;
        }
    }
	return 0;
}