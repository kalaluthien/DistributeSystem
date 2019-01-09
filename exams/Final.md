# 분산시스템 기말고사

## 2009 기출 (기말고사)
1. Transactional Memory를 구현하는데 있어서 하드웨어에서 제공하면 좋을 만한 기능을 아는대로 나열하고 어떤 도움이 되겠는지 설명하시오.
1. Lock-free stack은 lock을 사용하는 대신 CAS를 사용하여 `push()`와 `pop()`을 linearize한다. contention이 발생하는 경우에는 backoff를 해서 어느 정도 효과를 거둘 수 있지만, 근본적으로 sequential access라는 문제가 있다. 이를 해결하기 위한 방법을 제시하시오.
1. 2 input balancer를 사용하여 bitonic(8) counting network를 만들어보시오.
1. lock-free hash set을 구현하기 위해서는 recursive split ordering을 사용한다. 이유는?
1. tournamant tree barrier와 combining tree barrier의 차이점을 설명하시오.
1. 주어진 multithreaded application의 T(1)과 T(inf)가 알려져 있다. 이 프로그램을 n개의 프로세서를 사용해서 수행할 때 걸리는 시간을 T(n)이라 하자. T(1), T(inf), T(n) 사이의 관계를 보이시오.

## 2010 기출 (기말고사)
1. atomic 2-assignment를 사용해서 다음과 같이 3-consensus를 하려고 한다. 제대로 되었다면 맞다는 것을 증명하고, 그렇지 않다면 counter example을 보이시오. 우선 memory location (A, B, C, D)를 모두 0으로 초기화한다. phase 1: thread 1과 thread 3이 atomic 2-assignment를 사용해서 순서를 정한다. (trivial) phase 2: thread 1은 (A, B)에 1을 쓰고 thread 2는 (B, C)에 2를 쓰고 thread 3은 (C, D)에 3을 쓰도록 한다. phase 2의 결과로부터 B에서는 1,2 사이의 순서를, C에서는 2,3 사이의 순서를 알게 된다. 이때 2가 1,3 중 어느 하나보다 먼저라면 (전체 순서를) 결정할 수 있지만, 2가 1,3보다 나중에 쓰였다면 1,3 사이의 순서를 알 수 없다. 그 경우에는 phase 1에서 정한 순서에 따라 결정한다.
1. Transactional Memory의 구현을 보면 locking 대신에 optimistic concurrency control이나 timestamp based concurrency control이 쓰이게 된 것을 볼 수 있다. Database transaction management에서는 사실 이론적으로만 나와있지 거의 쓰이지 않는 방법들이 왜 Transactional Memory에서는 사용되고 있는지 설명하시오.
1. `P(x)`와 `Q(x)`를 각각 `x`의 `d(=2^i)`차 방정식이라고 하자. `P(x)=P0(x)+(P1(x)*x^(d/2))`, `Q(x)=Q0(x)+(Q1(x)*x^(d/2))`이므로 divide and conqure 형식으로 계산할 수 있다. (`P0(x)`, `P1(x)`, `Q0(x)`, `Q1(x)`은 `d/2`차 방정식) 이때 `P(x)+Q(x)`, `P(x)*Q(x)`의 work와 critical path length를 계산하시오.

## 2012 기출 (기말고사)
1. Pessimisteic concurrency control과 optimistic concurrency control 방법이 각각 어떻게 Transactional memory에 적용되는지 설명하시오.
1. Dequeue를 사용하여 load balancing을 할 때, host processor는 한 쪽에서 `push()`, `pop()`을 하고, steal processor는 다른 쪽에서 꺼내옴으로써 얻는 이득이 무엇인지 설명하시오.
1. Skiplist를 사용하여 unbounded priority queue를 구현하였다고 하자. 이때 `add()`와 `remove()`를 어떻게 구현하는지 설명하고 각각의 time complexity가 어떻게 되는지 보이시오.
1. Processor의 수가 많아질 수록 combining tree나 balancing network가 queue lock에 비해 높은 성능을 보일 수 있는 이유를 직관적으로 설명하시오.
1. Transactional memory를 만들기 위해 하드웨어에서 제공해야 하는 중요한 기능을 3가지 순서대로 나열하시오.
1. Tournament tree barrier의 Combining tree barrier에 대한 장단점을 설명하시오.
