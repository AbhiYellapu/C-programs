// Binary search tree for item domain

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAXIDLENGTH 20
#define MAXDESCRIPTIONLENGTH 50
#define DATAFILE "ItemDetails.dat"

typedef struct
{
	char itemId[MAXIDLENGTH];
	char itemDescription[MAXDESCRIPTIONLENGTH];
	float itemPrice;
} Item;

struct Node
{
	struct Node *leftChild;
	Item data;
	struct Node *rightChild;
} *root;

struct NodeWithParent
{
	struct Node *node;
	struct Node *parent;
};

struct Node* newNode(Item item);
struct Node* loadItems(struct Node *root);
void saveItems(struct Node *root, FILE *fpItems);
void saveHelper(struct Node *root);
Item getItems();
void createItem(struct Node *root);
char *getRandomItemId();
void printItemDetails(Item *item);
void showMenu();
char* getItemId();
int getItemsCount(struct Node *root);
struct Node* connectTreeNode(struct Node *root, Item item);
void inorderTraversal(struct Node *root);
void preorderTraversal(struct Node *root);
void postorderTraversal(struct Node *root);
struct NodeWithParent* getMatchingNode(struct Node*, char *itemId, struct Node *parent);
void searchItem(struct Node *root, char *itemId);
void updateHelper(struct Node *root);
void deleteHelper(struct Node *root);
void updateItemPrice(struct Node *matchedNode);
float getNewItemPrice(char *itemDescription);
void viewAllItems(struct Node *root);
struct Node* getminimumNode(struct Node *node);
void deleteItem(struct NodeWithParent *matchedNode);
struct Node** getParentNodeAddress(struct NodeWithParent *matchedNode);
void printItemNotFoundMessage(char *itemId, struct Node *matchedNode);
void printDeletionMessage(char *itemId);

void main()
{
	root = NULL;
	root = loadItems(root);
	printf("%d Items are available.\n", getItemsCount(root));

	do
	{
		showMenu();
	} while(1);
}

void showMenu()
{
	int option;

	printf("\n\t -------------- Menu -------------\n");
	printf("\t  0. Exit\n");
	printf("\t  1. Create an item\n");
	printf("\t  2. View all item details\n");
	printf("\t  3. Update item price\n");
	printf("\t  4. Delete item\n");
	printf("\t  5. Search item\n");
	printf("\t -----------------------------------\n");

	printf("Enter your choice: ");
	scanf("%d", &option);
	switch(option)
	{
		case 1: createItem(root);
				break;

		case 2:	viewAllItems(root);		
				break;

		case 3: updateHelper(root);
				break;

		case 4: deleteHelper(root);
				break;

		case 5: searchItem(root, getItemId());
				break;
		case 0: exit(0);
		default:printf("Please enter valid option!\n");
	}	
}

int getItemsCount(struct Node *root)
{
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + getItemsCount(root->leftChild) + getItemsCount(root->rightChild);
	}
}

struct Node* newNode(Item item)
{
	struct Node *newNode;
	newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->leftChild = NULL;
	newNode->data = item;
	newNode->rightChild = NULL;
	return newNode;
}

struct Node* loadItems(struct Node *root) 
{
    Item item;
    FILE *fpItems = fopen(DATAFILE, "r");
    while(fread(&item, sizeof(Item), 1, fpItems) == 1)
    {
	    root = connectTreeNode(root, item);
    }
	fclose(fpItems);
	return root;
}

void saveItems(struct Node *root, FILE *fpItems)
{
	if (root == NULL)
	{
		return;
	}
	if(fwrite(&root->data, sizeof(Item), 1, fpItems) == 0)
	{
		perror("Error in Writing into file:");
	}
	saveItems(root->leftChild, fpItems);
	saveItems(root->rightChild, fpItems);
}

char * getRandomItemId()
{
	srand(time(0));
	char *itemId = malloc(MAXIDLENGTH);
	sprintf(itemId, "%d", 10 + rand() % (150 - 10 + 1));
	
	return itemId;
}

Item getItems()
{
	Item item;
	strcpy(item.itemId, getRandomItemId());
	printf("Enter %s Item details:\n", item.itemId);
	printf("Enter Description: ");
	scanf("%s", item.itemDescription);
	printf("Enter %s Price: ", item.itemDescription);
	scanf("%f", &item.itemPrice);
	return item;
}

char* getItemId()
{
	char *itemId = (char*)malloc(MAXIDLENGTH);
	printf("Enter Item ID: ");
	scanf("%s", itemId);
	return itemId;
}

struct Node* connectTreeNode(struct Node *root, Item item)
{
	if (root == NULL)
	{
		return newNode(item);
	}
	else
	{
		// int comparsionValue = strcmp(item.itemId, root->data.itemId);
		int comparsionValue = -1;
		if(atoi(item.itemId) > atoi(root->data.itemId))
		{
			comparsionValue = 1;
		}
		else if(atoi(item.itemId) == atoi(root->data.itemId))
		{
			comparsionValue = 0;
		}
		if(comparsionValue < 0)
		{
			root->leftChild = connectTreeNode(root->leftChild, item);
		}
		else if (comparsionValue > 0)
		{
			root->rightChild = connectTreeNode(root->rightChild, item);
		}
		else
		{
			printf("\t%s is already existed\n", item.itemId);
		}
		return root;
	}
}

void inorderTraversal(struct Node *root)
{
	if (root != NULL)
	{
		inorderTraversal(root->leftChild);
		printItemDetails(&root->data);
		inorderTraversal(root->rightChild);
	}
}

void preorderTraversal(struct Node *root)
{
	if (root != NULL)
	{
		printItemDetails(&root->data);
		preorderTraversal(root->leftChild);
		preorderTraversal(root->rightChild);
	}
}
void postorderTraversal(struct Node *root)
{
	if (root != NULL)
	{
		postorderTraversal(root->leftChild);
		postorderTraversal(root->rightChild);
		printItemDetails(&root->data);
	}
}

void printItemDetails(Item *item)
{
	printf("\n\tItem ID: %s\n", item->itemId);
	printf("\tItem Description: %s\n", item->itemDescription);
	printf("\tItem Price: %f\n", item->itemPrice);
}

struct NodeWithParent* getMatchingNode(struct Node *root, char *itemId, struct Node *parent)
{
	struct NodeWithParent* matchedNode = (struct NodeWithParent*)malloc(sizeof(struct NodeWithParent));
	matchedNode->node = NULL;
	matchedNode->parent = parent;
	if (root == NULL)
	{
		return matchedNode;
	}
	else
	{
		if(atoi(root->data.itemId) == atoi(itemId))
		// if (strcmp(root->data.itemId, itemId) == 0)
		{
			matchedNode->node = root;
			return matchedNode;
		}
		if(atoi(root->data.itemId) > atoi(itemId))
		// if (strcmp(root->data.itemId, itemId) > 0)
		{
			matchedNode = getMatchingNode(root->leftChild, itemId, root);
		}
		else
		{
			 matchedNode = getMatchingNode(root->rightChild, itemId, root);
		}
	}
}

void searchItem(struct Node *root, char *itemId)
{
	struct NodeWithParent *matchedNode;
	matchedNode = getMatchingNode(root, itemId, NULL);
	if (matchedNode->node == NULL)
	{
		printItemNotFoundMessage(itemId, matchedNode->node);
	}
	else
	{
		printItemDetails(&(matchedNode->node)->data);
	}
}

float getNewItemPrice(char *itemDescription)
{
	float newItemPrice;
	printf("Enter new Price of %s: ", itemDescription);
	scanf("%f", &newItemPrice);
	return newItemPrice;
}

void updateItemPrice(struct Node *matchedNode)
{
	if (matchedNode != NULL)
	{	
		printItemDetails(&matchedNode->data);
		float newItemPrice = getNewItemPrice(matchedNode->data.itemDescription);
		matchedNode->data.itemPrice = newItemPrice;
		printf("Price updated successfully!\n");
	}
}

void viewAllItems(struct Node *root)
{
	printf("\n\t%d Items are available.\n", getItemsCount(root));
	printf("\t1. Preorder Traversal\n");
	printf("\t2. Inorder Traversal\n");
	printf("\t3. Postorder Traversal\n");
	printf("Enter: ");
	int option;
	scanf("%d", &option);
	switch(option)
	{
		case 1:	preorderTraversal(root);
				break;
		case 2: inorderTraversal(root);
				break;
		case 3: postorderTraversal(root);
				break;
		default:printf("\tPlease enter valid option!\n");
	}

}

void deleteItem(struct NodeWithParent *matchedNode)
{
	if (matchedNode->node != NULL)
	{
		struct Node *nodeTodeleteItem = NULL;
		struct Node **parentNodeAddress = NULL;
		if ((matchedNode->node)->rightChild == NULL)
		{
			nodeTodeleteItem = matchedNode->node;
			if (matchedNode->node == root)
			{
				parentNodeAddress = &root;
			}
			else
			{
				parentNodeAddress = getParentNodeAddress(matchedNode);
			}
			*parentNodeAddress = matchedNode->node->leftChild;
		}
		else if ((matchedNode->node)->leftChild == NULL)
		{
			nodeTodeleteItem = matchedNode->node;
			parentNodeAddress = getParentNodeAddress(matchedNode);
			*parentNodeAddress = matchedNode->node->rightChild;
		}
		else
		{
			struct Node *minimumNode = getminimumNode(matchedNode->node->rightChild);
			Item item = matchedNode->node->data;
			matchedNode->node->data = minimumNode->data;
			minimumNode->data = item;
			struct NodeWithParent *minimumNodeParent = getMatchingNode(root, minimumNode->data.itemId, NULL);
			deleteItem(&(struct NodeWithParent){minimumNode, matchedNode->node});
		}
		free(nodeTodeleteItem);
	}
}


struct Node* getminimumNode(struct Node *matchedNode)
{
	struct Node *minimumNode = matchedNode;
	while (minimumNode != NULL && minimumNode->leftChild != NULL)
	{
		minimumNode = minimumNode->leftChild;
	}
	return minimumNode;
}

struct Node** getParentNodeAddress(struct NodeWithParent *matchedNode)
{
	struct Node **parentNode = &(matchedNode->parent->rightChild);
	if(matchedNode->node == matchedNode->parent->leftChild)
	{
		parentNode = &(matchedNode->parent->leftChild);
	}
	return parentNode;
}

void createItem(struct Node *root)
{
	root = connectTreeNode(root, getItems());
	saveHelper(root);
}

void updateHelper(struct Node *root)
{
	char *itemId = getItemId();
	struct NodeWithParent *matchedNode;
	matchedNode = getMatchingNode(root, itemId, NULL);
	if (matchedNode->node == NULL)
	{
		printItemNotFoundMessage(itemId, matchedNode->node);
	}
	else
	{
		updateItemPrice(matchedNode->node);
		saveHelper(root);
	}
}
void deleteHelper(struct Node *root)
{
	char *itemId = getItemId();
	struct NodeWithParent *matchedNode;
	matchedNode = getMatchingNode(root, itemId, NULL);
	if (matchedNode->node == NULL)
	{
		printItemNotFoundMessage(itemId, matchedNode->node);
	}
	else
	{
		printItemDetails(&(matchedNode->node)->data);
		int confirmation;
		printf("Are you sure to delete this item?\n");
		printf("Press 1 to delete\n");
		printf("Press 0 to cancel\n");
		scanf("%d", &confirmation);
		if (confirmation == 1)
		{
			deleteItem(matchedNode);
			saveHelper(root);
			printDeletionMessage(itemId);
		}
		else
		{
			printf("\t%s Item Deletion canceled!\n", itemId);
		}
	}
}

void saveHelper(struct Node *root)
{
	FILE *fpItems = fopen(DATAFILE, "w");
	saveItems(root, fpItems);
	fclose(fpItems);
}

void printDeletionMessage(char *itemId)
{
	printf("\tItem with ID %s successfully deleted.\n", itemId);
}

void printItemNotFoundMessage(char *itemId, struct Node *matchedNode)
{
	if (matchedNode == NULL)
	{
		printf("\tItem with ID %s not found!\n", itemId);
	}
}
