#include "../common.h"

void test_queue()
{
	struct Queue* q = queue_new(4, sizeof(int64));
	queue_print(q);
	enqueue(q, (int64[]){ 2 });
	enqueue(q, (int64[]){ 4 });
	enqueue(q, (int64[]){ 6 });
	queue_print(q);

	DV(*(int64*)dequeue(q));
	DV(*(int64*)dequeue(q));
	DV(*(int64*)dequeue(q));
	queue_print(q);

	enqueue(q, (int64[]){ 7 });
	enqueue(q, (int64[]){ 8 });
	enqueue(q, (int64[]){ 9 });
	enqueue(q, (int64[]){ 10 });
	// queue_print(q);
	// enqueue(q, (int64[]){ 11 });
	queue_print(q);
	DV(*(int64*)dequeue(q));
	DV(*(int64*)dequeue(q));
	// DV(*(int64*)dequeue(q));
	queue_print(q);

	queue_free(q);
}

