//banker's algorithm
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

//전역 변수 n 프로세스 개수, m 자원 개수
int n = 0, m = 0;

//함수 선언
std::vector<int> readline(std::ifstream& file);
std::vector<std::vector<int>> readline(std::ifstream& file, int num);
bool is_safe(std::vector<std::vector<int>>& max, std::vector<std::vector<int>>& alloc,
             std::vector<int>& avail, std::vector<int> line);
bool alloc_assumption(std::vector<std::vector<int>> max, std::vector<std::vector<int>>alloc, 
                      std::vector<int> avail, int procnum, std::vector<int> line);
bool deadlock_check(std::vector<std::vector<int>> max, std::vector<std::vector<int>> alloc,
                    std::vector<int> avail);



// readline 한줄 읽어서 int배열에 저장
std::vector<int> readline(std::ifstream& file)
{
    std::vector<int> result;
    std::string line;

    while((getline(file, line))){
        if(!line.empty()){
            std::stringstream ss(line);
            std::string word;
            while(ss >> word){
                result.push_back(stoi(word));
            }
        }
        else continue; //빈줄 삭제

        break;
    }

    return result;
}


// readline 원하는 만큼 읽어서 2차원 int 배열에 저장
std::vector<std::vector<int>> readline(std::ifstream& file, int num)
{
    std::vector<std::vector<int>> result(num);
    std::string line;
    int i = 0;

    while (i < num && getline(file, line)){ //만약 i >= num일 경우 getline 실행 x
        if(!line.empty()){
            std::stringstream ss(line);
            std::string word;
            while(ss >> word){
                result[i].push_back(stoi(word));
            }
            i++;
        }

        else continue;
    }

    return result;
}

bool is_safe(std::vector<std::vector<int>>& max, std::vector<std::vector<int>>& alloc,
             std::vector<int>& avail, std::vector<int> line)
{
    int procnum = line[0];
    line.erase(line.begin());

    // 요청이 유효하지 않으면 false
    for (int i = 0; i < m; i++) {
        if (line[i] > avail[i] || line[i] > (max[procnum][i] - alloc[procnum][i])) {
            return false;
        }
    }

    // 요청을 가정한 상태에서 deadlock 발생 여부 확인
    if (alloc_assumption(max, alloc, avail, procnum, line)) {
        // 실제 자원 할당 수행
        for (int i = 0; i < m; i++) {
            alloc[procnum][i] += line[i];
            avail[i] -= line[i];
        }
        return true;
    }

    return false;
}


// alloc_assumption 자원을 할당했을때 교착상태가 일어나는가 확인 **핵심은 주소가 아닌 값을 복사하는 것
bool alloc_assumption(std::vector<std::vector<int>> max, std::vector<std::vector<int>>alloc, 
                      std::vector<int> avail, int procnum, std::vector<int> line){

    for (int i = 0; i < m; i++) {
        alloc[procnum][i] += line[i];
        avail[i] -= line[i];
    }

    return deadlock_check(max, alloc, avail);
}


// check 교착상태가 읽어나는지 확인 **핵심은 주소가 아닌 값을 복사하는 것
bool deadlock_check(std::vector<std::vector<int>> max, std::vector<std::vector<int>> alloc,
                    std::vector<int> avail){
    std::vector<bool> finish(n, false);
    bool is_procwork = true;

    while(is_procwork){

        is_procwork = false;
        for(int i = 0; i < n; i++){

            if(finish[i] == false){

                bool safe = true;
                for(int j = 0; j < m; j++){ //max - alloc < avail 확인

                    if (avail[j] < (max[i][j] - alloc[i][j])){

                        safe = false;
                        break;
                    }
                }

                if(safe){ //max - alloc < avail -> avail + alloc
                    finish[i] = true;
                    for(int j = 0; j < m; j++){
                    
                        avail[j] += alloc[i][j];
                        is_procwork = true;
                    }
                }
            }
        }
    }

    for( bool check : finish){ //프로세스 체크
        if (!check) return false;
    }

    return true;
}

void write_available(std::ofstream& outfile, const std::vector<int>& avail) {
    for (int i = 0; i < m; ++i) {
        outfile << avail[i];
        if (i != m - 1) outfile << " ";
    }
    outfile << "\n";
}


// main
int main()
{

    //파일 입출력
    std::ifstream banker_inp("banker.inp"); 
    std::ofstream banker_out("banker.out");

    if (!banker_inp.is_open()) {
    std::cerr << "banker.inp 파일을 열 수 없습니다!" << std::endl;
    return 1;
    }

    //n = 프로세스 개수, m = 자원 개수 (모두 50 이하)
    std::vector<int> line = readline(banker_inp);
    n = line[0];
    m = line[1];

    // 보유 자원 총 개수(avaliable)
    std::vector<int> avail = readline(banker_inp);

    // 프로세스 최대 자원 크기(MAX 행렬 N X M)
    std::vector<std::vector<int>> max = readline(banker_inp, n);

    //프로세스 현재 자원 크기(allocation 행렬 N X M)
    std::vector<std::vector<int>> alloc = readline(banker_inp, n);

    //현재 자원 업데이트
    for(int i = 0; i < m; i++){
        for(int j = 0; j < n; j++){
            avail[i] -= alloc[j][i];
        }
    }

    //자원 요청 및 반환(request or release)
    std::string command;
    std::vector<std::vector<int>> faillist;

    while(!(command == "quit")){
        banker_inp >> command;
        banker_inp.ignore();
        line = readline(banker_inp); //line 재활용 :)

        //만약 request다 -> is_ is_safe() == flase면 정보를 배열에 담기
        if (command == "request"){
            if(!is_safe(max, alloc, avail, line)){
                write_available(banker_out, avail);
                faillist.push_back(line);
            }
            else{
                write_available(banker_out, avail);
            }
        }

        //만약 release다 -> 배열에 있는 request 돌려보기!
        else if (command == "release"){
            int pid = line[0];
            for(int i = 0; i < m; i++){
                avail[i] += line[i+1];
                alloc[pid][i] -= line[i+1];
            }

            std::vector<std::vector<int>> new_faillist;
            for (const auto& retry : faillist) {
                if (!is_safe(max, alloc, avail, retry)) {
                    new_faillist.push_back(retry);
                }
            }
            faillist = new_faillist;

            write_available(banker_out, avail);

        }

    }
    
}

