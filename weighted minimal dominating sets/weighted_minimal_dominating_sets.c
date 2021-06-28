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
void check_cover(Node *);

/* main */

int main(){
	
	/* read file */	
	FILE *fpr;
	fpr = fopen("./data/test1.txt", "r");

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
			printf("%d", root->data);
		}else{
			int root_pick = is_pick(root);
			if(root_pick == 0 && root->data != -1){
				glob_count += root->data;
			}
			//check_pick(root);
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
	int child_data;
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
    	return 0;
	} 
    	
    int left_pick = is_pick(root->left);   //printf("left_pick = %d  ", left_pick);
	int right_pick = is_pick(root->right); //printf("right_pick = %d  ", right_pick);

	child_data = 0;	
	int valid = 0;

	if(left_pick && right_pick){   //both of the children are picked
	 	//printf("/case 1, 1/  ");
	 	root->data = -1; //covered
		return 0;
	}else{
		if(root->left && root->left->data != -1){  //left exist and not picked
			child_data += root->left->data; 
			valid = 1; 
		}
		if(root->right && root->right->data != -1){  //right exist and not picked  
			child_data += root->right->data;
			valid = 1;
		}
		if(valid){
			if(child_data < root->data){ //children are smaller, pick children
				//printf(" (pick children) + %d ", child_data);
				glob_count += child_data;
				root->data = -1; //mark as covered
				if(root->left){
					root->left->data = -1; //mark as covered
				}
				if(root->right){
					root->right->data = -1;
				}
				return 0;	
			}else{  //root is smaller, pick root
				//printf(" (pick root) + %d ", root->data);
				glob_count += root->data;
				root->data = -1;  //mark as covered
				if(root->left){
					root->left->data = -1; //mark as covered
				}
				if(root->right){
					root->right->data = -1;
				}
				return 1;  //picked
			}
		}else{
			return 0;
		}
	}
	

	
	//if(!left_pick && !right_pick){ //both of the children are not picked
		//printf("/case 0, 0/  ");
		
						
	//}

} 

void check_cover(Node *root){
	if(root){
		check_cover(root->left);
		check_cover(root->right);
		if(root->data != -1){
			printf("%d \n", root->data);
		}
	}
	return;
}



