#pragma once
#include <vector>

template <typename TypeKey, typename TypeData>
struct nodeSplay
{
    TypeKey key;
    TypeData data;

    nodeSplay<TypeKey, TypeData>* left;
    nodeSplay<TypeKey, TypeData>* right;
    nodeSplay<TypeKey, TypeData>* parent;

    nodeSplay(const TypeKey& k, const TypeData& d) : key(k), data(d), left(0), right(0), parent(0) {}
    nodeSplay(const TypeKey& k, const TypeData& d, nodeSplay<TypeKey, TypeData>* l, nodeSplay<TypeKey, TypeData>* r) : key(k), data(d), left(l), right(r), parent(0) {}
};

template <typename TypeKey, typename TypeData>
class SplayTree
{
    nodeSplay<TypeKey, TypeData>* root;

    void setParent(nodeSplay<TypeKey, TypeData>* child, nodeSplay<TypeKey, TypeData>* parent)
    {
        if (!child) return;
        child->parent = parent;
    }

    void keepParent(nodeSplay<TypeKey, TypeData>* p)
    {
        setParent(p->left, p);
        setParent(p->right, p);
    }

    void rotate(nodeSplay<TypeKey, TypeData>* parent, nodeSplay<TypeKey, TypeData>* child)
    {
        nodeSplay<TypeKey, TypeData>* gparent = parent->parent;
        if (gparent)
        {
            if (gparent->left == parent)
                gparent->left = child;
            else
                gparent->right = child;
        }

        if (parent->left == child)
        {
            parent->left = child->right;
            child->right = parent;
        }
        else
        {
            parent->right = child->left;
            child->left = parent;
        }

        keepParent(child);
        keepParent(parent);
        child->parent = gparent;
    }

    nodeSplay<TypeKey, TypeData>* splay(nodeSplay<TypeKey, TypeData>* v)
    {
        if (!(v->parent)) return v;
        nodeSplay<TypeKey, TypeData>* parent = v->parent;
        nodeSplay<TypeKey, TypeData>* gparent = parent->parent;
        if (!gparent)
        {
            rotate(parent, v);
            return v;
        }
        else
        {
            bool zigzig = (gparent->left == parent) == (parent->left == v);
            if (zigzig)
            {
                rotate(gparent, parent);
                rotate(parent, v);
            }
            else
            {
                rotate(parent, v);
                rotate(gparent, v);
            }
            return splay(v);
        }
    }

    nodeSplay<TypeKey, TypeData>* findElement(nodeSplay<TypeKey, TypeData>* v, const TypeKey& key)
    {
        if (!v) return 0;
        if (key == v->key)
        {
            return splay(v);
        }
        if ((key < v->key) && (v->left))
        {
            return findElement(v->left, key);
        }
        if ((key > v->key) && (v->right))
        {
            return findElement(v->right, key);
        }
        return splay(v);
    }

    std::pair<nodeSplay<TypeKey, TypeData>*, nodeSplay<TypeKey, TypeData>*> split(nodeSplay<TypeKey, TypeData>* proot, const TypeKey& key)
    {
        if (!proot) return { 0, 0 };
        proot = findElement(proot, key);
        if (proot->key == key)
        {
            setParent(proot->left, 0);
            setParent(proot->right, 0);
            return { proot->left, proot->right };
        }
        if (proot->key < key)
        {
            nodeSplay<TypeKey, TypeData>* right = proot->right;
            proot->right = 0;
            setParent(right, 0);
            return { proot, right };
        }
        else
        {
            nodeSplay<TypeKey, TypeData>* left = proot->left;
            proot->left = 0;
            setParent(left, 0);
            return { left, proot };
        }
    }

    nodeSplay<TypeKey, TypeData>* insertElement(nodeSplay<TypeKey, TypeData>* proot, const TypeKey& key, const TypeData& data)
    {
        std::pair<nodeSplay<TypeKey, TypeData>*, nodeSplay<TypeKey, TypeData>*> childs = split(proot, key);
        proot = new nodeSplay<TypeKey, TypeData>(key, data, childs.first, childs.second);
        keepParent(proot);
        return proot;
    }

    nodeSplay<TypeKey, TypeData>* merge(nodeSplay<TypeKey, TypeData>* left, nodeSplay<TypeKey, TypeData>* right)
    {
        if (!right) return left;
        if (!left) return right;
        right = findElement(right, left->key);
        right->left = left;
        left->parent = right;
        return right;
    }

    nodeSplay<TypeKey, TypeData>* removeElement(nodeSplay<TypeKey, TypeData>* proot, const TypeKey& key)
    {
        proot = findElement(proot, key);
        nodeSplay<TypeKey, TypeData>* left = proot->left;
        nodeSplay<TypeKey, TypeData>* right = proot->right;
        delete proot;
        setParent(left, 0);
        setParent(right, 0);
        return merge(left, right);
    }

    void clearElement(nodeSplay<TypeKey, TypeData>* p)
    {
        if (!p) return;
        nodeSplay<TypeKey, TypeData>* q = p->left;
        nodeSplay<TypeKey, TypeData>* r = p->right;
        delete p;
        if (!q) clearElement(q);
        if (!r) clearElement(r);
    }
public:
    SplayTree() : root(0) {}
    ~SplayTree() { clear(); }

    void insert(const TypeKey& key, const TypeData& data)
    {
        root = insertElement(root, key, data);
    }

    void remove(const TypeKey& key)
    {
        root = removeElement(root, key);
    }

    TypeData find(const TypeKey& key)
    {
        root = findElement(root, key);
        return root->data;
    }

    void clear()
    {
        clearElement(root);
        root = 0;
    }
};