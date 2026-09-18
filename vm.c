/*
Assignment:
vm - HW1 PM/0 virtual machine

Author: Dhyan Suresh, Osmany Leyva Aldana

Language: C only

To Compile:
    gcc -Wall -Wextra -std=c11 -O2 vm.c -o vm

To Execute (on Eustis):
    ./vm <input_file>

where:
    <input_file> is the path to a text file holding one PM/0 instruction
                 per line, as three integers OP L M

Notes:
    - Implements the PM/0 virtual machine described in the homework
      instructions.
    - No heap allocation and no function-like macros. The PAS array is
      indexed, not walked with a pointer.
    - Does not implement any VM instruction as a separate function; the
      fetch-execute cycle runs directly in main.
    - Defines at most three functions: main, base, and one print function.
    - Exits with status 0 after a normal halt and with a non-zero status
      after any error.
    - Tested on Eustis.

Class: COP 3402 - Systems Software

Instructor: Jie Lin, Ph.D.

Due Date: See Webcourses
*/

#include <stdio.h>
static int PAS[1000]; // array of 1000 ints.


int base(int bp, int L);

void print_trace(const char *mnemonic, int L, int M, int PC, int BP, int SP);

int main(int argc, char *argv[]) {
    // argc/file check
    if (argc != 2) {
        printf("Usage: ./vm <input file>\n");
        return 1;
    }
    FILE *input_file =  fopen(argv[1], "r");

    if (input_file == NULL) {
        printf("Error: cannot open %s\n", argv[1]);
        return 1;
    }

    int OP, L, M;
    int start = 200;
    const char *nmemonic = NULL;

    while (1) {
        if (fscanf(input_file, "%d %d %d", &OP, &L, &M) != 3) { // store ints from file
            break;
        }
        // store instructions
        if (start + 2 <= 999) {
            PAS[start] = OP;
            PAS[start + 1] = L;
            PAS[start + 2] = M;
            start += 3;
        } else { // error condition
            printf("\nError: program too large for the text segment\n");
            fclose(input_file);
            return 1;
        }
    }
    fclose(input_file);

    // init register values
    int PC = 200;
    int BP = 999;
    int SP = 1000;

    while (1) {
        if (PC < 200 || PC + 2 >= start) {
            printf("\nError: program counter left the text segment\n");
            return 1;
        }
        // get instructions
        OP = PAS[PC];
        L = PAS[PC + 1];
        M = PAS[PC + 2];
        PC += 3;

        switch (OP) {
            case 1: // LIT
                nmemonic = "LIT";
                if (SP - 1 < start) {
                    printf("\nError: stack overflow\n");
                    return 1;
                }
                SP--;
                PAS[SP] = M;
                break;

            case 2:  // OPR
                switch (M) {
                case 0: // RTN
                nmemonic = "RTN";

                        SP = BP + 1;
                        BP = PAS[SP - 2];
                        PC = PAS[SP - 3];
                        break;
                case 1: // ADD
                nmemonic = "ADD";

                        PAS[SP + 1] = PAS[SP] + PAS[SP + 1];
                        SP++;
                        break;
                case 2: // SUB
                nmemonic = "SUB";

                        PAS[SP + 1] = PAS[SP + 1] - PAS[SP];
                        SP++;
                        break;
                case 3: // MUL
                nmemonic = "MUL";

                        PAS[SP + 1] = PAS[SP] * PAS[SP + 1];
                        SP++;
                        break;
                case 4: // DIV
                nmemonic = "DIV";

                        if (PAS[SP] == 0) {
                            printf("\nError: division by zero\n");
                            return 1;
                        }
                        PAS[SP + 1] = PAS[SP + 1] / PAS[SP];
                        SP++;
                        break;
                case 5: // EQL
                nmemonic = "EQL";

                        if (PAS[SP] == PAS[SP + 1]) {
                            PAS[SP + 1] = 1;
                        }
                        else {
                            PAS[SP + 1] = 0;
                        }
                        SP++;
                        break;
                case 6: // NEQ
                nmemonic = "NEQ";

                        if (PAS[SP] != PAS[SP + 1]) {
                            PAS[SP + 1] = 1;
                        }
                        else {
                            PAS[SP + 1] = 0;
                        }
                        SP++;
                        break;
                case 7: // LSS
                nmemonic = "LSS";

                        if (PAS[SP + 1] < PAS[SP]) {
                            PAS[SP + 1] = 1;
                        }
                        else {
                            PAS[SP + 1] = 0;
                        }
                        SP++;
                        break;
                case 8: // LEQ
                nmemonic = "LEQ";

                        if (PAS[SP + 1] <= PAS[SP]) {
                            PAS[SP + 1] = 1;
                        }
                        else {
                            PAS[SP + 1] = 0;
                        }
                        SP++;
                        break;
                case 9: // GTR
                nmemonic = "GTR";

                        if (PAS[SP + 1] > PAS[SP]) {
                            PAS[SP + 1] = 1;
                        }
                        else {
                            PAS[SP + 1] = 0;
                        }
                        SP++;
                        break;
                case 10: // GEQ
                nmemonic = "GEQ";

                        if (PAS[SP + 1] >= PAS[SP]) {
                            PAS[SP + 1] = 1;
                        }
                        else {
                            PAS[SP + 1] = 0;
                        }
                        SP++;
                        break;
                default:
                        printf("\nError: unknown OPR sub-operation\n");
                        return 1;
                }
            break;

        case 3:  // LOD
               nmemonic = "LOD"; 
                if (M > 999 || M <= 200){
                  printf("Error: data address out of range");
                  return 1;
                }
                 SP--;
                 PAS[SP] = PAS[base(BP, L) - M];
            break;
        case 4:  // STO
               nmemonic = "STO"; 

                if (M > 999 || M <= 200){
                  printf("Error: data address out of range");
                  return 1;
                }

                 PAS[base(BP, L) - M] = PAS[SP];
                 SP++;
            break;

            case 5:  // CAL
               nmemonic = "CAL"; 

                if (  M < 200 || M > start ){
                  printf("Error: data address out of range");
                  return 1;
                }

                PAS[SP-1] = base(BP, L);
                PAS[SP-2] = BP;
                PAS[SP-3] = PC;

                BP = SP - 1;
                PC = M;

                break;

            case 6:  // INC
               nmemonic = "INC"; 

                if (SP - M < start) {
                    printf("\nError: stack overflow\n");
                    return 1;
                }
                SP = SP - M;
                break;

            case 7:  // JMP
               nmemonic = "JMP"; 
                PC = M;
                break;

            case 8: // JPC
               nmemonic = "JMC"; 


                if (PAS[SP] == 0) {
                    PC = M;
                }
                SP = SP + 1;
                break;

            case 9: // SYS
               nmemonic = "SYS"; 

                switch (M) {
                    case 1: // write
                        printf("Output result is: %d\n", PAS[SP]);
                        SP++;
                        break;
                    case 2: // read
                        if (SP - 1 < start) {
                            printf("\nError: stack overflow\n");
                            return 1;
                        }
                        printf("Please Enter an Integer: ");
                        SP--;
                        scanf("%d", &PAS[SP]);
                        printf("%d\n", PAS[SP]);
                        break;
                    case 3: // halt
                        // TODO Osmany:  print the final SYS trace lines before the return.
                        return 0;
                    default:
                        printf("\nError: unknown SYS operation\n");
                        return 1;
                }
                break;
            default:
                printf("\nError: unknown opcode\n");
                return 1;
        }


        print_trace(nmemonic, L, M, PC, BP, SP);
    }
    return 0;
}


int base(int bp, int L)
{
  int arb = bp;

  while (L > 0) {
    arb = PAS[arb];
    L--;
  }

  return arb;
}

void print_trace(const char *mnemonic, int L, int M, int PC, int BP, int SP) {
    printf("%s\t%d\t%d\t%d\t%d\t%d\t", mnemonic, L, M, PC, BP, SP);
}
