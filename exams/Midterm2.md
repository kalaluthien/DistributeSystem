# 분산시스템 중간고사2
![Prof. Yoem](http://eng.snu.ac.kr/sites/default/files/professor/20100122135859.jpg)

## 2012 기출 (중간고사2)
1. (SRSW, M-Valued, Regular)을 사용하여 어떻게 (SRSW, M-Valued, Atomic)을 만들 수 있겠는지 보이시오.
1. Atomic fetch and add (FAA) 연산을 사용하여 thread1은 `faa(1)`, thread2는 `faa(2)`, thread3은 `faa(4)`, thread5는 `faa(8)`을 수행한다고 하자. 각각 서로 다른 bit에 동작을 하기 때문에 overwrite를 하지 않고 각자가 add하기 전의 값을 읽어내기 때문에 어떤 thread들이 자신보다 먼저 faa(x)를 수행했는지를 알아서 전체적인 순서를 알 수 있게 된다. 이 경우에 8-bit를 사용한다면 7개의 thread 간에 consensus가 가능하겠는지 걸명하시오.
1. Linearizability와 Sequential consistency의 차이점을 설명하고, linearizable하지는 않지만 sequential consistent한 예를 드시오.
1. (1) 아래 Peterson's algorithm에서 flag와 victim이 atomic register가 아니고 regular register라면 이 알고리즘이 가지고 있는 mutual exclusion, deadlock-free, starvation-free 성질이 그대로 유지되겠는지 보이시오. (2) 만일 safe register라면 어떨지 보이시오.
1. FIFO queue (`enq()`, `deq()`)는 2-thread consensus를 해셜할 수 있다. 만일 `deq()`하지 않고 dequeue될 item을 읽을 수 있는 `peek()`이 제공된다면, 이 새로운 queue의 consensus number는 얼마이겠는가?

### 4번 소스 코드
```
void lock() {
  flag[i] = true;
  victim = i;
  while (flag[j] && victim == i);
}

void unlock() {
  flag[i] = false;
}
```
