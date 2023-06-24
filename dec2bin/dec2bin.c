#include <stdio.h>
#include <stdlib.h>

typedef enum {
	false = 0,
	true = 1
} bool;

typedef struct node_s {
	bool value;
	struct node_s* prev;
} node_t;

#define max_size 64

//#define ENABLE_NEGATIVE_CONVERT

#define Make_Reverse(a, b) { \
a = NULL; \
reversedump(&a, b, 0); \
freenode(*b); \
*b = a; } \

int count(node_t* node)
{
	int val = 0;
	while (node)
	{
		val++;
		node = node->prev;
	}
	return val;
}

int push(node_t** node, bool value)
{
	if (!node)
		return 1;

	node_t* pNew = (node_t*)malloc(sizeof(node_t));
	pNew->value = value;
	pNew->prev = *node;

	*node = pNew;
	return 0;
}

bool pop(node_t** node)
{
	if (!node)
		return false;

	bool val = (*node)->value;
	node_t* p = *node;

	*node = (*node)->prev;
	free(p);

	return val;
}

void reversedump(node_t** dest, node_t** source, int size)
{
	if (!dest || !source)
		return;

	int c = 0;

	node_t* p = *source;
	while (p)
	{
		push(dest, p->value);
		p = p->prev;

		if (size && ++c >= size)
			break;
	}
}

void printnode(node_t* node)
{
	//printf("Node print: ");
	while (node)
	{
		printf("%d", node->value);
		node = node->prev;
	}
	//printf("\n");
}

void freenode(node_t* node)
{
	node_t* t = NULL;
	while (node)
	{
		t = node;
		node = node->prev;
		free(t);
	}
}

void fnConvert(long long num, node_t** node)
{
#if !defined ENABLE_NEGATIVE_CONVERT
	if (num < 0)
	{
		printf("Invalid number input: %lld\n", num);
		return;
	}
#else
	bool reverse = false;
	if (num < 0)
	{
		reverse = true;
		num *= -1;
	}
#endif

	long long t = 0b01;
	int val = 0;
	while (t <= num)
	{
		if (val >= max_size)
		{
			printf("error: number is too large to convert (larger than 2^63 - 1)\n");
			freenode(*node);
			*node = NULL;
			return;
		}

		if (val == max_size - 1)
			t--;

		if (push(node, t & num ? true : false))
		{
			printf("error: invalid node input.\n");
			freenode(*node);
			return;
		}

		if (t >= num)
			break;

		t = (1i64 << ++val);
	}

#if defined ENABLE_NEGATIVE_CONVERT
	if (reverse && *node) // 2's complement
	{
		num *= -1;

		int c = max_size - count(*node);
		for (int i = 0; i < c; i++)
			push(node, false); // fill blank spaces to 0.

		node_t* p = *node;
		while (p)
		{
			p->value = true - p->value; // reverse (1's complement)
			p = p->prev;
		}

		node_t* dump = NULL; // temp value for reverse
		// Reverse after 1's complement, easy to make 2's complement.
		Make_Reverse(dump, node);

		p = *node;
		while (p)
		{
			p->value = true - p->value; // 2's complement

			if (p->value)
				break;

			p = p->prev;
		}

		Make_Reverse(dump, node);
	}
#endif
}

int main()
{
	printf("Enter decimal number: ");

	long long t;
	scanf_s("%lld", &t);

	node_t* convert = NULL;
	fnConvert(t, &convert);

	if (convert)
	{
		printf("Result of conversion %lld into binary:\n", t);
		printnode(convert);
		printf("\n");

		freenode(convert);
	}
	else
		printf("Conversion failed.\n");

	return 1;
}
