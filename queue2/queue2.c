#include <stdio.h>
#include <stdlib.h>

#define queue_size 10
//#define CIRCULAR_QUEUE

const int ERRCODE_EMPTY = -0x3796e;

// linked list
typedef struct node_s {
	int value;
	struct node_s* next;
} node_t;

node_t* head = NULL;
node_t* front = NULL;
node_t* rear = NULL;

int size(void)
{
	if (!head)
		return 0;

	int i = 0;
	node_t* temp = head;

	while (temp)
	{
		i++;
		temp = temp->next;
	}

	return i;
}

void free_allnode(void)
{
	node_t* t = head;
	node_t* tt = NULL;

	while (t)
	{
		tt = t;
		t = t->next;
		free(tt);
	}
}
//

int queue_full();
int queue_empty();

void enqueue(int value)
{
	if (queue_full())
		return;

	node_t* pNext = NULL;
#if defined CIRCULAR_QUEUE
	if (size() < queue_size)
		pNext = malloc(sizeof(node_t));
	else
	{
		if (!rear->next)
			pNext = head;
		else
			pNext = rear->next;
	}
#else
	pNext = malloc(sizeof(node_t));
#endif

	if (pNext)
	{
		pNext->value = value;

		if (size() < queue_size)
			pNext->next = NULL;

		if (!head)
			head = pNext;

		if (rear && head != pNext)
			rear->next = pNext;

		rear = pNext;
	}
}

int dequeue()
{
	if (queue_empty())
		return ERRCODE_EMPTY;

	if (!front)
		front = head;
	else
		front = front->next;

#if defined CIRCULAR_QUEUE
	if (!front)
		front = head;
#endif

	int val = front->value;

	return val;
}

int queue_full()
{
#if defined CIRCULAR_QUEUE
	if (rear)
	{
		node_t* pThis = rear->next;

		if (!pThis && size() >= queue_size)
			pThis = head;

		if (pThis && pThis == front)
			return 1;
	}
#else
	if (size() >= queue_size)
		return 1;
#endif

	return 0;
}

int queue_empty()
{
	if (front == rear)
		return 1;

	return 0;
}

int main()
{
	int i = 0;

	enqueue(5);
	enqueue(4);
	enqueue(3);
	enqueue(2);

	int j = 0;
	for (i = 0; i < 5; i++)
	{
		j = dequeue();
		if (j != ERRCODE_EMPTY)
			printf("dequeue: %d\n", j);
		else
			printf("Error: Queue is empty.\n");
	}

	for (i = 0; i < queue_size; i++)
		enqueue(i + 1);

	for (i = 0; i < queue_size; i++)
	{
		j = dequeue();
		if (j != ERRCODE_EMPTY)
			printf("dequeue: %d\n", j);
		else
			printf("Error: Queue is empty.\n");
	}

	free_allnode();
	return 1;
}