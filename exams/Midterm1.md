# 분산시스템 중간고사
![Yoem](http://eng.snu.ac.kr/sites/default/files/professor/20100122135859.jpg)

## 기출문제
1. Test-and-set로 mutual exclusion을 해결할 수 있지만 n-thread consensus는 해결할 수 없다. 차이점과 해결할 수 없는 이유를 설명하시오.
1. Wait-free와 Lock-free의 차이점을 설명하시오.
1. Bounded timestamping system (**T(k) = T(2) * T(k-1)**)이 concurrent한 환경에서 제대로 동작 안 하는 경우가 있다. Compare-and-set을 이용하여 **T(3)** 에서 thread들이 concurrent하게 동작할 수 있도록 timestamp를 만드는 방법을 제시하시오. (Wait-free하도록)
1. Linearizability와 Sequential consistency중 누가 더 강력한 조건인지 설명하고, 둘 중 하나는 만족하지만 다른 하나는 만족하지 못하는 예시를 드시오.
1. MCS Queue Lock와 CHL Queue Lock의 차이점을 말하고 어떤 환경에서 더 적합한지 설명하시오.

## 시험복기
1. Bounded timestamping system은 sequential하게 작동할 때는 consistent하지만 concurrent하게 작동할 때는 inconsistent할 때가 있다. **T(3)**에 thread 0, 1, 2가 각각 22, 11, 10에 있을 때 inconsistent해지는 경우가 2가지 있다. 이때의 스케줄을 보이시오. (이 순간 어떤 스레드든 next-TS()는 20임)

```
i = Thread.Id(); j = (i + 1) % 3; k = (i + 2) % 3;
read TS(i); read TS(j); read TS(k);
calc next-TS(); update TS(i);
```

1. Atomic-2-assignment를 이용하면 2-thread consensus를 해결할 수 있다. 이를 이용해서 다음과 같이 4-thread consensus를 해결하려고 한다. 스레드를 2개씩 묶어서 경쟁하고 다음 단계에 이긴 스레드끼리 승자를 정한다. 이것이 가능한지 불가능한지 설명하시오.
1. 가장 간단한 Safe, SRSW, Boolean Register를 이용해서 Atomic, 3-reader/3-writer 4-bit Register를 만들 때 필요한 레지스터의 수를 구하시오.
1. Filter Lock을 사용하고 thread 1, 2, 3이 있을 때, thread 1이 thread 2, 3에 2번, 3번, 4번 추월당하는 예시를 드시오.
1. Filter Lock이 Wait-free인지 아닌지 보이시오.

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

* `flag[i]` : thread i가 진입하고자 함
* `victim` : thread victim은 진입할 수 없음
* 초기 조건 : `flag[i] = true`, `victim = i`
* 진입 조건 : `flag[j] == faise || victim == j`
* Concurrent하지 않을 때 : `flag[j] == false`이므로 진입
* Concurrent할 때: `victim`이 하나의 값만 가지므로, thread i와 j 중 하나만 진입

The Filter Lock

* `level[i]` : thread i가 진입하고자 하는 최대 level
* `victim[j]` : level j에 진입하지 못하는 thread
* for j = 1 to n - 1
  * 초기 조건 : `level[i] = j`, `victim[j] = i`
  * 진입 조건 : `for all k != i (level[k] < j) || victim[j] != i`
* level 0에 thread N개 이하, level j에 thread N - j개 이하, level n - 1에 thread 1개 이하 (CS)
* Concurrent할 때, 마지막으로 `victim[j]`에 write한 thread는 level j 아래로 내려갈 수 없음

The Bakery Lock

* `flag[i]` : thread i가 진입하고자 함
* `label[i]` : thread i의 label (작을 수록 먼저)
* 초기 조건 : `flag[i] = true`, `label[i] = MAX(label[0 : n-1]) + 1`
* 진입 조건 : `for all k != i (flag[k] == false || (label[k], k]) >> (lable[i], i))`
* `(label[i], i)`를 비교하면 label이 겹치더라도 어느 thread가 진입할 것인지 알 수 있음
* Overflow의 위험성이 있다.

Bounded Timestamps

* T<sup>2</sup> : 0 < 1 < 2 < 0
* T<sup>k</sup> = T<sup>2</sup> * T<sup>k-1</sup>
  * T<sup>n</sup>는 n-1개의 digit이 있고, n-thread bounded sequential timestamping system의 basis
  * T<sup>3</sup> : 00 < 01 < 02 < 10 < 11 < 12 < 20 < 21 < 22 < 00
* 낮은 자리를 먼저 증가시키되, 해당 subgraph가 가득 차게 되면 높은 자리에서 다시 수행

Lower Bounds on the Number of Locations

* thread가 n개일 때, Deadlock-free Lock algorithm은 최악의 경우 최소 n개의 서로 다른 메모리를 할당하고 접근해야함
* 따라서 기본적인 load & store instruction  대신 synchronization operation이 필요함

### 3. Concurrent Object

Sequential Objects

* Object는 다음과 같이 정의할 수 있음: `class object = { state, methods }`
* precondition -> side effect -> postcondition
* invocation -> interval(method call) -> response
* 하지만 Concurrent Object는 어렵다

Quiescent Consistency

* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Method calls separated by a period of quiescence should appear to take effect in their real-time order
* 어떤 object에 pending된 method call이 하나도 없을 때, 그때까지의 실행 결과와 결과가 동등한 어떤 sequential execution order가 존재
* object가 quiescent한 구간 전후로 method 수행 순서가 보장됨
* 수행 순서가 무관한 concurrent execution에서 유용
* Quiescent Consistency는 _nonblocking_ correctness condition이며 compositinal함
* 용어 정리
  * total method
    * 모든 object state에 대해 정의되는 method (ex. `enq()` if unbounded)
  * partial method
    * 어떤 object state에 대해 정의되지 않는 method (ex. `deq()` because of EMPTY\_QUEUE\_EXCEPTION)
  * nonblocking
    * total method에 대한 임의의 pending call은 언제나 완료될 수 있음
  * compositinal
    * 시스템 내의 각 object가 P를 만족할 때, 시스템 전체도 P를 만족하면 P는 compositinal한 correctness condition

Sequential Consistency

* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Method calls should appear to take effect in program order
* (1) 각 thread 내에서의 program order를 보존하고 (2) object의 sequential specification을 만족하는 어떤 sequential execution order가 존재
* Quiescent Consistency와 Sequential Consistency는 incomparable (포함 관계 x)
  * Quiescent Consistency는 quiescent한 구간 전후로만 수행 순서가 보장되므로 program order를 보존하지 않음
  * ex) \[`A.enq(x)` call\] \[`B.enq(y)` call\] \[`A.enq(x)` return\] \[`A.enq(z)` call\] \[`B.enq(y)` return\] \[`A.enq(z)` return\] \[`A.deq(z)`\]
    * sequential consistent = `false`, quiescent consistent = `true`
  * Sequential Consistency는 real-time order와 무관하므로 object가 quiescent한 구간 전후로 서로 다른 thread의 method 수행 순서가 보장되지 않음
  * ex) \[`A.enq(x)`\] \[`B.enq(y)` call\] \[`A.deq(y)` call\] \[`B.enq(y)` return\] \[`A.deq(y)` return\]
    * sequential consistent = `true`, quiescent consistent = `false`
* 현대(?)의 멀티프로세서에서 read & write는 sequential consistency하지 않음
* Sequential Consistency는 nonblocking correctness condition이며 compositinal하지 않음
  * ex) \[`A.p.enq(x)`\] \[`B.q.enq(y)`\] \[`A.q.enq(x)`\] \[`B.p.enq(y)`\] \[`A.p.deq(y)`\] \[`B.q.deq(x)`\]
    * p에 대해서는 `true`, q에 대해서도 `true`이지만 전체에 대해서는 `false`

Linearizability

* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Each method call should appear to take effect instantaneously at some moment between its invocation and responce
* 서로 겹치지 않는 concurrent method call은 그 순서로 실행되고(program order는 저절로 보존), 서로 겹치는 concurrent method call은 object의 sequential specification을 만족하도록 하는 순서로 실행되어, 전체 History가 그와 동등한 어떤 sequential execution order가 있어야 함
* 모든 liniearizable execution은 sequentially consistent함 (역은 성립하지 않음) -> side effect moment를 기준으로 sequential order를 매기면 됨
* method가 side effect를 내는 linearization point를 찾아야 함 (object의 state가 바뀌는 순간)
  * Lock 있음: critical section
  * Lock 없음: a single step
* Linearizability는 nonblocking correctness condition이며 compositinal함

Progress Conditions

* Blocking
  * 한 thread의 unexpected delay가 다른 thread들이 progress하지 못하게 할 수 있으면 blocking implementation
  * ex) Mutual-exclusion
* Wait-free
  * 모든 method call이 유한한 step 후에 종료되는것이 보장되는 경우
  * thread의 수와 무관한 wait-free method를 population-oblivious라고 함
  * 모든 method가 wait-free인 object를 wait-free, 모든 instance가 wait-free인 class를 wait-free라고 함
* Bounded wait-free
  * method call의 step에 pre-determined bound가 있는 경우
  * Bounded wait-free -> (unbounded) Wait-free이므로 bounded wait-free가 더 강력한 조건
  * bound가 thread의 수에 의존하는 경우도 있음
* Lock-free
  * 어떤 method call이 유한한 step 후에 종료되는 것이 항상 보장되는 경우
  * Wait-free -> Lock-free 이므로 wait-free가 더 강력한 조건
  * 만약 실행이 유한하다면 wait-free = lock-free (항상 어떤 method call이 유한한 step 후에 종료해야 하므로)
  * 일부 thread는 starvation을 겪을 수 있으나, 실제로는 거의 일어나지 않는다면 wait-free보다 실용적일 수 있음
* Lock-free : Wait-free = Deadlock-free : Starvation-free
  * Lock-free & Deadlock-free는 전체 thread group의 progress를 보장
  * Wait-free & Starvation-free는 개별 thread의 process을 보장

### 4. Foundations of Shared Memory

Read-Write Register

* SRSW - MRSW - MRMW, Boolean - M-valued
* Safe - Regular - Atomic
  * Regular의 조건
    * R<sup>i</sup> -> W<sup>i</sup> 인 경우는 존재하지 않음 (겹치지 않는 미래에 쓰인 값을 읽는 경우)
    * W<sup>i</sup> -> W<sup>j</sup> -> R<sup>i</sup> 인 경우는 존재하지 않음 (겹치지 않는 과거에 덮어쓰인 값을 읽는 경우)
  * Atomic의 조건
    * 위의 조건과 더불어, R<sup>i</sup> -> R<sup>j</sup>일 때 i > j 인 경우는 존재하지 않음
* Weakest Register : SRSW Safe Boolean
  * 여기서부터 모든 다른 register(MRMW Atomic M-valued, Atomic Snapshot, etc)를 구성할 수 있음
  * 하지만 Consensus hierarchy를 뛰어넘을 수는 없음 (concensus number = 1)
* SRSW Safe -> MRSW Safe
  * 각 reader마다 SRSW를 하나씩 할당하면 MRSW가 됨
* MRSW Safe Boolean -> MRSW Regular Boolean
  * 할당되어 있는 값과 바꿀 값이 동일하면 write하지 않음
* MRSW Regular Boolean -> MRSW Regular M-valued
  * MRSW를 M개 두고 `read()` 수행시 reader는 0 to M-1, `write(x)` 수행시 writer는 x to 0로 scan함
* MRSW Regular -> SRSW Atomic
  * writer는 `value:stamp`를 write, reader는 마지막으로 읽은 `value:stamp`를 저장하여 stamp가 최신인 것을 read
* SRSW Atomic -> MRSW Atomic
  * SRSW를 (thread 수)<sup>2</sup>만큼 할당 (thread[0] = writer, thread[1:n] = reader) 구현에 따라서는 (reader 수)<sup>2</sup>면 충분
  * **writer**: column 0에 순서대로 write (또는, 대각선으로 write)
  * **reader**: row i의 최신값을 읽고, column i에 순서대로 write
  * 이렇게 하면 다른 reader가 읽은 값을 놓치는 경우는 (read 시간 역전?) 두 read가 겹쳤을 때 뿐이므로 문제가 없음
* MRSW Atomic -> MRMW Atomic
  * Bakery algorithm과 거의 동일함
  * MRSW를 writer 수만큼 할당
  * **writer**: array를 다 읽고 timestamp를 뽑아서 자기 자리에 write
  * **reader**: array를 다 읽고 timestamp가 최신인 것을 read
  * timestamp 받는 부분이 _write order_의 linearization point, max stamp 읽는 부분이 _read order_의 linearization point
  * -> 특정 code line이 linearization point가 아니고 실행에 따라 다름
* Atomic Snapshot
  * **update**(한 array element에 write) & **scan**(모든 array element를 read)
  * _clean double collect_: linearizable하지 않음 & **scan**이 wait-free가 아니어서 starvation을 겪을 수 있음
  * wait-free snapshot: **update**에서도 **scan**을 함

### 5. The Relative Power of Primitive Synchronization Operations

Consensus Number

* object가 consensus 문제를 해결할 수 있는 최대 thread 수
* consensus number < N인 object를 이용해서 consensus number = N인 wait-free(or lock-free)한 object를 만들 수 없음
  * No wait-free implementation of N-thread consensus from read-write atomic registers
  * -> Asynchronous computability different from Turing computability
* concurrent consensus object
  * `object.decide(input)`의 output은 다음 조건을 만족함
  * _consistent_ : 모든 thread가 같은 값을 decide
  * _valid_ : decide된 값은 어떤 thread의 input
  * `decide()`에 의해 처음으로 선택된 thread를 기준으로 해서 sequential consensus object으로 linearizable
* consensus number 비교
  * Atomic read-write register는 1
  * Multi-dequeuer FIFO queue는 n 이상 (맨 처음 `deq()`한 thread의 값으로 decide)
  * (n, n(n+1)/2)-Assignment Object는 n 이상 (총 n(n+1)/2 register에 n개를 동시에 write해서 덮어쓰인 쪽이 우선)
  * X로 Y를 구현했다면, n(X) ≥ n(Y)

Consensus protocol

* decide를 위한 protocol의 state transition을 binary tree 형태로 나타냄 (edge: move, node: state)
  * **bivalent** 어느 값이든 가능
    * **critical state** 다음 move에 따라 0-valent 또는 1-valent로 나뉘는 bivalent
    * protocol이 wait-free이면 언젠가는 critical state에 도달해야 함
  * **univalent** 한 가지 값만 가능 (그 값이 무엇인지는 모를 수 있음)
    * **x-valent** x만 가능 (ex. 0-valent, 1-valent)
  * protocol의 모순으로부터 Atomic register로는 (n > 1) consensus 문제를 풀 수 없음을 증명
    * A가 read할 때 (A는 read만 하거나 안 하거나 했는데 결과가 달라짐)
    * A는 r0에, B는 r1에 write할 때 (A와 B가 다른데 썼는데 순서에 따라 결과가 달라짐)
    * A와 B가 모두 r에 write할 때 (B는 무조건 쓴다고 하면 덮어썼는데 결과가 달라짐)

Read-Modify-Write Operation

* register의 기존 값 x를 f(x)로 바꾸고 x(기존 값)를 반환하는 operation
* `get()` : f(x) = x
* `get-and-set(v)` : f(x) = v
  * `test-and-set()` = `get-and-set(true)`
* `get-and-increment()` : f(x) = x + 1
* `fetch-and-add(k)` : f(x) = x + k
* `compare-and-set(e, u)` : f(x) = if (x == e) then (x := u, true) else (false)
  * `CAS(e, u)`는 x(기존 값)를 반환하지 않고 교체 여부(bool)를 반환함
* nontrivial한 RMW register는 consensus number가 2 이상임 (Atomic register로는 불가능하므로 hardware에서 RMW method를 지원해야 함)
  * `if (rmw(..) == init)`로 비교하여 i와 1-i 중 `decide()`할 수 있음
  * 그러나 어떤 `rmw(...)`는 thread가 셋 이상일 때 누가 이겼는지 알 수 없음 (자기가 이겼는지 여부와 누가 이겼는지 여부를 다 알아야함)
* Commute & Overwrite RMW object는 consensus number가 2임 ("weak" RMW instruction)
  * commute: f(g(v)) = g(f(v))
  * overwrite: f(g(v)) = f(v)
  * n > 2일 때, 누가 이겼는지 알 수 없다는 것을 binary tree를 이용해서 증명 (thread A, B, C)
* `CAS(e, u)`는 consensus number가 ∞ (자기가 이겼는지를 return 값으로, 누가 이겼는지를 register 값으로 판단)
* FIFO Queue, n-assignment 등은 RMW instruction (혹은 RMW register)를 이용하여 구현해야 함
