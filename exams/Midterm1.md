# 분산시스템 중간고사1

## 2009 기출 (중간고사1)
1. Bakery algorithm은 n-thread mutual exclusion을 해결할 수 있지만 n-thread consensus를 위해서는 부적절하다. 이유를 설명하시오.
1. 주어진 sequential object를 concurrent object로 바꾸는 방법을 간단히 설명하시오.
1. MRSW boolean safe register를 사용해서 MRSW boolean regular register를 만드시오.
1. Get-and-increment를 사용하여 mutual exclusion을 위한 lock을 만들어보시오. 이 경우 Get-and-increment의 return값은 실제 순서를 나타내게 되는데 이를 사용하여 lock을 더 효율적으로 만들 수 있는 방법이 있겠는지 제시하시오.
1. 수업시간에 배운 bounded timestamping system (**T(k) = T(2) * T(k-1)**)이 concurrent한 환경에서는 제대로 동작하지 않음을 **T(3)** 의 경우를 예를 들어서 보이시오. (처음 상태 : 10, 11, 22)

## 2012 기출 (중간고사1)
1. Test-and-set instruction을 사용하면 mutual exclusion을 해결할 수 있지만 n-thread consensus는 해결할 수 없다. 무슨 차이가 있는지 설명하시요.
1. Wait-free와 Lock-free의 차이를 설명하시오.
1. TAS나 CAS가 없는 경우에 64-bit write 연산을 사용해서 consensus를 할 수도 있다. (multiple-assignment를 흉내내면 된다) 이때 최대 consensus number가 얼마가 되겠는지 보이시오. (write granularity나 alignment 같은 가정을 정확히 하시오)
1. Register의 종류는 (SRSW, MRSW, MRMW) (B, MV), (safe, regular, atomic)의 조합으로 모두 18가지가 있을 수 있다. 이중에서 Boolean을 무시하면 9가지인데, 이들 사이의 관계를 도식하시오. *(reader의 수는 m, writer의 수는 n, value의 가짓수는 k)*
1. Bounded timestamping system **T(3)** 은 thread들이 concurrent하게 돌아갈 때 더 이상 순서를 정할 수 없는 상황이 발생할 수 있다. CAS instruction을 사용하여 concurrent하게 돌아가더라도 항상 순서를 정할 수 있도록 timestamp를 만드는 방법을 제시하시오.

## 2014 기출 (QUIZ)
1. 수업시간에 배운 bounded timestamping system (**T(k) = T(2) * T(k-1)**)이 concurrent한 환경에서는 제대로 동작하지 않음을 **T(3)** 의 경우를 예를 들어서 보이시오. (처음 상태 : 10, 11, 22)
1. Bakery algorithm은 n-thread mutual exclusion을 해결할 수 있다. 한 가지 문제점은 n개의 thread의 timestamp를 동시에 읽어야 한다는 점이다. 하지만 이 문제점은 atomic snapshot으로 해결할 수 있다. 그렇다면 atomic register만을 사용하여 mutual exclusion을 해결할 수 있다는 것이다. 다음과 같은 경우를 생각해보자. Bakery algorithm을 사용하여 두 개의 thread가 차례로 multiple assignment를 한다면 2-thread consensus가 해결된다. 따라서 atomic register만을 사용하여 2-thread consensus를 해결할 수 있다. 지금까지의 논리에 잘못된 점이 있는지 설명하시오.
1. SRSW, Multi-value, Regular 레지스터를 사용하여 SRSW, Multi-value, Atomic 레지스터를 만들어보시오. 몇 개가 필요하겠는가?
1. 다음은 atomic 2-assignment로 4 process consensus를 만드는(해결하는) 방법이다. 제대로 동작하는지 증명하거나, 아니면 counter example을 보이고 이를 제대로 만드는 방법이 있겠는지 보이시오. process A, B, C, D 중 A, B와 C, D가 각각 3 out of 2 atomic assignment로 consensus를 한다. 이후에 그 중 승자 둘을 가지고 다시 같은 방법으로 승자를 가리게 되면 4개 중 하나의 승자를 정할 수 있다. 이런 식으로 하면 2 -> 4 -> 8 -> 16 ... 이렇게 무한히 많은 process의 consensus가 가능하다.

## 2016 기출 (중간고사1)
1. Test-and-set로 mutual exclusion을 해결할 수 있지만 n-thread consensus는 해결할 수 없다. 차이점과 해결할 수 없는 이유를 설명하시오.
1. Wait-free와 Lock-free의 차이점을 설명하시오.
1. Bounded timestamping system (**T(k) = T(2) * T(k-1)**)이 concurrent한 환경에서 제대로 동작 안 하는 경우가 있다. Compare-and-swap을 이용하여 **T(3)** 에서 thread들이 concurrent하게 동작할 수 있도록 timestamp를 만드는 방법을 제시하시오. (Wait-free하도록)
1. Linearizability와 Sequential consistency중 누가 더 강력한 조건인지 설명하고, 둘 중 하나는 만족하지만 다른 하나는 만족하지 못하는 예시를 드시오.
1. MCS Queue Lock와 CHL Queue Lock의 차이점을 말하고 어떤 환경에서 더 적합한지 설명하시오.

## 2017 기출 (중간고사1)
1. Bounded timestamping system은 sequential하게 작동할 때는 consistent하지만 concurrent하게 작동할 때는 inconsistent할 때가 있다. **T(3)** 에 thread 0, 1, 2가 각각 22, 11, 10에 있을 때 inconsistent해지는 경우가 2가지 있다. 이때의 스케줄을 보이시오. (이 순간 어떤 스레드든 next-TS()는 20임)
1. Atomic-2-assignment를 이용하면 2-thread consensus를 해결할 수 있다. 이를 이용해서 다음과 같이 4-thread consensus를 해결하려고 한다. 스레드를 2개씩 묶어서 경쟁하고 다음 단계에 이긴 스레드끼리 승자를 정한다. 이것이 가능한지 불가능한지 설명하시오.
1. 가장 간단한 Safe, SRSW, Boolean Register를 이용해서 Atomic, 3-reader/3-writer 4-bit Register를 만들 때 필요한 레지스터의 수를 구하시오.
1. Filter Lock을 사용하고 thread 1, 2, 3이 있을 때, thread 1이 thread 2, 3에 2번, 3번, 4번 추월당하는 예시를 드시오.
1. Filter Lock이 Wait-free인지 아닌지 보이시오.

### 1번 소스 코드
``` C
i = Thread.Id(); j = (i + 1) % 3; k = (i + 2) % 3;
read TS(i); read TS(j); read TS(k);
calc next-TS(); update TS(i);
```

### 1번 정답
* \< A, B, C \> = \< 0, 1, 2 \>
* 첫 번째는 세 스레드가 AA, AB, AC에 위치하는 경우 (ex. i=20, j=21, k=22)
* 두 번째는 세 스레드가 A\_, B\_, C\_에 위치하는 경우 (ex. i=10, j=20, k=02)

### 2번 정답
* 불가능함.
* 첫 번째 경쟁에서의 두 승자 중 하나라도 살아있다면 `decide()`할 수 있다.
* 그러나 첫 번째 경쟁에서의 두 승자가 모두 죽어버린 경우, 첫 번째 경쟁에서의 두 패자가 최종 승자를 알 수 없어서 `decide()`할 수 없다. (wait-free가 아님)

### 3번 정답
* 해설 없음.

### 4번 정답
* 무한대로 추월당할 수 있음.

### 5번 정답
* 어떤 스레드가 `unlock()`을 안 하면 blocking 되어버리므로 wait-free일 수 없음.
