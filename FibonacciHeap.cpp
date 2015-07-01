#include <iostream>
#include <fstream>
#include <list>
#include <cmath>
using namespace std;

template<class T>
class NodeFH{
	public:
	int key;
	T value;
	int degree;
	bool mark;
	NodeFH<T> *right;
	NodeFH<T> *left;
	NodeFH<T> *child;
	NodeFH<T> *p;
	NodeFH(int key2, T value2){
		key = key2;
		value = value2;
	}
};
template<class T>
class FibonacciHeap{
	NodeFH<T> min;
	//Number of nodes currently
	int n;
	public:
	FibonacciHeap(){
		min = NULL;
	}
	void fibHeapInsert(NodeFH<T> *x){
		x.degree = 0;
		x.p = 0;
		x.child = 0;
		x.mark = false;
		if(min == NULL){
			min = x;
		} else {
			insertNodeFH(min, x);
			if(x.key < min.key){
				min = x;
			}
			n++;
		}
	}
	void fibHeapUnion(FibonacciHeap &H2){
		FibonacciHeap H = new FibonacciHeap<T>();
		H.min = min;
		//concatenate
		H.head.right = H2;
		H2.head.left = H.head;		
		//concatenate
		if(min == 0 || H2.min != 0 && H2.min.key < min.key){
			H.min = H2.min;
		}
		H.n = n + H2.n;
		this = H;
	}
	void fibExtractMin(){
		NodeFH<T> z = min;
		if(z != 0){
			NodeFH<T> x = z.child;
			while (x != 0 ){
				insertNodeFH(z,x);
				x.p = 0;
			}
			removeNodeFH(min,z);
			if(z == z.right){
				min = 0;
			} else {
				min = z.right;
				//consolidate();
			}
			n--;
		}
		return z;
	}
	void consolidate(){
		int D = ceil(log(n));
		int A[D];
		for (int i = 0; i<D;i++){
			A[i] =0;
		}
		NodeFH<T> w = min;
		while(w != 0){
			NodeFH <T> x = w;
			int d = x.degree;
			//If there is another node with same degree
			while(A[d] != 0){
				NodeFH<T> y = A[d];
				if(x.key > y.key){
					int tmp = y.key;
					y.key = x.key;
					x.key = y.key;
				}
				fibHeapLink(y,x);
				A[d] =0;
				d = d+1;
			}
			A[d] = x;
			w = min;
		}
		min = 0;
		for (int i = 0; i<D; i++){
			if (A[i] != 0){
				if(min == 0){
					min = A[i];
				} else {
					insertNodeFH(min, A[i]);
					if(A[i].key < min.key){
						min = A[i];
					}
				}
			}
		}	
	}
	void fibHeapLink(NodeFH<T> &y, NodeFH<T>x){
		removeNodeFH(y);
		if(x.child != 0){
			insertNodeFH(x.child, x);
		} else {
			x.child = y;
		}
		x.degree++;
		y.mark = false;
	}
	
	void insertNodeFH(NodeFH<T> &root, NodeFH<T> &x){
		x.right = root;
		x.left = root.left;
		root.left = x;
		root.left.right = x;
	}
	void removeNodeFH(NodeFH<T> &x){
		x.left.right = x.right;
		x.right.left = x.left;
	}	
};
int main(){
	FibonacciHeap fh = new FibonacciHeap();
	NodeFH x = NodeFH(1,1);
	fh.fibHeapInsert(x);
}
