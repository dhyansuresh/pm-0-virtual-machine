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

                        break;
                case 1: // ADD

                        break;
                case 2: // SUB

                        break;
                case 3: // MUL

                        break;
                case 4: // DIV

                        break;
                case 5: // EQL

                        break;
                case 6: // NEQ

                        break;
                case 7: // LSS

                        break;
                case 8: // LEQ

                        break;
                case 9: // GTR

                        break;
                case 10: // GEQ

                        break;
                }
                break;

            case 3:  // LOD

                break;

            case 4:  // STO

                break;

            case 5:  // CAL

                break;

            case 6:  // INC
                if (SP - M < start) {
                    printf("\nError: stack overflow\n");
                    return 1;
                }
                SP = SP - M;
                break;

            case 7:  // JMP
                PC = M;
                break;

            case 8: // JPC
                if (PAS[SP] == 0) {
                    PC = M;
                }
                SP = SP + 1;
                break;

            case 9: // SYS
                switch (M) {
                    case 1:
                        printf("Output result is: %d\n", PAS[SP]);
                        SP++;
                        break;
                    case 2:
                        if (SP - 1 < start) {
                            printf("\nError: stack overflow\n");
                            return 1;
                        }
                        printf("Please Enter an Integer: ");
                        SP--;
                        scanf("%d", &PAS[SP]);
                        printf("%d\n", PAS[SP]);
                        break;
                    case 3:
                        // TODO Osmany:  print the final SYS trace lines before the return.
                        return 0;
                    default:
                        printf("\nError: unknown SYS operation\n");
                        return 1;

                }

                break;

            default:

                break;
        }
    }
}
