#include <stdio.h>
#include <stdlib.h>

#define stack_size 10

const int ERRCODE_EMPTYSTACK = -0x3796e;

// Linked list
typedef struct node_s
{
	int value;
	struct node_s* prev;
} node_t;

node_t* tail = NULL;

int size(void)
{
	if (!tail)
		return 0;

	int i = 0;
	node_t* temp = tail;

	while (temp)
	{
		i++;
		temp = temp->prev;
	}

	return i;
}

void free_allnode(void)
{
	node_t* t = tail;
	node_t* tt = NULL;

	while (t)
	{
		tt = t;
		t = t->prev;
		free(tt);
	}
}
//

int stack_full();
int stack_empty();

void push(int value)
{
	if (stack_full())
		return;

	node_t* pNext = malloc(sizeof(node_t));
	if (pNext)
	{
		pNext->value = value;
		pNext->prev = tail;
		tail = pNext;
	}
}

int pop()
{
	if (stack_empty())
		return ERRCODE_EMPTYSTACK;

	int val = tail->value;

	node_t* t = tail;
	tail = tail->prev;
	free(t);

	return val;
}

int stack_full()
{
	if (size() >= stack_size)
		return 1;

	return 0;
}

int stack_empty()
{
	if (!tail)
		return 1;

	return 0;
}

int main()
{
	int i = 0;

	push(5);
	push(4);
	push(3);
	push(2);

	int j = 0;
	for (i = 0; i < 5; i++)
	{
		j = pop();
		if (j != ERRCODE_EMPTYSTACK)
			printf("Pop: %d\n", j);
		else
			printf("Error: Stack is empty.\n");
	}

	for (i = 0; i < stack_size; i++)
		push(i + 1);

	for (i = 0; i < stack_size; i++)
	{
		j = pop();
		if (j != ERRCODE_EMPTYSTACK)
			printf("Pop: %d\n", j);
		else
			printf("Error: Stack is empty.\n");
	}

	free_allnode();
	return 1;
}
