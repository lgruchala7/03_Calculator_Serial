#include "hFramework.h"

#define ASCII_DIFF 48

using namespace hFramework;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} Operation_T;

typedef enum
{
    READ_FIRST_NUM,
    READ_OPERATOR,
    READ_SECOND_NUM,
} Stage_T;

float calculate_result(int n1, int n2);
int calculate_result(int n1, int n2, Operation_T op);
void reset_values(void);

char c;
int operand_1 = 0;
int operand_2 = 0;
int operand_1_length = 0;
int operand_2_length = 0;

int result_int;
float result_float;

bool ready_to_calculate = false;

void hMain()
{
    Operation_T operation;
    Stage_T stage = READ_FIRST_NUM;

    sys.setLogDev(&Serial);

    printf("TWO-DIGIT NUMBERS CALCULATOR:\r\n");
    printf("1) Insert first number (any one or two-digit poositive integer number)\r\n");
    printf("2) Insert operation sign (+, -, *, /)\r\n");
    printf("3) Insert second number (any two-digit number)\r\n");
    printf("4) Press enter\r\n\n");
    printf("Correct examples: \"21 + 69\" or \"2 - 67\" or \"7 * 7\"\r\n");
    printf("Incorrect examples: \"01 + 69\" or \"a - 67\" or \"7 _ 7\"\r\n\n");

    while (true)
    {
        while (Serial.available())
        {
            c = Serial.getch();  // get one character from stdin

            if (operand_2_length >= 2)  // operation inserted - no need to read further
                break;

            if (c >= '0' && c <= '9')   // character is a digit
            {
                switch (stage)
                {
                    case READ_FIRST_NUM:
                        if (operand_1_length == 1)
                        {
                            if (operand_1 == 0)
                            {
                                printf("Incorrect input. Please try again from the start.\r\n\n");
                                reset_values();
                                while (Serial.getch())
                                    continue;  
                            }
                            else
                            {
                                operand_1 *= 10;
                                operand_1 += ((int)c - ASCII_DIFF);
                                stage = READ_OPERATOR;
                            }
                        }
                        else if (operand_1_length == 0)
                        {
                            operand_1 += ((int)c - ASCII_DIFF);
                            ++operand_1_length;
                        }
                        break;
                    case READ_SECOND_NUM:
                        if (operand_2_length == 1)
                        {
                            if (operand_2 == 0)
                            {
                                printf("Incorrect input. Please try again from the start.\r\n\n");
                                reset_values();
                                stage = READ_FIRST_NUM;
                                while (Serial.getch())
                                    continue;  
                            }
                            else
                            {
                                operand_2 *= 10;
                                operand_2 += ((int)c - ASCII_DIFF);
                                ready_to_calculate = true;
                            }
                        }
                        else if (operand_2_length == 0)
                        {
                            operand_2 += ((int)c - ASCII_DIFF);
                            ++operand_2_length;
                            ready_to_calculate = true;
                        }
                        break;
                    case READ_OPERATOR:
                        printf("Incorrect input. Please try again from the start.\r\n\n");
                        reset_values();
                        stage = READ_FIRST_NUM;
                        while (Serial.getch())
                            continue;
                        break;
                }
            }
            else if (c == ' ')  // character is a space
            {
                switch (stage)
                {
                    case READ_FIRST_NUM:
                        if (operand_1_length == 0)
                        {
                                /* do nothing */
                        }
                        else if (operand_1_length >= 1)
                        {
                            stage = READ_OPERATOR;
                        }
                        break;
                    case READ_SECOND_NUM:
                        if (operand_2_length == 1)
                        {
                           ready_to_calculate = true;
                           break;
                        }
                        break;
                    case READ_OPERATOR:
                        /* do nothing */
                        break;
                }
            }
            else    // character is neither a digit nor a space 
            {
                if (stage == READ_OPERATOR)
                {
                    switch (c)
                    {
                        case '+':
                            operation = ADD;
                            stage = READ_SECOND_NUM;
                            break;
                        case '-':
                            operation = SUB;
                            stage = READ_SECOND_NUM;
                            break;
                        case '*':
                            operation = MUL;
                            stage = READ_SECOND_NUM;
                            break;
                        case '/':
                            operation = DIV;
                            stage = READ_SECOND_NUM;
                            break;
                        default:
                            printf("Incorrect input. Please try again from the start.\r\n\n");
                            reset_values();
                            stage = READ_FIRST_NUM;
                            while (Serial.getch())
                                continue;  
                            break;
                    }
                }
                else
                {
                        printf("Incorrect input. Please try again from the start.\r\n\n");
                        reset_values();
                        stage = READ_FIRST_NUM;

                        while (Serial.getch())
                            continue;  
                }

            }
        }

        if (ready_to_calculate)
        {
            if (operation == DIV)
            {
                result_float = calculate_result(operand_1, operand_2);
                if (result_float == (float)NULL)
                    printf("You can't divide by 0!\r\n\n");
                else
                    printf("Result: %.2f\r\n\n", result_float);
            }
            else
            {
                result_int = calculate_result(operand_1, operand_2, operation);
                printf("Result: %d\r\n\n", result_int);
            }

            ready_to_calculate = false;
            reset_values();
            stage = READ_FIRST_NUM;
        }
    }
}

int calculate_result(int n1, int n2, Operation_T op)
{
    int result;

    switch((int)op)
    {
        case ADD:
            result = n1 + n2;
            break;
        case SUB:
            result = n1 - n2;
            break;
        case MUL:
            result = n1 * n2;
            break;
    }

    return result;
}

float calculate_result(int n1, int n2)
{
    float result;

    if (n2 == 0)
        return (float)NULL;
    else
        result = (1.0 * n1) / n2;
        return result;
}

void reset_values(void)
{
    operand_1 = 0;
    operand_2 = 0;
    operand_1_length = 0;
    operand_2_length = 0;
}