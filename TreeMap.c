#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TreeMap.h"

#define MAX(a, b) (((a) >= (b)) ? (a) : (b))

/* Creeaza un arbore cu o serie de metode asociate
 *
 * return: arborele creat
 */
TTree *createTree(void *(*createElement)(void *),
				  void (*destroyElement)(void *),
				  void *(*createInfo)(void *),
				  void (*destroyInfo)(void *),
				  int compare(void *, void *))
{

	TTree *tree = (TTree *)calloc(1, sizeof(TTree));
	tree->root = (TreeNode *)calloc(1, sizeof(TreeNode));
	tree->root->height = 0;
	tree->root = NULL;
	tree->createElement = createElement;
	tree->destroyElement = destroyElement;
	tree->createInfo = createInfo;
	tree->destroyInfo = destroyInfo;
	tree->compare = compare;
	tree->size = 0;

	return tree;
}

/* Verifica daca un arbore este gol (vid)
 * 		1 - daca arborele este gol
 * 		0 - in caz contrar
 */
int isEmpty(TTree *tree)
{
	// dacă rădăcina arborelui este nulă
	// atunci arborele este gol

	if (tree->root == NULL)
		return 1;
	return 0;
}

/* Cauta un anumit element in interiorul unui arbore
 *
 * tree: structura cu metodele asociate arborelui
 *   !folosit pentru a putea compara elementul curent cu cel cautat
 *
 * x: radacina arborelui curent (in care se face cautarea)
 * elem: elementul ce trebuie cautat
 */
TreeNode *search(TTree *tree, TreeNode *x, void *elem)
{

	// verificăm dacă arborele este vid

	if (isEmpty(tree))
		return NULL;

	// cat timp nu am parcurs tot arborele
	// sau nu am gasit elementul
	// continuam cu urmatoarele operatii

	while (x != NULL && tree->compare(x->elem, elem) != 0)
	{
		// daca elementul nodului curent este mai mic decat elementul cautat
		// cautam in subarborele drept al nodului
		if (tree->compare(x->elem, elem) < 0)
			x = x->right;
		// iar in caz contrar, in subarborele stang
		else if (tree->compare(x->elem, elem) > 0)
			x = x->left;
	}
	// daca elementul a fost gasit il returnam
	if (x != NULL && tree->compare(x->elem, elem) == 0)
		return x;
	return NULL;
}

/* Gaseste nodul cu elementul minim dintr-un arbore
 * avand radacina in x
 */
TreeNode *minimum(TreeNode *x)
{

	if (x == NULL)
		return NULL;

	if (x->left != NULL)
		minimum(x->left);

	else
		return x;
}

/* Gaseste nodul cu elementul maxim dintr-un arbore
 * avand radacina in x
 */
TreeNode *maximum(TreeNode *x)
{
	if (x == NULL)
		return NULL;

	if (x->right != NULL)
		maximum(x->right);

	else
		return x;
}

/* Functie pentru gasirea succesorului unui nod
 * (succesorul in inordine)
 */
TreeNode *successor(TreeNode *x)
{
	if (x == NULL)
		return NULL;
	TreeNode *p = x;
	while (p->parent != NULL)
	{
		p = p->parent;
	}

	if (maximum(p) == x)
		return NULL;

	if (x->right != NULL)
		return minimum(x->right);

	while (x->parent != NULL)
	{
		if (x->parent->left == x)
			return x->parent;
		x = x->parent;
	}
	return NULL;
}

/* Functie pentru gasirea predecesorului unui nod
 * (predecesorul in inordine)
 */
TreeNode *predecessor(TreeNode *x)
{
	if (x == NULL)
		return NULL;
	TreeNode *p = x;
	while (p->parent != NULL)
	{
		p = p->parent;
	}

	if (minimum(p) == x)
		return NULL;

	if (x->left != NULL)
		return maximum(x->left);

	while (x->parent != NULL)
	{
		if (x->parent->right == x)
			return x->parent;
		x = x->parent;
	}

	return NULL;
}

/* Actualizeaza inaltimea unui nod din arbore
 */
void updateHeight(TreeNode *x)
{

	int leftHeight = 0;
	int rightHeight = 0;

	if (x != NULL)
	{
		if (x->left != NULL)
			leftHeight = x->left->height;
		if (x->right != NULL)
			rightHeight = x->right->height;
		x->height = MAX(leftHeight, rightHeight) + 1;
	}
}
int max(int a, int b)
{
	return (a > b) ? a : b;
}

/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la stanga
 * a subarborelui care are varful in x
 *
 *   (x)	 Rotatie    (y)
 *	 / \     stanga     / \
 *  a  (y)    ---->   (x)  c
 *	   / \			  / \
 *    b   c          a   b
 */
void avlRotateLeft(TTree *tree, TreeNode *x)
{
	TreeNode *y = x->right;
	// mutam b
	x->right = y->left;
	if (y->left != NULL && y != NULL)
		y->left->parent = x;
	y->parent = x->parent;
	// daca x este copil stanga
	if (x->parent != NULL && x->parent->left != NULL && tree->compare(x->parent->elem, x->elem) > 0)
	{
		x->parent->left = y;
	}
	// daca x este copil dreapta
	else if (x->parent != NULL && tree->compare(x->parent->elem, x->elem) < 0)
	{
		x->parent->right = y;
	}
	// daca x e root
	else if (x->parent == NULL)
	{
		tree->root = y;
	}
	// x devine copilul lui y
	y->left = x;
	x->parent = y;
	updateHeight(x);
	updateHeight(y);
}

/* Functie ce primeste adresa unui arbore si
 * a unui nod x si realizeaza o rotatie la dreapta
 * a subarborelui care are varful in y
 *
 *     (y)	 Rotatie    (x)
 *	   / \   dreapta    / \
 *   (x)  c   ---->    a  (y)
 *	 / \			      / \
 *  a   b                b   c
 */
void avlRotateRight(TTree *tree, TreeNode *y)
{
	TreeNode *x = y->left;
	// mutam b
	y->left = x->right;
	if (x->right != NULL && x != NULL)
		x->right->parent = y;

	x->parent = y->parent;
	// daca y e copil stanga
	if (y->parent != NULL && tree->compare(y->parent->elem, y->elem) > 0)
		y->parent->left = x;
	// daca y e copil dreapta
	else if (y->parent != NULL && tree->compare(y->parent->elem, y->elem) < 0)
		y->parent->right = x;
	// daca y e root
	else if (y->parent == NULL)
		tree->root = x;

	x->right = y;
	y->parent = x;
	updateHeight(y);
	updateHeight(x);
}

/* Calculeaza factorul de echilibrare pentru un nod x
 * (AVL balance factor)
 */
int avlGetBalance(TreeNode *x)
{

	if (x == NULL)
		return 0;
	if (x->right == NULL && x->left == NULL)
		return 0;
	if (x->right != NULL && x->left != NULL)
		return (x->left->height - x->right->height);
	if (x->right == NULL)
		return x->left->height;
	if (x->left == NULL)
		return -x->right->height;
}

/* Functie pentru reechilibrarea unui arbore AVL
 * in urma unei inserari prin rotatii simple sau duble
 * si recalcularea inaltimii fiecarui nod intalnit parcurgand
 * arborele de jos in sus, spre radacina
 *
 */
void avlFixUp(TTree *tree, TreeNode *y)
{

	if (y == NULL)
		return;

	TreeNode *x = y;

	while (y != NULL)
	{
		updateHeight(y);
		int balance = avlGetBalance(y);

		// Left-Left
		if (balance == 2 && tree->compare(y->left->elem, x->elem) > 0)
			avlRotateRight(tree, y);

		// Right-Right
		else if (balance == -2 && tree->compare(y->right->elem, x->elem) < 0)
			avlRotateLeft(tree, y);

		// Left-Right
		else if (balance == 2 && tree->compare(y->left->elem, x->elem) < 0)
		{
			avlRotateLeft(tree, y->left);
			avlRotateRight(tree, y);
		}

		// Right-Left
		else if (balance == -2 && tree->compare(y->right->elem, x->elem) > 0)
		{
			avlRotateRight(tree, y->right);
			avlRotateLeft(tree, y);
		}
		y = y->parent;
	}
}

/* Functie pentru crearea unui nod
 *
 * value: valoarea/cheia din cadrul arborelui
 * info: informatia/valoarea din dictionar
 */
TreeNode *createTreeNode(TTree *tree, void *value, void *info)
{

	if (tree == NULL)
		return NULL;

	// Alocarea memoriei
	TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));

	// Setarea elementului si a informatiei
	node->elem = tree->createElement(value);
	node->info = tree->createInfo(info);

	// Initializarea legaturilor din cadrul arborelui
	node->parent = node->right = node->left = NULL;

	// Initializarea legaturilor din cadrul listei dublu inlantuite
	node->next = node->prev = node->end = NULL;

	// Inaltimea unui nod nou este 1
	// Inaltimea lui NULL este 0
	node->height = 1;

	return node;
}

/* Inserarea unul nou nod in cadrul multi-dictionarului
 * ! In urma adaugarii arborele trebuie sa fie echilibrat
 *
 */
void insert(TTree *tree, void *elem, void *info)
{

	TreeNode *nod_nou = createTreeNode(tree, elem, info);

	if (isEmpty(tree))
	{
		tree->root = nod_nou;
		nod_nou->end = nod_nou;
		nod_nou->next = nod_nou->prev = nod_nou;
		tree->size++;
		return;
	}

	TreeNode *x = tree->root;
	int ok = 1;

	while (x != NULL && ok == 1)
	{

		if (tree->compare(x->elem, elem) < 0)
		{

			if (x->right == NULL)
			{
				x->right = nod_nou;
				nod_nou->parent = x;
				nod_nou->next = NULL;
				nod_nou->prev = predecessor(nod_nou);
				nod_nou->end = nod_nou;
				tree->size++;
				ok = 0;
			}
			else
				x = x->right;
		}

		else if (tree->compare(x->elem, elem) > 0)
		{

			if (x->left == NULL)
			{
				x->left = nod_nou;
				nod_nou->parent = x;
				nod_nou->next = NULL;
				nod_nou->prev = predecessor(nod_nou);
				nod_nou->end = nod_nou;
				tree->size++;
				ok = 0;
			}
			else
				x = x->left;
		}
		else if (tree->compare(x->elem, elem) == 0)
		{
			nod_nou->next = NULL;
			nod_nou->prev = nod_nou->end;
			nod_nou->end = nod_nou;
			return;
		}
	}

	avlFixUp(tree, nod_nou);
}

/* Eliminarea unui nod dintr-un arbore
 *
 * ! tree trebuie folosit pentru eliberarea
 *   campurilor `elem` si `info`
 * */
void destroyTreeNode(TTree *tree, TreeNode *node)
{

	// Verificarea argumentelor functiei
	if (tree == NULL || node == NULL)
		return;

	// Folosirea metodelor arborelui
	// pentru de-alocarea campurilor nodului
	tree->destroyElement(node->elem);
	tree->destroyInfo(node->info);

	// Eliberarea memoriei nodului
	free(node);
}

/* Eliminarea unui nod din arbore
 *
 * elem: cheia nodului ce trebuie sters
 * 	! In cazul in care exista chei duplicate
 *	  se va sterge ultimul nod din lista de duplicate
 */
void delete (TTree *tree, void *elem)
{
}

/* Eliberarea memoriei unui arbore
 */
void destroyTree(TTree *tree)
{

	/* Se poate folosi lista dublu intalntuita
	 * pentru eliberarea memoriei
	 */
	if (tree == NULL || tree->root == NULL)
		return;
}