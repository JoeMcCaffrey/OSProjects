Comp Sci 377 Lab 2 Submission README

James Calabro 27970265
Joseph McCaffrey 27434201


Outline of Design Choices
	For the Java portion of the assignment, we created a Request class that had a semi-random wait time (max wait time was supplied as an input argument). We then implemented a bounded queue class that would contain Responses. If you tried to enqueue to a full queue, it would reject what you were trying to enqueue. This would never happen in our code however because when the Producer thread tries to call enqueue, it first checks to make sure that the queue isn't full and waits otherwise. We read the number of consumers to create as an input argument, create the producer and consumers, and start each thread by calling the start() method. Our Producer and Consumer classes extend Thread and override the run() method. The producer checks to make sure that the request queue isn't full, then generates a new request, then waits a semi-random time (a producer wait factor is supplied as a program argument). If the queue is full, it yields the remainder of its time to other threads. The consumer first checks to make sure that there is an element in the queue, then it dequeus it and executes the Request (waiting for a semi-random amount of time). If there is no item in the queue, the consumer yields the remainder of its time-slice. This continues until the user give an interrupt signal. The synchronization works by using the synchronized keywork to make a synchronized block whenever there is a critical section. For instance, whenever we want to enqueue to the request queue, we would need to place that in a synchronized block using the requestQueue object as a monitor.

	For the C portion of the assignment, 
First step in the C program was to build a data structure to hold the time and id number for the job, we went with a queue linked list. We have two functions: enqueue (put something into the queue) and dequeue( take something out of the queue and return the job and id number) to keep the job and id number together we used a struct with two fields. The queue is a linked list of struct nodes with structs inside of those nodes. This allowed us to keep track of the job number and seconds. Next we used pthreads for the producer and consumer, those require functions for each. The producer thread enqueues a calculated random number between the command line input and 1, and enqueues it along with the id number, then sleeps some random calculated value. The consumer then dequeues the job and executes it. The syncronization is very important to this problem, we used a binary mutex, and two counting semaphores. The binary mutex unlocks and locks the queue, so only one party can have access at a time to the queue. The first counting semaphore is initiated to the number of threads. This represents the amout the number of threads without jobs, when this is >0 then the producer should put things in the queue. The second one keeps track of the number of things in the queue which is initally zero, when this is zero the threads shouldnt dequeue because there are no jobs to take. When the program begins the consumers are all initialized and are in a wait because the queue semaphore is zero, when the producer comes up it sees that the threads semaphore is positive so it should start producing things into the queue, the threads see this and start extracting things from the queue and running them. This syncronization solves this problem and ensures that every structure is safe from deadlocks and race conditions.

Compilation and Running Instructions
	Both programs are pre-compiled on the edlab machines. In order to execute the C version, both the max wait time per request and the producer sleep factor must be greater than one.

	Java:
		Compile:
			$ make
		Run:
			$ java ProducerConsumer <# of threads (N)> <max wait time per request (M)> <producer sleep factor>
		Clean:
			$ make clean
		
	C:
		Compile:
			$ make
		Run:
			$ ./produceConsume <# of threads (N)> <max wait time per request (M)> <producer sleep factor>
		Clean:
			$ make clean

The amount of effort for coding in these two languages was equal. We are both much more comfortable coding in C and C++ than Java, so there were some minor issues using IntelliJ to code in Java. We probably should have just compiled our Java via command line from the start. Neither language was particularly challenging to implement this in.
