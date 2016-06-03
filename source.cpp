#include<iostream>
#include<fstream>
#include<cstdio>
#include<string>
#include <utility>
#include <vector>
#include <math.h>

using namespace std;

vector <pair <long long int, long long int> > v;//vector of pairs

struct node		//node structure of the Red Black Tree
{
	long long int key;
	long long int count;
	node *parent;
	char color;
	node *left;
	node *right;
};

class RBtree		//Class having the root node and other functions
{
	node *root;
	node *q;

public:

	RBtree()	//Constructor of the class
	{
		q = NULL;
		root = NULL;
	}

	void initial_insert();	  			/*Will initialize the tree*/
	void Increase(long long int, long long int);	/*Required Increase function*/
	void Insert(long long int, long long int);	/*Insert function when required in "Increase()"*/
	void Reduce(long long int, long long int);	/*Required Reduce function*/
	void Count(long long int);			/*Required Count function*/
	void InRange(long long int, long long int);	/*Required Inrange function*/
	void Next(long long int);			/*Required Next function*/
	void Previous(long long int);			/*Required Previous function*/
	void insertfix(node *);				/*To verify and fix the RB tree properties after "Insert()"*/
	void leftrotate(node *);			/*For doing leftrotate*/
	void rightrotate(node *);			/*For doing rightrotate*/
	node* successor(node *);			/*Finding successor before delete*/
	node *search(long long int);			/*To search for particular node*/
	node *searchnf(long long int);			/*To search for immediate node if exact ID not found*/
	node *sibling(node *);				/*To find sibling of a node*/
	void delete_prog2(node *);			/*Main delete function to call if count goes less than 1*/
	void delete_case_noChild1(node *);		/*Will be called if deleting node has no child*/
	void delete_case1(node *);			/*delete case 1*/
	void delete_case2(node *);			/*delete case 2*/
	void delete_case3(node *);			/*delete case 3*/
	void delete_case4(node *);			/*delete case 4*/
	void delete_case5(node *);			/*delete case 5*/
	void delete_case6(node *);			/*delete case 6*/
	void replace_node(node *, node *);		/*To replace deleting node before removing it*/
	long long int inRange(long long int, long long int);/*calculating the value for "InRange()" function*/
	node *next_IDptr(long long int);		/*Will be returning the next pointer*/
	node *previous_IDptr(long long int);		/*Will be returning the previous pointer*/
	node *min(node *);				/*Will return minimum of the node in its subtree*/
	node *max(node *);				/*Will return maximum of the node in its subtree*/
};

/*Will increase the count of the ID*/
void RBtree::Increase(long long int ID, long long int count)
{ 
	if (root == NULL)
	{
		Insert(ID, count);	//if root null, then insert this ID
		cout << count << endl;
		return;
	}
	else 
	{
		node *IDptr = search(ID);
		if (IDptr == NULL) 	//if ID not found, insert it
		{
			Insert(ID, count);
			cout << count << endl;
			return;
		}
		else 
		{
			IDptr->count += count;	//ID present, then increase the count
			cout << IDptr->count << endl;
			if (IDptr->count < 1)
				delete_prog2(IDptr);//if new count<1, delete the ID(possible when m is negative)
		}
	}
}

/*Will increase the count of the ID*/
void RBtree::Reduce(long long int ID, long long int value)
{
	if (root == NULL) 
	{
		cout << "0" << endl; //if root null, return 0
		return;
	}
	node *IDptr = search(ID);
	if (IDptr == NULL) 
	{
		cout << "0" << endl; //if ID not found, return 0
		return;
	}
	else 
	{
		IDptr->count -= value; //if ID found, decrease the count
		if (IDptr->count < 1) //if new count<1, delete the ID
		{
			delete_prog2(IDptr);
			cout << "0" << endl;
		}
		else 
		{
			cout << IDptr->count << endl;//if new count>0, display the count
		}
	}
}
/*will return count of the ID passed*/
void RBtree::Count(long long int ID)
{
	if (root == NULL) 
	{
		cout <<"0"<<endl;//if root null, return 0
		return;
	}
	node *IDptr = search(ID);
	if (IDptr == NULL) 
	{
		cout << "0";//if ID not found, return 0
		return;
	}
	else 
	{
		cout << IDptr->count << endl;//if ID found, return the count
	}
}

/*will return total count of IDs found b/w ID1 & ID2 inclusive*/ 
void RBtree::InRange(long long int ID1, long long int ID2)
{
	long long int result = inRange(ID1, ID2);
	cout << result << endl;
}

/*Will return the ID and Count of lowest ID greater than thisID*/ 
void RBtree::Next(long long int ID) 
{
	node *IDptr = next_IDptr(ID);
	if (IDptr == NULL) 
	{
		cout << "0 0" << endl;
	}
	else
	{
		cout << IDptr->key << " " << IDptr->count << endl;
	}
}

/*Will return the ID and Count of greatest ID lower than thisID*/ 
void RBtree::Previous(long long int ID)
{
	node *IDptr = previous_IDptr(ID);
	if (IDptr == NULL) 
	{
		cout << "0 0" << endl;
	}
	else
	{
		cout << IDptr->key << " " << IDptr->count << endl;
	}
}

/*used to insert a node in the RB tree*/
void RBtree::Insert(long long int ID, long long int count)
{
	if (count < 1) return; 
	long long int z, i = 0, c;
	z = ID;
	c = count;
	node *p, *q;
	node *t = new node;
	t->key = z;
	t->left = NULL;
	t->right = NULL;
	t->color = 'r';
	t->count = c;
	p = root;
	q = NULL;
	if (root == NULL)
	{
		root = t;
		t->parent = NULL;
	}
	else
	{
		while (p != NULL)
		{
			q = p;
			if (p->key<t->key)
				p = p->right;
			else
				p = p->left;
		}
		t->parent = q;
		if (q->key<t->key)
			q->right = t;
		else
			q->left = t;
	}
	insertfix(t); //after insert, the new tree is verified and fixed if any issue found
}

/*might be needed after the Insertion of a node*/
void RBtree::insertfix(node *t) 
{
	node *u;
	if (root == t)
	{
		t->color = 'b';
		return;
	}
	while (t->parent != NULL&&t->parent->color == 'r') //check if grand-parent's color is red
	{
		node *g = t->parent->parent;
		if (g->left == t->parent) //LL* and LR*   
		{
			if (g->right != NULL)
			{
				u = g->right;
				if (u->color == 'r') // LLr or LRr
				{
					t->parent->color = 'b';
					u->color = 'b';
					g->color = 'r';
					t = g; //focus shifts from node to grand parent
				}

			}
			else
			{
				if (t->parent->right == t) //LRb  
				{
					t = t->parent; // shifting focus from  child to parent
					leftrotate(t);
				}
				t->parent->color = 'b'; // LLb and LRb
				g->color = 'r';
				rightrotate(g);
			}
		}
		else  // RL* and RR*
		{
			if (g->left != NULL)
			{
				u = g->left;
				if (u->color == 'r')
				{
					t->parent->color = 'b';
					u->color = 'b';
					g->color = 'r';
					t = g;
				}
				else {
					goto A;
				}
			}
			else
			{
			A:
				if (t->parent->left == t)// RL*
				{						
					t = t->parent;
					rightrotate(t);
				}
				t->parent->color = 'b'; // RL* and RR*
				g->color = 'r';
				leftrotate(g);
			}
		}
		root->color = 'b';
	}
}

/*will be required to rebalance the tree after delete or insert*/
void RBtree::leftrotate(node *p)
{
	if (p->right == NULL)
		return;
	else //LRb or // LLb
	{
		node *y = p->right;
		if (y->left != NULL)
		{
			p->right = y->left;
			y->left->parent = p;
		}
		else
			p->right = NULL;
		if (p->parent != NULL)
			y->parent = p->parent; // LRb
		if (p->parent == NULL){
			root = y;
			root->parent = NULL;
			y->parent = NULL;
			y->color = 'b';
		}
		else
		{
			if (p == p->parent->left)
				p->parent->left = y;
			else
				p->parent->right = y;
		}
		y->left = p;
		p->parent = y;
	}
}

/*will be required to rebalance the tree after delete or insert*/
void RBtree::rightrotate(node *p)
{
	if (p->left == NULL)
		return;
	else
	{
		node *y = p->left;
		if (y->right != NULL)
		{
			p->left = y->right;
			y->right->parent = p;
		}
		else {
			p->left = NULL;
		}
		if (p->parent != NULL)
			y->parent = p->parent;

		if (p->parent == NULL)
		{
			root = y;
			root->parent = NULL;
		}
		else
		{
			if (p == p->parent->left)
				p->parent->left = y;
			else
				p->parent->right = y;
		}
		y->right = p;
		p->parent = y;
	}
}

/*required for Next() and Previous()*/
node* RBtree::successor(node *p)
{
	node *y = NULL;
	if (p->left != NULL)
	{
		y = p->left;
		while (y->right != NULL)
			y = y->right;
	}
	else
	{
		y = p->right;
		while (y->left != NULL)
			y = y->left;
	}
	return y;
}

/*used to search the tree for a given ID*/ 
node *RBtree::search(long long int ID)
{
	if (root == NULL) {
		return root;
	}
	node *temp = root;

	while (temp != NULL)
	{
		if (temp->key == ID)
			return temp;
		else {
			if (temp->key < ID)
				{temp = temp->right;}
			else
				{temp = temp->left;}
			}
	}
	return NULL;
}

/*similar to search but will return immediate node if ID not found*/
node *RBtree::searchnf(long long int ID)
{
	if (root == NULL) {
		return root;
	}
	node *temp = root;
	node *temp2 = root;
	while (temp != NULL)
	{
		if (temp->key == ID)
			return temp;
		else {
				if (temp->key < ID)
					{
						temp2 = temp;
						temp = temp->right;
					}
				else
					{
						temp2 = temp;
						temp = temp->left;
					}
			}
	}
	return temp2;
}

/*to return the sibling*/
node *RBtree::sibling(node *x)
{
	if (x == NULL) return NULL;
	if (x->parent == NULL){
		return NULL;
	}
	else{
		if (x->parent->left == x) {
			if (x->parent->right != NULL)
				return x->parent->right;
			else return NULL;
		}
		else{
			if (x->parent->left != NULL) {
				return x->parent->left;
			}
			else return NULL;
		}
	}
}

/*will be replacing old node(oldn) with new node(newn)*/
void RBtree::replace_node(node *oldn, node *newn)
{
	if (oldn->parent == NULL) 
	{
		root = newn;
		root->color = 'b';
	}
	else
	{
		if (oldn == oldn->parent->left)
			oldn->parent->left = newn;
		else
			oldn->parent->right = newn;
	}
	if (newn != NULL)
	{
		newn->parent = oldn->parent;
	}
}

/*main delete function*/
void RBtree::delete_prog2(node *n)
{
	node * pred = NULL;
	if (n == NULL)
	{
		return;
	}
	if (n->left != NULL && n->right != NULL) 
	{
		pred = successor(n);//finding successor if both child present
		n->key = pred->key;
		n->count = pred->count;
		n = pred;
	}
	node *child = (n->right == NULL ? n->left : n->right);
	if (n->color == 'b')
	{
		if (child == NULL)
		{
			delete_case_noChild1(n);//if no child found go to this case 
		}
		else 
		{
			n->color = child->color;
			delete_case1(n);
		}
	}
	replace_node(n, child);
	delete n;
}

/*will enter if n has 1 child*/
void RBtree::delete_case1(node *n)
{
	if (n->parent == NULL)//checking if n's parent exists or not 
	{
		return;
	}
	else 
	{
		delete_case2(n);
	}
}

/*will enter if n has a parent*/
void RBtree::delete_case2(node *n)
{
	node *sib = sibling(n);
	if (sib->color == 'r')//checking for sibling's color
	{
		n->parent->color = 'r';
		sib->color = 'b';
		if (n == n->parent->left)
			leftrotate(n->parent);
		else
			rightrotate(n->parent);
	}
	delete_case3(n);
}

/*will check for sibling's children*/
void RBtree::delete_case3(node *n)
{
	node *sib = sibling(n);
	if (sib->left == NULL || sib->right == NULL) delete_case4(n);
	else 
	{
		if (n->parent->color == 'b' && sib->color == 'b' &&
			sib->left->color == 'b' && sib->right->color == 'b')
		{//if parent, sibling and both children are black, then change sibling color and return
			sib->color = 'r';
			delete_case1(n->parent);
		}
		else
			delete_case4(n);
	}
}

/*if all are not black*/
void RBtree::delete_case4(node *n)
{
	node *sib = sibling(n);
	if (sib->left == NULL || sib->right == NULL)
		{delete_case5(n);}
	else
	{
		if (n->parent->color == 'r' && sib->color == 'b' &&
			sib->left->color == 'b' && sib->right->color == 'b')
			{//will swap parent and sibling's color if parent red and others black       
				sib->color = 'r';
				n->parent->color = 'b';
			}
		else
			{  
				delete_case5(n);
			}
		}
}

/*will check if both of sibling's children are not black*/
void RBtree::delete_case5(node *n)
{
	node *sib = sibling(n);
	if (sib->left == NULL)
	{
		n->parent->color = 'b';
		sib->color = 'r';
		if (sib->right != NULL)sib->right->color = 'b';
		return;
	}
	if (n == n->parent->left && sib->color == 'b' &&
		sib->left->color == 'r' && (sib->right == NULL || sib->right->color == 'b'))
	{
		cout << "inside if" << endl;
		sib->color = 'r';
		sib->left->color = 'b';
		rightrotate(sib);
	}
	else if (n == n->parent->right && sib->color == 'b' &&
		sib->right->color == 'r' && (sib->left == NULL || sib->left->color == 'b')) 
	{
		sib->color = 'r';
		sib->right->color = 'b';
		leftrotate(sib);
	}
	delete_case6(n);
}

/*further changes after case 5*/
void RBtree::delete_case6(node *n)
{
	node *sib = sibling(n);
	sib->color = n->parent->color;
	n->parent->color = 'b';
	if (n == n->parent->left) 
	{
		sib->right->color = 'b';
		leftrotate(n->parent);
	}
	else 
	{
		sib->left->color = 'b';
		rightrotate(n->parent);
	}
}

/*will be called when the node being deleted has no child*/
void RBtree::delete_case_noChild1(node *n)
{
	node *sib = sibling(n);
	if (sib == NULL) return;
	if (sib->color == 'r')
	{
		if (n->parent->left == n)
		{
			if (sib->left != NULL) sib->left->color = 'r';
			sib->color = 'b';
			leftrotate(n->parent); 

		}
		else
		{
			rightrotate(n->parent);
			if (sib->right != NULL)
				sib->right->color = 'r';
			sib->color = 'b';
		}
	}
	else 
	{
		if (n->parent->color == 'r')
		{
			if (sib->left == NULL && sib->right == NULL)
			{
				n->parent->color = 'b';
				sib->color = 'r';
			}
			else
			{
				if (n->parent->left == n)
				{
					n->parent->color = 'b';
					sib->color = 'r';
					if (sib->right != NULL) sib->right->color = 'b';
					leftrotate(n->parent);
				}
				else 
				{
					n->parent->color = 'b';
					sib->color = 'r';
					if (sib->left != NULL) sib->left->color = 'b';
					rightrotate(n->parent);
				}
			}
		}
		else
		{
			if (n->parent->left == n)
			{
				if (sib->right != NULL) sib->right->color = 'b';
				leftrotate(n->parent);
			}
			else
			{
				if (sib->left != NULL) sib->left->color = 'b';
				rightrotate(n->parent);
			}
		}
	}
}

/*actual implementation of inrange function*/
long long int RBtree::inRange(long long int ID1, long long int ID2)
{
	long long int totalCount = 0;
	node *searchPtr;
	node *id1ptr = NULL;
	node *id2ptr = NULL;
	if (search(ID1) == NULL)//if ID1 not found, it will return the immediate next
	{
		id1ptr = next_IDptr(ID1);
	}
	else
	{
		id1ptr = search(ID1);//when ID1 found
	}
	if (search(ID2) == NULL)//if ID2 not found, it will return the immediate previous
	{
		id2ptr = previous_IDptr(ID2);
	}
	else
	{
		id2ptr = search(ID2);//when ID2 found
	}
	if ((id1ptr->key)>(id2ptr->key))return 0; 
	if (id1ptr == NULL && id2ptr == NULL) return 0;
	else if (id1ptr == NULL) return id2ptr->count;
	else if (id2ptr == NULL) return id1ptr->count;
	else if (id1ptr == id2ptr) return (id1ptr->count);
	else {
		//traverse from ID1(smaller) to ID2 (bigger)
		totalCount += id1ptr->count;//add the counts while traversing
		searchPtr = id1ptr;
		while (searchPtr != id2ptr){
			searchPtr = next_IDptr(searchPtr->key);
			totalCount += searchPtr->count;
		}
		return totalCount;
	}
}

/*will return the next ID*/ 
node *RBtree::next_IDptr(long long int inID)
{
	node *inputPtr = searchnf(inID);
	if (inputPtr == max(root))
	{
		if (inID >= max(root)->key)
		{
			return NULL;
		}
		else if (inID < max(root)->key)
		{
			return inputPtr;
		}
	}
	if (inputPtr == min(root))
	{
		if (inID < min(root)->key)
		{
			return inputPtr;
		}
		if (inID >= min(root)->key)
		{
			goto A;
		}
	}
	if (inID < inputPtr->key)
	{
		return inputPtr;
	}
	else
	{
		A:
		node *searchPtr;
		if (inputPtr == NULL) return NULL;
		if (inputPtr->right != NULL) return min(inputPtr->right);
		if (inputPtr->parent == NULL) return NULL;
		if (inputPtr->parent->left == inputPtr) return inputPtr->parent;
		searchPtr = inputPtr->parent;
		while (searchPtr->parent->left != searchPtr){
			searchPtr = searchPtr->parent;
			if (searchPtr == NULL)  return NULL;
		}
		return searchPtr->parent;
	}
}

/*will return the previous ID */
node *RBtree::previous_IDptr(long long int inID)
{
	node *inputPtr = searchnf(inID);
	if (inputPtr == min(root))
	{
		if (inID <= min(root)->key)
		{
			return NULL;
		}
		else if (inID > min(root)->key)
		{
			return inputPtr;
		}
	}
	if (inputPtr == max(root))
	{
		if (inID>max(root)->key)
		{
			return inputPtr;
		}
		if (inID <= max(root)->key)
		{
			goto A;
		}
	}
	if (inID > inputPtr->key)
	{
		return inputPtr;
	}
	else
	{
		A:
		node *searchPtr;
		if (inputPtr == NULL) return NULL;
		if (inputPtr->left != NULL) return max(inputPtr->left);
		if (inputPtr->parent == NULL) return NULL;
		if (inputPtr->parent->right == inputPtr) return inputPtr->parent;
		searchPtr = inputPtr->parent;
		while (searchPtr->parent->right != searchPtr){
			searchPtr = searchPtr->parent;
			if (searchPtr == NULL)  return NULL;
		}
		return searchPtr->parent;
	}
}

/*returns minimum of the node in its subtree. If no min, returns the node*/
node *RBtree::min(node *x)
{
	if (x == NULL) return NULL;
	if (x->left == NULL) return x;
	while (x->left != NULL) {
		x = x->left;
	}
	return x;
}

/*returns maximum of the node in its subtree. If no max, returns the node*/
node *RBtree::max(node *x)
{
	if (x == NULL) return NULL;
	if (x->right == NULL) return x;
	while (x->right != NULL) {
		x = x->right;
	}
	return x;
}

/*finding the biggest index of form 2^n which is less than size of the vector(containing all IDs)*/
int bin_count(long long int m)
{
	long long int n = m;
	if (n == 0) {
		return 0;
	}
	int count = 0;
	while (n != 0) {
		n = n >> 1;
		count++;
	}
	if (m == (1 << count)) return m;
	return 1 << (count - 1);
}

/*creates nodes of the RB tree structure to initialize it*/
node *createNode(long long int ID, long long int count, char color)
{
	node *newNode = new node();
	newNode->key = ID;
	newNode->count = count;
	newNode->parent = NULL;
	newNode->color = color;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/*initializing the Red Black tree using sorted input IDs */
node *sortedArrayToBST(node *r, long long int start, long long int end, int flag) 
{
	/* Base Case */
	if (start > end) 
	{
		return NULL;
	}
	/* Get the middle element and make it root */
	long long int mid = (start + end) / 2;
	if (flag <= 0) 
	{
		r = createNode(v[mid].first, v[mid].second, 'r');
	}
	else 
	{
		r = createNode(v[mid].first, v[mid].second, 'b');
	}
	--flag;
	/* Recursively construct the left subtree and make it
	left child of root */
	r->left = sortedArrayToBST(r->left, start, mid - 1, flag);
	if (r->left != NULL) r->left->parent = r;
	/* Recursively construct the right subtree and make it
	right child of root */
	r->right = sortedArrayToBST(r->right, mid + 1, end, flag);
	if (r->right != NULL) r->right->parent = r;
	return r;
}

/*initialization of the Red Black tree */
void RBtree::initial_insert()
{
	long long int size = v.size();
	if (size == 0) return;
	long long int bin_size = bin_count(size + 1);
	int flag = log2(bin_size); /*will be giving the no of levels(1 less than total) for RB tree,
				     Last level will have all red nodes*/ 
	root = sortedArrayToBST(root, 0, size - 1, flag);
	return;
}

/*main function*/
int main(int argc, char* argv[])
{
	pair <long long int, long long int> p;
	ifstream fin, fin2;
	RBtree obj;
	long long int buffer1, buffer2, x, ID, ID2, count;
	string buffer3;
	fin.open(argv[1]);//opening test.txt file in read mode
	fin >> buffer1; //reading the total no of IDs
	for (x = 0; x<(buffer1); x++) //reading the IDs and Counts
	{
		fin >> p.first;
		ID = buffer2;
		fin >> p.second;
		count = buffer2;
		v.push_back(p); //pushing it to vector
	}
	obj.initial_insert(); //initializing the tree
	while (buffer3 != "quit")//read commands until "quit" found
	{
		cin >> buffer3;
		if (buffer3 == "increase")
		{
			cin >> ID >> count;
			obj.Increase(ID, count);//will be called for increase command
		}
		if (buffer3 == "reduce")
		{
			cin >> ID >> count;
			obj.Reduce(ID, count);//will be called for reduce command
		}
		if (buffer3 == "count")
		{
			cin >> ID;
			obj.Count(ID);//will be called for count command
		}
		if (buffer3 == "inrange")
		{
			cin >> ID >> ID2;
			obj.InRange(ID, ID2);//will be called for inrange command
		}
		if (buffer3 == "next")
		{
			cin >> ID;
			obj.Next(ID);//will be called for next command
		}
		if (buffer3 == "previous")
		{
			cin >> ID;
			obj.Previous(ID);//will be called for previous command
		}
	}
	return 0;
}
