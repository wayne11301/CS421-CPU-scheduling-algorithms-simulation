/*
This is a C++ program that simulate 3 CPU scheduling algorithms: Round Robin, Shortest job first, and Shortest remaining job first.
The input is the given .txt file "Lab3inputfileF20.txt"
The output will be saved to another .txt file called "output.txt"
*/


#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

//process class
class process {
public:
    string processID;
    int burst_time,arrival_time, wait_time, turnaround_time, wait_adjust;

    process();
    void loadID(string ID);
    void load_btime(int btime);
    void load_atime(int atime);
    void load_wadj(int wait);
    void print();
    int get_btime();
    int get_atime();
    int get_ttime();
    int get_wtime();
    string get_ID();
    void ttime_increase();
    void wtime_increase(int timer);
};
//constructor
process::process() {
    burst_time = 0;
    arrival_time = 0;
    wait_time = 0;
    turnaround_time = 0;
    wait_adjust = 0;
}
void process::loadID(string ID) {
    processID = ID;
}
void process::load_btime(int btime) {
    burst_time = btime;
}
void process::load_atime(int atime) {
    arrival_time = atime;
}
void process::load_wadj(int wait) {
    wait_adjust = wait;
}
void process::print() { //for checking if reading from file is successful
    cout << processID << " " << burst_time << " " << arrival_time << endl;
}
int process::get_btime() {
    return burst_time;
}
int process::get_atime() {
    return arrival_time;
}
int process::get_ttime() {
    return turnaround_time;
}
int process::get_wtime() {
    return wait_time;
}

string process::get_ID() {
    return processID;
}

//for simulating process run for 1 time unit, reduce remaining burst time by 1
void process::ttime_increase() {
    turnaround_time++;
    burst_time--;
}

//for calculating wait time
void process::wtime_increase(int timer) {

    //if process is not preempted, wait time is incremented by current_time minus arrival_time
    if (wait_adjust == 0) {
        wait_time = wait_time + (timer - arrival_time);
    }
    // if process is being preempted, wait time is incremented by current_time minus the time being preempted
    else {
        wait_time = wait_time + (timer - wait_adjust);
    } 
}

//bubble sort for getting the shortest burst time process in front
void sortvector(vector<process> &tosort) {
    process temp;
    if (tosort.empty()||(tosort.size()==1) || (tosort.size() == 2)) return;
    for (auto i = tosort.begin()+1; i != tosort.end()-1; i++) {
        for (auto j = i + 1; j != tosort.end(); j++) {
            if ((*i).get_btime() > (*j).get_btime()) {
                temp = *i;
                *i = *j;
                *j = temp;
                
            }
            if ((*i).get_btime() == (*j).get_btime()&& (*i).get_atime() > (*j).get_atime()) {
                temp = *i;
                *i = *j;
                *j = temp;
            }
        }
    }
}

//bubble sort for output in ABCDE... order
void sort_output(vector<process>& tosort) {
    process temp;
    for (auto i = tosort.begin(); i != tosort.end() - 1; i++) {
        for (auto j = i + 1; j != tosort.end(); j++) {
            if (((*i).get_ID().compare((*j).get_ID()))>0) {
                temp = *i;
                *i = *j;
                *j = temp;

            }
        }
    }
}

int main()
{
    ifstream in("Lab3inputfileF20.txt");
    ofstream out("output.txt");
    string token;
    int count = 0,process_timer=0,turnaround=0,size=0;
    process process_token; //for storing temp process

    queue<process> RR_all_jobs; //for RR calculation
    queue<process> SJF_all_jobs; //for SJF calculation
    queue<process> SRJF_all_jobs; //for SRJF calculation
    queue<process> ready_jobs; //RR ready queue

    vector<process> for_record; //for outputting stat
    vector<process> arrange; //for SJF order arrangement

    bool expire = false; //checker if process is being preempted

    //reading file into class process and push processes into queue
    do{
        in >> token;
        //first string is process ID, follow by burst_time, arrival_time
        if (count == 0) process_token.loadID(token);
        if (count == 1) process_token.load_btime(stoi(token)); 
        if (count == 2) process_token.load_atime(stoi(token));

        count++;
        //push processes in job queue
        if (count == 3) {
            count = 0;
            RR_all_jobs.push(process_token);
            SJF_all_jobs.push(process_token);
            SRJF_all_jobs.push(process_token);
            size++;
        }
    } while (in);

    //count is now timer!
    count = 0;

    //for RR
    out << "Round Robin Scheduling"<<endl;
    while (1) {
        //process timer has to be smaller than 4
        while (process_timer < 4) {
            //put job into ready queue
            if (!RR_all_jobs.empty()) {
                while (RR_all_jobs.front().get_atime() <= count){
                    process_token = RR_all_jobs.front();
                    ready_jobs.push(process_token);
                    RR_all_jobs.pop();
                    if (RR_all_jobs.empty()) break;
                } 
            }

            if (ready_jobs.empty()) {
                count++;
                break;
            }

            //increment turnaround time and decrease remaining burst time
            ready_jobs.front().ttime_increase();
            
            //print state when something occur
            if (process_timer == 0 && expire == false) {
                out << count << " " << ready_jobs.front().get_ID() << "    " << "Process terminated" << endl;
                ready_jobs.front().wtime_increase(count);
            }
            expire = false;
            //if remaining burst time reach 0, switch to the next process
            if (ready_jobs.front().get_btime() == 0) {
                for_record.push_back(ready_jobs.front());
                ready_jobs.pop();
                process_timer = 0;
                count++;
                break;
            }
            process_timer++;
            count++;
        }

        //if time expire, store the timer info in process, switch process to execute, output info to file
        if (process_timer == 4) {
            ready_jobs.front().load_wadj(count);
            process_token = ready_jobs.front();
            ready_jobs.push(process_token);
            ready_jobs.pop();
            out << count << " " << ready_jobs.front().get_ID() << "    " << "Quantum expired" << endl;
            ready_jobs.front().wtime_increase(count);
            expire = true;
        }

        process_timer = 0;

        //if no jobs in remaining, exit loop
        if (ready_jobs.empty() && RR_all_jobs.empty()) {
            out << count << " complete" << endl<<endl;
            break;
        }   
    }

    float avg_wait = 0, avg_turnaround=0;

    //printing record for RR
    out << "PrcessID  " << "Turnaround Time  " << "Waiting Time" << endl;
    while (!for_record.empty()) {
        //sort ordering before output
        sort_output(for_record);
        turnaround = for_record.front().get_ttime() + for_record.front().get_wtime();
        avg_turnaround += turnaround;
        avg_wait+= for_record.front().get_wtime();
        out << "    " << for_record.front().get_ID() << "           " << turnaround << "                " << for_record.front().get_wtime() << endl;
        for_record.erase(for_record.begin());
    }

    out << "Average    " << avg_turnaround <<"/"<<size<<"="<<(avg_turnaround/size)<< "        " << avg_wait << "/" << size << "=" << (avg_wait / size) << endl;

    //separator
    for (int i = 0; i < 25; i++) {
        out << "==";
    }
    
    //reset, do SJF
    count = 0;
    process_timer = 0;
    avg_turnaround = 0;
    avg_wait = 0;
    out << endl<<endl <<"Shortest Job First Scheduling" << endl;

    queue<process> test;
    while (1) {
        
        //put job into ready vector
        if (!SJF_all_jobs.empty()) {
            while (SJF_all_jobs.front().get_atime() <= count) {
                arrange.push_back(SJF_all_jobs.front());
                SJF_all_jobs.pop();
                if (SJF_all_jobs.empty()) break;
            }
        }

        //re-arranging vectors so that process with shorter burst time in front
        sortvector(arrange);

        //finishing station
        if (arrange.empty() && SJF_all_jobs.empty()) {
            out << count << " complete" << endl << endl;
            break;
        }

        if (arrange.empty()) {
            count++;
            continue;
        }

        //increment turnaround time and decrease remaining burst time
        arrange.front().ttime_increase();

        //print state when something occur
        if (process_timer == 0 ) {
            out << count << " " << arrange.front().get_ID() << "    " << "Process terminated" << endl;
            arrange.front().wtime_increase(count);
        }

        //if remaining burst time reach 0, switch to the next process
        if (arrange.front().get_btime() == 0) {
            for_record.push_back(arrange.front());
            arrange.erase(arrange.begin());
            process_timer = 0;
            count++;
            continue;
        }
        process_timer++;
        count++;

    }
    //printing record for SJF
    out << "PrcessID  " << "Turnaround Time  " << "Waiting Time" << endl;
    while (!for_record.empty()) {
        //sort before output
        sort_output(for_record);
        turnaround = for_record.front().get_ttime() + for_record.front().get_wtime();
        avg_turnaround += turnaround;
        avg_wait += for_record.front().get_wtime();
        out << "    " << for_record.front().get_ID() << "           " << turnaround << "                " << for_record.front().get_wtime() << endl;
        for_record.erase(for_record.begin());
    }
    out << "Average    " << avg_turnaround << "/" << size << "=" << (avg_turnaround / size) << "        " << avg_wait << "/" << size << "=" << (avg_wait / size) << endl;

    for (int i = 0; i < 25; i++) {
        out << "==";
    }

    //reset, do SRJF
    count = 0;
    process_timer = 0;
    avg_turnaround = 0;
    avg_wait = 0;
    out <<endl<<endl << "Shortest Remaining Job First Scheduling" << endl;

    while (1) {

        //put job into ready vector
        if (!SRJF_all_jobs.empty()) {
            while (SRJF_all_jobs.front().get_atime() <= count) {
                arrange.push_back(SRJF_all_jobs.front());
                SRJF_all_jobs.pop();
                if (SRJF_all_jobs.empty()) break;
            }
        }
        //put shortest remaining burst time process in front 
        sortvector(arrange);

        //finish state
        if (arrange.empty() && SRJF_all_jobs.empty()) {
            out << count << " complete" << endl << endl;
            break;
        }

        if (arrange.empty()) {
            count++;
            continue;
        }

        //increment turnaround time and decrease remaining burst time
        //Since its SRJF, one extra step is needed
        //if vector have more than 1 process is waiting, need to compare burst time to see if preemption is necessary
        if (arrange.size() > 1) {
            if (arrange[0].get_btime() <= arrange[1].get_btime()) {
                arrange.front().ttime_increase();
            }
            else {
                //perform preemption
                arrange.front().load_wadj(count);
                process_token = arrange[0];
                arrange[0] = arrange[1];
                arrange[1] = process_token;
                arrange.front().ttime_increase();
                out << count << " " << arrange.front().get_ID() << "    " << "Process preempted by process with shorter burst time" << endl;
            }
        }
        else {
            arrange.front().ttime_increase();
        }

        //print state when something occur
        if (process_timer == 0) {
            out << count << " " << arrange.front().get_ID() << "    " << "Process terminated" << endl;
            arrange.front().wtime_increase(count);
        }

        //if remaining burst time reach 0, switch to the next process
        if (arrange.front().get_btime() == 0) {
            for_record.push_back(arrange.front());
            arrange.erase(arrange.begin());
            process_timer = 0;
            count++;
            continue;
        }
        process_timer++;
        count++;

    }

    //printing record for SRJF
    out << "PrcessID  " << "Turnaround Time  " << "Waiting Time" << endl;
    while (!for_record.empty()) {
        //sort before output
        sort_output(for_record);
        turnaround = for_record.front().get_ttime() + for_record.front().get_wtime();
        avg_turnaround += turnaround;
        avg_wait += for_record.front().get_wtime();
        out << "    " << for_record.front().get_ID() << "           " << turnaround << "                " << for_record.front().get_wtime() << endl;
        for_record.erase(for_record.begin());
    }
    out << "Average    " << avg_turnaround << "/" << size << "=" << (avg_turnaround / size) << "        " << avg_wait << "/" << size << "=" << (avg_wait / size) << endl;

    in.close();
    out.close();
    
    return 0;
}
