// Binary Search Tree

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

struct Node* newNode(Item item);
void showMenu();
struct Node* loadItems(struct Node *root);
struct Node* connectTreeNode(struct Node *root, Item item);
void saveItemRecords(struct Node *root, FILE *fpItems);
void viewAllItems(struct Node *root);
Item getItems();
char* getItemId();
char * getRandomItemId();
float getNewitemPrice(char *itemDescription);
Item automatedItems();
void printRecord(Item *item);
void inorderTraversal(struct Node *root);
void preorderTraversal(struct Node *root);
void postorderTraversal(struct Node *root);
struct Node* getMatchingNode(struct Node*, char *itemId);
void search(struct Node *matchedNode);
void update(struct Node *matchedNode);
struct Node* delete(struct Node* root, char *itemId);
struct Node* getMinimumNode(struct Node *node);
struct Node* getMaximumNode(struct Node *node);
void printDeletionMessage(char *itemId);
void printIdNotFoundMessage(char *itemId, struct Node *matchedNode);
int treeNodesCount(struct Node *root);

void main()
{
	root = NULL;
	root = loadItems(root);
	printf("%d Nodes are available in the Tree.\n", treeNodesCount(root));

	do
	{
		showMenu();
	} while(1);
}

void showMenu()
{
	int option;
	char *itemId;
	struct Node *matchedNode;
	FILE *fpItems;

	printf("\n\t -------------- Menu -------------\n");
	printf("\t  0. Exit\n");
	printf("\t  1. Create Item\n");
	printf("\t  2. View All Item details\n");
	printf("\t  3. Update item price\n");
	printf("\t  4. Delete item\n");
	printf("\t  5. Search item\n");
	printf("\t -----------------------------------\n");
	printf("Enter your choice: ");
	scanf("%d", &option);

	switch(option)
	{
		case 1: //root = connectTreeNode(root, automatedItems());
				root = connectTreeNode(root, getItems());
				fpItems = fopen(DATAFILE, "w");
				saveItemRecords(root, fpItems);
				fclose(fpItems);
				break;
		case 2:	viewAllItems(root);		
				break;
		case 3: itemId = getItemId();
				matchedNode = getMatchingNode(root, itemId);
				printIdNotFoundMessage(itemId, matchedNode);
				update(matchedNode);
				fpItems = fopen(DATAFILE, "w");
				saveItemRecords(root, fpItems);
				fclose(fpItems);
				break;
		case 4: itemId = getItemId();
				root = delete(root, itemId);
				fpItems = fopen(DATAFILE, "w");
				saveItemRecords(root, fpItems);
				fclose(fpItems);
				break;

		case 5: itemId = getItemId();
				matchedNode = getMatchingNode(root, itemId);
				printIdNotFoundMessage(itemId, matchedNode);
				search(matchedNode);
				break;
		case 0: exit(0);
		default:printf("Please enter valid option!\n");
	}	
}

int treeNodesCount(struct Node *root)
{
	if (root == NULL)
	{
		return 0;
	}
	else
	{
		return 1 + treeNodesCount(root->leftChild) + treeNodesCount(root->rightChild);
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
    root = NULL;
    while(fread(&item, sizeof(Item), 1, fpItems) == 1)
    {
	    root = connectTreeNode(root, item);
    }
	fclose(fpItems);
	return root;
}

void saveItemRecords(struct Node *root, FILE *fpItems)
{
	if (root == NULL)
	{
		return;
	}
	if(fwrite(&root->data, sizeof(Item), 1, fpItems) == 0)
	{
		perror("Error:");
	}
	saveItemRecords(root->leftChild, fpItems);
	saveItemRecords(root->rightChild, fpItems);
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

Item automatedItems()
{
	Item item;
	srand(time(0));
	sprintf(item.itemId, "%d", 10 + rand() % (200 - 10 + 1));
	sprintf(item.itemDescription, "item%s", item.itemId);
	item.itemPrice = (float)rand() / 1000.0000;
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
			// printf("Left\n");
			root->leftChild = connectTreeNode(root->leftChild, item);
		}
		else if (comparsionValue > 0)
		{
			// printf("Right\n");
			root->rightChild = connectTreeNode(root->rightChild, item);
		}
		else
		{
			printf("%s is already existed\n", item.itemId);
		}
		return root;
	}
}

void inorderTraversal(struct Node *root)
{
	if (root != NULL)
	{
		inorderTraversal(root->leftChild);
		printRecord(&root->data);
		inorderTraversal(root->rightChild);
	}
}

void preorderTraversal(struct Node *root)
{
	if (root != NULL)
	{
		printRecord(&root->data);
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
		printRecord(&root->data);
	}
}

void printRecord(Item *item)
{
	printf("\n\tItem ID: %s\n", item->itemId);
	printf("\tItem Description: %s\n", item->itemDescription);
	printf("\tItem Price: %f\n", item->itemPrice);
}

struct Node* getMatchingNode(struct Node *root, char *itemId)
{
	if (root == NULL)
	{
		return NULL;
	}
	else
	{
		struct Node *matchedNode;
		if(atoi(root->data.itemId) == atoi(itemId))
		// if (strcmp(root->data.itemId, itemId) == 0)
		{
			return root;
		}
		if(atoi(root->data.itemId) > atoi(itemId))
		// if (strcmp(root->data.itemId, itemId) > 0)
		{
			matchedNode = getMatchingNode(root->leftChild, itemId);
		}
		else
		{
			 matchedNode = getMatchingNode(root->rightChild, itemId);
		}
		if (matchedNode != NULL)
		{
			return matchedNode;
		}
	}
}

void search(struct Node *matchedNode)
{
	if (matchedNode != NULL)
	{	
		printRecord(&matchedNode->data);
	}
}

float getNewitemPrice(char *itemDescription)
{
	float newitemPrice;
	printf("Enter new Price of %s: ", itemDescription);
	scanf("%f", &newitemPrice);
	return newitemPrice;
}

void update(struct Node *matchedNode)
{
	if (matchedNode != NULL)
	{	
		printRecord(&matchedNode->data);
		float newitemPrice = getNewitemPrice(matchedNode->data.itemDescription);
		matchedNode->data.itemPrice = newitemPrice;
		printf("Price updated successfully!\n");
	}
}

void viewAllItems(struct Node *root)
{
	printf("%d Nodes are available in the Tree.\n", treeNodesCount(root));
	printf("\t0. Exit\n");
	printf("\t1. preorderTraversal Traversal    Root-Left-Right\n");
	printf("\t2. inorderTraversal Traversal     Left-Root-Right\n");
	printf("\t3. postorderTraversal Traversal   Left-Right-Root\n");
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
		case 0: exit(0);
		default:printf("\tPlease enter valid option!\n");
	}

}

struct Node* delete(struct Node* root, char *itemId) 
{
    if (root == NULL) 
    {
    	printIdNotFoundMessage(itemId, NULL);
        return root;
    }

    if (atoi(itemId) < atoi(root->data.itemId)) 
    {
        root->leftChild = delete(root->leftChild, itemId);
    } 
    else if (atoi(itemId) > atoi(root->data.itemId)) 
    {
        root->rightChild = delete(root->rightChild, itemId);
    } 
    else 
    {
        if (root->leftChild == NULL) 
        {
            struct Node* replacementNode = root->rightChild;
            free(root);
            printDeletionMessage(itemId);
            return replacementNode;
        } 
        else if (root->rightChild == NULL) 
        {
            struct Node* replacementNode = root->leftChild;
            free(root);
            printDeletionMessage(itemId);
            return replacementNode;
        }
        struct Node* replacementNode = getMinimumNode(root->rightChild);
        root->data = replacementNode->data;
        root->rightChild = delete(root->rightChild, replacementNode->data.itemId);
    }
    return root;
}

struct Node* getMinimumNode(struct Node *matchedNode)
{
	struct Node *minimumNode = matchedNode;
	while (minimumNode != NULL && minimumNode->leftChild != NULL)
	{
		minimumNode = minimumNode->leftChild;
	}
	return minimumNode;
}

struct Node* getMaximumNode(struct Node *matchedNode)
{
	struct Node *maximumNode = matchedNode;
	while (maximumNode != NULL && maximumNode->rightChild != NULL)
	{
		maximumNode = maximumNode->rightChild;
	}
	return maximumNode;
}

void printDeletionMessage(char *itemId)
{
	printf("\tItem with ID %s successfully deleted.\n", itemId);
}

void printIdNotFoundMessage(char *itemId, struct Node *matchedNode)
{
	if (matchedNode == NULL)
	{
		printf("\tItem with ID %s not found!\n", itemId);
	}
}