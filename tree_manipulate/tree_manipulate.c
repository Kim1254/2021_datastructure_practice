#include <stdio.h>

#include <stdlib.h>
#include <time.h>

// for memset
#include <string.h>

#pragma warning(disable: 4996)

#define MAX_ELEM 50
#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

typedef struct node
{
	int data;
	struct node* left, * right;
	int ht;
}node;

node* insert(node*, int);
void preorder(node*); // using functon pointer for using many pointers
void inorder(node*); // using functon pointer for using many pointers
int height(node*);
node* rotateright(node*);
node* rotateleft(node*);
node* RR(node*);
node* LL(node*);
node* LR(node*);
node* RL(node*);
int BF(node*);

int count(node* T);
int find(node* T, int x);
int highest(node* T);
void search1(node* T, int x);
void search2(node* T, int x);
void search3(node* T, int x, int y);

int main()
{
	node* root = NULL;

	int i, j;
	srand(time(NULL));

	for (i = 0; i < 200; i++)
	{
		switch (i)
		{
		case 198:
			root = insert(root, 15);
			break;
		case 199:
			root = insert(root, 50);
			break;
		default:
			root = insert(root, rand() % 100 + 1);
		}

		if (count(root) >= MAX_ELEM)
			break;
	}

	printf("All node:");
	preorder(root);
	printf("\n\n");

	int target[] = { 15, 20, 50, 70, 90 };
	for (i = 0; i < ARRAYSIZE(target); i++)
	{
		printf("Target %d ", target[i]);
		if (find(root, target[i]))
			printf("exists on this tree.\n");
		else
			printf("doesn\'t exist on this tree.\n");
	}

	printf("\nHighest Node: %d\n", highest(root));

	printf("\nNodes below 15:");
	search1(root, 15);
	printf("\n");

	printf("\nNodes up to 50:");
	search2(root, 50);
	printf("\n");

	printf("\nNodes between 15 and 50 (expects boundary):");
	search3(root, 15, 50);
	printf("\n");

	return 0;
}

node* insert(node* T, int x)
{
	if (T == NULL)
	{
		T = (node*)malloc(sizeof(node));
		T->data = x;
		T->left = NULL;
		T->right = NULL;
	}
	else if (x > T->data)		// insert in right subtree
	{
		T->right = insert(T->right, x);
		if (BF(T) == -2)
		{
			if (x > T->right->data)
				T = RR(T);
			else
				T = RL(T);
		}
	}
	else if (x < T->data)
	{
		T->left = insert(T->left, x);
		if (BF(T) == 2)
		{
			if (x < T->left->data)
				T = LL(T);
			else
				T = LR(T);
		}
	}

	T->ht = height(T);

	return(T);
}

int height(node* T)
{
	int lh, rh;
	if (T == NULL)
		return(0);

	if (T->left == NULL)
		lh = 0;
	else
		lh = 1 + T->left->ht;

	if (T->right == NULL)
		rh = 0;
	else
		rh = 1 + T->right->ht;

	if (lh > rh)
		return(lh);

	return(rh);
}

node* rotateright(node* x)
{
	node* y;
	y = x->left;
	x->left = y->right;
	y->right = x;
	x->ht = height(x);
	y->ht = height(y);
	return(y);
}

node* rotateleft(node* x)
{
	node* y;
	y = x->right;
	x->right = y->left;
	y->left = x;
	x->ht = height(x);
	y->ht = height(y);

	return(y);
}

node* RR(node* T)
{
	T = rotateleft(T);
	return(T);
}

node* LL(node* T)
{
	T = rotateright(T);
	return(T);
}

node* LR(node* T)
{
	T->left = rotateleft(T->left);
	T = rotateright(T);

	return(T);
}

node* RL(node* T)
{
	T->right = rotateright(T->right);
	T = rotateleft(T);
	return(T);
}

int BF(node* T)
{
	int lh, rh;
	if (T == NULL)
		return(0);

	if (T->left == NULL)
		lh = 0;
	else
		lh = 1 + T->left->ht;

	if (T->right == NULL)
		rh = 0;
	else
		rh = 1 + T->right->ht;

	return(lh - rh);
}

void preorder(node* T)
{
	if (T != NULL)
	{
		printf(" %d", T->data);
		preorder(T->left);
		preorder(T->right);
	}
}

void inorder(node* T)
{
	if (T != NULL)
	{
		inorder(T->left);
		printf(" %d", T->data);
		inorder(T->right);
	}
}

int count(node* T)
{
	if (T != NULL)
	{
		int i = 0;
		i += count(T->left);
		i++;
		i += count(T->right);
		return i;
	}
	return 0;
}

int find(node* T, int x)
{
	if (T != NULL)
	{
		if (x < T->data)
			return find(T->left, x);
		else if (T->data == x)
			return 1;
		else
			return find(T->right, x);
	}
	return 0;
}

int highest(node* T)
{
	if (T != NULL)
	{
		if (T->right)
			return highest(T->right);
		else
			return T->data;
	}
	return 0;
}

void search1(node* T, int x)
{
	if (T != NULL)
	{
		if (x <= T->data)
			search1(T->left, x);
		else
		{
			search1(T->left, x);
			printf(" %d", T->data);
			search1(T->right, x);
		}
	}
}

void search2(node* T, int x)
{
	if (T != NULL)
	{
		if (x >= T->data)
			search2(T->right, x);
		else
		{
			search2(T->left, x);
			printf(" %d", T->data);
			search2(T->right, x);
		}
	}
}

void search3(node* T, int x, int y)
{
	if (T != NULL)
	{
		if (T->data <= x)
			search3(T->right, x, y);
		else if (T->data >= y)
			search3(T->left, x, y);
		else
		{
			search3(T->left, x, y);
			printf(" %d", T->data);
			search3(T->right, x, y);
		}
	}
	return 0;
}
