#include <stdio.h>
#include <stdlib.h>

struct node {
    int info;
    struct node* next;
};
typedef struct node* link;

struct _linkPair {
    link front;
    link back;
};
typedef struct _linkPair* link_pair;

typedef link_pair infoType;
struct _queue_node {
    infoType info;
    struct _queue_node* next;
};
typedef struct _queue_node* queue_node;

struct _queue {
    queue_node head;
    queue_node tail;
    int count;
};
typedef struct _queue* merge_queue;

link new_link(int, link);
link_pair new_link_pair(link a, link b);
queue_node new_queue_node(infoType info, queue_node next);
merge_queue init_queue();
void enqueue(merge_queue q, infoType info);
infoType dequeue(merge_queue q);


link_pair merge(link a, link b) {
    struct node dummy; link c = &dummy;
    while (a && b) {
        if (a->info <= b->info) {
            c->next = a; a = a->next; c = c->next;
        } else {
            c->next = b; b = b->next; c = c->next;
        }
    }
    c->next = (a == 0) ? b:a;
    link l = c->next;
    while (l->next != 0) l = l->next;
    if (l == 0) l = c;
    return new_link_pair(dummy.next, l);
}

void decomposeList(merge_queue mq, link head) {
    link nh, t, t2; link_pair p;
    while (head) {
        t = head; 
        while (head && head->next) {
            if (head->next->info > head->info) head = head->next;
            else break; 
        }
        if (t != head && head->next) { 
            //found a naturally occuring run
            nh = head->next;
            head->next = 0;
            p = new_link_pair(t, head);
            head = nh;
        } else if (t == head && head->next) {
            //found a singleton: swap positions with neighbor 
            //push new run of length 2 on to the queue.
            t2 = head->next;
            head = head->next->next;
            t2->next = t; 
            t->next = NULL;
            p = new_link_pair(t2, t);
        } else {
            //land here if theres a left over node at the end
            head = head->next;
            if (head == 0)
                p = new_link_pair(t, NULL);
        }
        enqueue(mq, p);
    }
}

link mergesort(link head) {
    if (head == 0 || head->next == 0)
        return head;
    link_pair a, b, p; //two pairs to dequeue, one to enqueue
    link al, ar, bl, br; //first and last of link_pair a and b
    merge_queue q = init_queue();
    decomposeList(q, head);
    while (q->count != 1) {
        a = dequeue(q);
        b = dequeue(q);
        al = a->front; ar = a->back;
        bl = b->front; br = b->back;
        if (ar && bl && ar->info <= bl->info) {
            ar->next = bl;
            p = new_link_pair(al, br);
            enqueue(q, p);
            continue;
        } else if (br != 0 && al != 0 && br->info < al->info) {
            br->next = al;
            p = new_link_pair(bl, ar);
            enqueue(q, p);
            continue;
        }
        p = merge(al, bl);
        enqueue(q, p);
    }
    return dequeue(q)->front;
}



link new_link(int info, link next) {
    link l = malloc(sizeof(struct node));
    l->info = info;
    l->next = next;
    return l;
}

link_pair new_link_pair(link a, link b) {
    link_pair lp = malloc(sizeof(struct _linkPair));
    lp->front = a;
    lp->back = b; 
    return lp;
}

queue_node new_queue_node(infoType info, queue_node next) {
    queue_node q = malloc(sizeof *q);
    q->info = info;
    q->next = next;
    return q;
}

merge_queue init_queue() {
    merge_queue mq = malloc(sizeof(struct _queue));
    mq->count = 0;
    mq->head = NULL;
    mq->tail = NULL;
    return mq;
}

void enqueue(merge_queue q, infoType info) {
    queue_node qn = new_queue_node(info, 0);
    if (q->count == 0) {
        q->head = qn;
        q->tail = qn;
    } else {
        q->tail->next = qn;
        q->tail = qn;
    }
    q->count++;
}

infoType dequeue(merge_queue q) {
    if (q->count == 0) {
        puts("Error: queue underflow.\n");
        return NULL;
    }
    infoType ret =  q->head->info;
    queue_node tmp = q->head;
    q->head = q->head->next;
    free(tmp);
    q->count--;
    return ret;
}

link randomlist(int size, int lo, int hi) {
    link head = NULL;
    for (int i = 0; i < size; i++) {
        head = new_link(rand() % (hi - lo + 1) + lo, head);
    }
    return head;
}

void print(link head) {
    for (link t = head; t != NULL; t = t->next)
        printf("%d ", t->info);
    printf("\n");
}

void isSorted(link h) {
    int g = 0;
    link t = h;
    while (h && h->next) {
        if (h->info > h->next->info) {
            puts("sort failed");
            return;
        }
        g++;
        h = h->next;
    }
    printf("%d\n", g+1);
    puts("sorted");
}

int main() {
    link head = randomlist(1131, 1, 313370);
    head = mergesort(head);
    isSorted(head);
    return 0;
}