#include <stdio.h>
#include <stdlib.h>

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))

#define DEGREE 2

typedef struct node_s {
	char value;
	struct node_s* child[DEGREE];
} node_t;

void Tree_InitNode(node_t* node) {
	node->value = '\0';
	for (int i = 0; i < ARRAYSIZE(node->child); i++) {
		node->child[i] = NULL;
	}
}

void visit(node_t* node) {
	if (!node)
		return;

	printf("Visit: %c\n", node->value);
	return;
}

void freenode(node_t* node) {
	if (!node)
		return;

	free(node);
}

int Tree_PostOrder(node_t* node, void(*pfnDo)(node_t*)) {
	if (!node)
		return 1;

	for (int i = 0; i < ARRAYSIZE(node->child); i++) {
		Tree_PostOrder(node->child[i], pfnDo);
	}
	pfnDo(node);
	return 0;
}

int Tree_PreOrder(node_t* node, void(*pfnDo)(node_t*)) {
	if (!node)
		return 1;

	pfnDo(node);
	for (int i = 0; i < ARRAYSIZE(node->child); i++) {
		Tree_PreOrder(node->child[i], pfnDo);
	}
	return 0;
}

int Tree_LevelOrder(node_t* node, int level, int levelnow, void(*pfnDo)(node_t*)) {
	if (!node)
		return 1;

	if (!level)
		pfnDo(node);

	if (levelnow < level)
		pfnDo(node);
	else
	{
		level += 1;
		for (int i = 0; i < ARRAYSIZE(node->child); i++) {
			Tree_LevelOrder(node->child[i], level, levelnow, pfnDo);
		}
		levelnow += 1;
		for (int i = 0; i < ARRAYSIZE(node->child); i++) {
			Tree_LevelOrder(node->child[i], level, levelnow, pfnDo);
		}
	}

	return 0;
}

int main()
{
	node_t* root = (node_t*)malloc(sizeof(node_t));
	if (!root)
		return;

	Tree_InitNode(root);
	root->value = 'X';

	node_t* temp = (node_t*)malloc(sizeof(node_t));
	if (temp)
	{
		Tree_InitNode(temp);
		temp->value = 'Y';
		root->child[0] = temp;

		temp = (node_t*)malloc(sizeof(node_t));
		if (temp)
		{
			Tree_InitNode(temp);
			temp->value = 'S';
			root->child[0]->child[0] = temp;
		}

		temp = (node_t*)malloc(sizeof(node_t));
		if (temp)
		{
			Tree_InitNode(temp);
			temp->value = 'T';
			root->child[0]->child[1] = temp;
		}
	}

	temp = (node_t*)malloc(sizeof(node_t));
	if (temp)
	{
		Tree_InitNode(temp);
		temp->value = 'Z';
		root->child[1] = temp;
	}

	printf("Postorder say:\n");
	Tree_PostOrder(root, visit);

	printf("\nPreorder say:\n");
	Tree_PreOrder(root, visit);

	Tree_PostOrder(root, freenode);

	return 1;
}
