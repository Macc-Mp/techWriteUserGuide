using namespace std;
#include <iostream>
#include <stdlib.h>
#include <limits.h>
//Manalo, Paule CS231A OS Lab Interim

struct processParam {
    char job;
    bool finished;
    int AT;
    int BT;
    int ST;
    int ET;
    int TT;
    int WT;
    int RT;
};

typedef struct processParam param;

int jobLength = 0, smallerJob = 0, minRT = 0, totalBT = 0;
float totalWT = 0, totalTT = 0;
int curr = 0, complete = 0;


static int findSmallestJob(param proc[], int currentTime) {
    smallerJob = -1;
    minRT = INT_MAX;

    for (int i = 0; i < jobLength; ++i) {
        if (proc[i].finished == false) {
            if (proc[i].AT <= currentTime && proc[i].RT < minRT) {
                minRT = proc[i].RT;
                smallerJob = i;
                //cout << proc[i].job << currentTime << ", ";
            }
            else if (proc[i].AT <= currentTime && proc[i].RT == minRT) {
                if (smallerJob == -1 || i < smallerJob) {
                    smallerJob = i;
                }
            }
        }

    }

    return smallerJob;
}

static int  getTotalBT(param proc[]) {
    totalBT = 0;
    for (int i = 0; i < jobLength; i++) {
        totalBT += proc[i].BT;
    }
    cout << "Total BT: " << totalBT << endl;
    return totalBT;
}

int shortest;

static int getST(param proc[], int index, int currentTime) {
    if (proc[index].ST == -1) {//occupy 0
        proc[index].ST = currentTime;
    }
    return proc[index].ST;
}


static float getAWT(param proc[]) {
    totalWT = 0;
    for (int i = 0; i < jobLength; i++) {
        totalWT += proc[i].WT;
    }
    float AWT = totalWT / jobLength;
    cout << totalWT << "/" << jobLength << " AWT: " << AWT << endl;
    return AWT;
}

static float getATT(param proc[]) {
    totalTT = 0;
    for (int i = 0; i < jobLength; i++) {
        totalTT += proc[i].TT;
    }
    float ATT = totalTT / jobLength;
    cout << totalTT << "/" << jobLength << " AWT: " << ATT << endl;
    return ATT;
}

int main() {
    bool flag = true;
    while (flag) {
        cout << "SRTF ALGO: " << endl;

        const int job = 5;
        jobLength = job;
        param proc[job];
        char jobsName[5] = { 'A', 'B', 'C', 'D', 'E' };

        for (int i = 0; i < job; ++i) {
            proc[i].job = jobsName[i];
            cout << "Enter AT of Job[" << proc[i].job << "]: ";
            cin >> proc[i].AT;

            if (proc[i].AT >= 0) {
                cout << "Enter BT of Job[" << proc[i].job << "]: ";
                cin >> proc[i].BT;

                if (proc[i].AT < 0 || proc[i].BT < 0 || proc[i].BT == 0) {
                    cout << "Invalid input on Job[" << proc[i].job << "]: " << endl;
                    cin.clear();
                    i--;
                }
                else if (proc[i].AT >= 0 && proc[i].BT > 0) {
                    proc[i].RT = proc[i].BT; //RT is equal as BT till it runs
                    proc[i].finished = false;
                    proc[i].ST = -1; //job not started
                    cout << "=========" << endl;
                }

            }
           
            else if (cin.fail()){
                cout << "\n===Input is not a valid number===\n";
				//flag = false;
				cin.clear();
                ;exit(1);
            }
            else {
				cout << "Invalid input on Job[" << proc[i].job << "]: " << endl;
                i--;
            }

        }

        while (complete != job) {
            shortest = findSmallestJob(proc, curr);
            getST(proc, shortest, curr);
            if (shortest == -1) {//update current time by 1 if no job arrives
                curr++;
                continue;
            }
            else if (proc[shortest].RT == 0) {//no remaining burst time ends the process. compute its ET,TT,WT    
               
                proc[shortest].ET = curr;// last updated current time till it reaches 0
                proc[shortest].TT = proc[shortest].ET - proc[shortest].AT;
                proc[shortest].WT = (proc[shortest].TT) - (proc[shortest].BT);
                proc[shortest].finished = true;//flag as true so it can ignored by findSmallestJob
                complete++;//process done, add to complete
            }
            else {//reduce rt if job arrives
                proc[shortest].RT--;
                curr++;
            }

            if (proc[shortest].finished == true) {
                cout << proc[shortest].job << curr << " ";
            }

        }
        cout << "\n*******Results*******" << endl;

        getTotalBT(proc);

        cout << "Jobs" << "\t" << "AT" << "\t" << "BT" << "\t" << "ST" << "\t" << "ET" << "\t" << "WT" << "\t" << "TT" << endl;
        for (int i = 0; i < jobLength; i++) {
            cout << proc[i].job << "\t";
            cout << proc[i].AT << "\t";
            cout << proc[i].BT << "\t";
            cout << proc[i].ST << "\t";
            cout << proc[i].ET << "\t";
            cout << proc[i].WT << "\t";
            cout << proc[i].TT << endl;
        }

        cout << "total WT: ";
        getAWT(proc);
        cout << "total TT: ";
        getATT(proc);

        //either flag as false or continue
        cout << "===Retry? Any Key to continue/N or n to exit===: " << endl;
        char inp;
        cin >> inp;
        inp = toupper(inp);
        if (inp == 'N') {
            flag = false;
        }
        else {
            system("cls");
            continue;
        }
    }
    return 0;
}
