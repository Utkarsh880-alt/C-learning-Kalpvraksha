#include<stdio.h>

int main(){
    float a , b, sum, sub, div, mul;
    char operator;

    printf("Enter the operator (+, -, *, /): ");
    scanf(" %c", &operator);

    printf("Enter two operands: ");
    scanf("%f %f", &a, &b);

    switch(operator){
    case '/':
        div = a / b;
        printf("Div = %f\n", div);
        break;
    case '*':
        mul = a * b;
        printf("Mul = %f\n", mul);
        break;
    case '+':
        sum = a + b;
        printf("Sum = %f\n", sum); 
        break;
    case '-':
        sub = a - b;
        printf("Sub = %f\n", sub);
        break;
    default:
        printf("Inavlid input\n Enter valid operator\n");
    }
    return 0;
}