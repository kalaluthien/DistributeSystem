## Exersice 15
In practice, almost all lock acquisitions are uncontended, so the most practical measure of a lock’s performance is the number of steps needed for a thread to acquire a lock when no other thread is concurrently trying to acquire the lock.

Scientists at Cantaloupe-Melon University have devised the following “wrapper” for an arbitrary lock, shown in Figure. They claim that if the base Lock class provides mutual exclusion and is starvation-free, so does the FastPath lock, but it can be acquired in a constant number of steps in the absence of contention.

Sketch an argument why they are right, or give a counterexample.

``` JAVA
class FastPath implements Lock {
  private static ThreadLocal<Integer> myIndex;
  private Lock lock;
  private int x, y = -1;

  public void lock() {
    int i = myIndex.get();
    x = i; // I’m here
    while (y != -1) {} // is the lock free?
    y = i; // me again?
    if (x != i) // Am I still here?
      lock.lock(); // slow path
  }

  public void unlock() {
    y = -1;
    lock.unlock();
  }
}
```

## Exercise 27
The AtomicInteger class (in the java.util.concurrent.atomic package) is a container for an integer value. One of its methods is

``` JAVA
boolean compareAndSet(int expect, int update)
```

This method compares the object’s current value to expect. If the values are equal, then it atomically replaces the object’s value with update and returns true. Otherwise, it leaves the object’s value unchanged, and returns false. This class also provides

``` JAVA
int get()
```

which returns the object’s actual value.

Consider the FIFO queue implementation shown in Figure. It stores its items in an array items, which, for simplicity, we will assume has unbounded size. It has two AtomicInteger fields: tail is the index of the next slot from which to remove an item, and head is the index of the next slot in which to place an item.

Give an example showing that this implementation is not linearizable.

``` JAVA
class IQueue<T> {
  AtomicInteger head = new AtomicInteger(0);
  AtomicInteger tail = new AtomicInteger(0);
  T[] items = (T[]) new Object[Integer.MAX_VALUE];

  public void enq(T x) {
    int slot;
    do {
      slot = tail.get();
    } while (! tail.compareAndSet(slot, slot+1));
    items[slot] = x;
  }
  
  public T deq() throws EmptyException {
    T value;
    int slot;
    do {
      slot = head.get();
      value = items[slot];
    
      if (value == null)
        throw new EmptyException();
    } while (! head.compareAndSet(slot, slot+1));
    return value;
  }
}
```

## Exercise 28
Consider the class shown in Figure. According to what you have been told about the Java memory model(JMM), will the reader method ever divide by zero?

``` JAVA
class VolatileExample {
  int x = 0;
  volatile boolean v = false;

  public void writer() {
    x = 42;
    v = true;
  }
  
  public void reader() {
    if (v == true)
      int y = 100/x;
  }
}
```

## Exercise 31
Consider the following rather unusual implementation of a method _m_. In every history, the _i-th_ time a thread calls _m_, the call returns after 2i steps. Is this method wait-free, bounded wait-free, or neither?

## Exercise 32
This exercise examines a queue implementation (Figure) whose enq() method does not have a linearization point.

The queue stores its items in an items array, which for simplicity we will assume is unbounded. The tail field is an AtomicInteger, initially zero. The enq() method reserves a slot by incrementing tail, and then stores the item at that location. Note that these two steps are not atomic: there is an interval after tail has been incremented but before the item has been stored in the array.

The deq() method reads the value of tail, and then traverses the array in ascending order from slot zero to the tail. For each slot, it swaps null with the current contents, returning the first non-null item it finds. If all slots are null, the procedure is restarted.

Give an example execution showing that the linearization point for enq() cannot occur at Line 15.

Hint: give an execution where two enq() calls are not linearized in the order they execute Line 15.

Give another example execution showing that the linearization point for enq() cannot occur at Line 16.

Since these are the only two memory accesses in enq(), we must conclude that enq() has no single linearization point. Does this mean enq() is not linearizable?

``` JAVA
public class HWQueue<T> {
  AtomicReference<T>[] items;
  AtomicInteger tail;
  static final int CAPACITY = 1024;

  public HWQueue() {
    items =(AtomicReference<T>[])Array.newInstance(AtomicReference.class, CAPACITY);
    for (int i = 0; i < items.length; i++)
      items[i] = new AtomicReference<T>(null);

    tail = new AtomicInteger(0);
  }
  
  public void enq(T x) {
    int i = tail.getAndIncrement();
    items[i].set(x);
  }
  
  public T deq() {
    while (true) {
      int range = tail.get();
      for (int i = 0; i < range; i++) {
        T value = items[i].getAndSet(null);
        if (value != null)
          return value;
      }
    }
  }
}
```
