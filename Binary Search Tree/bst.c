#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include "bst.h"



/*
   Returns the parent of an either existing or hypotetical node with the given value
 */
Node * find_parent(Node * root, int value) {
  assert(root != NULL);
  assert(value != root->data);

  Node * next = value < root->data ? root->left : root->right;

  if (next == NULL || next->data == value)
    return root;
  else
    return find_parent(next, value);
}

/*
   Constructs a new node
 */
Node * mk_node(int value) {
  Node * node = (Node *) malloc(sizeof(Node));
  node->data = value;
  node->left = node->right = NULL;
  return node;
}

Node * insertNode(Node * root, int value) {
  if (root == NULL)
    return mk_node(value);

  if (value == root->data)
    return root;

  Node * parent = find_parent(root, value);
  Node * child = value < parent->data ? parent->left : parent->right;
  assert(child == NULL || child->data == value);

  if (child == NULL) {
    // value not found, then insert and return node
    child = mk_node(value);
    if (value < parent->data)
      parent->left = child;
    else
      parent->right = child;

    return root;
  } else {
    // value found, then return null
    return NULL;
  }
}

bool is_ordered(Node * root) {
  if (root == NULL)
    return true;
  if (root->left && root->left->data > root->data)
    return false;
  if (root->right && root->right->data < root->data)
    return false;
  return true;
}

Node * deleteNode(Node * root, int value) {
  assert(is_ordered(root));

  // empty tree
  if (root == NULL)
    return NULL;

  // find node with value 'value' and its parent node
  Node * parent, * node;
  if (root->data == value) {
    parent = NULL;
    node = root;
  } else {
    parent = find_parent(root, value);
    node = value < parent->data ? parent->left : parent->right;
  }
  assert(node == NULL || node->data == value);

  // value not found
  if (node == NULL)
    return root;

  // re-establish consistency
  Node * new_node;
  if (node->left == NULL) {
    // node has only right child, then make right child the new node
    new_node = node->right;
  } else {
    // otherwise make right child the rightmost leaf of the subtree rooted in the left child
    // and make the left child the new node
    Node * rightmost = new_node = node->left;
    while (rightmost->right != NULL)
      rightmost = rightmost->right;
    rightmost->right = node->right;
  }

  free(node);

  Node * new_root;
  if (parent == NULL) {
    // if deleted node was root, then return new node as root
    new_root = new_node;
  } else {
    // otherwise glue new node with parent and return old root
    new_root = root;
    if (value < parent->data)
      parent->left = new_node;
    else
      parent->right = new_node;
  }

  assert(is_ordered(new_root));

  return new_root;
}

void printSubtree(Node * N) {
  if (N == NULL) return;

  printSubtree(N->left);
  printf("%d \n", N->data);
  printSubtree(N->right);
}

int countNodes(Node * N) {
  if (N == NULL)
    return 0;

  return 1+countNodes(N->left) + countNodes(N->right);
}

/*
   Frees the entire subtree rooted in 'root' (this includes the node 'root')
 */
Node* freeSubtree(Node * root) {
  if (root == NULL)
    return NULL;

  freeSubtree(root->left);
  freeSubtree(root->right);
  free(root);
  return NULL;
}

/*
   Deletes all nodes that belong to the subtree (of the tree of rooted in 'root')
   whose root node has value 'value'
 */
Node * deleteSubtree(Node * root, int value) {
  assert(is_ordered(root));

  // empty tree
  if (root == NULL)
    return NULL;

  // entire tree
  if (root->data == value) {
     freeSubtree(root);
    return NULL;
  }

  // free tree rooted in the left or right node and set the respective pointer to NULL
  Node * parent = find_parent(root, value);
  if (value < parent->data) {
    assert(parent->left == NULL || parent->left->data == value);
    freeSubtree(parent->left);
    parent->left = NULL;
  } else {
    assert(parent->right == NULL || parent->right->data == value);
    freeSubtree(parent->right);
    parent->right = NULL;
  }

  return root;
}

/*
   Compute the depth between root R and node N

   Returns the number of edges between R and N if N belongs to the tree rooted in R,
   otherwise it returns -1
 */
int depth (Node * R, Node * N) {
  if (R == NULL || N == NULL)
    return -1;
  if (R == N)
    return 0;

  int sub_depth = depth(R->data > N->data ? R->left : R->right, N);

  if (sub_depth >= 0)
    return sub_depth + 1;
  else
    return -1;
}


int sumSubtree(Node *N)
{
if (N==NULL) return 0;
return N->data+sumSubtree(N->left)+sumSubtree(N->right);

}

Node* arrayToTree(Node* hello,int *a, int begin , int end)
{
    if(begin>end)   return NULL;
    int middle = (begin+end)/2;
    hello = insertNode(hello,a[middle]);
    arrayToTree(hello,a,begin,middle-1);
    arrayToTree(hello,a,middle+1,end);
    return hello;

}
int index1 = 0;
void treeToArray(int *a , Node* root)
{
    if(root==NULL) return ;
    treeToArray(a,root->left);
    a[index1++]=root->data;
    treeToArray(a,root->right);

}

// This functions converts an unbalanced BST to a balanced BST
Node* balanceTree(Node* root)
{
    if(root == NULL) return NULL;
    Node *balance_root=NULL;
    int *a;
    int i = countNodes(root);
    a = (int*)malloc(i * sizeof(int));
    treeToArray(a,root);
    index1 = 0;
   balance_root =  arrayToTree(balance_root,a,0,i-1);
   free(a);
  return balance_root;

}

