#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Stack
{
    int top;
    unsigned capacity;
    int *array;
} Stack;

Stack *createStack(unsigned capacity)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    if (!stack)
        return NULL;
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
    if (!stack->array)
        return NULL;
    return stack;
}

int isEmpty(Stack *stack)
{
    return stack->top == -1;
}

char peek(Stack *stack)
{
    return stack->array[stack->top];
}

char pop(Stack *stack)
{
    if (!isEmpty(stack))
        return stack->array[stack->top--];
    return '$';
}

void push(Stack *stack, char op)
{
    stack->array[++stack->top] = op;
}

int isOperand(char ch)
{
    return isdigit(ch);
}

int precedence(char ch)
{
    switch (ch)
    {
    case '+':
    case '-':
        return 1;
    case '*':
    case '/':
        return 2;
    case '^':
        return 3;
    }
    return -1;
}

int applyOp(int a, int b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
    case '-':
        return a - b;
    case '*':
        return a * b;
    case '/':
        return a / b;
    }
    return 0;
}

int evaluatePostfix(char *postfix)
{
    Stack *stack = createStack(strlen(postfix));
    if (!stack)
        return -1;

    for (int i = 0; i < strlen(postfix); ++i)
    {
        if (postfix[i] == ' ')
            continue;
        else if (isOperand(postfix[i]))
        {
            int num = 0;
            while (isOperand(postfix[i]))
            {
                num = num * 10 + (int)(postfix[i] - '0');
                i++;
            }
            i--;
            push(stack, num);
        }
        else
        {
            int val2 = pop(stack);
            int val1 = pop(stack);
            push(stack, applyOp(val1, val2, postfix[i]));
        }
    }
    return pop(stack);
}

char *infixToPostfix(char *exp)
{
    int i, k;
    char *postfix = (char *)malloc((strlen(exp) + 1) * sizeof(char));
    Stack *stack = createStack(strlen(exp));
    if (!stack)
        return NULL;
    for (i = 0, k = -1; exp[i]; ++i)
    {
        if (isOperand(exp[i]))
        {
            while (isOperand(exp[i]))
            {
                postfix[++k] = exp[i++];
            }
            postfix[++k] = ' '; 
            i--;
        }
        else if (exp[i] == '(')
            push(stack, exp[i]);
        else if (exp[i] == ')')
        {
            while (!isEmpty(stack) && peek(stack) != '(')
                postfix[++k] = pop(stack);
            if (!isEmpty(stack) && peek(stack) != '(')
                return NULL;
            else
                pop(stack);
        }
        else
        {
            while (!isEmpty(stack) && precedence(exp[i]) <= precedence(peek(stack)))
                postfix[++k] = pop(stack);
            push(stack, exp[i]);
        }
    }
    while (!isEmpty(stack))
        postfix[++k] = pop(stack);
    postfix[++k] = '\0';
    return postfix;
}

int main() {
    char exp[100];
    printf("Ingrese la expresion aritmetica: ");
    fgets(exp, 100, stdin);
    exp[strcspn(exp, "\n")] = 0;
    char* postfix = infixToPostfix(exp);
    printf("Expresion Polaca: %s\n", postfix);
    int result = evaluatePostfix(postfix);
    printf("Resultado: %d\n", result);

    FILE* file = fopen("output.txt", "w");
    if (file == NULL) {
        printf("Error abriendo el archivo!\n");
        return 1;
    }
    fprintf(file, "Expresion Aritmetica: %s\n", exp);
    fprintf(file, "Exprosion Polaca: %s\n", postfix);
    fprintf(file, "Resultado: %d\n", result);
    fclose(file);

    free(postfix);
    return 0;
}
