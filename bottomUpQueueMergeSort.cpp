#include <iostream>
#include <queue>
using namespace std;

struct node {
    int info;
    node* next;
    node(int i = 0, node* n = nullptr) {
        info = i;
        next = n;
    }
};

typedef node* link;

link newlist() {
    link h = nullptr;
    for (int i = 0; i < 12; i++)
        h = new node(rand() % 50, h);
    return h;
}

void print(link h) {
    for (link t = h; t != nullptr; t = t->next)
        cout<<t->info<<" ";
    cout<<endl;
}

link merge(link a, link b) {
    node dummy(0, nullptr); link c = &dummy;
    while (a != nullptr && b != nullptr) {
        if (a->info <= b->info) {
            c->next = a; a = a->next; c = c->next;
        } else {
            c->next = b; b = b->next; c = c->next;
        }
    }
    c->next = (a == nullptr) ? b:a;
    return dummy.next;
}

inline link dequeue(queue<link>& q) {
    link tmp = q.front();
    q.pop();
    return tmp;
}

void decomposeList(queue<link>& fq, link& h) {
    link q, t, t2;
    while (h != nullptr) {
        t = h; 
        while (h && h->next) {
            if (h->next->info > h->info) h = h->next;
            else break; 
        }
        if (t != h && h->next != nullptr) { 
            //found a naturally occuring run
            q = h->next;
            h->next = nullptr;
            h = q;
        } else if (t == h && h->next) {
            //found a singleton: swap positions with neighbor 
            //push new run of length 2 on to the queue.
            t2 = h->next;
            h = t2->next;
            t2->next = t; 
            t->next = nullptr;
            t = t2;
        } else {
            //land here if theres a left over node at the end
            h = h->next;
        }
        fq.push(t);
    }
} 


void mergesort(link& h) {
    queue<link> fq;
    decomposeList(fq, h);
    while (fq.size() != 1) {
        link a = dequeue(fq);
        link b = dequeue(fq);
        h = merge(a, b);
        fq.push(h);
    }
    h = dequeue(fq);
}

