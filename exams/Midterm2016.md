# 분산시스템 중간고사
![Yoem](http://eng.snu.ac.kr/sites/default/files/professor/20100122135859.jpg)

## 기출문제
* Test-and-set로 mutual exclusion 을 해결할 수 있지만 n-thread consensus는 해결할 수 없다. 차이점과 해결할 수 없는 이유를 설명하시오.
* Wait-free와 Lock-free의 차이점을 설명하시오.
* Bounded timestamping system (**T(k) = T(2) * T(k-1)**)이 concurrent한 환경에서 제대로 동작 안 하는 경우가 있다. Compare-and-set을 이용하여 **T(3)** 에서 thread들이 concurrent하게 동작할 수 잇도록 timestamp를 만드는 방법을 제시하시오. (Wait-free하도록)
* Linearizability와 Sequential consistency중 누가 더 강력한 조건인지 설명하고, 둘 중 하나는 만족하지만 다른 하나는 만족하지 못하는 예시를 드시오.
* MCS Queue Lock와 CHL Queue Lock의 차이점을 말하고 어떤 환경에서 더 적합한지 설명하시오.

## 시험공부
### 2. Mutual Exclusion

좋은 Lock algorithm의 조건

* Mutual Exclusion
  * 서로 다른 스레드의 CS가 서로 겹치지 않는다.
* Deadlock-free
  * Lock을 얻으려하는 적어도 하나의 스레드는 언젠가 Lock을 얻게 된다.
* Starvation-free (= _lockout freedom_)
  * Lock을 얻으려하는 모든 스레드는 언젠가 Lock을 얻게 된다.
  * Starvation-free이면 Deadlock-free이다.

The Peterson Lock

* 'flag[i]' : thread i가 진입하고자 함
* 'victim' : thread victim은 진입할 수 없음
* 초기 조건 : 'flag[i] = true', 'victim = i'
* 진입 조건 : 'flag[j] == faise || victim == j'
* Concurrent하지 않을 때 : 'flag[j] == false'이므로 진입
* Concurrent할 때: 'victim'이 하나의 값만 가지므로, thread i와 j 중 하나만 진입

The Filter Lock

* 'level[i]' : thread i가 진입하고자 하는 최대 level
* 'victim[j]' : level j에 진입하지 못하는 thread
* for j = 1 to n - 1
  * 초기 조건 : 'level[i] = j', 'victim[j] = i'
  * 진입 조건 : 'for all k != i (level[k] < i || victim[i] != me)'
* level 0에 thread N개 이하, level j에 thread N - j개 이하, level n - 1에 thread 1개 이하 (CS)
* Concurrent할 때, 마지막으로 'victim[j]'에 write한 thread는level j 아래로 내려갈 수 없음

The Bakery Lock

* 'flag[i]' : thread i가 진입하고자 함
* 'label[i]' : thread i의 label (작을 수록 먼저)
* 초기 조건 : 'flag[i] = true', 'label[i] = MAX(label[0 : n-1]) + 1'
* 진입 조건 : 'for all k != i (flag[k] == false || (label[k], k]) >> (lable[i], i))'
* '(label[i], i)'를 비교하면 label이 겹치더라도 어느 thread가 진입할 것인지 알 수 있음
* Overflow의 위험성이 있다.

Bounded Timestamps

* T<sup>2</sup> : 0 < 1 < 2 < 0
* T<sup>k</sup> = T<sup>2</sup> * T<sup>k-1</sup>
  * T<sup>n</sup>는 n-1개의 digit이 있고, n-thread bounded sequential timestamping system의 basis
  * T<sup>3</sup> : 00 < 01 < 02 < 10 < 11 < 12 < 20 < 21 < 22 < 00
* 낮은 자리를 먼저 증가시키되, 해당 subgraph가 가득 차게 되면 높은 자리에서 다시 수행

Lower Bounds on the Number of Locations

* Deadlock-free Lock algorithm은 최악의 경우 최소 n개의 서로 다른 메모리를 할당하고 접근해야함
* 따라서 기본적인 load store instruction  대신 synchronization operation이 필요함

### 3. Concurrent Object

Sequential Objects

* 'class object = { state, methods }'
* precondition -> side effect -> postcondition
* 하지만 Concurrent Object는 어렵다

Quiescent Consistency

* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Method calls separated by a period of quiescence should appear to take effect in their real-time order
* 어떤 object에 pending된 method call이 하나도 없을 때, 그때까지의 실행 결과와 결과가 동등한 어떤 sequential execution order가 존재
* 하나 이상의 thread의 method 수행 순서가 object가 quiescent한 구간 전후로 보장됨
* 수행 순서가 무관한 concurrent execution에서 유용함
* Quiescent Consistency는 _nonblocking_ correctness condition이며 compositinal함
* 용어 정리
  * total method
    * 모든 object state에 대해 정의되는 method (ex. 'enq()' if unbounded)
  * partial method
    * 어떤 object state에 대해 정의되지 않는 method (ex. 'deq()')
  * nonblocking
    * total method에 대한 임의의 pending call은 언제나 완료될 수 있음
  * compositinal
    * 시스템 내의 각 object가 P를 만족할 때, 시스템 전체도 P를 만족하면 P는 compositinal함

Sequential Consistency

* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Method calls should appear to take effect in program order
* (1) 각 thread 내에서의 program order를 보존하고 (2) object의 sequential specification을 만족하는 어떤 sequential execution order가 존재
* Quiescent Consistency와 Sequential Consistency는 incomparable (포함 관계 x)
  * Quiescent Consistency는 quiescent한 구간 전후로만 수행 순서가 보장되므로 program order를 보존하지 않음
  * Sequential Consistency는 real-time order와 무관하므로 object가 quiescent한 구간 전후로 서로 다른 thread의 method 수행 순서가 보장되지 않음
  * ex1) ('A.enq(x)' call) ('A.enq(x)' return) ('B.enq(y)' call) ('A.deq(y)' call) ('B.enq(y)' return) ('A.deq(y)' return)
    * Seq(true), Qui(false)
  * ex2) ('A.p.enq(x)') ('B.q.enq(y)') ('A.q.enq(x)') ('B.p.enq(y)') ('A.p.deq(y)') ('B.q.deq(x)')
    * Seq.p(true), Seq.q(true), Seq(false) : compositinal하지 않음
* 현대(?)의 멀티프로세서에서 read & write는 sequential consistency하지 않음
* Sequential Consistency는 _nonblocking_ correctness condition이며 compositinal하지 않음

Linearizability

* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Each method call should appear to take effect instantaneously at some moment between its invocation and responce
* 서로 겹치지 않는 concurrent method call은 그 순서로 실행되고(program order는 저절로 보존), 서로 겹치는 concurrent method call은 object의 sequential specification을 만족하도록 하는 순서로 실행되어, 전체 History가 그와 동등한 어떤 sequential execution order가 있어야 함
* 모든 liniearizable execution은 sequentially consistent함 (역은 성립하지 않음) -> moment를 기준으로 sequential order를 매기면 됨
* method가 side effect를 내는 liniearization point를 찾아야 함 (object의 state가 바뀌는 순간)
  * Lock 있음: critical section
  * Lock 없음: a single step
* Linearizability는 _nonblocking_ correctness condition이며 compositinal함

Progress Conditions

* Blocking
  * 한 thread의 unexpected delay가 다른 thread들이 progress하지 못하게 할 수 있으면 blocking implementation
* Wait-free
  * 모든 method call이 유한한 step 후에 종료되는것이 보장되는 경우
  * thread의 수와 무관한 wait-free method를 population-oblivious라고 함
  * 모든 method가 wait-free인 object를 wait-free, 모든 instance가 wait-free인 class를 wait-free라고 함
* Bounded wait-free
  * method call의 step에 bound가 있는 경우
  * bound가 thread의 수에 의존하는 경우도 있음
* Lock-free
  * 어떤 method call이 유한한 step 후에 종료되는 것이 (무한히) 보장되는 경우
  * Wait-free -> Lock-free (역은 성립하지 않음)
  * 일부 thread는 starcation을 겪을 수 있으나, 실제로는 거의 일어나지 않는다면 wait-free보다 실용적일 수 있음

### 4. Foundations of Shared Memory












### 5. The Relative Power of Primitive Synchronization Operations









### 6. Universality of Concensus






