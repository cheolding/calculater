#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_STACK_SIZE 100
typedef char element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;
// 스택 초기화 함수
void init_stack(StackType* s)
{
	s->top = -1;
}
// 공백 상태 검출 함수
int is_empty(StackType* s)
{
	return (s->top == -1);
}
// 포화 상태 검출 함수
int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "스택 포화 에러\n");
		return;
	}
	else s->data[++(s->top)] = item;
}
// 삭제함수
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	else return s->data[(s->top)--];
}
int prec(char op)
{
	switch (op) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '*': case '/': return 2;
	}
	return -1;
}
int peek(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "스택 공백 에러\n");
		exit(1);
	}
	return s->data[s->top];
}

// 후위 표기식 계산 함수
int eval_postfix(char* exp)
{
	int i = 0;
	char ch;
	int len = strlen(exp);
	int val;
	StackType s;
	init_stack(&s); // 스택 초기화

	for (i = 0; i < len; i++) {
		ch = exp[i];
		if (ch >= '0' && ch <= '9') { // 피연산자면 스택에 push
			push(&s, ch - '0'); // 문자를 숫자로 변환해서 저장
		}
		else { // 연산자인 경우
			int op2 = pop(&s);
			int op1 = pop(&s);
			switch (ch) { // 연산자에 따라 연산 수행
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	val = pop(&s); // 마지막으로 스택에서 값을 꺼내서 반환
	return val;
}
char* infix_to_postfix(char exp[]) {
	int i = 0;
	char ch, top_op;
	int len = strlen(exp);
	StackType s;
	init_stack(&s); // 스택 초기화

	// 문자열 메모리 동적 할당
	char* postfix = (char*)malloc((len + 1) * sizeof(char));
	if (postfix == NULL) {
		fprintf(stderr, "Memory allocation error!\n");
		exit(1);
	}

	int idx = 0; // 후위식 문자열 인덱스
	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case '-': case '*': case '/': // 연산자
			// 스택에 있는 연산자의 우선순위가 더 크거나 같으면 출력
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
				postfix[idx++] = pop(&s);
			}
			push(&s, ch);
			break;
		case '(': // 왼쪽 괄호
			push(&s, ch);
			break;
		case ')': // 오른쪽 괄호
			top_op = pop(&s);
			// 왼쪽 괄호를 만날때까지 출력
			while (top_op != '(') {
				postfix[idx++] = top_op;
				top_op = pop(&s);
			}
			break;
		default: // 피연산자
			postfix[idx++] = ch;
			break;
		}
	}
	while (!is_empty(&s)) // 스택에 저장된 연산자들 출력
		postfix[idx++] = pop(&s);

	postfix[idx] = '\0'; // NULL 문자 추가

	return postfix;
}
int main(void)
{

	char exp[MAX_STACK_SIZE];
	int result;
	int leng;

	printf("수식을 입력하세요: ");
	scanf("%s", exp);

	printf("입력한 수식: %s\n", exp);

	printf("후위 표기식: ");
	char* new_str = infix_to_postfix(exp);
	printf("\n %s \n", new_str);

	result = eval_postfix(new_str);
	printf("\n\n%d ", result);

}