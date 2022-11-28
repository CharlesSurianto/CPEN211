/*
 * Machine language writer for Potato Machine™
 * How to use: edit the customizable portion of this file (line 187) then run
 * 
 * *** Any use of this program is at your own risk ***
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Counter = 0;
char *OutputBuffer;

char *decToBin(int width, int value)
{
    char *temp = (char *)malloc((width + 1) * sizeof(char));
    temp[0] = 0;
    int mask;
    if (width > 0)
        mask = 1 << (width - 1);
    for (int i = 0; mask > 0; i++, mask >>= 1)
    {
        if (mask & value)
            temp[i] = '1';
        else
            temp[i] = '0';
    }
    temp[width] = 0;
    return temp;
}

char *decToHex(int value)
{
    char *temp = (char *)malloc(3 * sizeof(char));
    if (value <= 15)
        sprintf(temp, "0%x", value);
    else
        sprintf(temp, "%x", value);
    return temp;
}

/*
Assembly syntax Potato Machine™ 16-bit encoding Operation
                    15 14 13 12 11 10 9 8 7 6 5 4 3 2 1 0

Move Instructions       opcode op   3b    8b
MOV Rn,#<im8>           1 1 0 1 0   Rn    im8           R[Rn] = sx(im8)
MOV Rd,Rm{,<sh op>}     1 1 0 0 0 0 0 0   Rd   sh   Rm  R[Rd] = sh Rm

ALU Instructions       opcode ALUop 3b    3b   2b   3b
ADD Rd,Rn,Rm{,<sh op>}  1 0 1 0 0   Rn    Rd   sh   Rm  R[Rd]=R[Rn]+sh Rm
CMP Rn,Rm{,<sh op>}     1 0 1 0 1   Rn  0 0 0  sh   Rm  status=f(R[Rn]-sh Rm)
AND Rd,Rn,Rm{,<sh op>}  1 0 1 1 0   Rn    Rd   sh   Rm  R[Rd]=R[Rn]&sh Rm
MVN Rd,Rm{,<sh op>}     1 0 1 1 1 0 0 0   Rd   sh   Rm  R[Rd]= ˜sh Rm

Memory Instructions     opcode op   3b    3b      5b
LDR Rd,[Rn{,#<im5>}]    0 1 1 0 0   Rn    Rd      im5   R[Rd]=M[R[Rn]+sx(im5)]
STR Rd,[Rn{,#<im5>}]    1 0 0 0 0   Rn    Rd      im5   M[R[Rn]+sx(im5)]=R[Rd]

Special Instructions    opcode      not used
HALT                    1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 go to halt state
*/

void mov_imm(int Rd, int im8)
{
    char *binRd = decToBin(3, Rd);
    char *binIm8 = decToBin(8, im8);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 11010%s%s\n", counterHex, binRd, binIm8);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binIm8);
    free(counterHex);
}
void mov(int Rd, int Rm, int sh)
{
    char *binRd = decToBin(3, Rd);
    char *binRm = decToBin(3, Rm);
    char *binSh = decToBin(2, sh);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 11000000%s%s%s\n", counterHex, binRd, binSh, binRm);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binRm);
    free(counterHex);
}
void add(int Rd, int Rn, int Rm, int sh)
{
    char *binRd = decToBin(3, Rd);
    char *binRn = decToBin(3, Rn);
    char *binRm = decToBin(3, Rm);
    char *binSh = decToBin(2, sh);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 10100%s%s%s%s\n", counterHex, binRn, binRd, binSh, binRm);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binRn);
    free(binRm);
    free(counterHex);
}
void cmp(int Rn, int Rm, int sh)
{
    char *binRn = decToBin(3, Rn);
    char *binRm = decToBin(3, Rm);
    char *binSh = decToBin(2, sh);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 10101%s000%s%s\n", counterHex, binRn, binSh, binRm);
    strcat(OutputBuffer, buffer);
    free(binRn);
    free(binRm);
    free(counterHex);
}
void and_(int Rd, int Rn, int Rm, int sh)
{
    char *binRd = decToBin(3, Rd);
    char *binRn = decToBin(3, Rn);
    char *binRm = decToBin(3, Rm);
    char *binSh = decToBin(2, sh);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 10110%s%s%s%s\n", counterHex, binRn, binRd, binSh, binRm);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binRn);
    free(binRm);
    free(counterHex);
}
void mvn(int Rd, int Rm, int sh)
{
    char *binRd = decToBin(3, Rd);
    char *binRm = decToBin(3, Rm);
    char *binSh = decToBin(2, sh);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 10111000%s%s%s\n", counterHex, binRd, binSh, binRm);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binRm);
    free(counterHex);
}
void ldr(int Rd, int Rn, int im5)
{
    char *binRd = decToBin(3, Rd);
    char *binRn = decToBin(3, Rn);
    char *binIm5 = decToBin(5, im5);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 01100%s%s%s\n", counterHex, binRn, binRd, binIm5);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binRn);
    free(counterHex);
}
void str(int Rd, int Rn, int im5)
{
    char *binRd = decToBin(3, Rd);
    char *binRn = decToBin(3, Rn);
    char *binIm5 = decToBin(5, im5);
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 10000%s%s%s\n", counterHex, binRn, binRd, binIm5);
    strcat(OutputBuffer, buffer);
    free(binRd);
    free(binRn);
    free(counterHex);
}
void halt()
{
    char *counterHex = decToHex(Counter++);
    char buffer[30];
    sprintf(buffer, "@%s 1110000000000000\n", counterHex);
    strcat(OutputBuffer, buffer);
    free(counterHex);
}

int main()
{
    OutputBuffer = (char *)malloc(10000 * sizeof(char));
    OutputBuffer[0] = 0;

    /* === start of customizable part === */

    char fileName[] = "output.txt"; // output file name (with extension)

    // start of assembly
    // mov_imm(...);

    /* ===  end of customizable part  === */

    char *lineBuffer = (char *)malloc(30 * sizeof(char));
    for (int i = Counter; i < 256; i++)
    {
        char *counterHex = decToHex(i);
        sprintf(lineBuffer, "@%s 0000000000000000\n", counterHex);
        strcat(OutputBuffer, lineBuffer);
        free(counterHex);
    }

    FILE *outputFile = fopen(fileName, "w");
    fprintf(outputFile, "%s", OutputBuffer);
    free(lineBuffer);
    free(OutputBuffer);
    fclose(outputFile);

    return 0;
}
