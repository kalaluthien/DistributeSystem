# 분산시스템 중간고사2

## 2009 기출 (중간고사2)
1. n-thread consensus를 정의하고 failure가 발생할 수 있는 asynchronos distributed system에서 왜 불가능한지 간단히 설명하시오.
1. linked list에서 optimistic synchronization과 lazy synchronization의 차이를 설명하고 어떤 부분이 어떻게 바뀌어야 하는지 설명하시오.
1. MRSW boolean regular register를 사용해서 MRMW boolean regular register를 만드시오.
1. 3개의 atomic register를 동시에 write할 수 있는 연산을 가지고 4-thread consensus가 가능함을 보이시오.
1. 다음 bounded timestamping system이 concurrent한 환경에서 제대로 동작할 수 있게 만들어보시오. (n=3, 초기 상태 : 10, 11, 22)

## 2010 기출 (중간고사)
1. 책의 그림 2.12에 있는 bounded timestamping system은 thread가 concurrent하게 진행한다면 inconsistent state로 갈 수가 있다. (3개의 thread가 각각 떨어지거나(00, 11, 22) 한 곳에 모이는(00, 01, 02) 경우) consistent state에서 시작하여 두 경우가 다 발생할 수 있는지, 아니면 한 가지 경우는 발생하지 않는 지 보이시오.
1. 다음은 LL/SC의 abstraction이다. consensus number가 CAS와 같음을 보이고, 또한 CAS와 달리 왜 ABA 문제가 생기지 않는지 설명하시오.
1. TSET이나 CAS가 없는 경우에 64bit write 연산을 사용해서 consensus를 할 수도 있다. multiple-assignment를 흉내내면 된다. 최대 consensus number가 얼마가 될 수 있겠는지 구성해보시오.
1. register의 종류는 (SRSW, MRSW, MRMW) (B, MV) (safe, regular, atomic)의 조합으로 모두 18가지가 있을 수 있고, 이중 boolean을 무시하면 9가지가 된다. 이들 사이의 관계를 도식하시오. 예를 들어 (MRSW, R) register는 O(m)개의 (SRSW, R) register를 사용해서 만들 수 있으므로 (MRSW, R) --O(m)--> (SRSW, R)라고 표시한다. (reader의 수는 m, writer의 수는 n, value의 가짓수는 k) 수업시간에 나오지 않은 transformation에 대해서는 어덯게 가능한지 간단히 설명하시오.

### 2번 소스 코드
```
public class LLSC {
	Object value;
	boolean busy;
	public LLSC(Object object) {
		this.value = object;
		this.busy = false;
	}
	public synchronized Object LL() {
		this.busy = true;
		return this.value;
	}
	public synchronized boolean SC(Object newValue) {
		if (this.busy) {
			this.busy = false;
			this.value = newValue;
			return true;
		}
		return false;
	}
}
```

## 2012 기출 (중간고사2)
1. (SRSW, M-Valued, Regular)을 사용하여 어떻게 (SRSW, M-Valued, Atomic)을 만들 수 있겠는지 보이시오.
1. Atomic fetch and add (FAA) 연산을 사용하여 thread1은 `faa(1)`, thread2는 `faa(2)`, thread3은 `faa(4)`, thread5는 `faa(8)`을 수행한다고 하자. 각각 서로 다른 bit에 동작을 하기 때문에 overwrite를 하지 않고 각자가 add하기 전의 값을 읽어내기 때문에 어떤 thread들이 자신보다 먼저 faa(x)를 수행했는지를 알아서 전체적인 순서를 알 수 있게 된다. 이 경우에 8-bit를 사용한다면 7개의 thread 간에 consensus가 가능하겠는지 걸명하시오.
1. Linearizability와 Sequential consistency의 차이점을 설명하고, linearizable하지는 않지만 sequential consistent한 예를 드시오.
1. (1) 아래 Peterson's algorithm에서 flag와 victim이 atomic register가 아니고 regular register라면 이 알고리즘이 가지고 있는 mutual exclusion, deadlock-free, starvation-free 성질이 그대로 유지되겠는지 보이시오. (2) 만일 safe register라면 어떨지 보이시오.
1. FIFO queue (`enq()`, `deq()`)는 2-thread consensus를 해결할 수 있다. 만일 `deq()`하지 않고 dequeue될 item을 읽을 수 있는 `peek()`이 제공된다면, 이 새로운 queue의 consensus number는 얼마이겠는가?

### 4번 소스 코드
```
public void lock() {
	flag[i] = true
	victim = i
	while (flag[j] && victim == i);
}

public void unlock() {
	flag[i] = false
}
```

## 2017 기출 (중간고사2)
1. FetchAndAdd operation `fa(1, x)`, `fa(2, x)`, `fa(4, x)`, `fa(8, x)`을 값이 0인 것을 시작으로 적용하면 순서를 알 수 있다. 각자 bit가 다른 위치에 있기 때문에. 하지만 FAA는 consensus number가 2이다. 무엇이 잘못된 것인가?
1. Skiplist based unbounded priority queue가 linearizable한가?
1. Atomic 3-assignment를 이용하여 4 thread consensus가 가능한가?
1. Multithread quicksort에서 Work(T1)와 CriticalPath(T∞)의 시간을 구하시오. (평균 혹은 최적과 최악)
1. Recursive split ordering이 Concurrent hashing에 어떻게 이용되는가?
1. Combining tree의 최적/최악의 경우에서의 throuput을 구하시오.
