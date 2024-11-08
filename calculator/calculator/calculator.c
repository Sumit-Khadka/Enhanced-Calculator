#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define MAX_EXPR_LEN 100

/*
 * Simple calculator that evaluates expressions with +, -, *, /, %, and parentheses.
 */

double evaluateExpression(const char* expression); // Evaluates the given arithmetic expression
double applyOperator(double a, double b, char op); // Applies an operator to two operands
int precedence(char op);                          // Returns precedence level of an operator
double parseNumber(const char** expr);            // Parses a number from the expression string

int main() {
    char expression[MAX_EXPR_LEN];
    double result;
    char choice = 'y';

    printf("Welcome to the Enhanced Calculator!\n");

    while (choice == 'y' || choice == 'Y') {
        printf("Enter an expression (e.g., 3 + 5 * 2): ");
        fgets(expression, MAX_EXPR_LEN, stdin);

        result = evaluateExpression(expression);
        printf("Result: %.2lf\n", result);

        printf("Do you want to evaluate another expression? (y/n): ");
        scanf(" %c", &choice);
        getchar();  // Consume leftover newline character
    }

    printf("Calculator Finished.\n");
    return 0;
}

double evaluateExpression(const char* expression) {
    double values[MAX_EXPR_LEN];  // Stack for numbers
    char ops[MAX_EXPR_LEN];       // Stack for operators
    int valuesTop = -1, opsTop = -1;
    const char* expr = expression;

    while (*expr != '\0' && *expr != '\n') {
        if (isspace(*expr)) {
            expr++;
            continue;
        }

        if (isdigit(*expr) || *expr == '.') {
            double value = parseNumber(&expr);  // Parse number from expression
            values[++valuesTop] = value;
        }
        else if (*expr == '(') {
            ops[++opsTop] = *expr;
            expr++;
        }
        else if (*expr == ')') {
            while (opsTop >= 0 && ops[opsTop] != '(') {
                double b = values[valuesTop--];
                double a = values[valuesTop--];
                char op = ops[opsTop--];
                values[++valuesTop] = applyOperator(a, b, op);  // Apply operator between numbers
            }
            opsTop--;  // Pop the '('
            expr++;
        }
        else if (*expr == '+' || *expr == '-' || *expr == '*' || *expr == '/' || *expr == '%') {
            while (opsTop >= 0 && precedence(ops[opsTop]) >= precedence(*expr)) {
                double b = values[valuesTop--];
                double a = values[valuesTop--];
                char op = ops[opsTop--];
                values[++valuesTop] = applyOperator(a, b, op);
            }
            ops[++opsTop] = *expr;
            expr++;
        }
        else {
            expr++;  // Skip any unrecognized characters
        }
    }

    while (opsTop >= 0) {
        double b = values[valuesTop--];
        double a = values[valuesTop--];
        char op = ops[opsTop--];
        values[++valuesTop] = applyOperator(a, b, op);
    }

    return values[valuesTop];
}

double applyOperator(double a, double b, char op) {
    switch (op) {
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/':
        if (b != 0) return a / b;
        printf("Error: Division by zero.\n");
        return 0;
    case '%':
        if (b != 0) return fmod(a, b);
        printf("Error: Division by zero.\n");
        return 0;
    default:
        return 0;
    }
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/' || op == '%') return 2;
    return 0;
}

double parseNumber(const char** expr) {
    double result = 0;
    sscanf(*expr, "%lf", &result);

    while (isdigit(**expr) || **expr == '.') (*expr)++;  // Move pointer to end of number

    return result;
}
