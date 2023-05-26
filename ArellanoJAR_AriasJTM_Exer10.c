#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>

#define N 10
#define BALANCED 0
#define LEFT_LEANING 1
#define RIGHT_LEANING 2

typedef struct node_tag{
	int x, height;
	struct node_tag *parent;
	struct node_tag *left;
	struct node_tag *right;
}avl_node;

int max(int a, int b){
	return(a>b?a:b);
}

void updateheight(avl_node *temp){
	if(temp!=NULL)
		temp->height = max(temp->left==NULL?-1:temp->left->height,temp->right==NULL?-1:temp->right->height)+1;
}

void left_rotate(avl_node **rootptr){
	avl_node *temp1;

	if(rootptr!=NULL && *rootptr!=NULL && (*rootptr)->right!=NULL){
		temp1 = (*rootptr)->right;
		
		(*rootptr)->right = temp1->left;
		if(temp1->left!=NULL)
			temp1->left->parent = (*rootptr);

		temp1->parent = (*rootptr)->parent;

		(*rootptr)->parent = temp1;
		temp1->left = (*rootptr);

		if(temp1->parent != NULL){
			if(temp1->parent->right == *rootptr)
				temp1->parent->right = temp1;
			else
				temp1->parent->left = temp1;
		}

		*rootptr = temp1;

		updateheight(temp1->left);
		updateheight(temp1);

	}

}

void right_rotate(avl_node **rootptr){
	avl_node *temp1;

	if(rootptr!=NULL && *rootptr!=NULL && (*rootptr)->left!=NULL){
		temp1 = (*rootptr)->left;

		(*rootptr)->left = temp1->right;
		if(temp1->right!=NULL)
			temp1->right->parent = (*rootptr);

		temp1->parent = (*rootptr)->parent;

		(*rootptr)->parent = temp1;
		temp1->right = (*rootptr);

		if(temp1->parent != NULL){
			if(temp1->parent->left == *rootptr)
				temp1->parent->left = temp1;
			else
				temp1->parent->right = temp1;
		}

		*rootptr = temp1;

		updateheight(temp1->right);
		updateheight(temp1);

	}

}

void insert_fixup(avl_node **rootptr, avl_node *temp){
	int current = BALANCED, previous, lh, rh;

	do{

		lh= (temp->left==NULL?-1:temp->left->height);
		rh= (temp->right==NULL?-1:temp->right->height);

		previous=current;
		current = (lh==rh?BALANCED:(lh>rh?LEFT_LEANING:RIGHT_LEANING));

		if(abs(lh-rh)>1){
			if(current==LEFT_LEANING){
				if(previous==LEFT_LEANING)
					right_rotate(&temp);
				else{
					right_rotate(&temp);
					left_rotate(&(temp->left));
				}
			}
			else{
				if(previous==RIGHT_LEANING)
					left_rotate(&temp);
				else{
					left_rotate(&temp);
					right_rotate(&(temp->right));
				}
			}
		} //end if

		updateheight(temp);

		if(temp->parent==NULL)
			*rootptr = temp;

		temp =temp->parent;

	}while(temp!=NULL);
}

void insert_node(avl_node **rootptr, avl_node *temp){
	if(*rootptr==NULL) *rootptr = temp;
	else{
		temp->parent = *rootptr;

		if((*rootptr)->x < temp->x)
			insert_node(&((*rootptr)->right),temp);
		else
			insert_node(&((*rootptr)->left),temp);
	}
}

void insert_value(avl_node **rootptr, int x){
	avl_node *temp;
	temp = (avl_node *)malloc(sizeof(avl_node));
	temp->x = x;
	temp->height = 0;
	temp->parent = temp ->left = temp->right = NULL;
	insert_node(rootptr, temp);
	insert_fixup(rootptr, temp);
}

avl_node* search(avl_node **rootptr, int x){
	avl_node *temp = (*rootptr);

	while(temp!=NULL && temp->x!=x){
		if(x < temp->x) {
			temp=temp->left;
		}
		else{
			temp=temp->right;
		}
	}
	return temp;
}

void delete_fixup(avl_node **rootptr, avl_node){

}

avl_node* minimum(avl_node *node){
	avl_node *min = node;
	while(min->left!=NULL && min!=NULL){
		min=min->left;
		// printf("searching min: %d\n", min->x);
	}
	// printf("Here!\n");
	return min;
}

avl_node* findSuccessor(avl_node *node){
	avl_node* temp = node;
	if(temp==NULL || temp->right==NULL){
		return temp;
	}
	return( minimum(temp->right) );
}

void delete_node(avl_node **rootptr, int toRemove){
	avl_node *toDelete = search(rootptr, toRemove);
	if(toDelete==NULL){
		printf("Node does not exist\n");
		return;
	}
	printf("Node exists!\n");

	if(toDelete->left!=NULL && toDelete->right!=NULL){
		avl_node* successor = findSuccessor(toDelete);

		toDelete->x = successor->x;
		//deleting successor
		successor->parent->right = successor->right;
		successor->right->parent = successor->parent;
		free(successor);
		

		//attempt 2
		// successor->parent->left=NULL;
		// successor->right = toDelete->right;
		// successor->left = toDelete->left;
		// successor->height = toDelete->height;
		// toDelete->left->parent = successor;

		// if(toDelete==*rootptr){
		// 	*rootptr = successor;
		// 	successor->parent = NULL;
		// }
		// else{
		// 	successor->parent = toDelete->parent;
		// }

		//attempt 1
		// successor->left = toDelete->left;
		// if(toDelete==*rootptr){
		// 	avl_node* newroot = toDelete->right;
		// 	*rootptr = newroot;
		// 	newroot->left = toDelete->left;
		// 	newroot->parent = NULL;
		// }
		// else{
		// 	successor->parent = toDelete->parent;
		// }
	}

	else{

	}
	free(toDelete);
}

void view(avl_node *root, int tabs){
	int i;
	if(root != NULL){
		view(root->right, tabs +1);
		for(i=0; i<tabs; i++) printf("\t");
		printf("%2i\n", root->x);
		view(root->left, tabs +1);
	}
}

void swap(int *a, int *b){
	int temp;
	temp = *a; *a= *b; *b =temp;
}

int main(){
	avl_node *root = NULL;
	int i,n=10;

	for(i=0; i<n; i++){
		insert_value(&root,i+1);
		view(root,0);
		printf("\n----------------------------------------\n");
	}
	insert_value(&root,0);	
	view(root,0);
		printf("\n----------------------------------------\n");
	
	delete_node(&root, 4);
	view(root,0);
	
}
