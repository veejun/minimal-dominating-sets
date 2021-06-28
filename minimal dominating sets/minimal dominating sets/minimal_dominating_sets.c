#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* struct */
struct TreeNode{
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
};
typedef struct TreeNode Node;

struct QueueNode{
    Node *data;
    struct QueueNode *next;
};
typedef struct QueueNode QNode;

QNode *front = NULL;
QNode *rear = NULL;

int glob_count = 0; //store the number of base here

/* function */
void enqueue(Node *);
void dequeue();
Node *newNode(int); //create new node
Node *build_tree(char *str, const char *del); 
int is_pick(Node *); 
//int _print_t(Node *, int, int, int , char s[][255]);
//void print_t(Node *);

/* main */

int main(){
	
	/* read file */	
	FILE *fpr;
	fpr = fopen("./data/test2.txt", "r");

	char *buffer = 0;
	int length;
	
	if(fpr){
		fseek(fpr, 0, SEEK_END);
		length = ftell(fpr) - 2;
		fseek(fpr, 1, SEEK_SET);
		buffer = malloc (length);
		
		if(buffer){
	    	fread (buffer, 1, length, fpr);
		}
	}
	//printf("%s", buffer);
	fclose(fpr);
	
	if(buffer){
		Node *root = build_tree(buffer, ",");
		if(root && !(root->left) && !(root->right)){ //only one node
			printf("1");
		}else{
			int root_pick = is_pick(root);
			if(root_pick == -1){
				glob_count++;
			}
			printf("%d\n", glob_count);
		}		
		//print_t(root);		
	}
		
	return 0;
}

void enqueue(Node *node){
	QNode *temp = (QNode *)malloc(sizeof(QNode));
	temp->data = node;
	temp->next = NULL;
	if(front == NULL && rear == NULL){ //Queue is empty
		front = rear = temp;
	}else{
		rear->next = temp;
		rear = temp;
	}
	
}

void dequeue(){
	if(front == NULL){
		printf("Queue is empty.\n");
		return;
	}
	if(front == rear){
		front = rear = NULL;
	}else{
		QNode *temp = front;
		front = front->next;
		free(temp);
	}	
}

Node *newNode(int data){

	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->data = data;
	new_node->left = NULL;
	new_node->right = NULL;

	return new_node;
}

Node *build_tree(char *str, const char *del){

	Node *root;
	Node *currentNode;
	//struct QueueNode* queue = createQueue(1000);
	char *token;
	
	/* get the first token */
	token = strtok(str, del);
	if(strcmp(token,"null")!=0){
		root = newNode(atoi(token));
		currentNode = root;
	}else{
		return NULL;
	}

	/* walk through other tokens */
	token = strtok(NULL, del);
	
		
	while( token != NULL ) {
		
		//left child
		if(strcmp(token,"null")!=0){
			Node *new_node  = newNode(atoi(token));
            currentNode->left = new_node;
            //printf("%d\n", new_node->data);
            enqueue(new_node);
		}
		
		token = strtok(NULL, del);
		if (token == NULL){
            break;
        }
		
		//right child
        if (strcmp(token,"null")!=0){
            Node *new_node  = newNode(atoi(token));
            currentNode->right = new_node;
            //printf("%d\n", new_node->data);
            enqueue(new_node);
        }
        currentNode = front->data;
        //printf( "currentNode:%d\n", currentNode->data );
        dequeue();            
				
		token = strtok(NULL, del);
	}
	return root;
}


int is_pick(Node *root) 
{ 	
	// if node is empty
    if(root == NULL){
    	//printf("/case NULL/  ");
        return 0; 
	}else{
		//printf("\nroot data = %d  ", root->data);
	}
    
    // leaf point
    if(root->left == NULL && root->right == NULL){
    	//printf("/case leaf/  ");
    	return -1;
	} 
    	
    int left_pick = is_pick(root->left);   //printf("left_pick = %d  ", left_pick);
	int right_pick = is_pick(root->right); //printf("right_pick = %d  ", right_pick);
	     
	if(left_pick == -1 || right_pick == -1){ //one or both of the children are leaves
		//printf("/case leaves' parent/  ");
		glob_count++;
		return 1;		
	}
	
	if(!left_pick && !right_pick){ //both of the children are not picked
		//printf("/case 0, 0/  ");
		if(root->left){
			if(!(root->left->data)){ //left is not covered
				root->data = 1;  //build base
				root->left->data = 1;
			}
		}
		if(root->right){
			if(!(root->right->data)){ //right is not covered
				root->data = 1;  //build base
				root->right->data = 1;
			}
		}
		if(root->data){ //pick root
			glob_count++;
			return 1; //pick
		}else{
			return -1; //both of the children are covered; treated as leaf
		}		
	}
	
	if(left_pick || right_pick){   //one or both of the children are picked
	 	//printf("/case 1, 0 or 0, 1 or 1, 1/  ");
	 	root->data = 1; //covered
		return 0;
	}
} 

/*
int _print_t(Node *tree, int is_left, int offset, int depth, char s[][255])
{
    char b[20];
    int width = 5;

    if (!tree) return 0;

    sprintf(b, "(%03d)", tree->data);

    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);

	int i;
    for (i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];

    if (depth && is_left) {
		int i;
        for (i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';

    } else if (depth && !is_left) {
		int i;
        for (i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';

        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }


    return left + width + right;
}

void print_t(Node *tree)
{
    char s[20][255];
    int i;
    for (i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");

    _print_t(tree, 0, 0, 0, s);
	
    for (i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}
*/
