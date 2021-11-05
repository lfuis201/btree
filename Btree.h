//
// Created by Mi PC on 5/11/2021.
//

#ifndef TXT1GB_BTREE_H
#define TXT1GB_BTREE_H
// C++ program for B-Tree insertion
/* The following program performs deletion on a B-Tree. It contains functions
specific for deletion along with all the other functions provided in the
previous articles on B-Trees. See https://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
for previous article.

The deletion function has been compartmentalized into 8 functions for ease
of understanding and clarity

The following functions are exclusive for deletion
In class BTreeNode:
	1) remove
	2) removeFromLeaf
	3) removeFromNonLeaf
	4) getPred
	5) getSucc
	6) borrowFromPrev
	7) borrowFromNext
	8) merge
	9) findKey

In class BTree:
	1) remove

The removal of a key from a B-Tree is a fairly complicated process. The program handles
all the 6 different cases that might arise while removing a key.

Testing: The code has been tested using the B-Tree provided in the CLRS book( included
in the main function ) along with other cases.

Reference: CLRS3 - Chapter 18 - (499-502)
It is advised to read the material in CLRS before taking a look at the code. */

#include<iostream>
using namespace std;

// A BTree node
class BTreeNode
{
    long int *keys; // array de keys
    int t;
    BTreeNode **C; // array de punteros a hijos
    long int n;	 // numero de llaves
    bool leaf;

public:

    BTreeNode(int _t, bool _leaf); // Constructor

    void traverse();

    BTreeNode *search(int k);


    int findKey(int k);

    // funcion de utilidad para insertar un subarbol
    void insertNonFull(int k);

    void splitChild(int i, BTreeNode *y);

    void remove(int k);

    // funciones de utilidad para remover un elemento
    void removeFromLeaf(int idx);


    void removeFromNonLeaf(int idx);

    int getPred(int idx);

    int getSucc(int idx);

    void fill(int idx);

    void borrowFromPrev(int idx);

    void borrowFromNext(int idx);

    void merge(int idx);

    friend class BTree;
};

class BTree
{
    BTreeNode *root;
    int t;
public:


    BTree(int _t)
    {
        root = NULL;
        t = _t;
    }

    void traverse()
    {
        if (root != NULL) root->traverse();
    }


    BTreeNode* search(int k)
    {
        return (root == NULL)? NULL : root->search(k);
    }

    //funcion para insertar una llave en el btree
    void insert(long int k);

    //funcion para remover una llaver en el btree
    void remove(int k);

};
// Constructor
BTreeNode::BTreeNode(int t1, bool leaf1)
{

    t = t1;
    leaf = leaf1;

    keys = new long int[2*t-1];
    C = new BTreeNode *[2*t];

    n = 0;
}

// Una función de utilidad que devuelve el índice de la primera clave que se
//mayor o igual que k
int BTreeNode::findKey(int k)
{
    int idx=0;
    while (idx<n && keys[idx] < k)
        ++idx;
    return idx;
}

// Una función para eliminar la clave k del subárbol enraizado con este nodo
void BTreeNode::remove(int k)
{
    int idx = findKey(k);

    if (idx < n && keys[idx] == k)
    {

        if (leaf)
            removeFromLeaf(idx);
        else
            removeFromNonLeaf(idx);
    }
    else
    {

        if (leaf)
        {
            cout << "The key "<< k <<" is does not exist in the tree\n";
            return;
        }

        bool flag = ( (idx==n)? true : false );

        if (C[idx]->n < t)
            fill(idx);
        if (flag && idx > n)
            C[idx-1]->remove(k);
        else
            C[idx]->remove(k);
    }
    return;
}

// funcion para eliminar la clave idx-th de este nodo, que es un nodo hoja
void BTreeNode::removeFromLeaf (int idx)
{

    for (int i=idx+1; i<n; ++i)
        keys[i-1] = keys[i];

    n--;

    return;
}

// funcion para eliminar la clave idx-th de este nodo, que no es un nodo hoja
void BTreeNode::removeFromNonLeaf(int idx)
{

    int k = keys[idx];
    if (C[idx]->n >= t)
    {
        int pred = getPred(idx);
        keys[idx] = pred;
        C[idx]->remove(pred);
    }

    else if (C[idx+1]->n >= t)
    {
        int succ = getSucc(idx);
        keys[idx] = succ;
        C[idx+1]->remove(succ);
    }

    else
    {
        merge(idx);
        C[idx]->remove(k);
    }
    return;
}

// Una función para obtener un predecesor de claves [idx]
int BTreeNode::getPred(int idx)
{
    BTreeNode *cur=C[idx];
    while (!cur->leaf)
        cur = cur->C[cur->n];

    return cur->keys[cur->n-1];
}

int BTreeNode::getSucc(int idx)
{

    BTreeNode *cur = C[idx+1];
    while (!cur->leaf)
        cur = cur->C[0];

    return cur->keys[0];
}

void BTreeNode::fill(int idx)
{

    if (idx!=0 && C[idx-1]->n>=t)
        borrowFromPrev(idx);

    else if (idx!=n && C[idx+1]->n>=t)
        borrowFromNext(idx);


    else
    {
        if (idx != n)
            merge(idx);
        else
            merge(idx-1);
    }
    return;
}


void BTreeNode::borrowFromPrev(int idx)
{

    BTreeNode *child=C[idx];
    BTreeNode *sibling=C[idx-1];

    for (int i=child->n-1; i>=0; --i)
        child->keys[i+1] = child->keys[i];

    if (!child->leaf)
    {
        for(int i=child->n; i>=0; --i)
            child->C[i+1] = child->C[i];
    }

    child->keys[0] = keys[idx-1];

    if(!child->leaf)
        child->C[0] = sibling->C[sibling->n];

    keys[idx-1] = sibling->keys[sibling->n-1];

    child->n += 1;
    sibling->n -= 1;

    return;
}

void BTreeNode::borrowFromNext(int idx)
{

    BTreeNode *child=C[idx];
    BTreeNode *sibling=C[idx+1];

    child->keys[(child->n)] = keys[idx];

    if (!(child->leaf))
        child->C[(child->n)+1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    for (int i=1; i<sibling->n; ++i)
        sibling->keys[i-1] = sibling->keys[i];

    if (!sibling->leaf)
    {
        for(int i=1; i<=sibling->n; ++i)
            sibling->C[i-1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;

    return;
}

// funcion para fusionar C [idx] con C [idx + 1]
void BTreeNode::merge(int idx)
{
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx+1];

    child->keys[t-1] = keys[idx];

    for (int i=0; i<sibling->n; ++i)
        child->keys[i+t] = sibling->keys[i];

    if (!child->leaf)
    {
        for(int i=0; i<=sibling->n; ++i)
            child->C[i+t] = sibling->C[i];
    }

    for (int i=idx+1; i<n; ++i)
        keys[i-1] = keys[i];

    for (int i=idx+2; i<=n; ++i)
        C[i-1] = C[i];

    child->n += sibling->n+1;
    n--;

    delete(sibling);
    return;
}

// funcion para insertar una llave
void BTree::insert(long int k)
{
    // si arbol vacio
    if (root == NULL)
    {
        root = new BTreeNode(t, true);
        root->keys[0] = k;
        root->n = 1;
    }
    else
    {
        // si raiz llena el arbol incrementa en altura
        if (root->n == 2*t-1)
        {
            //memoria para nueva raiz
            BTreeNode *s = new BTreeNode(t, false);

            s->C[0] = root;

            s->splitChild(0, root);

            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);

            root = s;
        }
        else
            root->insertNonFull(k);
    }
}

//insertar nueva llave en el nodo
void BTreeNode::insertNonFull(int k)
{
    // Inicialzar indice de el de la derecha
    int i = n-1;

    // es hoja?
    if (leaf == true)
    {

        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            i--;
        }

        keys[i+1] = k;
        n = n+1;
    }
    else // si no es hoja
    {
        while (i >= 0 && keys[i] > k)
            i--;

        if (C[i+1]->n == 2*t-1)
        {
            splitChild(i+1, C[i+1]);

            if (keys[i+1] < k)
                i++;
        }
        C[i+1]->insertNonFull(k);
    }
}

// funcion de utilidad para separar hijos
// y debe estar lleno para llamar a esta funcion
void BTreeNode::splitChild(int i, BTreeNode *y)
{

    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];

    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }

    y->n = t - 1;

    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];

    C[i+1] = z;

    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];

    keys[i] = y->keys[t-1];


    n = n + 1;
}

// viajar todos los nodos
void BTreeNode::traverse()
{

    int i;
    for (i = 0; i < n; i++)
    {
        if (leaf == false)
            C[i]->traverse();
        cout << " " << keys[i];
    }

    if (leaf == false)
        C[i]->traverse();
}

//buscar llave K en el nodo
BTreeNode *BTreeNode::search(int k)
{
    int i = 0;
    while (i < n && k > keys[i])
        i++;

    if (keys[i] == k)
        return this;

    if (leaf == true)
        return NULL;

    return C[i]->search(k);
}
//funcion para remover una llave
void BTree::remove(int k)
{
    if (!root)
    {
        cout << "The tree is empty\n";
        return;
    }

    root->remove(k);

    if (root->n==0)
    {
        BTreeNode *tmp = root;
        if (root->leaf)
            root = NULL;
        else
            root = root->C[0];

        delete tmp;
    }
    return;
}

#endif //TXT1GB_BTREE_H
