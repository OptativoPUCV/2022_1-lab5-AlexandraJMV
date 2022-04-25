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

    if (tree->root == NULL)
    {
        new = createTreeNode(key, value);
        tree->root = new;
        return;
    }

    tree->current = tree->root;

    while (tree->current != NULL)
    {
        if(is_equal(tree, key, tree->current->pair->key) == 1)
        {
            return;
        }
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
        }
        else if (node->parent->left == node)
                 node->parent->left = NULL;
             else
                 node->parent->right = NULL;
    }
    //Nodo con un hijo
    else if (node->left == NULL || node->right == NULL)
    {
        if(node == tree->root)
        {
            if(node->left != NULL)
            {
                tree->root = node->left;
                node->left->parent = NULL;
            }
            else
            {
                tree->root = node->right;
                node->right->parent = NULL;
            }
        }
        else if(node->parent->left == node)
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

        }
        else
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
        }
    }
    else //Nodo con mas hijos.
    {
        a_node = minimum(node->right);

        

        if (node->left != a_node)
        {
            a_node->left = node->left;
            node->left->parent = a_node;
        }   

        if (node->right != a_node)
        {
            a_node->right = node->right;
            node->right->parent = a_node;
        }

        if(a_node->parent != node)
        {
            a_node->parent->left = a_node->right;
            if(a_node->right != NULL)
                a_node->right->parent = a_node->parent;
        }

        if (node == tree->root)
        {
            tree->root = a_node;
            a_node->parent = NULL;
        }
        else 
        {
            if(node->parent->left == node)
            {
                node->parent->left = a_node;
                a_node->parent = node->parent;
            }
            else{
                node->parent->right = a_node;
                a_node->parent = node->parent;
            }
        }  
    }
    free(node);
    return;
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
    TreeNode * ub_node;
    tree->current = tree->root;

    ub_node = NULL;

    while (tree->current != NULL)
    {
        if (is_equal(tree, key, tree->current->pair->key) == 1)
        {
            ub_node = tree->current;
            break;
        }
        else if (tree->lower_than(key, tree->current->pair->key) == 1 )
        {
            ub_node = tree->current;
            if(tree->current->left == NULL)
            {
                break;
            }
            tree->current = tree->current->left;
        }
        else
        {
            if(tree->current->right == NULL) break;
            tree->current = tree->current->right;

        }
    }

    if (ub_node == NULL)
        return NULL;
    return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    tree->current = minimum(tree->root);
    if (tree->current == NULL) return NULL;
    return tree->current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * next;
    if(tree->current->right != NULL)
    {
        next = minimum(tree->current->right);
    }
    else 
    {
        if(tree->current->parent == NULL)
        {
            next = NULL;
        }
        else if (tree->current->parent->left == tree->current) 
        {
            next = tree->current->parent;
        }
        else
        {
            while(tree->current->parent != NULL && tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) != 1)
            {
                tree->current = tree->current->parent;
            }

            next = tree->current->parent;
        }
        
    }
    
    tree->current = next;

    if (next == NULL) return NULL;

    return next->pair;
}
