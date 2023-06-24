#include <stdio.h>
#include <stdlib.h>

#define queue_size 10

const int ERRCODE_EMPTY = -0x3796e;
const int ERRCODE_FULL = -0x3796d;

// linked list
typedef struct node_s
{
	int value;
	struct node_s* next;
} node_t;

node_t* front = NULL;
node_t* rear = NULL;

int count(void)
{
	if (!front)
		return 0;

	node_t* temp = front;

	int i = 0;

	while (temp)
	{
		i++;
		temp = temp->next;
	}

	return i;
}

void free_allnode(void)
{
	node_t* t = front;
	node_t* tt = NULL;

	while (t)
	{
		tt = t;
		t = t->next;
		free(tt);
	}
}

int queue_full();
int queue_empty();

int enqueue(int value)
{
	if (queue_full())
		return ERRCODE_FULL;

	node_t* pNext = (node_t*)malloc(sizeof(node_t));

	if (pNext)
	{
		pNext->value = value;
		pNext->next = NULL;

		if (!front)
			front = pNext;

		if (rear)
			rear->next = pNext;

		rear = pNext;
	}

	return 0;
}

int dequeue()
{
	if (queue_empty())
		return ERRCODE_EMPTY;

	int val = front->value;

	node_t* p = front;
	front = front->next;

	if (front == rear)
		rear = NULL;
	free(p);

	return val;
}

int queue_full()
{
#if defined queue_size
	if (count() >= queue_size)
		return 1;
#endif

	return 0;
}

int queue_empty()
{
	if (front == rear && front == NULL)
		return 1;

	return 0;
}

int main()
{
	int i = 0;

	for (i = 5; i > 1; i--)
	{
		if (enqueue(i) == ERRCODE_FULL)
			printf("[Error] Failed to enqueue %d: Queue is full.\n", i);
	}

	int j = 0;
	for (i = 0; i < 5; i++)
	{
		j = dequeue();
		if (j != ERRCODE_EMPTY)
			printf("dequeue: %d\n", j);
		else
			printf("[Error] Failed to dequeue: Queue is empty.\n");
	}

	for (i = 0; i < 15; i++)
	{
		if (enqueue(i + 1) == ERRCODE_FULL)
			printf("[Error] Failed to enqueue %d: Queue is full.\n", i + 1);
	}

	for (i = 0; i < 10; i++)
	{
		j = dequeue();
		if (j != ERRCODE_EMPTY)
			printf("dequeue: %d\n", j);
		else
			printf("[Error] Failed to dequeue: Queue is empty.\n");
	}

	free_allnode();
	return 1;
}
