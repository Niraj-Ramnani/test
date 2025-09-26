#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h> 


#define MAX_SIZE 100


int values[MAX_SIZE];
int val_top = -1;


char ops[MAX_SIZE];
int op_top = -1;

// --- Function Prototypes ---
void push_val(int val);
int pop_val();
void push_op(char op);
char pop_op();
char peek_op();
int precedence(char op);
void applyOp();
void handleError(const char* message);

// --- Main Program ---
int main() {
    char expression[MAX_SIZE];

    // Read the entire line of input, including spaces
    if (fgets(expression, MAX_SIZE, stdin) == NULL) {
        handleError("Error: Invalid expression.");
    }
    // Remove the trailing newline character from fgets()
    expression[strcspn(expression, "\n")] = 0;
    
    // --- Main Evaluation Loop ---
    for (int i = 0; i < strlen(expression); i++) {
        // 1. Skip whitespace
        if (isspace(expression[i])) {
            continue;
        }
        // 2. If the character is a digit, parse the full number
        else if (isdigit(expression[i])) {
            int val = 0;
            // Continue reading until a non-digit character is found
            while (i < strlen(expression) && isdigit(expression[i])) {
                val = (val * 10) + (expression[i] - '0');
                i++;
            }
            push_val(val);
            i--; // Decrement i because the outer for-loop will increment it
        }
        // 3. If the character is an operator
        else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
            // While the operator stack is not empty and the top operator has higher or equal precedence
            while (op_top != -1 && precedence(peek_op()) >= precedence(expression[i])) {
                applyOp();
            }
            // Push the current operator to the operator stack
            push_op(expression[i]);
        }
        // 4. If the character is invalid
        else {
            handleError("Error: Invalid expression.");
        }
    }

    // --- Final Calculation ---
    // After parsing the whole string, apply remaining operations
    while (op_top != -1) {
        applyOp();
    }

    // The final result is the only number left on the value stack
    if (val_top == 0 && op_top == -1) {
        printf("%d\n", pop_val());
    } else {
        // This catches malformed expressions like "5 5" or "3 *"
        handleError("Error: Invalid expression.");
    }

    return 0;
}


// --- Stack and Helper Function Implementations ---

/**
 * @brief Prints an error message and terminates the program.
 */
void handleError(const char* message) {
    printf("%s\n", message);
    exit(0);
}

/**
 * @brief Pushes an integer onto the value stack.
 */
void push_val(int val) {
    if (val_top >= MAX_SIZE - 1) {
        handleError("Error: Stack Overflow.");
    }
    values[++val_top] = val;
}

/**
 * @brief Pops an integer from the value stack. Handles underflow.
 */
int pop_val() {
    if (val_top < 0) {
        handleError("Error: Invalid expression.");
    }
    return values[val_top--];
}

/**
 * @brief Pushes a character onto the operator stack.
 */
void push_op(char op) {
    if (op_top >= MAX_SIZE - 1) {
        handleError("Error: Stack Overflow.");
    }
    ops[++op_top] = op;
}

/**
 * @brief Pops a character from the operator stack.
 */
char pop_op() {
    if (op_top < 0) {
        handleError("Error: Invalid expression.");
    }
    return ops[op_top--];
}

/**
 * @brief Returns the top character of the operator stack without removing it.
 */
char peek_op() {
    return ops[op_top];
}

/**
 * @brief Determines the precedence of an operator.
 * Multiplication and division have higher precedence (2) than addition and subtraction (1).
 */
int precedence(char op) {
    if (op == '*' || op == '/') return 2;
    if (op == '+' || op == '-') return 1;
    return 0;
}

/**
 * @brief Pops one operator and two values, performs the operation,
 * and pushes the result back onto the value stack.
 */
void applyOp() {
    // Pop two values and one operator
    int val2 = pop_val();
    int val1 = pop_val();
    char op = pop_op();

    switch (op) {
        case '+':
            push_val(val1 + val2);
            break;
        case '-':
            push_val(val1 - val2);
            break;
        case '*':
            push_val(val1 * val2);
            break;
        case '/':
            if (val2 == 0) {
                handleError("Error: Division by zero.");
            }
            push_val(val1 / val2); // Integer division
            break;
    }
}