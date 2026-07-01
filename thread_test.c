#include "base/base_include.h" // Thread_Make, Mutex_*, CondVar_*

#include <stdio.h>
#include <unistd.h> // for sleep() simulation

//---------------------------------------------------------------
// Shared Data (Protected by Mutex + CondVar)
//---------------------------------------------------------------

#define MAX_QUEUE_SIZE 8

typedef struct WorkItem
{
	U64 id;
	U64 data;
} WorkItem;

typedef struct SharedState
{
	Mutex	mutex;
	CondVar condvar;

	WorkItem queue[MAX_QUEUE_SIZE];
	U64		 head;	// Read index
	U64		 tail;	// Write index
	U64		 count; // How many items are in the queue

	B32 shutdown; // Tells consumers to stop
} SharedState;

static SharedState global_state = {0};

//---------------------------------------------------------------
// Producer Thread
//---------------------------------------------------------------

void* ProducerProc(void* arg)
{
	U64 producer_id = (U64)(UPtr)arg;

	// --- 1. Select the thread context (so Scratch_Begin works) ---
	// Note: Your OS layer already set the TLS variable 'current_thread',
	// but we call this to ensure the ThreadContext is fully initialized
	// (creates the scratch arenas if this is the main thread).
	ThreadContext_Selected();
	Thread_SetNameF("Producer %llu", producer_id);

	for (U64 i = 0; i < 10; i++)
	{
		// --- 2. Simulate producing work (e.g., loading a file) ---
		usleep(100000); // 0.1 seconds

		// --- 3. Use scratch memory to build a temporary log string ---
		Temp	scratch = Scratch_Begin(NULL, 0);
		String8 log_msg = String8_F(scratch.arena, "[Producer %llu] Produced item #%llu", producer_id, i);
		printf("%.*s\n", (int)log_msg.size, log_msg.str);
		Scratch_End(scratch);

		// --- 4. Lock the mutex and add the work item to the queue ---
		Mutex_Lock(global_state.mutex);

		while (global_state.count == MAX_QUEUE_SIZE)
		{
			// Queue is full, wait for consumer to catch up.
			// Note: We use the global condvar, but since we are the producer,
			// we don't wait here. We just spin.
			Mutex_Unlock(global_state.mutex);
			usleep(10000);
			Mutex_Lock(global_state.mutex);
		}

		WorkItem item = {.id = i, .data = producer_id * 1000 + i};

		global_state.queue[global_state.tail] = item;
		global_state.tail					  = (global_state.tail + 1) % MAX_QUEUE_SIZE;
		global_state.count++;

		// --- 5. Signal the consumer that work is available ---
		CondVar_Signal(global_state.condvar);
		Mutex_Unlock(global_state.mutex);
	}

	return NULL;
}

//---------------------------------------------------------------
// Consumer Thread
//---------------------------------------------------------------

void* ConsumerProc(void* arg)
{
	// --- 1. Set up the thread context ---
	ThreadContext_Selected();
	Thread_SetName(String8_Lit("Consumer"));

	U64 processed_count = 0;

	while (1)
	{
		// --- 2. Use scratch memory for temporary processing ---
		Temp scratch = Scratch_Begin(NULL, 0);

		Mutex_Lock(global_state.mutex);

		// --- 3. Wait for work (spurious wakeup safe loop) ---
		while (global_state.count == 0 && !global_state.shutdown)
		{
			CondVar_Wait(global_state.condvar, global_state.mutex);
		}

		// --- 4. Check if we should exit ---
		if (global_state.shutdown && global_state.count == 0)
		{
			Mutex_Unlock(global_state.mutex);
			Scratch_End(scratch);
			break;
		}

		// --- 5. Pop an item from the queue ---
		WorkItem item	  = global_state.queue[global_state.head];
		global_state.head = (global_state.head + 1) % MAX_QUEUE_SIZE;
		global_state.count--;

		Mutex_Unlock(global_state.mutex);

		// --- 6. Process the item (use scratch memory) ---
		String8 result = String8_F(scratch.arena, "[Consumer] Processed item #%llu (data = %llu)", item.id, item.data);
		printf("%.*s\n", (int)result.size, result.str);
		processed_count++;

		// Simulate heavy processing.
		usleep(150000); // 0.15 seconds

		Scratch_End(scratch);
	}

	printf("Consumer processed %llu items total.\n", processed_count);
	return NULL;
}

//---------------------------------------------------------------
// Main Program
//---------------------------------------------------------------

int main(void)
{
	// --- 1. Initialize the OS layer (creates lnx_arena) ---
	// Since your Thread_Make uses lazy init, we don't need an explicit OS_Init.
	// But we need the context arena. ThreadContext_Make will create context_arena lazily.

	// --- 2. Create the shared state (mutex + condvar) ---
	global_state.mutex	  = Mutex_Make();
	global_state.condvar  = CondVar_Make();
	global_state.head	  = 0;
	global_state.tail	  = 0;
	global_state.count	  = 0;
	global_state.shutdown = 0;

	// --- 3. Spawn the threads ---
	Thread producer1 = Thread_Make(ProducerProc, (void*)1);
	Thread producer2 = Thread_Make(ProducerProc, (void*)2);
	Thread consumer	 = Thread_Make(ConsumerProc, NULL);

	// --- 4. Wait for producers to finish ---
	Thread_Join(producer1);
	Thread_Join(producer2);

	// --- 5. Signal the consumer to shut down ---
	Mutex_Lock(global_state.mutex);
	global_state.shutdown = 1;
	CondVar_Broadcast(global_state.condvar); // Wake up the consumer
	Mutex_Unlock(global_state.mutex);

	// --- 6. Wait for the consumer to finish ---
	Thread_Join(consumer);

	// --- 7. Clean up ---
	Mutex_Destroy(global_state.mutex);
	CondVar_Destroy(global_state.condvar);

	// --- 8. Shutdown the OS layer (frees all handles and context arenas) ---
	// Note: This depends on how you manage lnx_arena / context_arena.
	// If you have OS_Shutdown() defined, call it.
	// If you use lazy arenas, you might want to add a cleanup function.
	// For this example, we'll just exit.

	printf("All threads finished!\n");
	return 0;
}

#include "base/base_include.c"
