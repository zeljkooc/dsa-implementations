#include "Heap.h"

int Heap::getCount() const {
	return counter;
}
void Heap::insert(int value, int min) {
	counter++;
	TreeNode* newNode = new TreeNode;
	newNode->degree = 0;

	//Ako nema elemenata on se postavlja za inicijalni
	if (head == nullptr) {
		head = newNode;
		head->value = value;
		head->prev = nullptr;
		head->next = nullptr;
		head->degree = 0;
		first = head;
	}
	//Ako ima novi cvor se ubacuje levo od trenutne glave
	else {
		TreeNode* p = head->prev;
		if (p)
			p->next = newNode;
		newNode->prev = p;
		head->prev = newNode;
		newNode->next = head;

		newNode->value = value;

		if (min) {
			if (value < head->value)
				head = newNode;
		}
		else
			if (value > head->value)
				head = newNode;
		if (!newNode->prev)
			first = newNode;
	}
}
int Heap::top() const{
	if (head)
		return head->value;
	else
		return 0;
}

//p-gornji, q-donji
TreeNode* Heap::link(TreeNode* p, TreeNode* q) {
	p->kids.push_back(q);
	p->degree++;

	if (q == first)
		first = q->next;

	//linking
	if (q->prev)
		q->prev->next = q->next;
	if (q->next)
		q->next->prev = q->prev;

	if (!p->prev)
		first = p;
	return p;
}

TreeNode* Heap::merge(TreeNode* p, TreeNode* q, int min) {
	if (min) {
		if (p->value < q->value)
			return link(p, q);
		else
			return link(q, p);
	}
	else {
		if (p->value > q->value) 
			return link(p, q);
		else 
			return link(q, p);
	}
}

void Heap::deleteTop(int min) {
	if (counter > 0)
		counter--;
	int change = 1;

	//Ako nema sinove samo se preveze dvostruka lista
	if (head->degree == 0) {
		if (head->prev)
			head->prev->next = head->next;
		if (head->next)
			head->next->prev = head->prev;
		if (head->prev == nullptr && head->next == nullptr)
			first = head;
		if (!head->prev)
			first = head->next;

		delete head;
		head = first;
	}
	//Ako ima oni se takodje uvezu u listu
	else {
		if (head->prev)
			head->prev->next = head->kids[0];
		head->kids[0]->prev = head->prev;
		for (int i = 1;i < head->degree;++i) {
			head->kids[i - 1]->next = head->kids[i];
			head->kids[i]->prev = head->kids[i - 1];
		}
		head->kids[head->degree - 1]->next = head->next;
		if (head->next)
			head->next->prev = head->kids[head->degree - 1];
		if (head == first)
			first = head->kids[0];

		delete head;
		head = first;

		if (first == nullptr)
			return;
		TreeNode* j = first;
		//j krece od pozadi
		while (j->next)
			j = j->next;

		//i od napred
		TreeNode* i = first;

		//Spajanje sinova obrisanog cvora sa ostatkom cvorova istog stepena
		while (1) {
			if (i == j)
				break;
			if (j->next == i)
				break;
			if (i->degree == j->degree) {
				merge(i, j, min);
			}
			if (i->degree > j->degree)
				j = j->prev;
			else if (j->degree > i->degree)
				i = i->next;
		}
	}

	//Prolazak kroz listu i spajanje cvorova istog stepena ako za to ima potrebe
	TreeNode* tren = first;
	if (head) {
		while (change || tren->next) {
			if (!tren->next) {
				change = 0;
				tren = first;
			}
			else if (tren->degree == tren->next->degree) {
				tren = merge(tren, tren->next, min);
				change = 1;
			}
			else
				tren = tren->next;
			if (min) {
				if (tren->value < head->value)
					head = tren;
			}
			else
				if (tren->value > head->value)
					head = tren;
		}
	}
}
void Heap::printDSL() const {
	TreeNode* tren = first;
	while (tren) {
		cout << tren->value << " ";
		tren = tren->next;
	}
	cout << endl;
}

void Heap::deleteHeap() {
	TreeNode* tren = first;
	queue <TreeNode*> kids;
	int kidsNum = 0, prevKids = 0;
	while (tren) {
		for (TreeNode* j : tren->kids) {
			kidsNum++;
			kids.push(j);
		}
		TreeNode* p = tren->next;
		delete tren;
		tren = p;
	}
	while (!kids.empty()) {
		for (TreeNode* j : kids.front()->kids)
			kids.push(j);

		TreeNode* p = kids.front();
		kids.pop();
		delete p;
	}
	head = nullptr;
	first = nullptr;
}
int Heap::isEmpty() const {
	if (head)
		return 0;
	else
		return 1;
}

void Heap::printHeap() const {
	TreeNode* tren = first;
	vector <TreeNode*> prevKids;
	vector <TreeNode*> kids;

	TreeNode* p = new TreeNode();
	p->value = 0;
	p->degree = 0;
	p->next = nullptr;


	while (tren) {
		prevKids.push_back(tren);

		if (tren->degree > 1)
			for (int i = 0;i < pow(2, tren->degree - 1) - 1;++i)
				prevKids.push_back(p);
		tren = tren->next;
	}
	int k = 0;
	while (k < prevKids.size()) {
		if (prevKids[k]->value) {
			cout << prevKids[k]->value;
			if (prevKids[k]->next == nullptr)
				break;
			if (k != prevKids.size() - 1) {
				if (prevKids[k]->value >= 100 && prevKids[k]->value < 1000)
					cout << "==";
				if (prevKids[k]->value < 100 && prevKids[k]->value >= 10)
					cout << "===";
				if (prevKids[k]->value < 10)
					cout << "====";
			}
		}
		else
			cout << "=====";
		++k;
	}
	cout << endl;
	int hasNodes = 1;
	while (hasNodes) {
		int i = 0;
		while (i < prevKids.size()) {
			if (prevKids[i]->degree > 0) {
				for (TreeNode* j : prevKids[i]->kids) {
					kids.push_back(j);
					++i;
				}
			}
			else {
				kids.push_back(p);
				++i;
			}
			if (i < prevKids.size())
				while (prevKids[i] == 0) {
					kids.push_back(p);
					++i;
				}
		}

		i = 0;
		hasNodes = 0;
		prevKids.clear();
		for (TreeNode* j : kids) {
			if (j->value) {
				cout << j->value;
				if (j->value >= 100 && j->value < 1000)
					cout << "  ";
				if (j->value < 100 && j->value >= 10)
					cout << "   ";
				if (j->value < 10)
					cout << "    ";
			}
			else
				cout << "     ";
			prevKids.push_back(j);
			if (j->value != 0)
				hasNodes = 1;
		}
		kids.clear();
		cout << endl;
	}
}

void MaxHeap::insert(int value) {
	Heap::insert(value, 0);
};
void MaxHeap::deleteTop() {
	Heap::deleteTop(0);
}
TreeNode* MaxHeap::merge(TreeNode* p, TreeNode* q) {
	return Heap::merge(p, q, 0);
}

void MinHeap::insert(int value) {
	Heap::insert(value, 1);
};
void MinHeap::deleteTop() {
	Heap::deleteTop(1);
}
TreeNode* MinHeap::merge(TreeNode* p, TreeNode* q) {
	return Heap::merge(p, q, 1);
}