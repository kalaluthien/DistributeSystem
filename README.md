# Distributed System
![Prof. Yeom1](https://cse.snu.ac.kr/sites/default/files/styles/scale-width-220/public/node--professor/%EC%97%BC%ED%97%8C%EC%98%81%20%20%EA%B5%90%EC%88%98%EB%8B%98_28%20%2811%29_0.jpg)

SNU Distributed System Course Homework & Exam Backup by Hyungmo Kim

**DO NOT COPY THE HOMEWORKS**

---

## HW1
### Specification
* 이번 학기에 사용할 수 있는 멀티코어 기계를 하나 골라서 CPU 사양, 시스템 사양을 조사
* CPU에서 제공하는 여러가지 synchronization operation들을 파악
  * Test and Set
  * Compare and Swap
  * Fetch and Add
  * Memory Barrier
  * and so on...
* Operation들의 성능을 측정
* 하나의 long integer variable을 하나의 core 에서 1,000,000,000회 증가시키는 시간과 여러 개의 core를 사용하고 synchronization을 해서 같이 작업할 경우의 시간 측정

1. 소스 코드

2. 리포트
pdf 파일로 제출

3. 제출 방식  
리포트를 하나의 파일로 압축해서 메일로 제출  
**압축파일명**: HW1-1\_학번\_이름.zip (e.g., HW1-1\_2017-26932\_김형모.zip)  
**EMAIL 제목**: [분산시스템]HW1\_학번\_이름  
**EMAIL 주소**: hanggicrown@gmail.com  
**제출기한**: 3월 14일 수요일 23시 59분

---

## HW2 (outdated)
### Specification
* The art of multiprocessor programming
  * 42쪽 연습문제 15
  * 67쪽 연습문제 27, 28, 31 32

작성한 문서를 pdf 파일로 변환해서 메일에 첨부로 제출  
**파일명**: HW2\_학번\_이름.pdf (e.g., HW2\_2017-12345\_나분산.pdf)  
**EMAIL 제목**: [분산시스템]HW2\_학번\_이름  
**EMAIL 주소**: joongsuk@snu.ac.kr  
**제출기한**: 3월 27일 월요일 23시 59분

---

## HW 3 (outdated)
### Specification
* n개의 thread로 shared counter를 1억까지 증가시키는데 걸리는 시간을 아래의 synchronization operation들을 이용해서 측정하라. 
  - *Test and set*
  - *Test and test and set*
  - *Compare and exchange*
  - *Fetch and add*

1. 소스코드  

2. 리포트  
pdf 파일로 제출

3. 제출방식  
소스파일, 리포트를 하나의 파일로 압축해서 메일로 제출  
**파일명**: HW3\_학번\_이름.zip (e.g., HW3\_2017-12345\_나분산.zip)  
**EMAIL 제목**: [분산시스템]HW3\_학번\_이름  
**EMAIL 주소**: joongsuk@snu.ac.kr  
**제출기한**: 4월 17일 월요일 23시 59분

4. 기타  
HW1과 이번 HW3의 차이점은 HW1은 하나의 core와 멀티 core를 쓰는 것의 성능 비교에 초점을 두었다면, 이번 HW3은 thread 개수가 증가함에 따라서 synchronization operation들간의 성능을 비교하는 것에 초점을 두고 진행하시면 됩니다.

---

