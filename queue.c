/* queue.c */
#include "queue.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct queue *make_queue() {
    // TODO: Allocate memory and initialize synchronization mechanisms
    queue_t *q = (queue_t*) malloc(sizeof(queue_t));
    q->head = NULL;
    q->tail = NULL;

    //intialize the lock on the queue
    pthread_mutex_init(&q->lock, NULL);

    return q;
}

void enqueue(struct queue *q, void *item) {
    // TODO: Add item to the queue and update necessary pointers

    
    //lock the queue
    pthread_mutex_lock(&q->lock);
    //create the new node and set item as the data
    queue_node_t *newNode = (queue_node_t*) malloc(sizeof(queue_node_t));
    newNode->item = item;
    //If head to queue is NULL the queue is empty therefore new node = head and tail
    if (q->head==NULL && q->tail==NULL){
        newNode-> next = NULL;
        newNode-> prev = NULL;
        q->head = newNode;
        q ->tail = newNode;
    }
    //if queue not empty new item = tail of queue
    else{
        newNode-> next = NULL;
        newNode-> prev = q->tail;
        q->tail->next = newNode;
        q -> tail = newNode;
        
    }
    //unlock the queue
    pthread_mutex_unlock(&q->lock);

}

void *dequeue(struct queue *q) {
    // TODO: Retrieve an item while ensuring thread safety
    
    //lock the queue
    pthread_mutex_lock(&q->lock);
    // If the queue is empty then there is nothing to dequeue
    if (q->head == NULL && q->tail == NULL){
        return NULL;
    }
    //Data to return
    void *item = q->head->item;
    //New head of queue is the previous 2nd in line
    q->head = q->head->next;
    q->head->prev = NULL;
    //checks to see if the queue is empty after dequeue
    if (q->head == NULL){
        q->tail = NULL;
    }
    //unlock before ending function
    pthread_mutex_unlock(&q->lock);
    return item;




}

void destroy_queue(struct queue *q) {
    // TODO: Clean up allocated resources and synchronization mechanisms
   
    //lock the queue
    pthread_mutex_lock(&q->lock);
    queue_node_t *cur = q->head;
    //iterate through queue and free all nodes
    while (cur != NULL) {
        //use pointer to hold current node
        queue_node_t *holder = cur;
        //move to next node
        cur = cur->next;
        free(holder);
    }
    //unlock
    pthread_mutex_unlock(&q->lock);
    //destroy the lock
    pthread_mutex_destroy(&q->lock);
    //free pointer to queue itself
    free(q);
    
}
