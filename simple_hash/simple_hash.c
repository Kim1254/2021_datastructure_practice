#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma warning(disable: 4996)

// TODO: node struct for overflow chaining
typedef struct node_s {
	char* pThis;
	struct node_s* pNext;
} node_t;

#define BUCKET_LEN 33

#define TRUE 1
#define FALSE 0

#define NullCheck(a) (a ? a : "(NULL)")
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

// TODO: insert key to bucket
void insert(node_t** pBucket, const char* key, int print)
{
	if (!pBucket)
		return;

	int sum = 0;
	for (const char* p = key; *p != 0; ++p)
		sum += (int)*p;

	sum %= BUCKET_LEN;

	node_t* pNode = (node_t*)malloc(sizeof(node_t)); // dynamic allocation to value
	pNode->pThis = strdup(key); // copy string
	pNode->pNext = NULL;

	if (!pBucket[sum])
	{
		if (print)
			printf("%s(%s): new index %d\n", __func__, key, sum);
		pBucket[sum] = pNode;
	}
	else // valid value on index: overflow chaining execution
	{
		if (print)
			printf("%s(%s): collison on %d, overflow\n", __func__, key, sum);

		node_t* t = pBucket[sum];
		while (t->pNext)
			t = t->pNext;

		t->pNext = pNode;
	}
}

// TODO: search for value is valid in bucket with key
// can get specific slot of value by using third parameter
const char* search(node_t** pBucket, const char* find, int* slot)
{
	int sum = 0;
	for (const char* p = find; *p != 0; ++p)
		sum += *p;

	sum %= BUCKET_LEN;

	node_t* p = pBucket[sum];
	while (p)
	{
		if (p->pThis && !strcmp(p->pThis, find))
		{
			if (slot)
				*slot = sum;

			return find;
		}
		p = p->pNext;
	}
	return NULL;
}

// TODO: delete a value from key
void deleteSlot(node_t** pBucket, const char* find)
{
	int i;
	const char* find2 = search(pBucket, find, &i);
	if (!find2)
	{
		printf("%s(%s): not found\n", __func__, find);
		return;
	}

	node_t* p = pBucket[i];
	node_t* q = NULL; // value for relink next(chained) key
	while (p)
	{
		if (p->pThis && !strcmp(p->pThis, find))
		{
			if (q) // relink on middle of list
				q->pNext = p->pNext;
			else // relink on begin of list
				pBucket[i] = p->pNext;

			// free mem
			free(p->pThis);
			free(p);
			break;
		}
		q = p;
		p = p->pNext;
	}
	printf("%s(%s): deleted\n", __func__, find);
}

// TODO: print all values of index
void print(node_t** pBucket)
{
	if (!pBucket)
		return;

	printf("\nBUCKET INFO:\n");

	node_t* t = NULL;
	for (int i = 0; i < BUCKET_LEN; i++)
	{
		printf("ID %d:", i);
		if (!pBucket[i]) // print NULL if value is 0(not allocated).
			printf(" (NULL)");
		else
		{
			printf(" %s", NullCheck(pBucket[i]->pThis));

			t = pBucket[i]->pNext;
			while (t) // print chained values
			{
				printf("->%s", NullCheck(t->pThis));
				t = t->pNext;
			};
		}
		printf("\n");
	}
	printf("\n");
}

// TODO: delete all values of bucket
void destroy(node_t** pBucket)
{
	node_t* t = NULL;
	node_t* s = NULL;
	for (int i = 0; i < BUCKET_LEN; i++)
	{
		t = pBucket[i];
		while (t)
		{
			s = t->pNext;

			if (t->pThis)
				free(t->pThis);
			free(t);

			t = s;
		}
	}
}

int main(void)
{
	node_t* buckets[BUCKET_LEN];
	memset(buckets, 0, sizeof(buckets));

	FILE* fp = fopen("keyinput.txt", "r");
	if (!fp)
	{
		printf("failed to open: %s", "keyinput.txt");
		exit(0);
	}

	char line[1024];
	while (fgets(line, 1024, fp))
	{
		if (line[strlen(line) - 1] == '\n') // clear escape char
			line[strlen(line) - 1] = 0;

		if (line[strlen(line) - 1] == '\r') // clear escape char
			line[strlen(line) - 1] = 0;

		insert(buckets, line, FALSE);
	}

	print(buckets);

	///// SEARCH 1 /////
	const char* list1[] = { "Blue", "black", "Purple" };

	int i;
	const char* p = NULL;

	for (i = 0; i < ARRAYSIZE(list1); i++)
	{
		p = search(buckets, list1[i], NULL);
		printf("search(%s): %s\n", list1[i], p ? "found" : "not found");
	}
	///// ... /////

	///// DELETE 1 /////
	const char* list2[] = { "Purple", "Blue", "Green" };

	for (i = 0; i < ARRAYSIZE(list2); i++)
		deleteSlot(buckets, list2[i]);
	///// ... /////

	///// INSERT 1 /////
	const char* list3[] = { "Green", "White", "Golden", "nedloG" };

	for (i = 0; i < ARRAYSIZE(list3); i++)
		insert(buckets, list3[i], TRUE);
	///// ... /////

	///// SERACH 2 /////
	const char* list4[] = {
		"Blue", "nedloG", "Yellow", "Green"
	};

	for (i = 0; i < ARRAYSIZE(list4); i++)
	{
		p = search(buckets, list4[i], NULL);
		printf("search(%s): %s\n", list4[i], p ? "found" : "not found");
	}
	///// ... /////

	print(buckets);

	destroy(buckets);

	return 1;
}