#include <stdio.h>
#include <cstring>

//#pragma GCC optimize("Ofast")

#define SZ 10000001
#define ALPHABET 256

struct DllNode
{
    char symbol;
    DllNode* prev;
    DllNode* next; 
};

struct Dll
{
    DllNode* head;
};

struct HuffNode
{
    char symbol;
    int freq;
    HuffNode *left, *right;
};

struct HuffHeap
{
    HuffNode *front;
};

// BASE
char Src[SZ];
char _Src[SZ];
char Dest[SZ];

// MTF 
char symbols[ALPHABET];
DllNode DllNodeArr[ALPHABET];

// Huffman
int freq[ALPHABET];
int code[ALPHABET];
int codelen[ALPHABET];

HuffNode HuffNodeArr[2*ALPHABET];
HuffNode* HuffHeapArr[2*ALPHABET];
int HuffNodeCnt;
int HuffHeapCnt;

void runlength_encode(char* src, char* dest, int& n)
{
    char ch = src[0];
    int length = 1;
    int index = 0;
    int num_start_idx;
    int digit_cnt;
    char temp;
    for(int i = 1; i < n; i++)
    {
        if(ch == src[i])
        {
            length++;
        }
        else
        {
            if(length > 1)
            {
                num_start_idx = index;
                digit_cnt = 0;
                while(length >= 1)
                {
                    dest[index++] = (length%10) + '0';
                    length /= 10;
                    digit_cnt++;
                }
                for(int j = 0; j < digit_cnt/2; j++)
                {
                    temp = dest[num_start_idx+j];
                    dest[num_start_idx + j] = dest[index - 1 -j];
                    dest[index - 1 -j] = temp;
                }
            }
            dest[index++] = ch;
            length = 1;
            ch = src[i];
        }
    }
    if(length > 1)
    {
        num_start_idx = index;
        digit_cnt = 0;
        while(length >= 1)
        {
            dest[index++] = (length%10) + '0';
            length /= 10;
            digit_cnt++;
        }
        for(int j = 0; j < digit_cnt/2; j++)
        {
            temp = dest[num_start_idx+j];
            dest[num_start_idx+j] = dest[index - 1 -j];
            dest[index - 1 -j] = temp;
        }
    }
    dest[index++] = ch;

    dest[index] = '\0';
    n = index;
}

void runlength_decode(char* src, char* dest, int& n)
{
    char ch;
    int length = 0;
    int index = 0;
    for(int i = 0; i < n; i++)
    {
        ch = src[i];
        if(ch >= '0' && ch <= '9')
        {
            length *= 10;
            length += ch - '0';
        }
        else
        {
            if(length)
            {
                while(length)
                {
                    dest[index++] = ch;
                    length--;
                }
            }
            else
            {
                dest[index++] = ch;
            }
        }
    }
    if(length)
    {
        while(length)
        {
            dest[index++] = ch;
            length--;
        }
    }
    dest[index] = '\0';
    n = index;
}

void mtf_encode(char* src, int& n)
{
    Dll mtf_Table;
    DllNode* pNode;
    int loc;
    mtf_Table.head = &DllNodeArr[0];
    mtf_Table.head ->symbol = 0;
    mtf_Table.head ->prev = NULL;
    mtf_Table.head ->next = NULL;
    pNode = mtf_Table.head;
    for(int i = 1; i < ALPHABET; i++)
    {
        pNode->next = &DllNodeArr[i];
        pNode->next->symbol = i;
        pNode->next->prev = pNode;
        pNode->next->next = NULL;
        pNode = pNode->next;
    }
    for(int i = 0; i < n; i++)
    {
        pNode = mtf_Table.head;
        loc = 0;
        while(pNode->symbol != src[i])
        {
            pNode = pNode->next;
            loc++;
        }
        if(loc == 0)
        {
            src[i] = 0;
            continue;
        }
        pNode->prev->next = pNode->next;
        if(pNode->next)
        {
            pNode->next->prev = pNode->prev;
        }
        pNode->prev = NULL;
        mtf_Table.head -> prev = pNode;
        pNode->next = mtf_Table.head;
        mtf_Table.head = pNode;
        src[i] = loc;
    }
}

void mtf_decode(char* src, int& n)
{
    Dll mtf_Table;
    DllNode* pNode;
    int loc;
    mtf_Table.head = &DllNodeArr[0];
    mtf_Table.head ->symbol = 0;
    mtf_Table.head ->prev = NULL;
    mtf_Table.head ->next = NULL;
    pNode = mtf_Table.head;
    for(int i = 1; i < ALPHABET; i++)
    {
        pNode->next = &DllNodeArr[i];
        pNode->next->symbol = i;
        pNode->next->prev = pNode;
        pNode->next->next = NULL;
        pNode = pNode->next;
    }
    for(int i = 0; i < n; i++)
    {
        pNode = mtf_Table.head;
        loc = src[i];
        if(loc == 0)
        {
            src[i] = mtf_Table.head->symbol;
            continue;
        }
        for(int j = 0; j < loc; j++)
        {
            pNode = pNode->next;
        }
        pNode->prev->next = pNode->next;
        if(pNode->next)
        {
            pNode->next->prev = pNode->prev;
        }
        pNode->prev = NULL;
        mtf_Table.head -> prev = pNode;
        pNode->next = mtf_Table.head;
        mtf_Table.head = pNode;
        src[i] = mtf_Table.head->symbol;
    }
}

inline int parent(int i)
{
    return (i>>1);
}

inline int left(int i)
{
    return (i<<1);
}

inline int right(int i)
{
    return ((i<<1)|1);
}

void HeapifyDown(int idx)
{
    HuffNode* temp;
    int l = left(idx);
    int r = right(idx);
    int smallest = idx;
    if(l < HuffHeapCnt && HuffHeapArr[l]->freq < HuffHeapArr[smallest]->freq)
    {
        smallest = l;
    }
    if(r < HuffHeapCnt && HuffHeapArr[r]->freq < HuffHeapArr[smallest]->freq)
    {
        smallest = r;
    }
    if(smallest != idx)
    {
        temp = HuffHeapArr[smallest];
        HuffHeapArr[smallest] = HuffHeapArr[idx];
        HuffHeapArr[idx] = temp;
        HeapifyDown(smallest);
    }
}

void HeapifyUp(int idx)
{
    HuffNode* temp;
    int p = parent(idx);
    if(p != idx && HuffHeapArr[idx]->freq < HuffHeapArr[p]->freq)
    {
        temp = HuffHeapArr[idx];
        HuffHeapArr[idx] = HuffHeapArr[p];
        HuffHeapArr[p] = temp;
        HeapifyUp(p);
    }
}

HuffNode* insert(char _symbol, int freq)
{
    /*
    if(_symbol > 0)
    {
        printf("INSERT: %c - %d\n", _symbol, freq);
    }
    */

    HuffNodeArr[HuffNodeCnt].symbol = _symbol;
    HuffNodeArr[HuffNodeCnt].freq = freq;
    HuffNodeArr[HuffNodeCnt].left = NULL;
    HuffNodeArr[HuffNodeCnt].right = NULL;
    HuffHeapArr[HuffHeapCnt] = &HuffNodeArr[HuffNodeCnt];
    HuffNodeCnt++;
    HuffHeapCnt++;
    HeapifyUp(HuffHeapCnt-1);
    return &HuffNodeArr[HuffNodeCnt-1];
}

HuffNode* top()
{
    return HuffHeapArr[0];
}

void pop()
{
    HuffHeapArr[0] = HuffHeapArr[--HuffHeapCnt];
    HeapifyDown(0);
}

void BuildHuffmanTree(HuffNode* pNode, int _code, int _codelen)
{
    if(pNode)
    {
        if(pNode->left == NULL && pNode->right == NULL)
        {
            code[pNode->symbol] = _code;
            codelen[pNode->symbol] = _codelen;
            //printf("%c: %x - %d\n", pNode->symbol, _code, _codelen);
        }
        else
        {
            if(pNode->left)
            {
                BuildHuffmanTree(pNode->left, (_code<<1), _codelen+1);
            }
            if(pNode->right)
            {
                BuildHuffmanTree(pNode->right, ((_code<<1)|1), _codelen+1);
            }
        }
    }
}

void huffman_encode(char* src, char* dest, int& n)
{
    int symbolCnt = 0;
    int byteIdx = 0;
    int bitIdx = 0;
    int sentinel = ALPHABET;
    int _code;
    int _codelen;
    HuffNode *A, *B, *C;
    HuffNodeCnt = HuffHeapCnt = 0;

    for(int i = 0; i < SZ; i++)
    {
        dest[i] = 0;
    }

    for(int i = 0; i < ALPHABET; i++)
    {
        freq[i] = 0;
    }

    for(int i = 0; i < n; i++)
    {
        freq[src[i]]++;
    }

    for(int i = 0; i < ALPHABET; i++)
    {
        if(freq[i] > 0)
        {
            insert(i, freq[i]);
            symbolCnt++;
        }
        else if(sentinel == ALPHABET)
        {
            insert(i, 1);
            sentinel = i;
        }
    }
    src[n++] = sentinel;
    dest[byteIdx++] = sentinel;
    dest[byteIdx++] = symbolCnt;
    for(int i = 0; i < ALPHABET; i++)
    {
        if(freq[i] > 0)
        {
            dest[byteIdx++] = i;
            dest[byteIdx++] = (freq[i]&0x00FF0000)>>16;
            dest[byteIdx++] = (freq[i]&0x0000FF00)>>8;
            dest[byteIdx++] = (freq[i]&0x000000FF);
        }
    }

    while(HuffHeapCnt > 1)
    {
        A = top();
        pop();
        B = top();
        pop();
        C = insert(255, A->freq + B->freq);
        C->left = A;
        C->right = B;
    }
    BuildHuffmanTree(C, 0, 0);
    for(int i = 0; i < n; i++)
    {
        _code = code[src[i]];
        _codelen = codelen[src[i]];
        if(bitIdx + _codelen < 8)
        {
            dest[byteIdx] |= _code << (8 - (_codelen + bitIdx));
            bitIdx += _codelen;
        }
        else
        {
            dest[byteIdx++] |= _code >> ((_codelen + bitIdx) - 8);
            _codelen -= 8 - bitIdx;
            bitIdx = 0;
            while(_codelen >= 8)
            {
                _codelen -= 8;
                dest[byteIdx++] = ((_code >> _codelen)&0xFF);
            }
            if(_codelen > 0)
            {
                dest[byteIdx] = (_code&((1<<_codelen)-1)) << (8-_codelen);
                bitIdx = _codelen;
            }
        }
    }
    n = byteIdx + (bitIdx > 0);
}

void huffman_decode(char* src, char* dest, int& n)
{
    int symbolCnt = 0;
    int byteIdx = 0;
    int idx = 0;
    int bit = 0;
    char _symbol;
    char sentinel;
    int _freq;
    HuffNode *A, *B, *C;
    HuffNodeCnt = HuffHeapCnt = 0;

    sentinel = src[byteIdx++];
    insert(sentinel, 1);
    symbolCnt = src[byteIdx++];

    for(int i = 0; i < symbolCnt; i++)
    {
        _symbol = src[byteIdx++];
        _freq  = ((unsigned char)src[byteIdx++]) << 16;
        _freq |= ((unsigned char)src[byteIdx++]) << 8;
        _freq |= ((unsigned char)src[byteIdx++]);
        insert(_symbol, _freq);
    }

    while(HuffHeapCnt > 1)
    {
        A = top();
        pop();
        B = top();
        pop();
        C = insert(255, A->freq + B->freq);
        C->left = A;
        C->right = B;
    }
    BuildHuffmanTree(C, 0, 0);
    A = C;
    _symbol = 0;
    while(1)
    {
        for(int j = 7; j >= 0; j--)
        {
            bit = (src[byteIdx]&(1<<j))>>j;
            if(bit)
            {
                A = A->right;
            }
            else
            {
                A = A->left;
            }
            if(A->left == NULL && A->right == NULL)
            {
                _symbol = A->symbol;
                if(_symbol == sentinel)
                {
                    n = idx;
                    return;
                }
                dest[idx++] = _symbol;
                A = C;
            }
        }
        byteIdx++;
    }
}

void mystring_printf(char* str, int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%c ", str[i]);
    }
    printf("\n");
}

int main(void)
{
    int n = 0;
    printf("Enter string :\n");
    scanf("%s",Src);
    //printf("SOURCE: %s\n", src);
    while(Src[n] != '\0')
    {
        _Src[n] = Src[n];
        n++;
    }
    _Src[n] = '\0';
    //printf("SRC  SIZE : %d\n", n);

    //runlength_encode(Src, Dest, n);
    //printf("RLE   : ");
    //mystring_printf(Src, n);
    //printf("RLE  SIZE : %d\n", n);
    //mtf_encode(Dest, n);
    //printf("MTFE  : ");
    //mystring_printf(Src, n);
    //printf("MTFE SIZE : %d\n", n);

    huffman_encode(Src, Dest, n);
    //printf("HUFE  : ");
    //mystring_printf(Src, n);
    //printf("HUFE SIZE : %d\n", n);

    huffman_decode(Dest, Src, n);
    //printf("HUFD  : ");
    //mystring_printf(Dest, n);
    //printf("HUFD SIZE : %d\n", n);
    //mtf_decode(Dest, n);
    //printf("MTFD  : ");
    //mystring_printf(Src, n);
    //printf("MTFD SIZE : %d\n", n);
    //runlength_decode(Dest, Src, n);
    //printf("RLD   : ");
    //mystring_printf(Src, n);
    //printf("RLD  SIZE : %d\n", n);


    if(strcmp(Src, _Src) != 0)
    {
        printf("\nRESULT: ERROR\n");
    }
    else
    {
        printf("\nRESULT: PASS\n");
    }
    return 0;
}