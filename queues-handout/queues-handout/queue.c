#include <stdlib.h>
#include <stdbool.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"
#include <queue.h>


typedef void free_fn(void *x);
typedef struct list_node list;
struct list_node {
  void* data;
  list* next;
};

typedef struct queue_header queue;
struct queue_header {
  list* front;
  list* back;
  int size;
};

bool is_inclusive_segment(list* start, list* end, int i) {
  /* FIX ME */
  if(i == 1){
    if(start == end) {
      return true;
    } else {
      return false;
    }
  } else if(start == NULL) {
    if(i == 0) {
      return true;
    } else {
      return false;
    }
  }
  return is_inclusive_segment(start->next, end, i-1);
}

bool is_queue(queue* Q) {
  /* FIX ME */
  return Q != NULL && is_inclusive_segment(Q->front, Q->back, Q->size);
}

void print_queue(queue* Q, print_data_fn* print_data) {
  /* OPTIONAL (but recommended) */
  return;
}

/* Other functions go there */

queue* queue_new()
/*@ensures \result != NULL && is_queue(\result); @*/
{
    
    queue* Q = xmalloc(sizeof(queue));
    Q->front = NULL;
    Q->size  = 0;
    return Q;
    ENSURES(Q != NULL && is_queue(Q));
    return Q;



  }

/* O(1) */
int queue_size(queue* Q)
  /*@requires Q != NULL; @*/
  /*@ensures \result >= 0; @*/ 
  {
    REQUIRES(Q != NULL);
    int size_Q = Q->size;
    ENSURES(size_Q >= 0);
    return size_Q;
  }

/* O(1) -- adds an item to the back of the queue */
void enq(queue* Q, void* x)
/*@requires Q != NULL; @*/
  {
    REQUIRES(Q != NULL);
    if(queue_size(Q) == 0){
      list* last = xmalloc(sizeof(list));
      last->data = x;
      Q->front = last;
      Q->back = last;
      Q->size++;

    } else {
      list* last = xmalloc(sizeof(list));
      last->data = x;
      Q->back->next = last;
      Q->back = last;
      Q->size++;

    }
  }

/* O(1) -- removes an item from the front of the queue */
void* deq(queue* Q)
/*@requires Q != NULL && queue_size(Q) > 0; @*/ 
  {
    REQUIRES(Q != NULL && queue_size(Q));
    void* item = Q->front->data;
    Q->front = Q->front->next;
    Q->size--;
    return item;
  }

/* O(i) -- doesn't remove the item from the queue */
void* queue_peek(queue* Q, int i)
/*@requires Q != NULL && 0 <= i && i < queue_size(Q); @*/
  {
    REQUIRES(Q != NULL && 0 <= i && i < queue_size(Q));
    list* node = Q->front;
    // int size = queue_size(Q);
    for(int j = 0; j < i; j++)
    {
      node = node->next;
    }
    return node->data;

  }

/* O(n) */
void queue_reverse(queue* Q)
/*@requires Q != NULL; @*/
  {
    REQUIRES(Q != NULL);
    if(Q->size == 0 || Q->size == 1){
      return;
    } else if (Q->size == 2) {
      list* p = Q->front;
      list* q = p->next;
      list* r = q->next;
      q->next = p;
      p->next = r;
      Q->back = Q->front;
      Q->front = q;
    } else {
      list* p = Q->front;
      list* q = p->next;
      list* r = q->next;
      while(q != Q->back) {
        q->next = p;
        p = q;
        q = r;
        r = r->next;
      }
      q->next = p;
      Q->front->next = r;
      Q->back = Q->front;
      Q->front = q;
    }

  }

  /* O(n) worst case, assuming P is O(1) */
bool queue_all(queue* Q, check_property_fn* P)
/*@requires Q != NULL && P != NULL; @*/
{
    REQUIRES(Q != NULL && P != NULL);
    if(Q->size == 0){
    return true;
    } else {

    list* node = Q->front;

    while(node != Q->back->next){
      if(!*P(node->data)){
        return false;
      }
      node = node->next;

    }
    return true;
  }

}

/* O(n) worst case, assuming F is O(1) */
void* queue_iterate(queue* Q, void* base, iterate_fn* F)
  /*@requires Q != NULL && F != NULL; @*/
  {
    REQUIRES(Q != NULL && F != NULL);
      if(Q->size == 0){
      return base;
    } 
    list* node = Q->front;
    void* result = *F(base, node->data);
    node = node->next;
    while(node != Q->back->next) {
      result = *F(result, node->data);
      node = node->next;
    }

    return result;
  }

 /* Frees all the data elements using F if F != NULL @*/
void queue_free(queue* Q, free_fn *F)
{
    REQUIRES(Q != NULL);
    if (F == NULL)
    {
        while (Q->size != 0)
        {
            list* node = Q->front;
            void *temp = deq(Q);
            free(node);
        }
        free(Q);
        
    }
    else
    {
        while (Q->size != 0)
        {
            list* node = Q->front;
            void *temp = deq(Q);
            (*F)(temp);
            free(node);
        }
        free(Q);
        
    }
    
}
// Client type
typedef queue* queue_t;

