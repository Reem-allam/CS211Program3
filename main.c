/*-------------------------------------------
Program 3: TwentyFour Part 2
Course: CS 211, Spring 2023, UIC
System: Replit
Author: Reem Allam
------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void intro() {
  printf("Welcome to the game of TwentyFour Part Two!\n");
  printf("Use each of the four numbers shown exactly once, \n");
  printf("combining them somehow with the basic mathematical operators "
         "(+,-,*,/) \n");
  printf("to yield the value twenty-four.\n");
}

struct Node {
  // node Class with parameters value, operate, and next
  int value;
  char operate;
  struct Node *next;
};

struct Stack {
  // creates a stack structure that holds struct Node*
  struct Node *Head;
};

int character(char op1) {
  // Function to return vaid character options
  if (op1 == '+' || op1 == '-') {
    return 1;
  } else if (op1 == '*' || op1 == '/') {
    return 2;
  }
  return 0;
}

char firstOp(struct Stack *main) {
  // returns the first operator value in the stack
  return main->Head->operate;
}

int is_Empty(struct Stack *main) {
  // returns if a stack is empty
  if (main->Head == NULL) {
    return 1;
  }
  return 0;
}

int popFront(struct Stack *main) {
  // pop a value from the first value, delete the node and returning the value
  struct Node *temp;
  temp = main->Head;
  main->Head = main->Head->next;
  int value = temp->value;
  free(temp);
  return value;
}

void pushFront(struct Node *newHead, struct Stack *main) {
  // pushes a node to the front of the stack
  if (is_Empty(main) == 1) {
    main->Head = newHead;
    main->Head->next = NULL;
    return;
  }
  (*newHead).next = main->Head;
  main->Head = newHead;
}

char popfrontOp(struct Stack *main) {
  // pops a value from the first value for the operator stack, deletes the node
  // and returns the value
  if (is_Empty(main) == 0) {
    struct Node *temp;
    temp = main->Head;
    main->Head = main->Head->next;
    char value = temp->operate;
    free(temp);
    return value;
  }
}

int compute(struct Stack *operand, struct Stack *operators) {
  // calculates based on the two stacks operand and operator and return the
  // value
  int num1 = popFront(operand);
  int num2 = popFront(operand);
  char sign = popfrontOp(operators);
  if (sign == '-') {
    printf("%d - %d = %d.\n", num2, num1, num2 - num1);
    return num2 - num1;
  } else if (sign == '+') {
    printf("%d + %d = %d.\n", num2, num1, num2 + num1);
    return num1 + num2;
  } else if (sign == '/') {
    if (num1 == 0) {
      return 0;
    }
    printf("%d / %d = %d.\n", num2, num1, num2 / num1);
    return num2 / num1;
  } else if (sign == '*') {
    printf("%d * %d = %d.\n", num2, num1, num2 * num1);
    return num1 * num2;
  }
  return 0;
}

void pushfrontOp(struct Node *newHead, struct Stack *operand,
                 struct Stack *operators) {
  // pushes new node with an operator to front of the stack
  if (is_Empty(operators)) {
    pushFront(newHead, operators);
    return;
  }
  char first = firstOp(operators);
  while (is_Empty(operators) == 0 &&
         character(newHead->operate) <= character(firstOp(operators))) {
    int result = compute(operand, operators);
    struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
    aNode->value = result;
    pushFront(aNode, operand);
  }
  pushFront(newHead, operators);
}

int pemdasStack(char *equation, int *errorflag) {
  // calculates the equation user inputs and displays steps taken and result
  struct Stack operand;
  struct Stack operators;
  int balanced = 0;
  int noBalanced = 0;
  operators.Head = NULL;
  operand.Head = NULL;

  for (int i = 0; i < strlen(equation); i++) {
    if (equation[i] == ' ') {
      continue;
    }
    if (equation[i] >= 48 && equation[i] <= 57) {
      struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
      char x = equation[i];
      aNode->value = x - '0';
      pushFront(aNode, &operand);
    } else if (equation[i] == '(') {
      balanced++;
      struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
      aNode->operate = equation[i];
      pushFront(aNode, &operators);
    } else if (equation[i] == ')') {
      balanced--;
      if (balanced < 0) {
        noBalanced = 1;
        balanced++;
        break;
      }
      while (firstOp(&operators) != '(') {
        int result = compute(&operand, &operators);
        struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
        aNode->value = result;
        pushFront(aNode, &operand);
      }
      popfrontOp(&operators);
    } else if (equation[i] == '+' || equation[i] == '-' || equation[i] == '/' ||
               equation[i] == '*') {
      struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
      aNode->operate = equation[i];
      pushfrontOp(aNode, &operand, &operators);
    } else {
      *errorflag = -1;
    }
  }
  if (balanced != 0) {
    *errorflag = 2;
  }
  while (is_Empty(&operators) != 1) {
    int result = compute(&operand, &operators);
    struct Node *aNode = (struct Node *)malloc(sizeof(struct Node));
    aNode->value = result;
    pushFront(aNode, &operand);
  }
  int sum = popFront(&operand);
  if (noBalanced) {
    *errorflag = 1;
    while (!is_Empty(&operand)) {
      popFront(&operand);
    }
    while (!is_Empty(&operators)) {
      popFront(&operators);
    }
    return 0;
  }
  while (!is_Empty(&operand)) {
    popFront(&operand);
  }
  while (!is_Empty(&operators)) {
    popFront(&operators);
  }
  return sum;
}

int numCheck(int *array, int check) {
  // makes sure all the numbers are being used once
  int true = 0;
  for (int i = 0; i < 4; i++) {
    int x = array[i];
    if (x == check) {
      array[i] = -1;
      true = 1;
      return true;
    }
  }
  return true;
}

int charCheck(char *input) {
  // makes sure ther are the correct number of operands
  int check = 0;
  int check2 = 0;
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == ' ' || input[i] == '\n') {
      continue;
    }
    if (input[i] >= 48 && input[i] <= 57) {
      check2++;
    } else if (input[i] == '+' || input[i] == '-' || input[i] == '/' ||
               input[i] == '*' || input[i] == ' ') {
      check++;
    }
  }
  return check - check2 > 1 || check - check2 < -1;
}

int symbolCheck(char *input) {
  // makes sure symbols entered are valid
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == ' ' || input[i] == '\n') {
      continue;
    }
    if (input[i] >= 48 && input[i] <= 57) {
      continue;
    } else if (input[i] == '+' || input[i] == '-' || input[i] == '/' ||
               input[i] == '*' || input[i] == ' ' || input[i] == '(' ||
               input[i] == ')') {
      continue;
    } else {
      return 0;
    }
  }
  return 1;
}

int numCheck2(const int array[4], char *input) {
  // makes sure same correct numbers are being used in the equation
  int array2[4];
  for (int i = 0; i < 4; i++) {
    int x = array[i];
    array2[i] = x;
  }
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] >= 48 && input[i] <= 57) {
      int check = numCheck(array2, input[i] - '0');
      if (check == 0) {
        return 0;
      }
    }
  }
  return 1;
}

int main(void) {
  srand(1);
  intro();
  int array[100][4];
  int count = 0;
  int option = 2;
  char fileName[81];
  char fileName2[81];
  char difficulty;
  while (option != 3) {
    if (option == 2) {
       count = 0;
      printf("Choose your difficulty level: E for easy, M for medium, and H "
             "for hard (default is easy).\n");
      printf("Your choice --> ");
      scanf(" %c", &difficulty);

      if (difficulty == 'H') {
        strcpy(fileName2, "hard.txt");
      } else if (difficulty == 'M') {
        strcpy(fileName2, "medium.txt");
      } else {
        strcpy(fileName2, "easy.txt");
      }
      FILE *filePtr = fopen(fileName2, "r");

      // Checks if that file was able to be opened
      if (filePtr == NULL) {
        printf("Error: could not open %s for reading\n", fileName);
        exit(-1);
      }
      while (fscanf(filePtr, "%d %d %d %d", &array[count][0], &array[count][1],
                    &array[count][2], &array[count][3]) != EOF) {

        count++;
      }
    }
    int random = rand() % count;
    printf("The numbers to use are: ");
    for (int i = 0; i < 4; i++) {
      if (i == 3) {
        printf("%d", array[random][i]);
      } else {
        printf("%d, ", array[random][i]);
      }
    }
    printf(".\n");
    printf("Enter your solution: ");
    char input[256];
    scanf(" %[^\n]", input);
    int charchecknum = charCheck(input);
    int check1 = symbolCheck(input);
    int newArray[4] = {};
    for (int i = 0; i < 4; i++) {
      int x = array[random][i];
      newArray[i] = x;
    }
    int check2 = numCheck2(newArray, input);
    if (check1 == 0) {
      printf("Error! Invalid symbol entered. Please try again.\n\n");
      option = 1;
      continue;
    }
    if (check2 == 0) {
      printf("Error! You must use all four numbers, and use each one only "
             "once. Please try again.\n\n");
      option = 1;
      continue;
    }
    int errorcheck = 0;
    int result = pemdasStack(input, &errorcheck);
    if (errorcheck == 1) {
      printf("Error! Too many closing parentheses in the expression.\n\n");
    } else if (errorcheck == 2) {
      printf("Error! Unbalanced parentheses in the expression.\n\n");
    } else if (charchecknum == 1) {
      printf("Error! Too many values in the expression.\n\n");
    } else if (result == 24) {
      printf("Well done! You are a math genius.\n");
    }

    else {
      printf("Sorry. Your solution did not evaluate to 24.\n");
    }
    printf("Enter: \t1 to play again,\n");
    printf("\t2 to change the difficulty level and then play again, or\n");
    printf("\t3 to exit the program.\n");
    printf("Your choice --> ");
    char playOptions;
    scanf(" %c", &playOptions);
    option = playOptions - '0';
  }

  printf("\nThanks for playing!\n");
  printf("Exiting...\n");
  return 0;
}
