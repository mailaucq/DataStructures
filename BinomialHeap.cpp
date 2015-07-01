#include <iostream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <climits>
using namespace std;

class Node {
	public:
	int key;
	int degree;
	Node *p;
	Node *child;
	Node *sibling;	
	Node(int key1){
		key = key1;
		p = NULL;
		child = NULL;
		sibling = NULL;
		degree = 0;	
	}	
};

class BinomialHeap {
	public:
		Node  *head;
        BinomialHeap(){
			head = NULL;
        }
	Node* binomialHeapMinimun(){
		Node  *x = head;
		Node  *min;
		int keymin = INT_MAX;
		while (x != 0){
			if (x->key < keymin){
				min = x;
				keymin = min->key;
			}
			x = x->sibling;			
		}
		return min;
	}
	
	void binomialLink(Node &y, Node &z){
		y.p = &z;
		y.sibling = z.child;
		z.child = &y;
		z.degree++;
	}
	
	void binomialHeapUnion (BinomialHeap &H2){
		binomialHeapMerge(H2);
		if (head == 0){
			return ;
		}
		Node *prev = 0;
		Node *x = head;
		Node *next = x->sibling;
		while (next != 0){
			if(x->degree != next->degree || (next->sibling != 0 && next->sibling->degree == x->degree)){
				prev = x;
				x = next;				
			} else{
				if (x->key <= next->key){
					//X se mantiene y next pasa a ser hijo de X
					x->sibling = next->sibling;
					binomialLink(*next, *x);					
				} else{
					//Next se mantiene y X pasa a ser hijo de Next
					if(prev == NULL){
						head = next;
					} else{
						prev->sibling = next;
					}
					binomialLink(*x, *next);
					x = next;
				}				
			}
			next = x->sibling;
		}
		return ;
	}
	
	void binomialHeapInsert(Node *x){
		BinomialHeap *H2 = makeBinomialHeap();
		x->p = 0;
		x->child = 0;
		x->sibling = 0;
		x->degree = 0;
		H2->head = x;
		binomialHeapUnion(*H2);
	}
	
	Node* binomialHeapExtractMin(){
		Node *x = head;
		Node *prev = 0;
		Node *next = 0;
		Node *min = 0;
		int keymin = INT_MAX;
		while(x != 0){
			if(x->key < keymin){
				min = x;
				keymin = min->key;
				prev = next;
				next = x;
			}
			x = x->sibling;
		}
		if(prev == 0){
			head = head->sibling;
		} else {
			prev->sibling = next->sibling;
		}
		BinomialHeap *H2 = makeBinomialHeap();
		//Reverse List
		Node *child = 0;
		Node *lastChild = min->child;
		Node *tmpLastChild = 0;
		while(lastChild != 0){
			lastChild->p = 0;
			tmpLastChild = lastChild->sibling; 
			lastChild->sibling = child;
			child = lastChild;
			lastChild = tmpLastChild;
		}
		H2->head = child;	
		binomialHeapUnion(*H2);
		return min;
	}
	
	void binomialHeapDecreaseKey(Node *x, int k){
		if(k > x->key){
			printf("new key is greater than current key");
		}		
		Node *y = binomialSearch(head, x->key);		
		y->key = k;
		Node *z = y->p;
		while(z != 0 && (y->key < z->key)){
			int temp = z->key;
			z->key = y->key;
			y->key = temp;
			y = z;
			z = y->p;
		}		
	}
	void binomialHeapDelete(Node *x){
		binomialHeapDecreaseKey(x,-INT_MAX);
		binomialHeapExtractMin();
	}
	
	Node* binomialSearch(Node *root, int x){
		Node *result = 0;
        Node *k = root;
        do{
			if(k != 0){
				if(k->key == x){
					return k;
				}
				result = binomialSearch(k->child, x);
				if(result != 0){
                    return result;
				}
				if(k == k->sibling){
					k = 0;
				} else {
					k = k->sibling;
				}
			}
		}while(root != k && k != 0);
        return 0;
	}
	
	
	BinomialHeap* makeBinomialHeap(){
		BinomialHeap *H = new BinomialHeap();
		H->head = NULL;
		return H;
	}
	
	void binomialHeapMerge(BinomialHeap H2){
		BinomialHeap *H = makeBinomialHeap();
		Node *headH1 = H2.head;
		Node *headH2 = head;
		Node *prev = 0;
		Node *next = 0;
		if(headH1 != 0 && headH2 != 0){
			while (headH1 != 0 && headH2 != 0){
				if(headH1->degree <= headH2->degree){
					prev = next;
					next = headH1;
					if(prev != 0){
						prev->sibling = next;
					} else {
						H->head = next;
					}
					headH1 = headH1->sibling;
					if(prev == 0 && headH1 == 0){
						next->sibling = headH2;
					}
				} else {
					prev = next;
					next = headH2;
					if(prev != 0){
						prev->sibling = next;
					} else {
						H->head = next;
					}
					headH2 = headH2->sibling;
					if(prev == 0 && headH2 == 0){
						next->sibling = headH1;
					}
				}
			}
			if(next != 0 && headH1 != 0){
				next->sibling = headH1;
			} else if(next != 0 && headH2 != 0){
				next->sibling = headH2;
			}
		} else if(headH1 != 0) {
			H->head = headH1;
		} else{
			H->head = headH2;
		}
		head = H->head;
	}
	void printBHeap(Node* node, ofstream &myfile){
		while(node != 0){
			if(node->p != 0){
				myfile << node->p->key;
				myfile << " -> ";
				myfile << node->key;
				myfile << ";";
			} else {
				myfile << node->key;
				myfile << ";";
			}
			printBHeap(node->child, myfile);
			node = node->sibling;
		}
	}
	
	void printArbol(){
		ofstream myfile;
		myfile.open ("salida.dot");
		myfile << "digraph graphname {";
		printBHeap(head, myfile);
		myfile << "}";
		myfile.close();
		system ("dot -Tpng salida.dot -o salida.png");
		system("nohup display salida.png &");
	}
};

int main(){
	BinomialHeap H;
	for (int i = 1;i<=5;i++) {
		H.binomialHeapInsert(new Node(i));
	}
	H.printArbol();
	H.binomialHeapExtractMin();
	H.printArbol();
	H.binomialHeapDecreaseKey(new Node(5), 1);
	H.printArbol();
	H.binomialHeapDelete(new Node(3));
	H.printArbol();
}
