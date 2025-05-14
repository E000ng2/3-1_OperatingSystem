#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>
#include <queue>

int cpu_Time = -1;
int cpu_Idletime = 0;
int process_N = 0;


std::vector<int> process_Endtime(process_N);
std::vector<bool> process_Endcheck(process_N, false);

class processInfo{

public:
    int process_Id;
    int process_createtime;
    int readyqueue_time;

    std::list<int> process_cyclelist;
    int process_cycletime;

    bool is_running = false;
    bool is_ready = false;
    bool is_blocked = false;

    processInfo(int id, std::list<int>& cycles)
    : process_Id(id)
    {
        process_createtime = cycles.front();
        cycles.pop_front();
        process_cyclelist = cycles;
    }

};

std::vector<processInfo> process_List;

struct ReadyQueueCompare {
    bool operator()(const processInfo* a, const processInfo* b) const {
        if (a->process_cycletime != b->process_cycletime)
            return a->process_cycletime > b->process_cycletime;
        if (a->readyqueue_time != b->readyqueue_time)
            return a->readyqueue_time > b->readyqueue_time;
        return a->process_Id > b->process_Id;
    }
};

std::priority_queue<processInfo*, std::vector<processInfo*>, ReadyQueueCompare> readyQueue;

void EnqueueReadyQueue(int i)
{
    processInfo* p = &process_List[i];

    p -> process_cycletime = p -> process_cyclelist.front();
    p -> process_cyclelist.pop_front();

    readyQueue.push(p);
    
    p -> readyqueue_time = cpu_Time;
    p -> is_ready = true;
}


void rebuildReadyQueue() {
    std::priority_queue<processInfo*, std::vector<processInfo*>, ReadyQueueCompare> newQueue;

    while (!readyQueue.empty()) {
        newQueue.push(readyQueue.top());
        readyQueue.pop();
    }

    readyQueue = std::move(newQueue);
}

void fistEnqueue()
{
    for(int i = 0; i < process_List.size(); i++){
        if (process_List[i].process_createtime == cpu_Time)
        {
            EnqueueReadyQueue(i);

        }
    }
};

void processEnd(int Id, bool end)
{
    processInfo* p = &process_List[Id];
    if(end)
    {
        process_Endtime[Id] = cpu_Time + 1;
    }
    else
    {
        int time = p -> process_cycletime;
        process_Endtime[Id] = cpu_Time + time + 1;
    }
    process_Endcheck[Id] = true;
    p -> is_blocked = false;
    p -> is_ready = false;
}

void printReadyQueue(std::priority_queue<processInfo*, std::vector<processInfo*>, ReadyQueueCompare> pq) {
    std::cout << "📋 Ready Queue 상태 (우선순위 순):\n";
    while (!pq.empty()) {
        processInfo* p = pq.top();
        std::cout << "  [PID: " << p->process_Id
                  << ", 남은 시간: " << p->process_cycletime
                  << ", 레디큐 도착 시간: " << p->readyqueue_time
                  << "]\n";
        pq.pop();
    }
}

void isCpuTime()
{
    int Id = -1;
    for (int i = 0; i < process_List.size(); i++) {
        if (process_List[i].is_running) {
            Id = i;
            std::cout << "지금 사용즁인 cpu" << Id << "현재 cpu 시간" << cpu_Time <<std::endl;
            break;
        }
    }
    
    processInfo* p = &process_List[Id];
    p -> process_cycletime -= 1;
    p -> is_running = false;

    if (p -> process_cycletime == 0)
    {
        readyQueue.pop();
        p -> process_cycletime = p -> process_cyclelist.front();
        p -> process_cyclelist.pop_front();
        p -> is_blocked = true;
        p -> is_ready = false;

        if (p -> process_cycletime == -1) 
        {
            processEnd(p -> process_Id, true);
            std::cout <<"끝나유 / 프로세스 : "<< p -> process_Id<<"cup시간"<< cpu_Time << std::endl;   
        }

        else if(p -> process_cyclelist.front() == -1) 
        {
            processEnd(p -> process_Id, false);
            std::cout <<"io로 끝나유 / 프로세스 : "<<p-> process_Id <<"남은 시간"<< p -> process_cycletime <<"cup시간"<< cpu_Time << std::endl;
        }

    }
}

void isIoTime(int i)
{
    processInfo* p = &process_List[i];
    if (p -> process_cycletime == 0)
    {
        // if (p -> process_cyclelist.front() == -1) processEnd(i, true);

        {
            EnqueueReadyQueue(i);
            p -> is_blocked = false;
        }

    }
    p -> process_cycletime -= 1;

}

int updateInfo()
{
    int i = -1;

    if(!readyQueue.empty())
    {
        processInfo* p = readyQueue.top();
        p -> is_running = true;    
        i = 0;        

    }  
    
    return i;
}

int main(){

    std::ifstream srtf_inp("srtf.inp");

    srtf_inp >> process_N;
    srtf_inp.ignore();

    process_Endtime.resize(process_N);
    process_Endcheck.resize(process_N, false);

    std::string line;

    int id = 0;

    while(std::getline(srtf_inp, line))
    {
        std::stringstream ss(line);
        std::list<int> cycles;
        int value;
        while(ss >> value)
        {
            cycles.push_back(value);
        }
        process_List.push_back(processInfo(id, cycles));
        id++;
    }

    // while(!std::all_of(process_Endcheck.begin(), process_Endcheck.end(), [](bool b){ return b;}))
    while(cpu_Time<500)
    {

        cpu_Time++;
        fistEnqueue();
        rebuildReadyQueue();

        int id = updateInfo();

        for(int i = 0; i < process_List.size(); i++)
        {
            if (process_List[i].is_ready == false && process_List[i].is_blocked == true)
            {
                isIoTime(i);
            }
        }

        if (id == -1) {
            cpu_Idletime++;
        } else {
            isCpuTime();
        }
    }


    std::ofstream srtf_out("srtf.out");


    srtf_out << cpu_Idletime << "\n";
    
    for (int i = 0; i < process_Endtime.size(); i++) {
        srtf_out << process_Endtime[i] << "\n";
    }
    
    srtf_out.close();

    return 0;
}
