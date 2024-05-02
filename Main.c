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

// BASE
char src[SZ];
char _src[SZ];
char dest[SZ];

// BWT
//int cnt[ALPHABET];
//int bucket_start[ALPHABET];
//int bucket_end[ALPHABET];
int pos[SZ];
int pos_next[SZ];
//int ls_array[SZ];


int BufIdx;
int cnt[SZ];
int classes;
int class_curr[SZ];
int class_next[SZ];


// MTF 
char symbols[ALPHABET];
DllNode DllNodeArr[ALPHABET];



void BWT(char* src, char* dest, int& n)
{
    int j, k;
    src[n] = '$';
    n++;
    src[n] = '\0';

    BufIdx = 0;
    for(int i = 0; i < ALPHABET; i++)
    {
        cnt[i] = 0;
    }

    for(int i = 0; i < n; i++)
    {
        cnt[src[i]]++;
    }

    for(int i = 1; i < ALPHABET; i++)
    {
        cnt[i] += cnt[i-1];
    }

    for(int i = 0; i < n; i++)
    {
        pos[--cnt[src[i]]] = i;
    }

    class_curr[pos[0]] = 0;
    classes = 0;
    for(int i = 1; i < n; i++)
    {
        if(src[pos[i]] != src[pos[i-1]])
        {
            classes++;
        }
        class_curr[pos[i]] = classes;
    }
    classes++;

    for(k = 1; k < n; k <<= 1)
    {
        for(int i = 0; i < classes; i++)
        {
            cnt[i] = 0;
        }

        for(int i = 0; i < n; i++)
        {
            j = pos[i] - k;
            if(j < 0)
            {
                j += n;
            }
            cnt[class_curr[j]]++;
            pos_next[i] = j;
        }

        for(int i = 1; i < classes; i++)
        {
            cnt[i] += cnt[i-1];
        }
        
        for(int i = n-1; i >=0; i--)
        {
            j = pos_next[i];
            pos[--cnt[class_curr[j]]] = j;
        }

        class_next[pos[0]] = 0;
        classes = 0;
        for(int i = 1; i < n; i++)
        {
            if(class_curr[pos[i-1]] != class_curr[pos[i]] || class_curr[(pos[i-1]+k)%n] != class_curr[(pos[i]+k)%n])
            {
                classes++;
            }
            class_next[pos[i]] = classes;
        }
        classes++;
        for(int i = 0; i < n; i++)
        {
            class_curr[pos[i]] = class_next[pos[i]];
        }
        if(classes == n)
        {
            break;
        }
    }

    for(int i = 0; i < n; i++)
    {
        pos[i] = pos[i] - 1;
        if(pos[i] < 0)
        {
            pos[i] += n;
        }
        dest[i] = src[pos[i]];
    }
    dest[n] = '\0';
}

/*
void BWT(char* src, char* dest, int& n)
{
    int j, k, temp;
    src[n] = '$';
    n++;
    src[n] = '\0';
    ls_array[n-1] = 1;

    for(int i = n-2; i >=0; i--)
    {
        if(src[i] == src[i+1])
        {
            ls_array[i] = ls_array[i+1];
        }
        else
        {
            ls_array[i] = (src[i] < src[i+1]);
        }
    }

    for(int i = 0; i < ALPHABET; i++)
    {
        cnt[i] = 0;
    }

    for(int i = 0; i < n; i++)
    {
        cnt[src[i]]++;
        pos[i] = -1;
    }

    bucket_start[0] = 0;
    bucket_end[0] = cnt[0];
    for(int i = 1; i < ALPHABET; i++)
    {
        bucket_start[i] = cnt[i-1];
        cnt[i] += cnt[i-1];
        bucket_end[i] = cnt[i];
    }

    for(int i = n-1; i > 0; i--)
    {
        if(ls_array[i] == 1 && ls_array[i-1] == 0)
        {
            pos[--bucket_end[src[i]]] = i;
            ls_array[i] = -1;
        }
    }

    printf("---------------------------------\n");
    for(int i = 0; i < n; i++)
    {
        printf("pos[%2d]: %2d\n", i, pos[i]);
    }
    printf("---------------------------------\n");

    for(int i = 0; i < n; i++)
    {
        if(ls_array[i] == 0)
        {
            pos[bucket_start[src[i]]++] = i;
        }
    }

    printf("---------------------------------\n");
    for(int i = 0; i < n; i++)
    {
        printf("pos[%2d]: %2d\n", i, pos[i]);
    }
    printf("---------------------------------\n");

    for(int i = n-1; i >= 0; i--)
    {
        if(ls_array[i] == 1)
        {
            pos[--bucket_end[src[i]]] = i;
        }
    }

    printf("---------------------------------\n");
    for(int i = 0; i < n; i++)
    {
        printf("pos[%2d]: %2d\n", i, pos[i]);
    }
    printf("---------------------------------\n");

    for(int i = 0; i < n; i++)
    {
        pos[i] = pos[i] - 1;
        if(pos[i] < 0)
        {
            pos[i] += n;
        }
        dest[i] = src[pos[i]];
    }
    dest[n] = '\0';
}
*/

void IBWT(char* src, char* dest, int& n)
{
    for(int i = 0; i < ALPHABET; i++)
    {
        cnt[i] = 0;
    }

    for(int i = 0; i < n; i++)
    {
        cnt[src[i]]++;
    }

    for(int i = 1; i < ALPHABET; i++)
    {
        cnt[i] += cnt[i-1];
    }

    for(int i = n-1; i >=0; i--)
    {
        pos[--cnt[src[i]]] = i;
    }
    
    int index = pos[0];
    for(int i = 0; i < n-1; i++)
    {
        index = pos[index];
        dest[i] = src[index];
    }
    dest[--n] = '\0';
}

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

void arithmetic_encode(char* src, char* dest, int& n)
{
    src[n++] = '|';
    src[n] = '\0';
    for(int i = 0; i < ALPHABET; i++)
    {
        cnt[i] = 0;
    }

    for(int i = 0; i < n; i++)
    {
        cnt[src[i]]++;
    }

    for(int i = 1; i < ALPHABET; i++)
    {
        cnt[i] += cnt[i-1];
    }

    int index = 0;
    char ch;
    unsigned int lo = 0;
    unsigned int hi = -1;
    unsigned int range = hi - lo;


    for(int i = 0; i < ALPHABET; i++)
    {
        dest[index++] = cnt[i]>>24;
        dest[index++] = (cnt[i] >> 16) & 0xFF;
        dest[index++] = (cnt[i] >> 8)  & 0xFF;
        dest[index++] = cnt[i] & 0xFF;
    }

    for(int i = 0; i < n; i++)
    {
        ch = src[i];
        lo += ((unsigned long long)range*(unsigned long long)cnt[ch-1])/cnt[ALPHABET-1];
        hi -= ((unsigned long long)range*(unsigned long long)cnt[ch])/cnt[ALPHABET-1];
        range = hi - lo;
        if(range < (1<<24))
        {
            dest[index++] = lo >> 24;
            lo <<= 8;
            hi <<= 8;
            range = hi - lo;
        }
    }
    dest[index++] = lo >> 24;
    dest[index++] = (lo >> 16) & 0xFF;
    dest[index++] = (lo >> 8)  & 0xFF;
    dest[index++] = lo & 0xFF;
    dest[index] = '\0';
    n = index;
}

void arithmetic_decode(char* src, char* dest, int& n)
{
    int index = 0;
    char ch;
    unsigned int lo = 0;
    unsigned int hi = -1;
    unsigned int range = hi - lo;

    for(int i = 0; i < ALPHABET; i++)
    {
        cnt[i]  = src[index++] << 24;
        cnt[i] |= src[index++] << 16;
        cnt[i] |= src[index++] << 8;
        cnt[i] |= src[index++];
    }

    while(index < n)
    {
        ch = src[index];
        for(int i = 0; i < ALPHABET; i++)
        {
            if(ch == 'a')
            {
                
            }
        }
    }
}

void mystring_printf(char* str, int n)
{
    for(int i = 0; i < n; i++)
    {
        printf("%c",str[i]);
    }
    printf("\n");
}

int main(void)
{
    int n = 0;
    printf("Enter string for BWT:\n");
    scanf("%s",src);
    //printf("SOURCE: %s\n", src);
    while(src[n] != '\0')
    {
        _src[n] = src[n];
        n++;
    }
    _src[n] = '\0';
    printf("SRC  SIZE : %d\n", n);

    BWT(src, dest, n);
    //printf("BWT   : ");
    //mystring_printf(dest, n);
    printf("BWT  SIZE : %d\n", n);
    runlength_encode(dest, src, n);
    //printf("RLE   : ");
    //mystring_printf(src, n);
    printf("RLE  SIZE : %d\n", n);
    mtf_encode(src, n);
    //printf("MTFE  : ");
    //mystring_printf(src, n);
    printf("MTFE SIZE : %d\n", n);



    mtf_decode(src, n);
    //printf("MTFD  : ");
    //mystring_printf(src, n);
    printf("MTFD SIZE : %d\n", n);
    runlength_decode(src, dest, n);
    //printf("RLD   : ");
    //mystring_printf(dest, n);
    printf("RLD  SIZE : %d\n", n);
    IBWT(dest, src, n);
    //printf("IBWT  : ");
    //mystring_printf(src, n);
    printf("IBWT SIZE : %d\n", n);
    if(strcmp(src, _src) != 0)
    {
        printf("\nRESULT: ERROR\n");
    }
    else
    {
        printf("\nRESULT: PASS\n");
    }
    return 0;
}