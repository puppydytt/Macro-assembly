#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define instructionQuantity 4

enum {
    MOV,
    ADD,
    SUB,
    JNE
};

char *instructionList[instructionQuantity] = {"MOV", "ADD", "SUB", "JNE"};
int a = 0;
int b = 0;
int c = 0;
int d = 0;

bool srcOrImm(char *str) { //if num 0 | reg 1
    int i = 0;
    if (str[0] == '-') ++i;
    for (; i < strlen(str); i++) {
        if (str[i] - '0' > 9) return 1;
    }
    return 0;
}

int strToNum(char *str) {
    int i = 0;
    int res = 0;
    int sign = 1;
    if (str[0] == '-') {
        sign = -1;
        ++i;
    }
    for (; i < strlen(str); i++) {
        res *= 10;
        res += str[i] - '0';
    }

    return res * sign;
}


int *returnAddressOfSpecifiedRegister(char reg) {
    switch (reg) {
        case 'a':
            return &a;
        case 'b':
            return &b;
        case 'c':
            return &c;
        case 'd':
            return &d;
        default:
            printf("Called for not existing register");
            exit(0x123);
    }
}

int returnInstruction(char *instruction) {
    for (int i = 0; i < instructionQuantity; i++) {
        if (!strcmp(instruction, instructionList[i])) return i;
    }
    printf("None existing instruction were provided %s", instruction);
    exit(321);
}

int operandValue(char *operand) {
    if (srcOrImm(operand)) {
        return *returnAddressOfSpecifiedRegister(operand[0]);
    } else return strToNum(operand);
}

int setOperands(char ***callStack, char *instruction) {
    static int sizeOfStack = 0;
    int j = 0;
    int prevPos = 0;
    for (int i = 0; i <= strlen(instruction); i++) {
        if (instruction[i] == ' ' || instruction[i] == '\000') {
            callStack[sizeOfStack] = realloc(callStack[sizeOfStack], (j + 1) * sizeof(char *));
            callStack[sizeOfStack][j] = calloc(i + 1, sizeof(char));
            memcpy(callStack[sizeOfStack][j], &instruction[prevPos], i - prevPos);
            ++j;
            prevPos = i + 1;
        }
    }
    return ++sizeOfStack;
}

int main() {
    scanf("%d%d%d%d", &a, &b, &c, &d);
    int n = 0;
    scanf("%d", &n);
    fgetc(stdin);
    char ***callStack = calloc(n, sizeof(char *));

    for (int i = 0; i < n; i++) {
        char instruction[17];
        scanf("%[^\n]s", instruction);
        fgetc(stdin);
        setOperands(callStack, instruction);
    }

    for (int i = 0; i < n; i++) {
        switch (returnInstruction(callStack[i][0])) {
            case MOV:
                (*returnAddressOfSpecifiedRegister(callStack[i][1][0])) = operandValue(callStack[i][2]);
                break;
            case ADD:
                (*returnAddressOfSpecifiedRegister(callStack[i][1][0])) =
                        operandValue(callStack[i][2]) + operandValue(callStack[i][3]);
                break;
            case SUB:
                (*returnAddressOfSpecifiedRegister(callStack[i][1][0])) =
                        operandValue(callStack[i][2]) - operandValue(callStack[i][3]);
                break;
            case JNE:
                if (operandValue(callStack[i][2]) != operandValue(callStack[i][3])) {
                    i = operandValue(callStack[i][1]) - 1;
                } else break;
        }
    }
    printf("%d %d %d %d\n", a, b, c, d);
    return 0;
}