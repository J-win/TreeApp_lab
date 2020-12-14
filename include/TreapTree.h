#pragma once
#include<vector>

template <typename TypeKey, typename TypePriority, typename TypeData>
struct nodeTreap
{
    TypeKey key;
    TypePriority priority;
    TypeData data;

    nodeTreap<TypeKey, TypePriority, TypeData>* left;
    nodeTreap<TypeKey, TypePriority, TypeData>* right;

    nodeTreap(const TypeKey& key_, const TypePriority& priority_, const TypeData& data_,
        nodeTreap<TypeKey, TypePriority, TypeData>* left_ = 0,
        nodeTreap<TypeKey, TypePriority, TypeData>* right_ = 0) :
        key(key_), priority(priority_), data(data_), left(left_), right(right_) {}
};

template <typename TypeKey, typename TypePriority, typename TypeData>
class TreapTree
{
    nodeTreap<TypeKey, TypePriority, TypeData>* root;

    void merge(nodeTreap<TypeKey, TypePriority, TypeData>*& temp, nodeTreap<TypeKey, TypePriority, TypeData>* left, nodeTreap<TypeKey, TypePriority, TypeData>* right)
    {
        if (!left)
        {
            temp = right;
        }
        else if (!right)
        {
            temp = left;
        }
        else if (left->priority < right->priority)
        {
            merge(left->right, left->right, right);
            temp = left;
        }
        else
        {
            merge(right->left, left, right->left);
            temp = right;
        }
    }

    void split(nodeTreap<TypeKey, TypePriority, TypeData>* temp,
            const TypeKey &key,
            nodeTreap<TypeKey, TypePriority, TypeData>*& left,
            nodeTreap<TypeKey, TypePriority, TypeData>*& right)
    {
        if (!temp)
        {
            left = 0;
            right = 0;
        }
        else if (temp->key > key)
        {
            split(temp->left, key, left, temp->left);
            right = temp;
        }
        else
        {
            split(temp->right, key, temp->right, right);
            left = temp;
        }
    }

    void insertElement(nodeTreap<TypeKey, TypePriority, TypeData>*& temp, nodeTreap<TypeKey, TypePriority, TypeData>* elem)
    {
        if (!temp)
        {
            temp = elem;
        }
        else if (elem->priority < temp->priority)
        {
            split(temp, elem->key, elem->left, elem->right);
            temp = elem;
        }
        else
        {
            if (temp->key < elem->key)
            {
                insertElement(temp->right, elem);
            }
            else
            { 
                insertElement(temp->left, elem);
            }
        }
    }

    void removeElement(nodeTreap<TypeKey, TypePriority, TypeData>*& temp, const TypeKey& key)
    {
        if (!temp) return;
        if (temp->key == key)
        {
            nodeTreap<TypeKey, TypePriority, TypeData>* tmp = temp;
            merge(temp, temp->left, temp->right);
            delete tmp;
        }
        else
        {
            if (temp->key < key)
            {
                removeElement(temp->right, key);
            }
            else
            {
                removeElement(temp->left, key);
            }
        }
    }

    nodeTreap<TypeKey, TypePriority, TypeData>* findElement(nodeTreap<TypeKey, TypePriority, TypeData>* temp, const TypeKey& key)
    {
        if (!temp) return 0;
        if (temp->key == key)
        {
            return temp;
        }
        else if (temp->key < key)
        { 
            return findElement(temp->right, key);
        }
        else
        {
            return findElement(temp->left, key);
        }
        
    }

    void clearElement(nodeTreap<TypeKey, TypePriority, TypeData>* p)
    {
        if (!p) return;
        nodeTreap<TypeKey, TypePriority, TypeData>* q = p->left;
        nodeTreap<TypeKey, TypePriority, TypeData>* r = p->right;
        delete p;
        if (!q) clearElement(q);
        if (!r) clearElement(r);
    }
public:
    TreapTree() : root(0) {}
    ~TreapTree() { clear(); }
    void insert(const TypeKey& key, const TypePriority& priority, const TypeData& value)
    {
        nodeTreap<TypeKey, TypePriority, TypeData>* elem = new nodeTreap<TypeKey, TypePriority, TypeData>(key, priority, value);
        insertElement(root, elem);
    }

    void erase(const TypeKey& key)
    {
        removeElement(root, key);
    }

    TypeData find(const TypeKey& key)
    {
        auto element = findElement(root, key);
        return element->data;
    }

    void clear()
    {
        clearElement(root);
        root = 0;
    }
};
