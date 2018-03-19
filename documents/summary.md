# 2. Mutual Exclusion

## Thread와 Event
* Thread는 Event a0, a1, a2, ...의 나열이다.
* Program order: `thread A: a0, a1, a2, ...`
* Thread는 State Machine이다.
  * Thread state = (PC, Local variables)
  * System state = (Object fields, ∪Thread states)
* Interval `A = (a, b)`은 두 Event `a, b` 사이의 시간이다.
* 두 Interval은 concurrent하게 interleaving 할 수 있다.
* 두 Interval 이 disjoint하면 precedence를 정할 수 있고, 이것은 partial order에 해당함.
  * Irreflexive: `￢(A⇒A)`
  * Antisymmetric: `(A⇒B) → ￢(B⇒A)`
  * Transitive: `(A⇒B)∧(B⇒C) → A⇒A)`
  * 하지만 겹칠 수 있으므로 `∀A≠B: (A⇒B)∨(B⇒A)`이 성립하지 않는다.
* Event a<sub>i</sub><sup>k</sup>: Thread i의 k번째 Event a
* Interval A<sub>i</sub><sup>k</sup>: Thread i의 k번째 Interval A

## Lock algorithm의 특성
* Mutual Exclusion
  * 서로 다른 스레드의 CS가 서로 겹치지 않는다.
  * 따라서 서로 다른 스레드의 CS 사이에는 반드시 순서가 있다.
  * `∀i,j∀p,q: (CSip⇒CSjq)∨(CSjq⇒CSip)`
* Deadlock-free
  * Lock을 얻으려하는 적어도 하나의 스레드는 언젠가 Lock을 얻게 된다.
* Starvation-free (= _lockout freedom_)
  * Lock을 얻으려하는 모든 스레드는 언젠가 Lock을 얻게 된다.
  * Starvation-free ⊆ Deadlock-free이다.
* Fairness
  * Stall하고 있는 스레드에 대한 Overtake 횟수가 제한된다.

## The Peterson Lock
* 두 스레드 i, j만 있는 경우
* `flag[i]` : thread i가 진입하고자 함
* `victim` : victim에 해당하는 thread는 진입할 수 없음
* 초기 조건 : `flag[i] = true` 이고 `victim = i`
* 진입 조건 : `flag[j] == faise` 또는 `victim == j`
* Sequential할 때 : `flag[j] == false`이므로 진입
* Concurrent할 때 : `victim`이 하나의 값만 가지므로, thread i와 j 중 하나가 진입

## The Filter Lock
* n개의 스레드가 있는 경우
* `level[i]` : thread i가 진입하고자 하는 level
* `victim[L]` : level L에 진입하지 못하는 thread
* `for L = 1 to n-1`
  * 초기 조건 : `level[i] = L` 이고 `victim[L] = i`  
  * 진입 조건 : `∀k != i: (level[k] < L)` 또는 `victim[L] != i`
* **level i랑 `level[i]`의 용법이 다르므로 주의! 전자는 i번째 level을 의미하고 후자는 위에 쓰인 대로 thread i가 진입하고자 하는 level을 나타낸다.**
* 임의의 level L에 진입하고자 하는 스레드 중 적어도 하나는 성공한다. 한편, level L에 진입하고자 하는 스레드가 여럿인 경우 적어도 하나는 level L에 진입하지 못하고 대기한다. 진입하고자 했으므로 `level[i] == L`이 되었으나, `victim[L] == i`가 되어 `while()`을 빠져나가지 못하는 것이다.
* level 0은 CS를 빠져나온 것이고, level n-1은 CS에 들어간 것이다. `lock()`을 실행한 후 level n-1에 진입하게 되면 곧바로 CS를 실행하게 된다. 그리고 CS를 빠져나가면서 `unlock()`을 실행하면 level 0이 된다.
* level L에는 최대 n-L개의 스레드만 진입할 수 있다. 예를 들어 level 1에는 n-1개의 스레드만 진입할 수 있고, level n-1에는 1개의 스레드만 진입할 수 있다. `victim[L]`에 마지막으로 write한 스레드는 level L로 진입할 수 없기 때문이다. **level L에 진입한다는 것은 level L에 해당하는 `while()`을 통과한다는 의미이다.**

### Filter Lock 작동 시나리오
* 가장 쉬운 예로 level 0에서부터 n개 스레드가 달려들면 level 1 ~ level n-1까지 총 n-1개 스레드는 하나씩 victim이 되어 level에 진입하지 못하고 대기하고, 1개 스레드만 level n-1에 진입한다.
* 만약 m (< n)개 스레드가 달려들면 level 1 ~ level m-1까지 총 m-1개 스레드는 위에서부터 하나씩 victim이 되어 대기하고, thread i는 level m-1에서 `level[i] = m-1`, `victim[m-1] = i`을 수행한 후, `∀k != i: (level[k] < m-1)` 진입 조건을 만족해서 (왜냐하면 `k != i: level[k] ≥ m-1`이려면 코드를 수행해야 하므로 level m-1에 thread k가 도달했어야 함) 아래로 쭉쭉 내려가 level n-1에 도달한다. for문을 빠져나가면 `level[i] == n`이 되므로 다른 스레드들은 모두 대기해야 한다. thread i가 CS를 빠져나가 `level[i] = 0`을 수행하면 level m-1에서의 victim이 level m-1에 진입할 수 있게 되고, 아래로 쭉쭉 내려갈 수 있게 된다.

### Filter Lock 추월 시나리오
* 마지막으로 `victim[L]`에 write한 thread는 level L에 진입할 수 없지만, 누군가 `victim[L]`에 write만 해주면 반드시 들어갈 수 있다.
* level L에 thread 1이 진입 시도를 하여 `∀k != i: (level[k] < L)` 조건을 만족시킨 상태에서 잠시 멈추었다고 하자. 이때 thread 2가 들어오면 `level[1] == L`이고 `victim[L] == 2`이므로 대기하지만, thread 3이 들어오면 `victim[L] == 3`이 되므로 thread 2가 level L을 지나 진행할 수 있게 된다. thread 2가 한 바퀴 돌아 다시 level L로 돌아와 `victim[L] = 2`를 수행하면, 그때까지 대기하고 있던 thread 3이 `victim[L] == 2`이므로 진행할 수 있게 된다. (thread 2가 CS를 빠져나가 `level[2] == 0`가 되더라도 `level[1] == L`이 버티고 있으므로 어차피 만족 불가여서 `victim[L]`만 보면 됨) 이렇게 thread 2와 thread 3이 서로를 victim으로 하여 무한히 thread 1을 추월하는 일이 이론적으로 가능하므로, filter lock은 fair하지 않은 lock algorithm이다.

## Bounded Waiting
* `lock()`을 **Doorway interval**(finite step)과 **Waiting interval**(unbounded step)로 나누어보자.
* `for thread A, B` (Bounded waiting)
  * if D<sub>A</sub><sup>k</sup>⇒D<sub>B</sub><sup>j</sup>
  * then CS<sub>A</sub><sup>k</sup>⇒CS<sub>B</sub><sup>j</sup>
  * B가 A를 추월 할 수 없다.
* `for thread A, B` (r-Bounded waiting)
  * if D<sub>A</sub><sup>k</sup>⇒D<sub>B</sub><sup>j</sup>
  * then CS<sub>A</sub><sup>k</sup>⇒CS<sub>B</sub><sup>j+r</sup>
  * B가 A을 r회를 넘어서 추월할 수 없다. (추월 가능한 경우: j, j+1, ..., j+r-1로 총 r회)

## Lamport's Bakery Lock
* Fairness 의 측면을 고려하여 FCFS를 도입
* `flag[i]` : thread i가 진입하고자 함
* `label[i]` : thread i의 label (작을수록 우선순위가 높음)
* 초기 조건 : `flag[i] = true`, `label[i] = MAX(label[0:n-1]) + 1`
* 진입 조건 : `∀k != i: (flag[k] == false) || (label[k], k]) > (label[i], i)` (진입하고자 하는 스레드 중에서 i의 우선순위가 가장 높음)
* `(label[n], n)`은 label을 먼저 비교하고 그 다음 스레드 번호를 비교해서 tie-breaking한다. Overflow의 위험성이 있다.

## Bounded Timestamps
* T<sup>2</sup> : 0 < 1 < 2 < 0
* T<sup>k</sup> = T<sup>2</sup> * T<sup>k-1</sup>
  * T<sup>3</sup> : (00 < 01 < 02 < 00) < (10 < 11 < 12 < 10) < (20 < 21 < 22 < 00) < (00 < 01 < 02 < 00)
  * T<sup>n</sup>는 n-1개의 digit이 있고, n-thread bounded sequential timestamp system의 basis임.
  * T<sup>k</sup>에는 k개의 thread가 배당되는데, 이를 살펴보면 T<sup>2</sup>에서 1개의 스레드와 k-1개의 스레드가 나뉘고, k-1개의 thread가 T<sup>k-1</sup>에 재귀적으로 배당됨.
* Sequential하게 Timestamp를 할당하는 경우라면 항상 overflow 없이 timestamp를 완벽하게 할 수 있음.
* Concurrent하게 Timestamp를 할당하는 경우라면 문제가 생길 수 있음. 예를 들어 T<sup>3</sup>의 경우, Concurrent하게 instruction을 수행하다가 <0x 1y 2z>로 배치되거나 <x0 x1 x2>로 배치되는 경우가 있을 수 있다.
* CAS instruction을 사용해야 concurrent하게 timestamp를 배정할 수 있다. _TODO_

## Lower Bounds on the Number of Locations
* thread가 n개일 때, Deadlock-free Lock algorithm은 최악의 경우 최소 n개의 서로 다른 공유메모리(MRSW Boolean Register)를 할당하고 접근해야한다.
  * 증명은 간단히, 먼저 진입하려는 스레드가 flag에 1을 대입하려는 순간 멈추고, 자기의 공유메모리가 할당되지 않은 스레드가 거기다가 자기가 1을 대입하고 들어가버리고, 먼저 진입하려던 스레드가 살아나면 1을 덮어쓰고 들어가버리므로 MRSW이든 MRMW이든 결국 atomic하지 않은 연산 때문에 문제가 생기게 된다. (그렇다고 n개로 되냐고 하면 그것은 아직 알 수 없음)
* 알려져 있는 것으로 최소는 2n개의 공유메모리를 쓰는 Bakery Lock이 있으므로, 최소해 `x: n ≤ x ≤ 2n`이다.
* 따라서 기본적인 load & store instruction 대신 synchronization operation이 필요하다.

# 3. Concurrent Object

## Sequential and Concurrent Object
* Object = (fields, methods)
  * 예를 들어 FIFO-queue는 fields(`item[n], head, tail`) methods(`enq(), deq()`)
  * Concurrent하면 method끼리 서로 영향을 줄 수 있다.
* Sequential Specification
  * If (precondition) Then (postcondition) 이렇게 자기의 state(fields)의 변경만 잘 살펴보면 된다.
* Method call = Interval (invocation, response)
  * 일종의 interval이므로 method call 사이에는 precedence가 있음 (partial order)
  * Invocation notation) `A q.enq(x)` <- `thread object.method` (이때 method는 명시적)
  * Response notation) `A q: void` <- `thread object:result` (이때 method는 암시적)
  * 이때 result는 return value일 수도 있고, exception일 수도 있다.
* History = Sequence of invocations and responses
  * H = [`A q.enq(3)`, `A q:void`, `A q.enq(5)`, `B p.enq(4)`, `B p:void`, `B q.deq()`, `B q:3`]
  * H|q (q is object) : Object projections
  * H|A (A is thread) : Thread projections
  * Complete subhistory : 반영이 되었는지 알 수 없는 pending invocation을 빼버린 것.
  * Sequential history : method call이 interleave하지 않고 다닥 다닥 붙은 경우. (마지막 pending이나 seq spec 무시는 괜찮음)
  * Well-formed history : 각 thread 별 history(H|A, H|B, ...)가 sequential한 경우.
  * Equivalent history : `∀T: H|T = G|T`를 만족시키면 H랑 G는 equivalent하다.
  * Legal history : 모든 object x에 대하여 H|x가 sequential spec을 만족하는 Sequential history H.

## Quiescent Consistency
* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order.
* **Principle** Method calls separated by a period of quiescence should appear to take effect in their real-time order.
* 어떤 object에 pending된 method call이 하나도 없을 때, 그때까지의 실행 결과와 결과가 동등한 어떤 sequential execution order가 존재함.
* object가 quiescent한 구간 전후로 method 수행 순서가 보장됨.
* 수행 순서가 무관한 concurrent execution에서 유용하다.
* Quiescent Consistency는 _non-blocking_ correctness condition이며 compositinal함.
* 용어 정리
  * total method
    * 모든 object state에 대해 정의되는 method (ex. `enq()` if unbounded)
  * partial method
    * 어떤 object state에 대해 정의되지 않는 method (ex. `deq()` because of EMPTY\_QUEUE\_EXCEPTION)
  * nonblocking
    * total method에 대한 임의의 pending call은 언제나 완료될 수 있음
  * compositinal
    * 시스템 내의 각 object가 P를 만족할 때, 시스템 전체도 P를 만족하면 P는 compositinal한 correctness condition

## Sequential Consistency
* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order.
* **Principle** Method calls should appear to take effect in program order.
* (1) 각 thread 내에서의 program order를 보존하고 (2) object의 sequential specification을 만족하는 어떤 sequential execution order가 존재함.
* Quiescent Consistency와 Sequential Consistency는 incomparable (포함 관계 x)
  * Quiescent Consistency는 quiescent한 구간 전후로만 수행 순서가 보장되므로 program order를 보존하지 않음
  * ex) \[`A.enq(x)` call\] \[`B.enq(y)` call\] \[`A.enq(x)` return\] \[`A.enq(z)` call\] \[`B.enq(y)` return\] \[`A.enq(z)` return\] \[`A.deq(z)`\]
    * sequential consistent = `false`, quiescent consistent = `true`
  * Sequential Consistency는 real-time order와 무관하므로 object가 quiescent한 구간 전후로 서로 다른 thread의 method 수행 순서가 보장되지 않음
  * ex) \[`A.enq(x)`\] \[`B.enq(y)` call\] \[`A.deq(y)` call\] \[`B.enq(y)` return\] \[`A.deq(y)` return\]
    * sequential consistent = `true`, quiescent consistent = `false`
* 현대(?)의 멀티프로세서에서 read & write는 sequential consistency하지 않음.
* Sequential Consistency는 nonblocking correctness condition이며 compositinal하지 않음
  * ex) \[`A.p.enq(x)`\] \[`B.q.enq(y)`\] \[`A.q.enq(x)`\] \[`B.p.enq(y)`\] \[`A.p.deq(y)`\] \[`B.q.deq(x)`\]
    * p에 대해서는 `true`, q에 대해서도 `true`이지만 전체에 대해서는 `false`

## Linearizability
* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order
* **Principle** Each method call should appear to take effect instantaneously at some moment between its invocation and responce
* 서로 겹치지 않는 concurrent method call은 그 순서로 실행되고(program order는 저절로 보존), 서로 겹치는 concurrent method call은 object의 sequential specification을 만족하도록 하는 순서로 실행되어, 전체 History가 그와 동등한 어떤 sequential execution order가 있어야 함
* 모든 liniearizable execution은 sequentially consistent함 (역은 성립하지 않음) -> side effect moment를 기준으로 sequential order를 매기면 됨
* method가 side effect를 내는 linearization point를 찾아야 함 (object의 state가 바뀌는 순간)
  * Lock 있음: critical section
  * Lock 없음: a single step
* Linearizability는 nonblocking correctness condition이며 compositinal함

## Progress Conditions
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

# 4. Foundations of Shared Memory

## Read-Write Register
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
  * timestamp 받는 부분이 write order의 linearization point, max stamp 읽는 부분이 read order의 linearization point
    * 특정 code line이 linearization point가 아니고 실행에 따라 다름
* Atomic Snapshot
  * **update**(한 array element에 write) & **scan**(모든 array element를 read)
  * _clean double collect_: linearizable하지 않음 & **scan**이 wait-free가 아니어서 starvation을 겪을 수 있음
  * wait-free snapshot: **update**에서도 **scan**을 함

# 5. The Relative Power of Primitive Synchronization Operations

## Consensus Number
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

## Consensus protocol
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

## Read-Modify-Write Operation
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
    * running thread는 상대 thread가 sleep하더라도 `decide()`할 수 있어야 함
  * 그러나 어떤 `rmw(...)`는 thread가 셋 이상일 때 누가 이겼는지 알 수 없음 (자기가 이겼는지 여부와 누가 이겼는지 여부를 다 알아야함)
* Commute & Overwrite RMW object는 consensus number가 2임 ("weak" RMW instruction)
  * commute: f(g(v)) = g(f(v))
  * overwrite: f(g(v)) = f(v)
  * n > 2일 때, 누가 이겼는지 알 수 없다는 것을 binary tree를 이용해서 증명 (thread A, B, C)
* `CAS(e, u)`는 consensus number가 ∞ (자기가 이겼는지를 return 값으로, 누가 이겼는지를 register 값으로 판단)
* FIFO Queue, n-assignment 등은 consensus number가 충분한 RMW instruction (혹은 RMW register)를 이용하여 구현해야 함

# 6. Universality of Consensus

# 7. Spin Locks and Contentions

# 8. Monitors and Blocking Synchronization

# 9. Linked Lists: The Role of Locking

# 10. Concurrent Queues and the ABA Problem

# 11. Concurrent Stacks and Elimination

# 12. Counting, Sorting, and Distributed Coordination

# 13. Concurrent Hashing and Natural Parallelism

# 14. Skiplists and Balanced Search

# 15. Priority Queues

# 16. Futures, Scheduling, and Work Distribution

# 17. Barriers

# 18. Transactional Memory
