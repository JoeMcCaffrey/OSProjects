import java.util.LinkedList;


public class ProducerConsumer
{
    public static void main(String [] args)
    {
        // check to make sure input arguments are correct
        // will print usage and call System.exit() if incorrect
        ProducerConsumer.checkInputArguments(args);

        // parse the program's input arguments
        int nConsumers = Integer.parseInt(args[0]);
        double maxWaitTimePerRequest = Double.parseDouble(args[1]) * 1000;
        double producerSleepFactor = Double.parseDouble(args[2]) * 1000;

        // create the empty request queue
        BoundedQueue<Request> requestQueue = new BoundedQueue<>(nConsumers);

        // create a new producer
        Producer producer = new Producer(requestQueue, maxWaitTimePerRequest, producerSleepFactor);

        // create the array of N consumers
        Consumer[] consumerArray = new Consumer[nConsumers];
        for (int consumerIndex = 0; consumerIndex < nConsumers; ++consumerIndex)
        {
            consumerArray[consumerIndex] = new Consumer(requestQueue, consumerIndex + 1);
        }

        // start the producer
        producer.start();

        // start the consumers
        for (Consumer consumer : consumerArray)
        {
            consumer.start();
        }
    }

    public static void checkInputArguments(String[] args)
    {
        if (args.length < 3 || args.length > 3)
        {
            ProducerConsumer.printUsage();
        }

        try
        {
            Integer.parseInt(args[0]);
            Double.parseDouble(args[1]);
            Double.parseDouble(args[2]);
        }
        catch (Exception e)
        {
            ProducerConsumer.printUsage();
        }
    }

    public static void printUsage()
    {
        System.out.println("usage: ");
        System.out.println("    $ java ProducerConsumer <# of threads (N)> <max wait time per request (M)> <producer sleep factor>");
        System.exit(0);
    }
}

/*
    The Producer class is used to generate new requests and random times that are influenced by the
    producerSleepFactor variable (passed as 3rd input argument). It puts these requests in the request
    queue that is shared by the producer and all consumers.
 */
class Producer extends Thread
{
    private int highestRequestID;
    private double maxWaitTimePerRequest, producerSleepFactor;
    private final BoundedQueue<Request> requestQueue;

    // constructor
    Producer(BoundedQueue<Request> requestQueue, double maxWaitTimePerRequest, double producerSleepFactor)
    {
        this.maxWaitTimePerRequest = maxWaitTimePerRequest;
        this.producerSleepFactor = producerSleepFactor;
        highestRequestID = 0;
        this.requestQueue = requestQueue;
    }

    /*
        run() method overrides the virtual method run() in the Thread class and is called when the thread
        is started. The producer's run method loops forever (until an interrupt signal is given) and first
        synchronizes on the request queue, then checks if it is full. If it is not full, it enqueues a new
        request, exits the critical section, then waits for a random time. If it is full, it exits the
        synchronized block (critical section) then yields the remainder of its time-slice to the consumer
        threads.
    */
    @Override
    public void run()
    {
        boolean requestQueueIsFull;
        // producer thread will execute this
        while (true)
        {
            // wait until there is a free spot in the bounded queue before enqueuing again
            synchronized (requestQueue)
            {
                if (!requestQueue.isFull())
                {
                    requestQueueIsFull = false;
                    // produce a request and then wait between 0 and M seconds
                    requestQueue.enqueue(new Request(highestRequestID++, maxWaitTimePerRequest));
                }
                else
                {
                    requestQueueIsFull = true;
                }
            }

            // if the queue is full, can't queue another request,
            // so yield remaining CPU time for producer thread
            if (requestQueueIsFull)
            {
                Thread.yield();
            }
            // else wait for some time
            else
            {
                try
                {
                    Thread.sleep((long) (Math.random() * (producerSleepFactor + 1)));
                }
                catch (InterruptedException interruptedException)
                {
                    System.exit(1);
                }
            }
        }
    }
}

/*
    The Consumer class is used to process requests. The number of consumers produced (the number of threads
    that will be created) is passed as the first input argument. Each consumer listens on the request queue
    and if it is not empty, it will execute a request and print some diagnostic information about its state.
 */
class Consumer extends Thread
{
    private final BoundedQueue<Request> requestQueue;
    private Request request;
    private final int consumerID;

    // constructor
    Consumer(BoundedQueue<Request> requestQueue, int consumerID)
    {
        this.requestQueue = requestQueue;
        this.consumerID = consumerID;
        request = null;
    }

    /*
        run() method overrides the virtual method run() in the Thread class and is called when the thread
        is started. The consumer's run() method loops until an interrupt signal occurs, first checking to
        see if there is a request in the request queue. If there is, it dequeues it and assigns it to the
        request variable. We then exit the synchronized block (critical section) and test to see if the
        request we just tried to get is null. If it isn't, that means we have a request to process, so
        we print diagnostic information about the request and the consumer processing it, then we execute
        the request. We then print more infomation once the request has finished processing. If the request
        was null, that means that the request queue was empty and therefore there are no jobs to process,
        so we yield the remainder of the thread's time-slice.
     */
    @Override
    public void run()
    {
        // consumer threads will execute this
        while (true)
        {
            request = null;
            // grab a lock on the request queue to check if it is empty
            synchronized (requestQueue)
            {
                // if there is a request in the queue
                if (!requestQueue.isEmpty())
                {
                    try
                    {
                        request = requestQueue.dequeue();
                    }
                    catch (Exception e)
                    {
                    }
                }
            }

            if (request != null)
            {
                System.out.println("Consumer " + consumerID + " assigned request ID:" +
                        request.getRequestID() + ", processing request for the next " +
                        request.getWaitTime() / 1000.0 + " seconds, current time is " +
                        System.currentTimeMillis());

                // execute the request
                request.executeRequest();

                System.out.println("Consumer " + consumerID + ": completed request ID: " +
                        request.getRequestID() + " at time " + System.currentTimeMillis());
            }
            else
            {
                Thread.yield();
            }
        }
    }
}


/*
    BoundedQueue class is a simple implementation of a queue that checks to see if the amount of objects
    in it are less than the max number of objects allowed in it while enqueuing. It takes advantage of
    Java's LinkedList class.
 */
class BoundedQueue<T>
{
    private int size;
    private final int maxSize;
    private LinkedList<T> queue;

    public BoundedQueue(int maxSize)
    {
        size = 0;
        this.maxSize = maxSize;
        queue = new LinkedList<>();
    }

    public synchronized boolean enqueue(T val)
    {
        if (size < maxSize)
        {
            queue.add(size, val);
            ++size;
            return true;
        }
        else
        {
            return false;
        }
    }

    public synchronized T dequeue()
    {
        if (size > 0)
        {
            --size;
            return queue.remove(0);
        }
        else
        {
            throw new IndexOutOfBoundsException("Cannot dequeue an empty queue in thread: " + Thread.currentThread().getId());
        }
    }

    public int getSize()
    {
        return size;
    }

    public synchronized int getMaxSize()
    {
        return maxSize;
    }

    public boolean isEmpty()
    {
        return size == 0;
    }

    public synchronized boolean isFull()
    {
        return size == maxSize;
    }

    @Override
    public String toString()
    {
        return queue.toString();
    }
}


/*
    The Request class holds the amount of time to wait per a request, the request ID, and a state boolean
    about whether or not the request has already been taken (extra concurrency sanity check). The
    executeRequest() method simply checks to make sure that the request has not already been executed,
    then executes the request by sleeping for a semi-random amount of time.
 */
class Request
{
    private final long waitTime;
    private final int requestID;
    private boolean alreadyExecuted;

    public Request(int requestID, double maxSeconds)
    {
        waitTime = (long)(Math.random() * (long)maxSeconds) + 1;
        this.requestID = requestID;
        alreadyExecuted = false;
    }

    public double getWaitTime()
    {
        return waitTime;
    }

    public int getRequestID()
    {
        return requestID;
    }

    public void executeRequest()
    {
        if (!alreadyExecuted)
        {
            try
            {
                Thread.sleep(waitTime);
                alreadyExecuted = true;
            }
            catch (InterruptedException interruptedException)
            {
                System.exit(1);
            }
        }
        else
        {
            throw new IllegalStateException("Cannot execute a request twice");
        }
    }
}
