/** 
    @file math.c 
    @author Alex Knight (ajknigh2)
    
    This program takes a simple math problem from the user and uses different processes 
    to solve each part, and then sends the value back to the parent process. It is expected
    that the input expression has numbers and operators separated by spaces.
    
    Compile instructions: This program is intended to be compiled and used on the NCSU
    Unity/Linux environment. Once logged-in (gained access to the environment), perform
    the following commands in the command-line interface:
    
    Command 1 (used for compiling): "gcc -Wall -std=c99 math.c -o math"
    Command 2 (used for running): "./math"
*/

#include <stdio.h>
#include <stdlib.h>     //exit
#include <stdbool.h>
#include <sys/wait.h>   //wait
#include <unistd.h>     //fork
#include <string.h>

/**
    Determines the number of digits within a number.
*/
int numDigs(int num) {
    int c = 0;
    int n = num;
    while(n != 0) {
        n = n / 10;
        c++;
    }
    return c;
}

/**
    Performs the actual math calculations that are a part of the expression.
*/
int performCalculation(char operator, int num1, int num2) {
    int result;
    if (operator == '+') {
        result = num1 + num2;
    } else if (operator == '-') {
        result = num1 - num2;
    } else if (operator == '*') {
        result = num1 * num2;
    } else if (operator == '/') {
        if (num2 != 0) {
            result = num1 / num2;
        } else {
            fprintf(stderr, "Error: Division by zero\n");
            exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Error: Invalid operator %c\n", operator);
        exit(EXIT_FAILURE);
    }
    return result;
}

// Function to evaluate the expression using multiple processes
int evaluateExpression(char* expression) {
    int result;
    int num1, num2;
    char operator;

    // Process the expression
    sscanf(expression, "%d", &num1);
    int offset = numDigs(num1);
    
    int numOperators = 0;
    for (int i = 0; expression[i] != '\0'; i++) {
        if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            numOperators++;
        }
    }

    // Create child processes dynamically based on the number of operators
    //int numProcesses = numOperators + 1;

    for ( int i = 0; i < numOperators; i++) {
        sscanf(expression + offset, " %c %d", &operator, &num2);
        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            result = performCalculation(operator, num1, num2);
            printf("PID %d calculated %d%c%d as %d\n", getpid(), num1, operator, num2, result);
            exit(result);
        } else {
            waitpid(pid, &result, 0);
            num1 = WEXITSTATUS(result);
            offset += 3 + numDigs(num2); // Move the offset to the next part of the expression
        }
    }

    return num1;
}

int main() {
    char expression[100];
    printf("Enter problem: ");
    fgets(expression, sizeof(expression), stdin);

    // Evaluate the expression using multiple processes
    int finalResult = evaluateExpression(expression);

    // Parent process
    printf("Final result: %d\n", finalResult);

    return 0;
}