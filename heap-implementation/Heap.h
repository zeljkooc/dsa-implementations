#pragma once
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct TreeNode {
	vector <TreeNode*> kids;
	struct TreeNode* parent= nullptr;
	struct TreeNode* prev=nullptr;
	struct TreeNode* next= nullptr;
	int degree=0;
	int value=0;
};

class Heap {
public:
	Heap() {};

	int getCount() const;

	virtual void insert(int value, int min);

	int top() const;
	
	virtual void deleteTop(int min);
		
	void deleteHeap();
		
	int isEmpty() const;

	void printHeap() const;
		
private:
	TreeNode* link(TreeNode* p, TreeNode* q);
	void printDSL() const;
	TreeNode* first = nullptr;
	TreeNode* head = nullptr;
	int counter = 0;
protected:
	virtual TreeNode* merge(TreeNode* p, TreeNode* q, int min);
};

//Podklase za lakse pristupanje hipovima

class MinHeap : public Heap {
public:

	MinHeap() {};

	void insert(int value);

	void deleteTop();

	TreeNode* merge(TreeNode* p, TreeNode* q);

};

class MaxHeap : public Heap {
public:

	MaxHeap() {};

	void insert(int value);

	void deleteTop();

	TreeNode* merge(TreeNode* p, TreeNode* q);

};