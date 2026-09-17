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

int main(int argc, char *argv[]) {

    // registers
    int PC = 200;
    int BP = 999;
    int SP = 1000;

    int OP = PAS[PC];
    int L = PAS[PC + 1];
    int M = PAS[PC + 2];

    switch (OP) {
        case 1: // LIT
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
                 SP--;
                 PAS[SP] = PAS[base(BP, L) - M];
            break;
        case 4:  // STO
                 PAS[base(BP, L) - M] = PAS[SP];
                 SP++;
            break;

        case 5:  // CAL

            break;

        case 6:  // INC

            break;

        case 7:  // JMP
            PC = M;
            break;

        case 8: // JPC

            break;

        case 9: // SYS

            break;

        default:

            break;

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
