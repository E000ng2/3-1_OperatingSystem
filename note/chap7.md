# 교착상태
## 1. 교착상태 문제 제기
- 교착상태 : 자원을 소유한 채, 모두 상대방이 소유한 자원을 기다리면서 무한 대기
- 식사하는 철학자 문제 
    - 문제 상황 : 모두 거의 동시에 왼쪽 포크를 든 후 오른쪽 포크를 들려고 할 때, 모두 상대가 가진 포크를 기다리면서 먹을 수 없는 교착 상태 발생. 환형 요청/대기(circular wait)
    - 해결 방법 : 한사람을 제외하고 모두 왼쪽부터 잡고 남은 한 사람은 오른쪽부터 잡음.
  
## 2. 교착상태
- 교착상태 : 자원을 소유한 스레드들 사이에서 각 스레드는 다른 스레드가 소유한 자원을 요청하여 무한정 대기하는 현상
  - deadly embrace : 풀지 못하는 포옹

 
```
void* thread(void *a)
{
    ...
    pthread_mutex_lock(&LockA)
    pthread_mutex_lock(&LockB)
    pthread_mutex_unlock(&LockB)
    pthread_mutex_unlock(&LockA)
    ...
}
void* thread2(void *a)
{
    ...
    pthread_mutex_lock(&LockB)
    pthread_mutex_lock(&LockA)
    pthread_mutex_unlock(&LockA)
    pthread_mutex_unlock(&LockB)
    ...
}
```
- 교착상태 모델링   
  - 자원 할당 그래프(Resource Allocation Graph, RAG)
    - 꼭지점(vertex) - 스레드(원), 자원(사각형)
    - 간선(edge) - 소유/요청 관계. 할당 간선과 요청 간선

- 교착상태가 발생하는 4가지 필요 조건
   - 코프만 조건(Coffman condition)(충분조건 X)
      - 상호 배제(Mutual Exclusion) : 각 자원은 한 번에 하나의 스레드에게만 할당
      - 소유하면서 대기(Hold & Wait) : 스레드가 한 자원을 소유(hold)하면서 다른 자원을 기다리기
      - 강제 자원 반환 불가(No Preemption) : 스레드에게 할당된 자원을 강제로 빼앗지 못함
      - 환형 대기(Circular Wait) : 한 그룹의 스레드들에 대해, 각 스레드는 다른 스레드가 요청하는 자원을 소유하는 원형 고리 형성

- 교착상태 해결 방법
  - 예방(prevention)
    - 4가지 조건 중 하나 이상의 조건이 성립되지 못하는 시스템 구성
  - 회피(avoidance)
    - banker's algorithm 미래의 교착 상태 가능성 확인
  - 감지 및 복구(detection and recovery)
    - 교착 상태 감지 프로그램 구동
  - 무시
    - 대비책 없음. 이상이 있을 경우 재실행 할 것이라고 믿음

- 교착 회피 : Banker's algorithm
  - 교착 상태가 발생하지 않는 범위 내에서만 자원을 할당하고, 교착 상태가 발생하는 범위에 있으면 프로세스를 대기 시킴
  - 즉, 할당되는 자원의 수를 조절하여 교착 상태를 피함     

  - 안정 상태(safe state) 와 불안정 상태(unsafe state) : 할당된 자원 수와 안정 상태는 반비례관계, 불안정 상태가 커질수록 교착 상태 발생 가능성이 높아짐
  - banker's algorithm 변수
    ```
    전체 자원(Total) : 시스템 내 전체 자원의 수
    가용 자원(Avaliable) : 시스템 내 현재 사용할 수 있는 자원의 수(가용 자원 = 전체 자원 - 모든 프로세스의 할당 자원)
    최대 자원(Max) : 각 프로세스가 선언한 최대 자원의 수
    할당 자원(Allocation) : 각 프로세스에 현재 할당된 자원의 수
    기대 자원(Expect or Need) : 각 프로세스가 앞으로 사용할 자원의 수(기대 자원 = 최대 자원 - 할당 자원)  
    ```
  - 자원 할당 기준
    ```
    요청한 자원을 할당해 준 뒤에도 시스템이 안정상태에 있다면 그 요청을 수락
    안정 상태 : 시스템이 안정 순서(safe sequence)를 가질 때 안정상태에 있다고 말함
    Safe sequence : 각 프로세스의 Need를 어떤 순서대로 만족시켜 줄 수 있을 때, 그 순서
    ```

  - 교착 상태 회피의 문제점
    - 프로세스가 자신이 사용할 모든 자원을 미리 선언해야 함
    - 시스템의 전체 자원 수가 고정적이어야 함
    - 자원이 낭비됨
    - 알고리즘 적용에 따른 오버헤드가 큼
      
- 교착 상태 감지 및 복구
  - 자원 장제 선점(preemption)
  - 롤백(rollback)
  - 스레드 강제 종료(kill process)

- 교착 상태 무시 : 타조(Ostroch)
  - 교착 상태는 반드시 발생하나 빈도가 적다
  - Put your head in the sand

**대부분의 운영체제 : Ostrich 알고리즘 사용**



