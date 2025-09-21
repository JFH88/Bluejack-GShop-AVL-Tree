#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct gameNode
{
	char title[27];
	char type[12];
	int stock;
	int key;
	int height;
	struct gameNode *right;
	struct gameNode *left;
	struct gameNode *parent;
};

struct gameNode *root = NULL;

void strToLowercase(char *str)
{
	int i;
	for(i=0;i<strlen(str);i++)
	{
		str[i] = tolower(str[i]);
	}
}

void insertBST(struct gameNode *ptrRoot, struct gameNode *newGame)
{
	if(ptrRoot->key > newGame->key && ptrRoot->left == NULL)
	{
		ptrRoot->left = newGame;
		newGame->parent = ptrRoot;
	}
	else if(ptrRoot->key < newGame->key && ptrRoot->right == NULL)
	{
		ptrRoot->right = newGame;
		newGame->parent = ptrRoot;
	}
	else if(ptrRoot->key < newGame->key)
	{
		insertBST(ptrRoot->right, newGame);
	}
	else if(ptrRoot->key > newGame->key)
	{
		insertBST(ptrRoot->left, newGame);
	}
}

struct gameNode *searching(struct gameNode *ptrRoot, struct gameNode *newGame)
{
	while(ptrRoot != NULL)
	{
		if(ptrRoot->key > newGame->key)
		{
			ptrRoot = ptrRoot->left;
		}
		else if(ptrRoot->key < newGame->key)
		{
			ptrRoot = ptrRoot->right;
		}
		else
		{
			return ptrRoot;
		}
	}
	return NULL;
}

int max(int a, int b)
{
	if(a > b)
		return a;
	else
		return b;
}

void updateHeight(struct gameNode *ptr)
{
	while(ptr != NULL)
	{
		int a = 0;
		int b = 0;
		
		if(ptr->left != NULL)
		{
			a = ptr->left->height;
		}
		
		if(ptr->right != NULL)
		{
			b = ptr->right->height;
		}
		
		ptr->height = max(a, b) + 1;
		
		ptr = ptr->parent;
	}
}

int checkBalanceFactor(struct gameNode *leftSubtree, struct gameNode *rightSubtree)
{
	int left = 0;
	int right = 0;
	
	if(leftSubtree != NULL)
	{
		left = leftSubtree->height;
	}
	
	if(rightSubtree != NULL)
	{
		right = rightSubtree->height;
	}
	
	return left-right;
}

void leftRotation(struct gameNode *ptr)
{
	if(ptr->parent != NULL)
	{
		if(ptr->parent->left == ptr)
		{
			ptr->parent->left = ptr->right;
		}
		else if(ptr->parent->right == ptr)
		{
			ptr->parent->right = ptr->right;
		}
	}
	else
	{
		root = ptr->right;
	}
	
	ptr->right->parent = ptr->parent;
	
	ptr->parent = ptr->right;
	ptr->right = ptr->parent->left;
	ptr->parent->left = ptr;
	
	if(ptr->right != NULL)
	{
		ptr->right->parent = ptr;
	}
}

void rightRotation(struct gameNode *ptr)
{
	if(ptr->parent != NULL)
	{
		if(ptr->parent->left == ptr)
		{
			ptr->parent->left = ptr->left;
		}
		else
		{
			ptr->parent->right = ptr->left;
		}
	}
	else
	{
		root = ptr->left;
	}
	
	ptr->left->parent = ptr->parent;
	
	ptr->parent = ptr->left;
	ptr->left = ptr->parent->right;
	ptr->parent->right = ptr;
	
	if(ptr->left != NULL)
	{
		ptr->left->parent = ptr;
	}
}

void balancing(struct gameNode *ptrRoot, struct gameNode *newGame)
{
	struct gameNode *ptrNewGame = newGame;
	
	updateHeight(ptrNewGame);
	
	struct gameNode *ptr = ptrNewGame->parent;
	while(ptr != NULL)
	{
		int balanceFactor = checkBalanceFactor(ptr->left, ptr->right);
		
		if(balanceFactor < -1 || balanceFactor > 1)
		{
			if(balanceFactor > 1)
			{
				int leftBalanceFactor = checkBalanceFactor(ptr->left->left, ptr->left->right);
				
				if(leftBalanceFactor > 0)
				{
					rightRotation(ptr);
				}
				else
				{
					leftRotation(ptr->left);
					rightRotation(ptr);
					updateHeight(ptr->parent->left);
				}
			}
			else if(balanceFactor < -1)
			{
				int rightBalanceFactor = checkBalanceFactor(ptr->right->left, ptr->right->right);
				
				if(rightBalanceFactor > 0)
				{
					rightRotation(ptr->right);
					leftRotation(ptr);
					updateHeight(ptr->parent->right);
				}
				else
				{
					leftRotation(ptr);
				}
			}
			
			updateHeight(ptr);
		}
		ptr = ptr->parent;
	}
}

void insertNode(struct gameNode *newGame)
{
	struct gameNode *ptrRoot = root;
	
	if(root == NULL)
	{
		root = newGame;
	}
	else
	{
		insertBST(ptrRoot, newGame);
	}
	
	balancing(ptrRoot, newGame);
}

int validateTitle(struct gameNode *newGame)
{
	struct gameNode *ptr = searching(root, newGame);
	if(strlen(newGame->title) < 5 || strlen(newGame->title) > 25)
	{
		return 0;
	}
	
	if(ptr != NULL)
	{
		return 0;
	}
	
	return 1;
}

int validateType(char *type)
{
	if(strcmp(type, "Action") == 0 || strcmp(type, "RPG") == 0 || strcmp(type, "Adventure") == 0 || strcmp(type, "Card Game") == 0)
	{
		return 1;
	}
	return 0;
}

int validateStock(int stock)
{
	if(stock >= 1)
	{
		return 1;
	}
	return 0;
}

int makeKey(char *title)
{
	int total = 0, i;
	
	for(i=0;i<strlen(title);i++)
	{
		total += (int)title[i];
	}
	
	return total;
}

void insertGame()
{
	struct gameNode *newGame = (struct gameNode*)calloc(sizeof(struct gameNode), 1);
	newGame->left = NULL;
	newGame->parent = NULL;
	newGame->right = NULL;
	
	do{
		printf("Input game title[5-25][unique]: ");
		fflush(stdin);
		scanf("%[^\n]", newGame->title);
	}while(validateTitle(newGame) != 1);
	
	do{
		printf("Input game type[Action|RPG|Adventure|Card Game]: ");
		fflush(stdin);
		scanf("%[^\n]", newGame->type);
		
	}while(validateType(newGame->type) != 1);
	
	do{
		printf("Input game stock[>= 1]: ");
		fflush(stdin);
		scanf("%d", &newGame->stock);
	}while(validateStock(newGame->stock) != 1);
	
	newGame->key = makeKey(newGame->title);
	
	insertNode(newGame);
	printf("Insert success !\n");
}

void printInOrder(struct gameNode *ptrRoot)
{
	if(ptrRoot->left != NULL)
	{
		printInOrder(ptrRoot->left);
	}
	
	printf("| %-27s | %-12s | %-12d |\n", ptrRoot->title, ptrRoot->type, ptrRoot->stock);
		
	int i;
	for(i=0;i<61;i++) printf("-");
	printf("\n");
	
	if(ptrRoot->right != NULL)
	{
		printInOrder(ptrRoot->right);
	}
}

void viewGame()
{
	if(root == NULL)
	{
		printf("Warehouse is empty!\n\n");
	}
	else
	{
		int i;
		for(i=0;i<61;i++) printf("-");
		printf("\n");
		
		printf("| %-27s | %-12s | %-12s |\n", "Game Title", "Game Genre", "Game Stock");
		
		for(i=0;i<61;i++) printf("-");
		printf("\n");
		
		printInOrder(root);
	}
}

struct gameNode *searchTitle(char *inputTitle)
{
	int inputKey = makeKey(inputTitle);
	
	struct gameNode *ptr = root;
	
	while(ptr != NULL)
	{
		if(inputKey < ptr->key)
		{
			ptr = ptr->left;
		}
		else if(inputKey > ptr->key)
		{
			ptr = ptr->right;
		}
		else
		{
			if(strcmp(ptr->title, inputTitle) == 0)
			{
				return ptr;
			}
		}
	}
}

void deleteNode(struct gameNode *currNode)
{
	struct gameNode *ptr;
	struct gameNode *fromHere;
	
	if(currNode->left == NULL && currNode->right == NULL)
	{
		fromHere = currNode->parent;
		
		if(currNode->parent != NULL)
		{
			if(currNode->parent->left == currNode)
			{
				currNode->parent->left = NULL;
			}
			else if(currNode->parent->right == currNode)
			{
				currNode->parent->right = NULL;
			}
		}
		else
		{
			root = NULL;
		}
	}
	else if(currNode->left == NULL || currNode->right == NULL)
	{
		fromHere = currNode->parent;
		
		if(currNode->parent != NULL)
		{
			if(currNode->parent->left == currNode)
			{
				if(currNode->left != NULL)
				{
					currNode->parent->left = currNode->left;
				}
				else if(currNode->right != NULL)
				{
					currNode->parent->left = currNode->right;
				}
			}
			else if(currNode->parent->right == currNode)
			{
				if(currNode->left != NULL)
				{
					currNode->parent->right = currNode->left;
				}
				else if(currNode->right != NULL)
				{
					currNode->parent->right = currNode->right;
				}
			}
		}
		else
		{
			if(currNode->left != NULL)
			{
				root = currNode->left;
			}
			else if(currNode->right != NULL)
			{
				root = currNode->right;
			}
		}
	}
	else
	{
		if(currNode->left != NULL)
		{
			ptr = currNode->left;
		
			if(ptr->right == NULL)
			{
				fromHere = currNode->right;
				
				ptr->right = currNode->right;
				ptr->parent = currNode->parent;
				
				currNode->right->parent = ptr;
				
				if(currNode->parent != NULL)
				{
					if(currNode->parent->left == currNode)
					{
						currNode->parent->left = ptr;
					}
					else if(currNode->parent->right == currNode)
					{
						currNode->parent->right  = ptr;
					}
				}
				else
				{
					root = ptr;
				}
			}
			else
			{
				while(ptr->right != NULL)
				{
					ptr = ptr->right;
				}
				
				ptr->parent->right = ptr->left;
				
				if(ptr->left != NULL)
				{
					ptr->left->parent = ptr->parent;
				}
				
				fromHere = ptr->parent;
				
				if(currNode->parent != NULL)
				{
					if(currNode->parent->left == currNode)
					{
						currNode->parent->left = ptr;
					}
					else if(currNode->parent->right == currNode)
					{
						currNode->parent->right  = ptr;
					}
				}
				else
				{
					root = ptr;
				}
				currNode->left->parent = ptr;
				currNode->right->parent = ptr;
				
				ptr->parent = currNode->parent;
				ptr->left = currNode->left;
				ptr->right = currNode->right;
			}
		}
		else if(currNode->right != NULL)
		{
			ptr = currNode->right;
			
			if(ptr->left == NULL)
			{
				fromHere = currNode->left;
				
				ptr->left = currNode->left;
				ptr->parent = currNode->parent;
				
				currNode->left->parent = ptr;
				
				if(currNode->parent != NULL)
				{
					if(currNode->parent->left == currNode)
					{
						currNode->parent->left = ptr;
					}
					else if(currNode->parent->right == currNode)
					{
						currNode->parent->right  = ptr;
					}
				}
				else
				{
					root = ptr;
				}
			}
			else
			{
				while(ptr->left != NULL)
				{
					ptr = ptr->left;
				}
				
				fromHere = ptr->parent;
				
				if(ptr->right != NULL)
				{
					ptr->right->parent = ptr->parent;
				}
				ptr->parent->left = ptr->right;
				
				if(currNode->parent != NULL)
				{
					if(currNode->parent->left == currNode)
					{
						currNode->parent->left = ptr;
					}
					else if(currNode->parent->right == currNode)
					{
						currNode->parent->right = ptr;
					}
				}
				else
				{
					root = ptr;
				}
				currNode->right->parent = ptr;
				currNode->left->parent = ptr;
				
				ptr->parent = currNode->parent;
				ptr->right = currNode->right;
				ptr->left = currNode->left;
			}
		}
	}
	
	free(currNode);
	
	balancing(root, fromHere);
}

int validUpdateType(char *str)
{
	if(strcmp(str, "add") == 0)
	{
		return 2;
	}
	else if(strcmp(str, "remove") == 0)
	{
		return 1;
	}
	return 0;
}

void updateStock()
{
	char inputTitle[27];
	printf("Input game title: ");
	fflush(stdin);
	scanf("%[^\n]", inputTitle);
	
	struct gameNode *theGame = searchTitle(inputTitle);
	
	if(theGame == NULL)
	{
		printf("Data not found !\n\n");
	}
	else
	{
		printf("Current stock: %d\n\n", theGame->stock);
		
		char updateType[8];
		int updateCode = 0;
		
		do{
			printf("Input update type[add|remove][case insensitive]: ");
			fflush(stdin);
			scanf("%s", updateType);
			strToLowercase(updateType);
			updateCode = validUpdateType(updateType);
		}while(updateCode == 0);
		
		int updateStock = 0;
		
		if(updateCode == 1)
		{
			do{
				printf("Input stock to remove[1-%d]: ", theGame->stock);
				fflush(stdin);
				scanf("%d", &updateStock);
			}while(updateStock < 1 || updateStock > theGame->stock);
			
			theGame->stock -= updateStock;
		}
		else if(updateCode == 2)
		{
			do{
				printf("Input stock to add: ");
				fflush(stdin);
				scanf("%d", &updateStock);
			}while(updateStock < 1);
			
			theGame->stock += updateStock;
		}
		
		printf("Data updated successfully !\n");
		
		if(theGame->stock == 0)
		{
			deleteNode(theGame);
			printf("%s is removed from the Warehouse !\n", inputTitle);
		}
	}
}

void printMenu()
{
	printf("Bluejack GShop\n");
	printf("==============\n");
	printf("1. Insert Game\n");
	printf("2. View Game\n");
	printf("3. Update Stock\n");
	printf("4. Exit\n");
	printf(">> ");
}

void menu()
{
	int operation = 0;
	
	do{
		do{
			system("cls");
			printMenu();
			scanf("%d", &operation);
		}while(operation < 1 || operation > 4);
		
		switch(operation)
		{
			case 1:
				insertGame(root);
				break;
			case 2:
				viewGame(root);
				break;
			case 3:
				updateStock(root);
				break;
			case 4:
				printf("Thank you for using this program\n\n");
		}
		
		printf("Press enter to continue...");
		getch();
	}while(operation != 4);
}

int main()
{
	menu();
	
	return 0;
}
