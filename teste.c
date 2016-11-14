#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


struct myCache
{
    int valid;
    char *tag;
    char *block;
};

/*
sim [-h] <cache size> <associativity> <block size> <replace alg> <write policy>
<trace file>
*/

//God willing I come up with a better Hex to Bin convertion that maintains the beginning 0s...
void hex2bin(char input[], char output[])
{
    int i;
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    int x = 4;
    int size;
    size = strlen(input);

    for (i = 0; i < size; i++)
    {
        if (input[i] == '0')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '0';
            output[i*x +c] = '0';
            output[i*x +d] = '0';
        }
        else if (input[i] == '1')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '0';
            output[i*x +c] = '0';
            output[i*x +d] = '1';
        }
        else if (input[i] == '2')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '0';
            output[i*x +c] = '1';
            output[i*x +d] = '0';
        }
        else if (input[i] == '3')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '0';
            output[i*x +c] = '1';
            output[i*x +d] = '1';
        }
        else if (input[i] == '4')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '1';
            output[i*x +c] = '0';
            output[i*x +d] = '0';
        }
        else if (input[i] == '5')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '1';
            output[i*x +c] = '0';
            output[i*x +d] = '1';
        }
        else if (input[i] == '6')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '1';
            output[i*x +c] = '1';
            output[i*x +d] = '0';
        }
        else if (input[i] == '7')
        {
            output[i*x +a] = '0';
            output[i*x +b] = '1';
            output[i*x +c] = '1';
            output[i*x +d] = '1';
        }
        else if (input[i] == '8')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '0';
            output[i*x +c] = '0';
            output[i*x +d] = '0';
        }
        else if (input[i] == '9')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '0';
            output[i*x +c] = '0';
            output[i*x +d] = '1';
        }
        else if (input[i] == 'a')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '0';
            output[i*x +c] = '1';
            output[i*x +d] = '0';
        }
        else if (input[i] == 'b')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '0';
            output[i*x +c] = '1';
            output[i*x +d] = '1';
        }
        else if (input[i] == 'c')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '1';
            output[i*x +c] = '0';
            output[i*x +d] = '0';
        }
        else if (input[i] == 'd')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '1';
            output[i*x +c] = '0';
            output[i*x +d] = '1';
        }
        else if (input[i] == 'e')
        {
            output[i*x +a] = '1';
            output[i*x +b] = '1';
            output[i*x +c] = '1';
            output[i*x +d] = '0';
        }
        else if (input[i] == 'f') {
            output[i*x +a] = '1';
            output[i*x +b] = '1';
            output[i*x +c] = '1';
            output[i*x +d] = '1';
        }
    }

    output[32] = '\0';
}




int main(int argc, char* argv[])
{
    FILE *tracefile;
    char readwrite;
    int trash;
    int cachesize;
    int blocksize;
    int setnumber;
    int blockbytes;
    int setbits;
    int blockbits;
    int tagsize;
    int m;
    int count = 0;
    int count2 = 0;
    int count3 = 0;
    int i;
    int j;
    int xindex;
    int jindex;
    int kindex;
    int lindex;
    int setadd;
    int totalset;
    int writeMiss = 0;
    int writeHit = 0;
    int cacheMiss = 0;
    int cacheHit = 0;
    int read = 0;
    int write = 0;
    int size;
    int extra;

    char bbits[100];
    char sbits[100];
    char tbits[100];
    char output[100];
    char input[100];
    char origtag[100];


    if (argc != 7)
    {
        if (strcmp(argv[0], "-h"))
        {
            printf("./sim2 <cache size> <associativity> <block size> <replace alg> <write policy> <trace file>\n");
            return 0;
        }
        else
        {
            fprintf(stderr, "Error: wrong number of parameters.\n");
            return -1;
        }
    }

    tracefile = fopen(argv[6], "r");

    if(tracefile == NULL)
    {
        fprintf(stderr, "Error: File is NULL.\n");
        return -1;
    }

    //Determining size of sbits, bbits, and tag
    cachesize = atoi(argv[1]);
    blocksize = atoi(argv[3]);
    setnumber = (cachesize/blocksize);
    printf("setnumber: %d\n", setnumber);
    setbits = (round((log(setnumber))/(log(2))));
    printf("sbits: %d\n", setbits);
    blockbits = log(blocksize)/log(2);
    printf("bbits: %d\n", blockbits);
    tagsize = 32 - (blockbits + setbits);
    printf("t: %d\n", tagsize);

    struct myCache newCache[setnumber];

    //Allocating Space for Tag Bits, initiating tag and valid to 0s
    for(i=0;i<setnumber;i++)
    {
        newCache[i].tag = (char *)malloc(sizeof(char)*(tagsize+1));
        for(j=0;j<tagsize;j++)
        {
            newCache[i].tag[j] = '0';
        }
        newCache[i].valid = 0;
    }

    while(fgetc(tracefile)!='#')
    {
        setadd = 0;
        totalset = 0;
        //read in file
        fseek(tracefile,-1,SEEK_CUR);
        fscanf(tracefile, "%x: %c %s\n", &trash, &readwrite, origtag);

        //shift input Hex
        size = strlen(origtag);
        extra = (10 - size);
        for(i=0; i<extra; i++)
            input[i] = '0';

        for(i=extra, j=0; i<(size-(2-extra)); j++, i++)
            input[i]=origtag[j+2];

        input[8] = '\0';

        // Convert Hex to Binary
        hex2bin(input, output);

        //Resolving the Address into tbits, sbits, bbits
        for (xindex=0, jindex=(32-blockbits); jindex<32; jindex++, xindex++)
    {
            bbits[xindex] = output[jindex];
        }
    bbits[xindex]='\0';

    for (xindex=0, kindex=(32-(blockbits+setbits)); kindex<32-(blockbits); kindex++, xindex++){
        sbits[xindex] = output[kindex];
    }
    sbits[xindex]='\0';

    for (xindex=0, lindex=0; lindex<(32-(blockbits+setbits)); lindex++, xindex++){
        tbits[xindex] = output[lindex];
    }
    tbits[xindex]='\0';

    //Convert set bits from char array into ints
    for(xindex = 0, kindex = (setbits -1); xindex < setbits; xindex ++, kindex--)
        {
            if (sbits[xindex] == '1')
                setadd = 1;
            if (sbits[xindex] == '0')
                setadd = 0;
            setadd = setadd * pow(2, kindex);
            totalset += setadd;
        }

        //Calculating Hits and Misses
        if (newCache[totalset].valid == 0)
        {
            newCache[totalset].valid = 1;
            strcpy(newCache[totalset].tag, tbits);
        }

        else if (newCache[totalset].valid == 1)
        {
            if(strcmp(newCache[totalset].tag, tbits) == 0)
            {
                if (readwrite == 'W')
                {
                    cacheHit++;
                    write++;
                }
                if (readwrite == 'R')
                    cacheHit++;
            }
            else
            {
                if (readwrite == 'R')
                {
                    cacheMiss++;
                    read++;
                }
                if (readwrite == 'W')
                {
                    cacheMiss++;
                    read++;
                    write++;
                }
                strcpy(newCache[totalset].tag, tbits);
            }
        }
    }
    printf("Hits: %d\n", cacheHit);
    printf("Misses: %d\n", cacheMiss);
    printf("Writes: %d\n", write);
    printf("Reads: %d\n", read);
}
