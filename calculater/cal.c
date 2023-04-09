#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_STACK_SIZE 100
typedef char element;

typedef struct {
	element data[MAX_STACK_SIZE];
	int top;
} StackType;
// ���� �ʱ�ȭ �Լ�
void init_stack(StackType* s)
{
	s->top = -1;
}
// ���� ���� ���� �Լ�
int is_empty(StackType* s)
{
	return (s->top == -1);
}
// ��ȭ ���� ���� �Լ�
int is_full(StackType* s)
{
	return (s->top == (MAX_STACK_SIZE - 1));
}
void push(StackType* s, element item)
{
	if (is_full(s)) {
		fprintf(stderr, "���� ��ȭ ����\n");
		return;
	}
	else s->data[++(s->top)] = item;
}
// �����Լ�
element pop(StackType* s)
{
	if (is_empty(s)) {
		fprintf(stderr, "���� ���� ����\n");
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
		fprintf(stderr, "���� ���� ����\n");
		exit(1);
	}
	return s->data[s->top];
}

// ���� ǥ��� ��� �Լ�
int eval_postfix(char* exp)
{
	int i = 0;
	char ch;
	int len = strlen(exp);
	int val;
	StackType s;
	init_stack(&s); // ���� �ʱ�ȭ

	for (i = 0; i < len; i++) {
		ch = exp[i];
		if (ch >= '0' && ch <= '9') { // �ǿ����ڸ� ���ÿ� push
			push(&s, ch - '0'); // ���ڸ� ���ڷ� ��ȯ�ؼ� ����
		}
		else { // �������� ���
			int op2 = pop(&s);
			int op1 = pop(&s);
			switch (ch) { // �����ڿ� ���� ���� ����
			case '+': push(&s, op1 + op2); break;
			case '-': push(&s, op1 - op2); break;
			case '*': push(&s, op1 * op2); break;
			case '/': push(&s, op1 / op2); break;
			}
		}
	}
	val = pop(&s); // ���������� ���ÿ��� ���� ������ ��ȯ
	return val;
}
char* infix_to_postfix(char exp[]) {
	int i = 0;
	char ch, top_op;
	int len = strlen(exp);
	StackType s;
	init_stack(&s); // ���� �ʱ�ȭ

	// ���ڿ� �޸� ���� �Ҵ�
	char* postfix = (char*)malloc((len + 1) * sizeof(char));
	if (postfix == NULL) {
		fprintf(stderr, "Memory allocation error!\n");
		exit(1);
	}

	int idx = 0; // ������ ���ڿ� �ε���
	for (i = 0; i < len; i++) {
		ch = exp[i];
		switch (ch) {
		case '+': case '-': case '*': case '/': // ������
			// ���ÿ� �ִ� �������� �켱������ �� ũ�ų� ������ ���
			while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
				postfix[idx++] = pop(&s);
			}
			push(&s, ch);
			break;
		case '(': // ���� ��ȣ
			push(&s, ch);
			break;
		case ')': // ������ ��ȣ
			top_op = pop(&s);
			// ���� ��ȣ�� ���������� ���
			while (top_op != '(') {
				postfix[idx++] = top_op;
				top_op = pop(&s);
			}
			break;
		default: // �ǿ�����
			postfix[idx++] = ch;
			break;
		}
	}
	while (!is_empty(&s)) // ���ÿ� ����� �����ڵ� ���
		postfix[idx++] = pop(&s);

	postfix[idx] = '\0'; // NULL ���� �߰�

	return postfix;
}
int main(void)
{

	char exp[MAX_STACK_SIZE];
	int result;
	int leng;

	printf("������ �Է��ϼ���: ");
	scanf("%s", exp);

	printf("�Է��� ����: %s\n", exp);

	printf("���� ǥ���: ");
	char* new_str = infix_to_postfix(exp);
	printf("\n %s \n", new_str);

	result = eval_postfix(new_str);
	printf("\n\n%d ", result);

}