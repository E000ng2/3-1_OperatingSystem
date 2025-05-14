#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <functional>


using namespace std;

int presentTime = 0; //현재시간
int cpuTime = 0;
int idleTime = 0; //cpu유휴시간
int selectedProcess = 0; //선택된 프로그램
int isend = 0; //종료 프로그램개수


//프로그램 정보
class programInfo{
public:
    int generationTime;
    int nextCpuTime = 0;
    int nextIoTime = 0;
    int idNumber;
    int enqueueTime;
    bool ready = false;
    bool isinqueue = false;
    list<int> cycleList;
    
    programInfo(int id, list<int>& cycleLi)
    {
        cycleList = cycleLi;
        generationTime = cycleList.front();
        cycleList.pop_front();
        idNumber = id;
    }

    void putCpuTime();
    void putIoTime();
    void infoStrat(std::vector<programInfo*>& programs);
    int infoUpdate(int passingTime, vector<programInfo*>& programs, int nowCpu);

};

priority_queue<programInfo*, vector<programInfo*>, function<bool(programInfo*, programInfo*)>> readyQueue(
    [](programInfo* a, programInfo* b) {
        if (a->nextCpuTime != b->nextCpuTime)
            return a->nextCpuTime > b->nextCpuTime;
        if (a->enqueueTime != b->enqueueTime)
            return a->enqueueTime > b->enqueueTime;
        return a->idNumber > b->idNumber;
    }
);

void programInfo::putCpuTime()//cpu중~
{
    if(!cycleList.empty())
    {
        nextCpuTime = cycleList.front();
        cycleList.pop_front();

        ready = true;
    }
              
}
void programInfo::putIoTime() //io중~
{
    if(!cycleList.empty())
    {
        nextIoTime = cycleList.front();
        cycleList.pop_front(); 
        ready = false;
    }
    
}

void programInfo::infoStrat(vector<programInfo*>& programs)
{
    if(generationTime == 0 && !isinqueue)
    {
        putCpuTime();
        readyQueue.push(programs[idNumber]);
        isinqueue = true;
        enqueueTime = presentTime;
    }

}

int programInfo::infoUpdate(int passingTime, vector<programInfo*>& programs, int nowCpu)
{  
    if(generationTime > 0)
    {
        while(generationTime)
        {
            if(passingTime == 0)
            {
                break;
            }

            --passingTime;
            --generationTime;
        }
    } 

    if(idNumber == nowCpu && !isinqueue) //cup사용중일 경우
    {
        putIoTime();
        if(nextIoTime == -1)
        {
            int endTime = presentTime;
            ++isend;
            ready = true;
            isinqueue = true;
            return endTime; //종료시각 반환
        }
    } 

    if(idNumber != nowCpu)
    {
        if(generationTime == 0)
        {
            if(ready && !isinqueue) //cpu시간인데 큐에 없는 경우
            {
                //큐에 넣어줌
                isinqueue = true;
                enqueueTime = presentTime  - passingTime;            
                readyQueue.push(programs[idNumber]);
            }
            
            if(!ready && !isinqueue)//io일 중일 경우, 아직 시작하지 않은 경우 (큐에 없는 경우)
            {
                while(passingTime)
                {
                    if(!nextIoTime) //Io가 끝날 경우
                    {
                        putCpuTime();
                        if(nextCpuTime == -1)
                        {
                            int endTime = presentTime - passingTime;
                            ++isend;
                            return endTime; //종료 시각 반환
                        }
                        isinqueue = true;
                        enqueueTime = presentTime - passingTime;
                        readyQueue.push(programs[idNumber]);
                        ////큐에 넣는다////
                        break;
                    }
                    else
                    {
                        --passingTime;
                        --nextIoTime;
                    }
                }
        
            }
    
    
        }
    
    }
    

    return 0;

}
    


void allupdate(int n, int passingTime, vector<programInfo*>& programs, int nowCpu, vector<int>& endTime)
{
    for(int i = 0; i < n; i++)
    {
        int end = programs[i]->infoUpdate(passingTime, programs, nowCpu);
        if (end != 0)
            endTime[i] = end;
    }
}

void printReadyQueue()
{
    auto tempQueue = readyQueue;  // 복사본 생성

    cout << "\n[현재 readyQueue 상태]\n";
    while (!tempQueue.empty()) {
        programInfo* p = tempQueue.top();  // 포인터로 꺼냄
        tempQueue.pop();

        cout << "ID: " << p->idNumber 
             << ", nextCpuTime: " << p->nextCpuTime 
             << ", enqueueTime: " << p->enqueueTime << endl;
    }
}

int main()
{
    //파일 입출력
    ifstream sjf_inp("sjf.inp");
    ofstream sjf_out("sjf.out");

    string line;
    int n;

    sjf_inp >> n; //프로그램 개수
    sjf_inp.ignore(); 

    vector<list<int>> cycleList(n); //각 프로그램의 Cycle저장
    int i = 0;

    while(getline(sjf_inp, line))
    {
        stringstream ss(line);
        int num;

        while(ss >> num)
        {
            cycleList[i].push_back(num);
        i++;
        }
    }

    //객체 생성
    vector<programInfo*> programs;

    for(int i = 0; i < n; i++)
    {
        programs.push_back(new programInfo(i, cycleList[i]));
    }

    //초기 등록
    for(int i = 0; i < n; i++)
    {
        programs[i]->infoStrat(programs);
    }

    vector<int> endTime(n, 0);  //종료시각 저장


    cout << "presentTime" << presentTime << endl;
    cout << "idleTime" << idleTime << endl;
    printReadyQueue();
    

    while(isend < n)
    {
        //큐에 작업이 있을 경우
        if(!readyQueue.empty())
        {
            
            //큐의 첫번쨰에 들어있는 작업의 cpu 시간을 꺼내어 presenttimed에 더한다 넣는다
            programInfo* current = readyQueue.top();
            readyQueue.pop();
            current->isinqueue = false;

            cpuTime = current -> nextCpuTime;
            presentTime += cpuTime;

            cout << "presentTime" << presentTime << endl;
            cout << "idleTime" << idleTime << endl;

            printReadyQueue();


            //모두 업데이트
            allupdate(n, cpuTime, programs, current->idNumber, endTime);

        }

        //큐가 비었을 경우
        else
        {
            allupdate(n, 1, programs, -1, endTime); // 업데이트
            ++idleTime;
            ++presentTime;
        }
    }


    sjf_out << "Total CPU Usage Time: " << presentTime << "\n";
    cout << "Total CPU Usage Time: " << presentTime << "\n";

    for (int i = 0; i < n; i++) {
        sjf_out << "Program " << i << " End Time: " << endTime[i] << "\n";
        cout << "Program " << i << " End Time: " << endTime[i] << "\n";
    }

    cout << "CPU : " << idleTime << endl;

}