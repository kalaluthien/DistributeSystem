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
* 만약 m (< n)개 스레드가 달려들면 level 1 ~ level m-1까지 총 m-1개 스레드는 위에서부터 하나씩 victim이 되어 대기하고, thread i는 level m-1에서 `level[i] = m-1`, `victim[m-1] = i`을 수행한 후, `∀k != i: (level[k] < m-1)` 진입 조건을 만족해서 (_왜냐하면 `k != i: level[k] ≥ m-1`이려면 코드를 수행해야 하므로 level m-1에 thread k가 도달했어야 함_) 아래로 쭉쭉 내려가 level n-1에 도달한다. for문을 빠져나가면 `level[i] == n-1`이 되므로 다른 스레드들은 모두 대기해야 한다. thread i가 CS를 빠져나가 `level[i] = 0`을 수행하면 level m-1에서의 victim이 level m-1에 진입할 수 있게 되고, 아래로 쭉쭉 내려갈 수 있게 된다.

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
* `(label[n], n)`은 label을 먼저 비교하고 그 다음 스레드 번호를 비교해서 tie-breaking한다.
* `label[n]`에서 Overflow 위험성이 있다.

## Bounded Timestamps
* T<sup>2</sup> : 0 < 1 < 2 < 0
* T<sup>k</sup> = T<sup>2</sup> * T<sup>k-1</sup>
  * T<sup>3</sup> : (00 < 01 < 02 < 00) < (10 < 11 < 12 < 10) < (20 < 21 < 22 < 00) < (00 < 01 < 02 < 00)
  * T<sup>n</sup>는 n-1개의 digit이 있고, n-thread bounded sequential timestamp system의 basis임.
  * T<sup>k</sup>에는 k개의 thread가 배당되는데, 이를 살펴보면 T<sup>2</sup>에서 1개의 스레드와 k-1개의 스레드가 나뉘고, k-1개의 thread가 T<sup>k-1</sup>에 재귀적으로 배당됨.
* Sequential하게 Timestamp를 할당하는 경우라면 항상 overflow 없이 timestamp를 완벽하게 할 수 있음.
* Concurrent하게 Timestamp를 할당하는 경우라면 문제가 생길 수 있음. 예를 들어 T<sup>3</sup>의 경우, Concurrent하게 instruction을 수행하다가 {0x, 1y, 2z}로 배치되거나 {x0, x1, x2}로 배치되는 경우가 있을 수 있다.
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
  * Total method
    * 모든 object state에 대해 정의되는 method. (ex. `q.enq()` if `q` is unbounded)
  * Partial method
    * 어떤 object state에 대해 정의되지 않는 method. (ex. `q.deq()` because of EMPTY\_QUEUE\_EXCEPTION)
  * Nonblocking
    * total method에 대한 임의의 pending call은 언제나 완료될 수 있다는 의미.

## History
* History는 Sequence of invocations and responses이다.
  * H = [`A q.enq(3)`, `A q:void`, `A q.enq(5)`, `B p.enq(4)`, `B p:void`, `B q.deq()`, `B q:3`]
* Projection은 History 중에서 일부만 따로 떼낸 것.
  * H|q (q is object) : Object projections
  * H|A (A is thread) : Thread projections
* Complete subhistory : 반영이 되었는지 알 수 없는 pending invocation을 빼버리는 것.
* Sequential history : 서로 다른 thread나 object에 대한 method call들이 서로 겹치지 아니하고 다닥 다닥 붙은 경우. (마지막 pending이나 seq spec 무시는 괜찮음) Interleaving/Overlapping이 없음.
  * H = [`A q.enq(3)`, `A q:void`, `B p.enq(4)`, `B p:void`, `B q.deq()`, `B q:3`, `A q.enq(5)`]
  * `A q.enq` match, `B p.enq` match, `B q.deq` match, `A q.enq` pending (OK)
* Well-formed history
  * thread 별 history(H|A, H|B, ...)가 sequential한 경우. 우리가 이야기하는 History들은 전부 Well-formed여야 한다. Well-formed가 아닌 history는 method가 끝나기 전에 다른 method가 끝난다거나, method가 불리기 전에 반환부터 한다거나 하는 뒤틀린 황천의 history이다.
* Equivalent history (history의 동등성)
  * `∀T: H|T = G|T`를 만족시키면 H랑 G는 동등하다. Interleaving/Overlapping을 어떻게 하건 간에 각각의 thread들의 입장에서 보면 동등하니까. 결과는 history의 entry들이 같다는 것만 확인해도 되는데 어차피 history에 input과 output이 다 들어있으니까.
* Legal (sequential) history
  * 모든 object x에 대하여 H|x가 sequential spec을 만족하는 Sequential history H. 다시 말해 Sequential하게 method call들을 배치하는 거야 그냥 막 가져다 붙이면 되지만 그게 아니고 정말로 프로그램을 잘 수행해서 말이 되는 결과가 나오는 history.
  * Legal history ⇒
    * `A p.enq(1)`
    * `A p:void`
    * `A p.enq(2)`
    * `A p:void`
    * `B p.deq()`
    * `B p:1`
  * Illegal history ⇒
    * `A p.enq(1)`
    * `A p:void`
    * `A p.enq(2)`
    * `A p:void`
    * `B p.deq()`
    * `B p:2`
  * 왜 Sequential을 붙이냐면 어차피 legal한지 판단하는 기준이 sequential이니까... (다시 말해 concurrent가 valid한지를 legal sequential history와의 equivalent로 이야기할 것이기 때문)
* History가 동등(Equivalent)한 것은 thread에 대해 검사하고, history가 부합(Legal)하는 것은 object에 대해 검사한다.

## Quiescent Consistency (QC)
* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order.
* **Principle** Method calls separated by a period of quiescence should appear to take effect in their real-time order.
* 어떤 object x에 pending된(inv-res 쌍을 이루지 못한) method call이 하나도 없을 때, 그때까지의 history와 동등한 어떤 sequential execution order (history)가 존재한다. 즉, 모든 x에 대하여 H|x와 동등한 S|x가 존재한다.
* object가 quiescent한 구간(경계) 전후로 method 수행 순서가 보장된다. quiescent하지 않은 동안의 method 수행 순서는 program order조차 보장되지 않는다.
* 수행 순서가 무관한 concurrent execution에서 유용하다. (OOO)
* Quiescent Consistency는 nonblocking and compositinal.
  * nonblocking: total method에 대한 임의의 pending call은 언제나 완료될 수 있다는 의미이다.
  * compositinal: 시스템 내의 각 object가 P를 만족할 때, 시스템 전체도 P를 만족하면 P는 compositinal한 correctness condition이라고 한다. Consistency에 대해 적용해보면 대충 `∀x:P(H|x) ⇒ P(H)`인 P라고 하면 될 것 같다. 다시 말해, 각 object들이 QC를 만족시키면 전체 history (system)도 QC를 만족시킨다.

## Sequential Consistency (SC)
* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order.
* **Principle** Method calls should appear to take effect in program order.
* 원본 history H의 pending method call을 적당히 완료시킨 extended history G가 있어서, G에서의 partial order는 딱히 신경쓰지 않는데 G와 동등한 legal sequential history S가 존재한다. (G는 H의 Complete Subhistory) 그러면 서로 다른 thread 사이의 method call은 order를 보장받지 않는다. Program order는 보장하니 같은 thread 사이의 method call은 order를 보장받는다. 즉, method call이 한 thread 내에서 순서를 바꾸지만 않는다면 다른 thread의 method call과의 order를 자유롭게 옮길 수 있다.
* 여기에 overlap되지 않은 경우의 order까지 보장하게 하면 Linearizable execution이 된다.
* Quiescent Consistency와 Sequential Consistency는 incomparable하다. (포함 관계 x)
  * Quiescent Consistency는 quiescent한 구간 전후로만 수행 순서가 보장되므로 program order를 보존할 필요가 없다. 여기서는 `A p.enq(x)`보다 `A p.enq(z)`가 먼저 실행되고 말았다.
  * ex) SC = `false`, QC = `true`
    * `A p.enq(x)`
    * `B p.enq(y)`
    * `A p:void`
    * `A p.enq(z)`
    * `B p:void`
    * `A p:void`
    * `A p.deq()`
    * `A p:z`
  * Sequential Consistency는 real-time order와 무관하므로 object가 quiescent한 구간 전후로 서로 다른 thread의 method 수행 순서가 보장되지 않는다. 여기서는 `A p.enq(x)`보다 `B p.enq(y)`가 먼저 실행되고 말았다.
  * ex) SC = `true`, QC = `false`
    * `A p.enq(x)`
    * `A p:void`
    * `B p.enq(y)`
    * `A p.deq()`
    * `B p:void`
    * `A p:y`
* 현대(?)의 멀티프로세서에서 read & write는 sequential consistency하지 않음. (_cf. relaxed consistency_)
* Sequential Consistency는 _nonblocking_ correctness condition이며 compositinal하지 않다. 각 object에 대해 SC가 성립하더라도, 전체 execution은 SC가 성립하지 않을 수 있다.
  * ex) p에 대해서는 `true`, q에 대해서도 `true`이지만 전체에 대해서는 `false`
    * `A p.enq(x)`
    * `A p:void`
    * `B q.enq(y)`
    * `B q:void`
    * `A q.enq(x)`
    * `A q:void`
    * `B p.enq(y)`
    * `B p:void`
    * `A p.deq()`
    * `A p:y`
    * `B q.deq()`
    * `B q:x`

## Linearizability
* **Principle** Method calls should appear to happen in a one-at-a-time, sequential order.
* **Principle** Each method call should appear to take effect instantaneously at some moment between its invocation and responce.
* 원본 history H의 pending method call을 적당히 완료시킨 extended history G가 있어서, G에서의 partial order를 모두 포함하면서 G와 동등한 legal sequential history S가 존재한다. (G는 H의 Complete Subhistory) 그러면 S가 G에서의 (서로 다른 thread에서의) partial order를 포함하므로 real-time order를 보존하게 된다. S는 sequential이니까 order가 더 많으며, 그것들은 G에서 overlap된 method call들에 legal하도록 순서를 부여한 것이다. Program order는 당연히 보존한다.
* 모든 liniearizable execution은 sequentially consistent하다. (역은 성립하지 않음) -> side effect moment를 기준으로 sequential order를 매기면 된다. 그 moment가 instant하다는 가정이다.
* method가 side effect를 내는 linearization point를 찾아야 한다. (object의 state가 바뀌는 한 순간)
  * 항상 같은 line은 아닐 수도 있다.
  * Lock 있음: critical section (하여튼 atomic하면 되니까)
  * Lock 없음: a single step
  * 보통은 method가 여러 군데 건드릴테니 그런 건 없다.
* Linearizability is nonblocking and compositinal.

## Progress Conditions
* Blocking
  * 한 thread의 unexpected delay가 다른 thread들이 progress하지 못하게 할 수 있으면 blocking implementation이다.
  * ex) Mutual-exclusion
* Wait-free
  * (무한한 실행 동안) 모든 method call이 유한한 step 후에 종료되는것이 보장되는 경우를 말한다.
  * 모든 method가 wait-free인 object를 wait-free, 모든 instance가 wait-free인 class를 wait-free라고 한다. (참고)
  * Wait-free이면 mutual exclusion이 없다.
* Bounded wait-free
  * method call의 step에 pre-determined bound가 있는 경우를 말한다.
  * Bounded wait-free -> (unbounded) Wait-free이므로 bounded wait-free가 더 강력한 조건임을 알 수 있다.
  * bound가 thread의 수에 의존하는 경우도 있는데 이것도 bounded이다. 어쨋거나 무한하지 않으니까.
* Lock-free
  * (무한한 실행 동안) 어떤 method call이 유한한 step 후에 종료되는 것이 항상 보장되는 경우를 말한다.
  * Wait-free -> Lock-free 이므로 Wait-free가 더 강력한 조건임을 알 수 있다.
  * 만약 실행이 유한하다면 Wait-free = Lock-free (항상 어떤 method call이 유한한 step 후에 종료해야 하므로)
  * 일부 thread는 starvation을 겪을 수 있으나, 실제로는 거의 일어나지 않는다면 wait-free보다 실용적일 수 있다.
  * Lock-free이면 lock이 없다.
* Lock-free : Wait-free = Deadlock-free : Starvation-free
  * Lock-free & Deadlock-free는 "all threads"의 progress를 보장한다.
  * Wait-free & Starvation-free는 "some threads"의 process을 보장한다.

# 4. Foundations of Shared Memory

## Read-Write Register
* SRSW ~ MRSW ~ MRMW
* Boolean ~ M-valued
  * M-valued는 M개의 값을 나타낸다는 의미이다. (말하자면 Boolean은 0, 1 2개의 값을 나타내므로 2-valued)
  * M-valued는 log(M)-bit라고도 할 수 있긴 한데, 뒤에서 M-valued를 construct할 때는 M-bit를 써서 One-Hot encoding 한다.
* Safe ~ Regular ~ Atomic
  * Safe의 조건
    * Read와 Write가 Sequential한 경우, 우리 생각대로 되어야 함.
    * Read와 Write가 Concurrent한 경우, 무엇을 읽던 간에 valid한 값(High-impedence 이런 거 안 됨)을 읽어야 한다. 예를 들어 1010에 0111을 쓰고 있는 동안 동시에 읽으면 1010이든 0111이든 0101이든 0000이든 다 상관 없고 0이랑 1로 되어있으면 됨.
    * real-time order도 상관 없다. Read-Write-Read가 다 겹쳤을 때, 앞에서 update가 된 값을 잘 읽었더라도 뒤에서는 틀려도 된다.
  * Regular의 조건
    * Safe의 아무 Valid → 항상 Old || New (역시 real-time order는 고려하지 않음)
    * R<sup>i</sup> → W<sup>i</sup> 인 경우는 존재하지 않음 (겹치지 않는 미래에 쓰일 값을 읽는 경우)
    * W<sup>i</sup> → W<sup>j</sup> → R<sup>i</sup> 인 경우는 존재하지 않음 (겹치지 않는 과거에 이미 덮어쓰인 값을 읽는 경우)
  * Atomic의 조건
    * 위의 조건과 더불어, R<sup>i</sup> -> R<sup>j</sup>일 때 i > j 인 경우는 존재하지 않음 (real-time order를 고려함)
    * 따라서 Atomic해지면 Read Write로 이루어진 history가 linearizable해진다.
* Weakest Register: SRSW Safe Boolean
  * SRSW로부터 다른 모든 레지스터와 mutual exclusion을 구현하려는 것이 챕터의 목표이다.
  * 하지만 Consensus hierarchy를 뛰어넘을 수는 없다. (concensus number = 1)
* SRSW Safe Boolean → MRSW Safe Boolean
  * N개의 reader가 있을 때, 각 reader마다 SRSW를 하나씩 할당하면 MRSW가 된다.
  * writer는 모든 register에 임의의 순서로 write를 하고, reader는 자신의 register를 읽는다.
* MRSW Safe Boolean → MRSW Regular Boolean
  * 할당되어 있는 값과 바꿀 값이 동일하면 write하지 않는다.
  * 그러려면 writer가 local variable로 `old_value`를 들고 있으면 된다. 어차피 이거는 shared memory가 아니니까 아무 거나 상관 없다.
* MRSW Regular Boolean → MRSW Regular M-valued
  * MRSW Regular Boolean register를 M개 가지고 One-Hot encoding을 한다.
  * `read()` 수행시 reader는 0 to M-1으로 순회하면서 최초로 1이 되는 지점을 찾는다.
  * `write(x)` 수행시 writer는 x to 0로 순회하면서 x에는 1을, x 이하에는 0을 대입한다.
  * reader는 항상 old를 읽거나 new를 읽게 되므로 Regular하다.
* MRSW Regular → SRSW Atomic
  * Regular register는 Old || New를 읽어오고, overlap되지 않은 method call 사이의 순서를 보장한다. 그러나 overlap된 method call 사이의 real-time order가 보장되지 않는다.
    * `r`의 초기값은 1이다.
    * `A r.read()`
    * `B r.write(2)`
    * `A r:2`
    * `A r.read()`
    * `B r:void`
    * `A r:1`
    * H|A를 보면 `read(2)` → `read(1)`인데 이것이 모순이다.
  * writer는 `value:stamp`를 write하고, reader는 마지막으로 읽은 `value:stamp`를 local memory에 saved로 저장하여 `saved(value:stamp)`와 `read(value:stamp)` 중에서 stamp가 최신인 것을 read한다. 그리고 마지막으로 읽은 값을 saved에 update한다. 이렇게 real-time order를 보장한다.
  * 따라서 SRSW Atomic 만드는데 MRSW Regular M-valued + timestamp + reader's copy가 필요하다.
* SRSW Atomic → MRSW Atomic
  * SRSW Safe → MRSW Safe는 N개면 되는데, Atomic은 그것만으로는 linearizable하지 못해서 못쓴다. 왜냐면 Safe는 real-time order와 상관 없어서 그냥 reader 별로 1개씩 배당하고 writer가 거기 아무 순서로 써주고 reader는 아무 순서로 가져가도 "하여간에 writer가 값을 쓰는 중"이었으면 아무래도 상관이 없는데, Atomic register는 배당된 register를 reader가 읽을 때 다른 reader가 읽은 값(의 timestamp)에 따라 내 값(의 timestamp)이 그 이후의 것이 맞는지 체크할 수 있어야 real-time order를 보장한다.
  * SRSW를 N<sup>2</sup>만큼 할당한다. (thread[1] = writer, thread[2:N] = reader)
  * **writer**: column 1에 순서대로 write한다. _또는, 대각선으로 (1,1), (2,2), ..., (N, N)의 위치에다 write해도 된다._
  * **reader**: thread i는 row i의 최신값을 read하여 그 값을 column i에 순서대로 write한 후에 return한다.
  * 이렇게 하면 다른 reader가 읽은 값을 놓치는 경우는 두 read가 직접 겹쳤을 때 뿐이므로 이럴 때는 문제가 없다. 문제가 되었던 건 두 read가 직접 겹치지는 않는데 (즉, order가 있는데) 둘 다 writer랑 겹치는 경우이다.
* MRSW Atomic → MRMW Atomic
  * Bakery algorithm과 거의 동일하게, MRSW를 writer 수인 N만큼 할당한다. (`label[n]`처럼)
  * **writer**: array를 다 읽고 다음 timestamp를 뽑아서 자기 자리에 write한다.
  * **reader**: array를 다 읽고 timestamp가 최신인 것을 read한다.
  * next(timestamp)가 write order의 linearization point, max(timestamp)가 read order의 linearization point이다.
    * 특정 code line이 linearization point가 아니고 실행에 따라 다르다는 것에 주의한다.
* SRMW Atomic → Atomic Snapshot
  * SRMW/MRMW Atomic의 array를 이용하여, snapshot = multiple read를 수행하는 것을 말한다.
    * Multi reader != Multiple read
  * **update()**(자신의 thread id에 따라 지정된 array element에 write) & **scan()**(모든 array element를 read)
  * 쉽게 말해 multiple read에서 각 read를 동시에(하는 것처럼) 한다. multiple assignment는 consensus랑 연관되어 불가능하다.
  * 해결책: clean double collect
    * read하는 thread는 **scan()** 2번 해서 그 두 값이 동일하면 그것을 snapshot이라 하고, 같지 않으면 재시도한다. 이때, 값에다가 label이나 timestamp를 달아야 한다.
    * 문제점은 **scan()** = !wait-free이다. 따라서 starvation을 겪을 수 있다. 계속 누군가 **update()** 하고 있으면 clean double collect에 계속해서 실패할 수 있다.
  * 해결책: wait-free snapshot
    * **update()** 에서도 **scan()** 한다.

# 5. The Relative Power of Primitive Synchronization Operations

## Consensus Number
* object가 consensus 문제를 해결할 수 있는 최대 thread 수
* consensus number < N인 object를 이용해서 consensus number = N인 wait-free(or lock-free)한 object를 만들 수 없다. X로 Y를 구현했다면, n(X) ≥ n(Y)라는 의미이다.
* concurrent consensus object
  * `thread object.decide(input)`의 `output`은 다음 조건을 만족함
  * _consistent_ : 모든 thread가 같은 값을 decide
  * _valid_ : decide된 값은 어떤 thread의 input
  * (위의 조건에 의하여) _Lock-free_ : 살아 있는 thread는 죽은 thread를 기다리지 않는다. 이후의 경쟁에서 계속 질 수는 있다.
  * `decide()`에 의해 처음으로 선택된 thread를 기준으로 해서 sequential consensus object으로 linearizable한다.
  * decide 과정 중에 누가 죽어도 반드시 끝나야 한다. 그러니까 결과를 만들어 놓고, 대결을 해야 내가 죽더라도 남이 결과를 잘 들고간다.
* consensus number 비교
  * Atomic register의 CN = 1이다.
  * Multi-dequeuer FIFO-queue의 CN = 수용 가능한 dequeuer의 수이다. (맨 처음 `deq()`한 thread의 값으로 decide)
  * n-Assignment Object의 CN = (2n-2)이다. 거꾸로, CN = k이려면 (k+1)/2+1 assignment가 가능해야 한다.
    * 토너먼트 방식으로 컨센서스를 진행하면 된다. 처음에는 2개씩 2-assignment를 하고, 그 다음에는 4개씩 3-assignment를 하고, ...

## Consensus protocol
* Decide를 위한 protocol의 state transition을 binary tree 형태로 나타낼 수 있다. (edge: move, node: state)
  * **bivalent** 0과 1 중 어느 값이든 가능하다.
    * subtree의 leaf node의 값에 0도 있고 1도 있다는 의미이다.
    * **critical state** 다음 move에 따라 0-valent 또는 1-valent로 나뉘는 bivalent.
    * protocol이 wait-free이면 언젠가는 critical state에 도달해야 한다.
  * **univalent** 한 가지 값만 가능하다. (그 값이 무엇인지는 모를 수 있음)
    * **0-valent** 0만 가능하다.
    * **1-valent** 1만 가능하다.
  * protocol의 모순으로부터 Atomic register로는 (n > 1) consensus 문제를 풀 수 없음을 증명하면 된다.
    * Critical state로부터 시작한다고 가정한다.
    * `x.read()`, `y.read()`, `x.write()`, `y.write()` 이 4가지의 operation에 대해 A와 B가 interaction하므로, 4 x 4 테이블을 작성해보면 된다. 즉, 다음의 3가지 경우에 대해 테스트해보면 된다.
    * A나 B가 같거나 다른 register에 read할 때 (read만 하거나 안 하거나 했는데 결과가 달라짐) → ⊥
    * A와 B가 서로 다른 register에 write할 때 (A와 B가 다른데 썼는데 AB 순서에 따라 결과가 달라짐) → ⊥
    * A와 B가 서로 같은 register에 write할 때 (A가 B를 덮어썼다고 하면 그냥 A만 쓴 것인데 결과가 달라짐) → ⊥

## Read-Modify-Write (RMW) Operation
* register의 값이 x라면, 이를 f(x)로 갱신(Write)하고 x(old)를 반환(Read)하는 operation을 의미한다.
* `identity(x)` : f(x) = x
* `get-and-set(v)` : f(x) = v
  * `test-and-set()` = `get-and-set(true)`
* `get-and-increment()` : f(x) = x + 1
* `fetch-and-add(k)` : f(x) = x + k
* `compare-and-set(e, u)` : f(x) = if (x == e) then (x := u, true) else (false)
  * `CAS(e, u)`는 특별히 x(기존 값)를 반환하지 않고 교체 여부(bool)를 반환한다.
* nontrivial한 RMW register는 consensus number가 2 이상이다. (Atomic register로는 불가능하므로 hardware에서 RMW method를 지원해야 함)
  * `if (rmw(..) == init)`로 비교하여 i와 1-i 중 `decide()`할 수 있다.
    * running thread는 상대 thread가 죽거나 멈추더라도 `decide()`할 수 있어야 한다.
  * 그러나 어떤 `rmw(...)`는 thread가 셋 이상일 때 누가 이겼는지 알 수 없다. 자기가 이겼는지 여부와 누가 이겼는지 여부를 다 알아야하는데, 둘이면 내가 졌으면 남이 이긴건데 셋부터는 그런 거 없다.
* Commute & Overwrite RMW object는 CN = 2이다. ("weak" RMW instruction)
  * commute: f(g(v)) = g(f(v))
  * overwrite: f(g(v)) = f(v)
  * n > 2일 때, 누가 이겼는지 알 수 없다는 것을 binary tree를 이용해서 증명하면 된다.
    * thread A, B, C가 있을 때, A와 B가 다른 순서로 연산을 적용한 뒤 (그러면 상태가 동일) C가 계속 돌면 최종 결과가 같아야 하는데, Critical state에서는 다른 결과가 나오므로 모순이다.
    * thread A, B, C가 있을 때, A만 연산을 적용하거나 B→A 순으로 연산을 적용한 뒤 (그러면 상태가 동일) C가 계속 돌면 최종 결과가 같아야 하는데, Critical state에서는 다른 결과가 나오므로 모순이다.
* `CAS(e, u)`는 CN = ∞이다. (자기가 이겼는지를 return 값으로, 누가 이겼는지를 register 값으로 판단)
* FIFO Queue, n-assignment 등은 consensus number가 충분한 RMW instruction (혹은 RMW register)를 이용하여 구현한다.

# 6. Universality of Consensus

## Universality
* Consensus는 Universal하다.
* n-thread consensus protocol이 있다면, 항상 이를 이용하여 어떤 Sequential object에 대해 Wait-free, Linearizable, n-threaded version을 만들 수 있다.
  * Sequential object는 method `Apply: Invocation → Response`를 가지고 있다.
* Naive Idea
  * 일단 object가 현재 cell에 대한 포인터를 들고 있도록 한다.
  * thread는 그 포인터가 가리키는 cell을 복사해서 연산을 수행한 뒤, object의 포인터를 결과물로 바꾼다.
* Lock-free universal construction
  * head 배열이 있고, thread i는 head[i]를 업데이트한다.
  * head[i]는 thread들이 날린 query의 list node들을 가리키는데, max(head)를 택하면 node 중 가장 seq 넘버가 높은 것을 가리키게 된다.
  * head가 하나만 있으면 thread들이 같은 곳을 update하려고 하므로 거기서 consensus를 해야 하는데, 애초에 list를 쓰는 이유가 consensus object가 일회용이기 때문. 따라서 head는 경쟁이 없어야 한다.
  * 각각의 node에는 query하고 consensus object가 들어있다. 새로운 method call이 들어오면 그걸 object에 적용하는 게 아니라 contension하는 thread끼리 decide해서 승자의 것을 list에다 매단다. 그 다음, tail부터 쭉쭉 list의 query들을 apply한 결과를 만들어서 승자에게 반환한다.
  * 증간에 매단 thread가 죽어도 매달린 node는 남아 있으므로 다음 thread는 올바른 결과를 가져갈 수 있어서 상관없다.
* Wait-free universal construction
  * Lock-free construction + announce array (실패한 query를 넣어놓음)
  * announce array를 이용하여, 내가 실패해도 재시도하지 않고 남이 해준다.
* Shared memory computability는 Turing computability처럼 가능성에만 신경을 쓴다.
  * Wait-free/Lock-free computable = threads with n-consensus objects

# 7. Spin Locks and Contentions
* TAS lock
  * TestAndSet을 계속하여 수행하여, 0을 1로 바꾼 thread만 critical section으로 진입한다.
  * TestAndSet은 1을 1로 바꾼다 하더라도 update이기 때문에 cache miss를 유도한다.
```
lock():
  while (TAS(s, 1) == 1) {}

unlock():
  s = 0
```
* TTAS lock
  * TestAndSet을 시도하기 전에, cache에 올려두고 계속 그걸 확인하도록 해서 overhead를 줄인다.
  * Cache를 누군가 invalidate 시켜주면 일부 thread는 s를 새로 읽어와 0을 보고 TAS를 시도한다.
```
lock():
  do {
    while (s) {}
  } whlie(TAS(s, 1) == 1)

unlock():
  s = 0
```
* L1 and L2 caches
  * L1은 1~2 cycle이지만, L2는 10~ cycle이다.
  * Write-back cache를 사용하면 항상 flush 하지 않고, cache에 변경 사항을 누적시키다가 evict되면 memory에 write back한다.
  * 그러면 다른 프로세서의 cache line을 invalidate해야 하므로 Bus-based architecture에 적절한 Cache coherence protocal을 사용한다.
* Exponential backoff lock
  * TAS는 항상 바쁘고, TTAS는 일부 시점에 몰린다.
  * backoff를 주는 것만으로 경쟁을 줄일 수 있고, 구현이 간단하다.
  * 근데 상수에 따라 성능이 달라지는 등 portability가 좋지 않다.
* Anderson queue lock
* CLH queue lock
* MCS queue lock

# 8. Monitors and Blocking Synchronization
_TODO_

# 9. Linked Lists: The Role of Locking
_TODO_

# 10. Concurrent Queues and the ABA Problem
_TODO_

# 11. Concurrent Stacks and Elimination
_TODO_

# 12. Counting, Sorting, and Distributed Coordination
_TODO_

# 13. Concurrent Hashing and Natural Parallelism

# 14. Skiplists and Balanced Search

# 15. Priority Queues

# 16. Futures, Scheduling, and Work Distribution

# 17. Barriers

# 18. Transactional Memory
