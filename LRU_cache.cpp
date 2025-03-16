#include<iostream>
#include<unordered_map>
using namespace std;

struct Node {
    int key,value;
    Node *prev;
    Node *next;

    Node(int k,int v) {
        key=k;
        value=v;
        prev=next=nullptr;
    }
};

class LRU_cache {
private:
    int capacity;
    unordered_map<int,Node*> cache;
    Node *head;
    Node *tail;

public:
    LRU_cache(int cap) {
        capacity=cap;
        head=new Node(-1,-1);//dummy head
        tail=new Node(-1,-1);//dummy tail
        head->next=tail;
        tail->prev=head;
    } 

    ~LRU_cache() {
        Node *curr=head;
        while(curr) {
            Node *nextNode=curr->next;
            delete curr;
            curr=nextNode;
        }
    }
    
    void moveToHead(Node *node) {
        removeNode(node);
        addToFront(node);
    }

    void removeNode(Node *node) {
        node->prev->next=node->next;
        node->next->prev=node->prev;
    }

    void addToFront(Node *node) {
        node->next=head->next;
        head->next->prev=node;
        head->next=node;
        node->prev=head;
    }

    int get(int key) {
        if(cache.find(key)==cache.end()) return -1;
        Node *node =cache[key];
        moveToHead(node);
        return node->value;
    }
    //put value in cache
    void put(int key,int value) {
        if(cache.find(key) != cache.end()) {
            Node *node=cache[key];
            node->value=value;
            moveToHead(node);
        } else {
            //insert into a new Node
            if(cache.size()==capacity) {
                //remove lru node before tail
                Node *lru=tail->prev;
                cache.erase(lru->key);
                removeNode(lru);
                delete lru;
            }

            Node *newNode=new Node(key,value);
            cache[key]=newNode;
            addToFront(newNode);
        }
    }
};    

int main() {
    LRU_cache lru(2);
    lru.put(1,10);
    lru.put(2,20);
    cout<<lru.get(1)<<endl;
    lru.put(3,30);
    cout<<lru.get(2)<<endl;
    return 0;
}

