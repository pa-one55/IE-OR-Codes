#include <bits/stdc++.h>
using namespace std;

bool johnson_not_applicable( int min_m1_time, int max_m2_time, int min_m3_time ){
    // conditon for johnson to be applicable :
    // 1 -> min_m1_time >= max_m2_time
    // 2 -> min_m3_time >= max_m2_time
    // at least one of the above 2 should hold true
    if( min_m1_time >= max_m2_time ) return 0; // means johnson 'is' applicable
    if( min_m3_time >= max_m2_time ) return 0;
    return 1; // else return 'true' - mtlb johnson not applicable
}

void calculate_sequence(int n, vector<int> & sequence, vector<int> & assigned, priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> & jobs ){
    int left_ptr = 0;
    int right_ptr = n-1;
    while( !jobs.empty() ){
        vector<int> temp = jobs.top();
        jobs.pop();
        int time = temp[0];
        int machine = temp[1];
        int job = temp[2];
        // cout<<"time - "<<time<<endl;
        // cout<<"machine - "<<machine<<endl;
        // cout<<"job - "<<job<<endl;
        // cout<<"assigned["<<job<<"] = "<<assigned[job]<<endl;

        if( assigned[job] == 0 ){//means that job hasn't been assigned yet
            // cout<<"not assigned yet"<<endl;
            assigned[job] = 1;  // mark the job as assigned
            if( machine == 1 ){ // if the time is for the 1st machine
                sequence[left_ptr] = job; // put the job on the seq list
                // cout<<"assigned position "<<left_ptr<<" to job - "<<job<<endl;
                left_ptr++; // move the pointer
            }
            else if( machine == 2 ){
                sequence[right_ptr] = job;
                // cout<<"assigned position "<<right_ptr<<" to job - "<<job<<endl;
                right_ptr--;
            }
        }
        // else cout<<"already assigned\n";
    }
}

void show_sequence( vector<int> & sequence ){
    cout<<"Here's the sequence : \n";
    for( auto it : sequence ) cout<<it<<" ";
    cout<<endl;
    for( auto it : sequence ) {
        char ch = 65 + it -  1;
        cout<<ch<<" ";
    }
    cout<<endl;
    
}

void calculate_elapsed_time( vector<vector<int>> & all_jobs, vector<int> & sequence, vector<vector<int>> & elapse_time_array ){
    int time1 = 0;
    int time2 = 0;
    int prev_time2 = 0;
    int time3 = 0;
    int prev_time3 = 0;
    for( int i : sequence ){
        vector<int> temp = {i,time1,time1+all_jobs[i-1][0]};
        time1 += all_jobs[i-1][0];
        time2 = max(time1,prev_time2);
        temp.push_back(time2);
        temp.push_back(time2+all_jobs[i-1][1]);
        time2 += all_jobs[i-1][1];
        prev_time2 = time2;
        time3 = max(time2,prev_time3);
        temp.push_back(time3);
        temp.push_back(time3+all_jobs[i-1][2]);
        time3 += all_jobs[i-1][2];
        prev_time3 = time3;
        elapse_time_array.push_back(temp); // push
    }
}

void show_elapsed_time_grid(int n, vector<vector<int>> & elapse_time_array ){
    cout<<"printing the elapse_time_array \n";
    int m1_idle_time = 0; // its always '0' so need to do anything...
    int m2_idle_time = 0;
    int prev_m2_out_time = 0;
    int m3_idle_time = 0;
    int prev_m3_out_time = 0;
    for( auto v : elapse_time_array ){
        char ch = 65 + v[0] -  1;
        cout<<ch<<"    ";
        for( int i = 1; i<7; i++ ) cout<<v[i]<<"    ";
        cout<<endl;
        // calculating idle time
        m2_idle_time += v[3] - prev_m2_out_time; // m2
        prev_m2_out_time = v[4];
        m3_idle_time += v[5] - prev_m3_out_time; // m3
        prev_m3_out_time = v[6];
    }
    cout<<"Minimum elapsed time = "<<elapse_time_array[n-1][6]<<endl;
    cout<<"Machine-1 idle time = "<<m1_idle_time<<endl;
    cout<<"Machine-2 idle time = "<<m2_idle_time<<endl;
    cout<<"Machine-3 idle time = "<<m3_idle_time<<endl;
}

int main() {
    cout<<"--- Program for JOHNSON METHOD for 3 machines N jobs. ---"<<endl;
	cout<<"input the number of jobs : ";
    cout<<endl;
    int n; cin>>n;
    cout<<"Number of jobs = "<<n<<endl;
    cout<<"Enter the Machine-1 Machine-2 and Machine-3 time for the jobs (separated by a space) : "<<endl;
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> jobs; // <time,machine,job>
    vector<int> sequence(n,0);
    vector<int> assigned(n+1,0); // n+1 q ki we are checkign assigned[job] and jobs range from 1 to n , means 'n' included that's why we'll need n+1 cells
    vector<vector<int>> all_jobs; // {m1_time,m2_time} - the index will tell us the name of the job
    // min/max times to check if JOHNSON is applicable or not, on the given data
    int min_m1_time = INT_MAX;
    int max_m2_time = INT_MIN;
    int min_m3_time = INT_MAX;
    // taking input 
    for( int i = 1; i<=n; i++ ){
        int m1_time,m2_time,m3_time;  cin>>m1_time>>m2_time>>m3_time;
        all_jobs.push_back({m1_time,m2_time,m3_time}); // pushing in the main "all_jobs" array
        jobs.push({m1_time+m2_time,1,i});  // <time,machine,job> Here time = t1+t2 for m1 and t2+t3 for m2
        jobs.push({m2_time+m3_time,2,i}); // and m1 and m2 are virtual machines introduced to convert the 3 machine problem into 2 machines and then apply JOHNSON METHOD
        // updating min/max time 
        min_m1_time = min(min_m1_time, m1_time);
        max_m2_time = max(max_m2_time, m2_time);
        min_m3_time = min(min_m3_time, m3_time);
    }

    if( johnson_not_applicable(min_m1_time,max_m2_time,min_m3_time) ){
        cout<<"Johnson not applicable for the given data. "<<endl;
        return 0;
    }
    else cout<<"Johnson's method is applicable for the given data."<<endl;
    // note : its - johnson_"NOT"_applicable

    // calculate the sequence of jobs 
    calculate_sequence(n,sequence,assigned,jobs);
    show_sequence(sequence);

    // now calculate the elapsed time
    vector<vector<int>> elapse_time_array; //{job,m1_in_time,m1_out_time,m2_in_time,m1_out_time}
    calculate_elapsed_time(all_jobs,sequence,elapse_time_array);
    show_elapsed_time_grid(n,elapse_time_array); // shows idle time as well
    
	return 0;
}

// seq calc : done
// the seq calc will be the same as that of njobs2machines
// just that we we'll replace m1_time by "m1_time+m2_time" 
// and m2_time by "m2_time+m3_time" while pushing into the queue

// elapsed time calc : done
// just need few extra vars, no new logic..

// t.c. - 0
// 7
// 3 4 6
// 8 3 7
// 7 2 5
// 4 5 11
// 9 1 5
// 8 4 6
// 7 3 12
// applicable