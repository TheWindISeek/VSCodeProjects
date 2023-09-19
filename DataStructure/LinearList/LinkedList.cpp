/*
 * @Author: TheWindISeek FAIZ030612@163.com
 * @Date: 2023-09-18 14:20:04
 * @LastEditors: TheWindISeek FAIZ030612@163.com
 * @LastEditTime: 2023-09-18 22:54:58
 * @FilePath: \VSCodeProject\DataStructure\LinearList\LinkedList.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
// #include <exception>
#include "../myExceptions.h"

using namespace std;

template<class T>
class linearList
{
    public:
        virtual ~linearList() {};
        virtual bool empty() const = 0;

        virtual int size() const = 0;
        virtual T& get(int theIndex) const = 0;
        virtual int indexOf(const T& theElement) const = 0;
        virtual void erase(int theIndex) = 0;
        virtual void insert(int theIndex, const T& theElement) = 0;
        virtual void output(ostream& out) const = 0;
};



template <class T>
struct chainNode {
    T element;
    chainNode<T> *next;

    chainNode() {

    }
    
    chainNode(const T& element) {
        this->element = element;
    }

    chainNode(const T& element, chainNode<T>* next) {
        this->element = element;
        this->next = next;
    }
};

template<class T>
class chain : public linearList<T>
{
    public:
        chain(int initialCapacity = 10);
        chain(const chain<T>&);
        ~chain();

        bool empty() const {
            return listSize == 0;
        }

        int size() const {
            return listSize;
        }

        T& get(int theIndex) const;
        int indexOf(const T& theElement) const;
        void erase(int theIndex);
        void insert(int theIndex, const T& theElement);
        void output(ostream& out) const;

    protected:
        void checkIndex(int theIndex) const;
        chainNode<T>* firstNode;
        int listSize;
};

template<class T>
chain<T>::chain(int initialCapacity) {
    if(initialCapacity < 1) {
        ostringstream s;
        s << "Initiall capacity = " << initialCapacity << " Must be > 0";
        throw illegalParameterValue(s.str());
    }
    firstNode = NULL;
    listSize = 0;
}

template<class T>
chain<T>::chain(const chain<T>& theList) {
    listSize = theList.listSize;

    if(listSize == 0) {
        firstNode = NULL;
        return;
    }

    chainNode<T>* sourceNode = theList.firstNode;
    firstNode = new chainNode<T>(sourceNode->element);

    sourceNode = sourceNode->next;
    chainNode<T>* targetNode = firstNode;

    while(sourceNode != NULL) {
        targetNode->next = new chainNode<T>(sourceNode->element);
        targetNode = targetNode->next;
        sourceNode = sourceNode->next;
    }
    
    targetNode->next = NULL;
}

template<class T>
chain<T>::~chain() {
    while(firstNode != NULL) {
        chainNode<T>* nextNode = firstNode->next;
        delete firstNode;
        firstNode = nextNode;
    }
}

template<class T>
T& chain<T>::get(int theIndex) const {
    checkIndex(theIndex);

    chainNode<T>* currentNode = firstNode;
    for(int i = 0; i < theIndex; ++i) {
        currentNode = currentNode->next;
        return currentNode->element;
    }
}

template<class T>
int chain<T>::indexOf(const T& theElement) const {
    chainNode<T>* currentNode = firstNode;
    int index = 0;
    while(currentNode != NULL 
        && currentNode->element != theElement) {
            currentNode = currentNode->next;
            index++;
        }

    if(currentNode == NULL) {
        return -1;
    } else {
        return index;
    }
}

template<class T>
void chain<T>::erase(int theIndex) {
    checkIndex(theIndex);

    chainNode<T>* deleteNode;
    if(theIndex == 0) {
        deleteNode = firstNode;
        firstNode = firstNode->next;
    } else {
        chainNode<T>* p = firstNode;
        for(int i = 0; i < theIndex - 1; ++i) {
            p = p->next;
        }

        deleteNode = p->next;
        p->next = p->next->next;
    }
    listSize--;
    delete deleteNode;
}


template<class T>
void chain<T>::insert(int theIndex, const T& theElement) {
    if(theIndex < 0 || theIndex > listSize) {
        ostringstream s;
        s << "index = " << theIndex << " size = " << listSize;
        throw illegalIndex(s.str());
    }

    if(theIndex == 0) {
        firstNode = new chainNode<T>(theElement, firstNode);
    } else {
        chainNode<T>* p = firstNode;
        
        for(int i = 0; i < theIndex - 1; ++i) {
            p = p->next;
            p->next = new chainNode<T>(theElement, p->next);
        }

        listSize++;
    }
}

template<class T>
void chain<T>::output(ostream& out) const {
    for(chainNode<T>* currentNode = firstNode; currentNode != NULL; currentNode = currentNode->next) {
        out << currentNode->element << " ";
    }
}

template<class T>
ostream& operator<<(ostream& out, const chain<T>& x) {
    x.output(out);
    return out;
}


//字典类数据结构
template<class K, class E>
class dictionary 
{
    public:
        virtual ~dictionary() {}
        virtual bool empty() const = 0;
        virtual int size() const = 0;
        virtual pair<const K, E>* find(const K&) const = 0;
        virtual void erase(const K&) = 0;
        virtual void insert(const pair<const K, E>&) = 0;
};


template<class K, class E>
struct skipNode
{
    typedef pair<const K, E> pairType;

    pairType element;
    skipNode<K, E> **next;

    skipNode(const pairType& thePair, int size) : element(thePair) {
        next = new skipNode<K, E>* [size];
    }

};



template<class K, class E>
class skipList : dictionary<K, E>
{
    protected:
        //用来确定层数
        float cutoff;
        //当前最大的非空链表
        int levels;
        //字典的数对个数
        int dSize;
        //允许的最大链表层数
        int maxLevel;
        //最大关键字
        K tailKey;
        //头节点指针
        skipNode<K, E>* headerNode;
        //尾结点指针
        skipNode<K, E>* tailNode;
        //last[i] 表示 i 层的最后节点
        skipNode<K, E>** last;

    public:
        skipList(K, int, float);
        pair<const K, E>* find(const K&) const;
        int level() const;
        skipNode<K, E>* search(const K&) const;
        void insert(const pair<const K, E>&);
        void erase(const K&);
};

template<class K, class E>
skipList<K, E>::skipList(K largeKey, int maxPairs, float prob) {
    cutoff = prob * RAND_MAX;
    maxLevel = (int) ceil(logf((float) maxPairs) / logf(1/prob)) - 1;
    levels = 0;
    dSize = 0;
    tailKey = largeKey;

    pair<K, E> tailPair;
    tailPair.first = tailKey;
    headerNode = new skipNode<K, E> (tailPair, maxLevel + 1);
    tailNode = new skipNode<K, E> (tailPair, 0);
    last = new skipNode<K, E> * [maxLevel+1];

    for(int i = 0; i <= maxLevel; ++i) {
        headerNode->next[i] = tailNode;
    }
}

template<class K, class E>
pair<const K, E>* skipList<K, E>::find(const K& theKey) const {
    if(theKey >= tailKey) {
        return NULL;
    }

    //位置beforeNode是关键字为theKey的节点之前最右边的位置
    skipNode<K, E>* beforeNode = headerNode;
    for(int i = levels; i >= 0; --i) {
        while(beforeNode->next[i]->element.first < theKey) {
            beforeNode = beforeNode->next[i];
        }
    }

    if(beforeNode->next[0]->element.first == theKey) {
        return &beforeNode->next[0]->element;
    }

    return NULL;
}

template<class K, class E>
int skipList<K, E>::level() const {
    //返回一个表示链表级数的随机数 这个数不大于maxLevel
    int lev = 0;
    while(rand() <= cutoff) {
        lev++;
    }
    return (lev <= maxLevel) ? lev : maxLevel;
}

//搜索关键字theKey 把每一级链表中要查看的最后一个节点存储在数组last中
template<class K, class E>
skipNode<K, E>* skipList<K,E>::search(const K& theKey) const {
    skipNode<K, E>* beforeNode = headerNode;
    for(int i = levels; i >= 0; --i) {
        while(beforeNode->next[i]->element.first < theKey) {
            beforeNode = beforeNode->next[i];
        }
        last[i] = beforeNode;//最后一级链表i的节点
    }
    return beforeNode->next[0];
}

template<class K, class E>
void skipList<K, E>::insert(const pair<const K, E>& thePair) {
    //把数对pair插入到字典 会覆盖关键字相同的已存在的数对
    if(thePair.first >= tailKey) {
        ostringstream s;
        s << "Key = " << thePair.first << " Must be < " << tailKey;
        throw illegalParameterValue(s.str());
    }

    //查看关键字为theKey的数对是否已经存在
    skipNode<K, E>* theNode = search(thePair.first);
    if(theNode->element.first == thePair.first) {
        theNode->element.second = thePair.second;
        return;
    }

    int theLevel = level();//使theLevel 为 <= levels+1
    if(theLevel > levels) {
        theLevel = ++levels;
        last[theLevel] = headerNode;
    }

    //在节点theNode之后插入新节点
    skipNode<K, E>* newNode = new skipNode<K, E>(thePair, theLevel+1);
    for(int i = 0; i <= theLevel; ++i) {
        newNode->next[i] = last[i]->next[i];//插入第i级链表
        last[i]->next[i] = newNode;
    }

    dSize++;
    return;
}

template<class K, class E>
void skipList<K, E>::erase(const K& theKey) {
    if(theKey >= tailKey) {
        return;
    }

    skipNode<K, E>* theNode = search(theKey);
    if(theNode->element.first != theKey) {
        return;
    }

    //先从跳表中删除掉这个节点相关的指针
    for(int i = 0; i <= levels && last[i]->next[i] == theNode; ++i) {
        last[i]->next[i] = theNode->next[i];
    }

    //删除掉从头到尾空的关系
    while (levels > 0 && headerNode->next[levels] == tailNode) {
        levels--;
    }

    
    delete theNode;
    dSize--;
} 
 



int main () {
    printf("hello linked list!\n");
    return 0;
}