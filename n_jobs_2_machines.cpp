#include <bits/stdc++.h>
using namespace std;

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
    for( int i : sequence ){
        vector<int> temp = {i,time1,time1+all_jobs[i-1][0]};
        time1 += all_jobs[i-1][0];
        time2 = max(time1,prev_time2);
        temp.push_back(time2);
        temp.push_back(time2+all_jobs[i-1][1]);
        time2 += all_jobs[i-1][1];
        prev_time2 = time2;
        elapse_time_array.push_back(temp); // push
    }
}

void show_elapsed_time_grid(int n, vector<vector<int>> & elapse_time_array ){
    cout<<"printing the elapse_time_array \n";
    int m1_idle_time = 0; // its always '0' so need to do anything...
    int m2_idle_time = 0;
    int prev_m2_out_time = 0;
    for( auto v : elapse_time_array ){
        char ch = 65 + v[0] -  1;
        cout<<ch<<"    ";
        for( int i = 1; i<5; i++ ) cout<<v[i]<<"    ";
        cout<<endl;
        m2_idle_time += v[3] - prev_m2_out_time;
        prev_m2_out_time = v[4];
    }
    cout<<"Minimum elapsed time = "<<elapse_time_array[n-1][4]<<endl;
    cout<<"Machine-1 idle time = "<<m1_idle_time<<endl;
    cout<<"Machine-2 idle time = "<<m2_idle_time<<endl;
}


int main() {
    cout<<"--- Program for JOHNSON METHOD for 3 machines N jobs. ---"<<endl;
	cout<<"input the number of jobs : ";
    cout<<endl;
    int n; cin>>n;
    cout<<"Number of jobs = "<<n<<endl;
    cout<<"Enter the Machine-1 and Machine-2 time for the jobs (separated by a space) : "<<endl;
    priority_queue<vector<int>, vector<vector<int>>, greater<vector<int>>> jobs; // <time,machine,job>
    vector<int> sequence(n,0);
    vector<int> assigned(n+1,0); // n+1 q ki we are checkign assigned[job] and jobs range from 1 to n , means 'n' included that's why we'll need n+1 cells
    vector<vector<int>> all_jobs; // {m1_time,m2_time} - the index will tell us the name of the job
    // taking input 
    for( int i = 1; i<=n; i++ ){
        int m1_time,m2_time;  cin>>m1_time>>m2_time;
        all_jobs.push_back({m1_time,m2_time}); // pushing in the main "all_jobs" array
        jobs.push({m1_time,1,i});  // <time,machine,job>
        jobs.push({m2_time,2,i});
    }

    // calculate the sequence of jobs 
    calculate_sequence(n,sequence,assigned,jobs);
    show_sequence(sequence);

    // now calculate the elapsed time
    vector<vector<int>> elapse_time_array; //{job,m1_in_time,m1_out_time,m2_in_time,m1_out_time}
    calculate_elapsed_time(all_jobs,sequence,elapse_time_array);
    show_elapsed_time_grid(n,elapse_time_array); // shows idle time as well
    
	return 0;
}

// calculate the sequence : done
// designing the in/out function : done
// display : done
// file IO ??
// 

// t.c. - 0
// 7
// 6 16
// 24 20
// 30 20
// 12 13
// 20 24
// 22 2
// 18 6