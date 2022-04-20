#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *) malloc (sizeof(TreeMap));
    if (new == NULL) return NULL;

    new->current = new->root = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    TreeNode * new;
    tree->current = tree->root;

    while (tree->current != NULL)
    {
        if(is_equal(tree, key, tree->current->pair->key) == 1)
            return;

        if (tree->lower_than(key, tree->current->pair->key) == 1)
        {
            if (tree->current->left == NULL)
            {
                new = createTreeNode(key, value);
                tree->current->left = new;
                new->parent = tree->current;
                tree->current = tree->current->left;
                return;
            }
            tree->current = tree->current->left;
        }
        else
        {
            if (tree->current->right == NULL)
            {
                new = createTreeNode(key, value);
                tree->current->right = new;
                new->parent = tree->current;
                tree->current = tree->current->right;
                return;
            }
            tree->current = tree->current->right;
        }
    }
}

TreeNode * minimum(TreeNode * x){
    TreeNode * current = x;

    while(current != NULL)
    {
        if (current->left == NULL)
            return current;
        else current = current->left;
    }
    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    TreeNode * a_node;

    // Nodo sin hijos.
    if(node->left == NULL && node->right == NULL)
    {
        if (node == tree->root)
        {
            tree->root = NULL;
            tree->current = NULL;
            free(node);

            return;
        }
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
            free(node);
            return;
        }
        if (node->parent->right == node)
        {
            node->parent->right = NULL;
            free(node);
            return;
        }
    }
    //Nodo con un hijo
    if (node->left == NULL || node->right == NULL)
    {
        if(node == tree->root)
        {
            if(node->left != NULL)
            {
                tree->root = node->left;
                node->left->parent = NULL;
                free(node);

                return;
            }
            if(node->right != NULL)
            {
                tree->root = node->right;
                node->right->parent = NULL;
                free(node);

                return;
            }
        }
        if(node->parent->left == node)
        {
            if(node->left != NULL)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            } 
            else
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            free(node);
            return;
        }
        if(node->parent->right == node)
        {
            if(node->left != NULL)
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
            else 
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
            free(node);

            return;
        }
    }

    //Nodo con mas hijos, paja.
    a_node = minimum(node->right);

    if (node == tree->root)
    {
        tree->root = a_node;
        a_node->left = node->left;
        a_node->right = node->right;


        node->left->parent = a_node;
        node->right->parent = a_node;

        a_node->parent->left = a_node->right;
        if(a_node->right != NULL)
            a_node->right->parent = a_node->parent;
        free(node);

        return;
    }
    if(nextTreeMap)


}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    tree->current = tree->root;

    while (tree->current != NULL)
    {
        if (is_equal(tree, key, tree->current->pair->key) == 1)
            break;

        if (tree->lower_than(key, tree->current->pair->key) == 1 )
            tree->current = tree->current->left;
        else
            tree->current = tree->current->right;
    }
    if (tree->current == NULL) return NULL;
    
    return tree->current->pair;
}


Pair * upperBound(TreeMap * tree, void* key) {
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    return minimum(tree->root);
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
