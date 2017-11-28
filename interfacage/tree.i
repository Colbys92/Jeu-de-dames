%module tree

%{
#define SWIG_FILE_WITH_INIT
#include "tree.h"
%}

template<class T>
struct Tree {
	typedef T first_type;
	Tree( T );
	Tree(const Tree<T>&);
	~Tree();
	T getData();
	void setData(T);
	int nbSons();
	Tree<T>* getSon(int);
	void setSon(int,Tree<T>*);
	void addAsLastSon(Tree<T>*);
};

%template(treeii) Tree<int>;