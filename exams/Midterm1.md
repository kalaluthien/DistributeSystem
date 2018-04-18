# 분산시스템 중간고사1

## 2012 기출 (중간고사1)
1. Test-and-set instruction을 사용하면 mutual exclusion을 해결할 수 있지만 n-thread consensus는 해결할 수 없다. 무슨 차이가 있는지 설명하시요.
1. Wait-free와 Lock-free의 차이를 설명하시오.
1. TAS나 CAS가 없는 경우에 64-bit write 연산을 사용해서 consensus를 할 수도 있다. (multiple-assignment를 흉내내면 된다) 이때 최대 consensus number가 얼마가 되겠는지 보이시오. (write granularity나 alignment 같은 가정을 정확히 하시오)
1. Register의 종류는 (SRSW, MRSW, MRMW) (B, MV), (safe, regular, atomic)의 조합으로 모두 18가지가 있을 수 있다. 이중에서 Boolean을 무시하면 9가지인데, 이들 사이의 관계를 도식하시오. *(reader의 수는 m, writer의 수는 n, value의 가짓수는 k)*
1. Bounded timestamping system **T(3)** 은 thread들이 concurrent하게 돌아갈 때 더 이상 순서를 정할 수 없는 상황이 발생할 수 있다. CAS instruction을 사용하여 concurrent하게 돌아가더라도 항상 순서를 정할 수 있도록 timestamp를 만드는 방법을 제시하시오.

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
