# 분산시스템 중간고사
![Yoem](http://eng.snu.ac.kr/sites/default/files/professor/20100122135859.jpg)

## 기출문제
* Test-and-set로 mutual exclusion 을 해결할 수 있지만 n-thread consensus는 해결할 수 없다. 차이점과 해결할 수 없는 이유를 설명하시오.
* Wait-free와 Lock-free의 차이점을 설명하시오.
* Bounded timestamping system (**T(k) = T(2) * T(k-1)**)이 concurrent한 환경에서 제대로 동작 안 하는 경우가 있다. Compare-and-set을 이용하여 **T(3)** 에서 thread들이 concurrent하게 동작할 수 잇도록 timestamp를 만드는 방법을 제시하시오. (Wait-free하도록)
* Linearizability와 Sequential consistency중 누가 더 강력한 조건인지 설명하고, 둘 중 하나는 만족하지만 다른 하나는 만족하지 못하는 예시를 드시오.
* MCS Queue Lock와 CHL Queue Lock의 차이점을 말하고 어떤 환경에서 더 적합한지 설명하시오.

## 시험공부
### Mutual Exclusion

좋은 Lock algorithm의 조건

* Mutual Exclusion
  * 서로 다른 스레드의 CS가 서로 겹치지 않는다.
* Deadlock-free
  * Lock을 얻으려하는 적어도 하나의 스레드는 언젠가 Lock을 얻게 된다.
* Starvation-free (= _lockout freedom_)
  * Lock을 얻으려하는 모든 스레드는 언젠가 Lock을 얻게 된다.
  * Starvation-free이면 Deadlock-free이다.

The Peterson Lock

* flag[i] : thread i가 진입하고자 함
* victim : thread victim은 진입할 수 없음
* 초기 조건 : flag[i] = true, victim = i
* 진입 조건 : flag[j] == faise || victim == j
* Concurrent하지 않을 때 : flag[j] == false이므로 진입
* Concurrent할 때: victim이 하나의 값만 가지므로, thread i와 j 중 하나만 진입

The Filter Lock

* level[i] : thread i가 진입하고자 하는 최대 level
* victim[j] : level j에 진입하지 못하는 thread
* for j = 1 to n - 1
  * 초기 조건 : level[i] = j, victim[j] = i
  * 진입 조건 : for all k != i (level[k] < i || victim[i] != me)
* level 0에 thread N개 이하, level j에 thread N - j개 이하, level n - 1에 thread 1개 이하 (CS)
* Concurrent할 때, 마지막으로 victim[j]에 write한 thread는level j 아래로 내려갈 수 없음

The Bakery Lock

* flag[i] : thread i가 진입하고자 함
* label[i] : thread i의 label (작을 수록 먼저)
* 초기 조건 : flag[i] = true, label[i] = MAX(label[0 : n-1]) + 1
* 진입 조건 : for all k != i (flag[k] == false || (label[k], k]) >> (lable[i], i))
* (label[i], i)를 비교하면 label이 겹치더라도 어느 thread가 진입할 것인지 알 수 있음
* Overflow의 위험성이 있다.

Bounded Timestamps

* T<sup>2</sup> : 0 < 1 < 2 < 0
* T<sup>k</sup> = T<sup>2</sup> * T<sup>k-1</sup>
  * T<sup>n</sup>는 n-1개의 digit이 있고, n-thread bounded sequential timestamping system의 basis
  * T<sup>3</sup> : 00 < 01 < 02 < 10 < 11 < 12 < 20 < 21 < 22 < 00
* 낮은 자리를 먼저 증가시키되, 해당 subgraph가 가득 차게 되면 높은 자리에서 다시 수행

Lower Bounds on the Number of Locations

* Deadlock-free Lock algorithm은 최악의 경우 최소 n개의 서로 다른 메모리를 할당하고 접근해야함
* 따라서 기본적인 load store instruction  대신 synchronization operation이 필요함.


### Concurrent Object












### Foundations of Shared Memory












### The Relative Power of Primitive Synchronization Operations









### Universality of Concensus






