#pragma once
#include <vector>

template <typename TypeKey, typename TypeData>
struct nodeAVL
{
	TypeKey key;
	TypeData data;

	int height;

	nodeAVL<TypeKey, TypeData>* left;
	nodeAVL<TypeKey, TypeData>* right;

	nodeAVL(const TypeKey &k, const TypeData &d) : key(k), data(d), left(0), right(0), height(1) {}
};

template <typename TypeKey, typename TypeData>
class AVLTree
{
	nodeAVL<TypeKey, TypeData>* root;
	int numInsert;
	int numRemove;
	int numFind;
	int numnodeAVL;

	int height(nodeAVL<TypeKey, TypeData>* p)
	{
		return p ? p->height : 0;
	}

	int balanceFactor(nodeAVL<TypeKey, TypeData>* p)
	{
		return height(p->right) - height(p->left);
	}

	void fixHeight(nodeAVL<TypeKey, TypeData>* p)
	{
		int hl = height(p->left);
		int hr = height(p->right);
		p->height = (hl > hr ? hl : hr) + 1;
	}

	nodeAVL<TypeKey, TypeData>* rotateRight(nodeAVL<TypeKey, TypeData>* p)
	{
		nodeAVL<TypeKey, TypeData>* q = p->left;
		p->left = q->right;
		q->right = p;
		fixHeight(p);
		fixHeight(q);
		return q;
	}

	nodeAVL<TypeKey, TypeData>* rotateLeft(nodeAVL<TypeKey, TypeData>* q)
	{
		nodeAVL<TypeKey, TypeData>* p = q->right;
		q->right = p->left;
		p->left = q;
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	nodeAVL<TypeKey, TypeData>* balance(nodeAVL<TypeKey, TypeData>* p)
	{
		fixHeight(p);
		if (balanceFactor(p) == 2)
		{
			numInsert++;
			numRemove++;
			if (balanceFactor(p->right) < 0)
				p->right = rotateRight(p->right);
			return rotateLeft(p);
		}
		if (balanceFactor(p) == -2)
		{
			numInsert++;
			numRemove++;
			if (balanceFactor(p->left) > 0)
				p->left = rotateLeft(p->left);
			return rotateRight(p);
		}
		return p;
	}

	nodeAVL<TypeKey, TypeData>* findMin(nodeAVL<TypeKey, TypeData>* p)
	{
		return p->left ? findMin(p->left) : p;
	}

	nodeAVL<TypeKey, TypeData>* removeMin(nodeAVL<TypeKey, TypeData>* p)
	{
		if (p->left == 0)
			return p->right;
		p->left = removeMin(p->left);
		return balance(p);
	}

	nodeAVL<TypeKey, TypeData>* insertElement(nodeAVL<TypeKey, TypeData>* p, const TypeKey &k, const TypeData &d)
	{
		if (!p)
		{
			numnodeAVL++;
			return new nodeAVL<TypeKey, TypeData>(k, d);
		}
		if (k < p->key)
		{
			numInsert++;
			p->left = insertElement(p->left, k, d);
		}
		else
		{
			numInsert++;
			p->right = insertElement(p->right, k, d);
		}
		return balance(p);
	}

	nodeAVL<TypeKey, TypeData>* removeElement(nodeAVL<TypeKey, TypeData>* p, const TypeKey &k)
	{
		if (!p) return 0;
		if (k < p->key)
		{
			numRemove++;
			p->left = removeElement(p->left, k);
		}
		else if (k > p->key)
		{
			numRemove++;
			p->right = removeElement(p->right, k);
		}
		else
		{
			numRemove++;
			nodeAVL<TypeKey, TypeData>* q = p->left;
			nodeAVL<TypeKey, TypeData>* r = p->right;
			numnodeAVL--;
			delete p;
			if (!r) return q;
			nodeAVL<TypeKey, TypeData>* min = findMin(r);
			min->right = removeMin(r);
			min->left = q;
			return balance(min);
		}
		return balance(p);
	}

	TypeData findElement(nodeAVL<TypeKey, TypeData>* p, const TypeKey &k)
	{
		if (!p) return 0;
		if (k == p->key)
		{
			numFind++;
			return p->data;
		}
		else
		{
			if (k < p->key)
			{
				numFind++;
				return findElement(p->left, k);
			}
			else
			{
				numFind++;
				return findElement(p->right, k);
			}
		}
	}

	void clearElement(nodeAVL<TypeKey, TypeData>* p)
	{
		if (!p) return;
		nodeAVL<TypeKey, TypeData>* q = p->left;
		nodeAVL<TypeKey, TypeData>* r = p->right;
		delete p;
		if (!q) clearElement(q);
		if (!r) clearElement(r);
	}
public:
	AVLTree() : root(0), numInsert(0), numRemove(0), numFind(0), numnodeAVL(0) {}
	~AVLTree() { clear(); }

	void insert(const std::pair<TypeKey, TypeData> &value)
	{
		numInsert = 0;
		root = insertElement(root, value.first, value.second);
	}

	void erase(const TypeKey &key)
	{
		numRemove = 0;
		root = removeElement(root, key);
	}

	TypeData find(const TypeKey &key)
	{
		numFind = 0;
		return findElement(root, key);
	}

	int getLastNumInsert() const
	{
		return numInsert;
	}

	int getLastNumRemove() const
	{
		return numRemove;
	}

	int getLastNumFind() const
	{
		return numFind;
	}

	int getNumnodeAVL() const
	{
		return numnodeAVL;
	}

	void clear()
	{
		clearElement(root);
		root = 0;
	}
};