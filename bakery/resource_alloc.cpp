#include <bits/stdc++.h>
using namespace std;
#define N 2500

int pr, r, q;

vector<int> p;                    // process_ids
multimap<int, int> arr_pr;        // arrival time of a process
vector<int> r_avail;              // resources_available
vector<int> max_req[N];           // total requirement of res by pr
vector<int> curr_al[N];           // current allocated resources
vector<int> remain_need[N];       // remaining need of a process
vector<vector<int> > que[N];      // request query
vector<int> in(N, 0);             // maintaing index of current request for a process
set<pair<int, int> > s1;          // set to store processes whcih can run now

queue<int> seq;                   // execution sequence of processes
long long int t = 0;
multimap<int, int>::iterator it;

void take_data()
{
    cin >> pr >> r >> q;
    for (int i = 0; i < pr; i++)
    {
        int a;
        cin >> a;
        p.push_back(a);
    }

    for (int i = 0; i < pr; i++)
    {
        int a;
        cin >> a;
        arr_pr.insert({ a, p[i] });
    }
    for (int i = 0; i < r; i++)
    {
        int a;
        cin >> a;
        r_avail.push_back(a);
    }

    for (int i = 0; i < pr; i++)
    {
        for (int j = 0; j < r; j++)
        {
            int a;
            cin >> a;
            max_req[p[i]].push_back(a);
        }
    }

    for (int i = 0; i < pr; i++)
    {
        for (int j = 0; j < r; j++)
        {
            int a;
            cin >> a;
            curr_al[p[i]].push_back(a);
        }
    }

    for (int i = 0; i < pr; i++)
    {
        for (int j = 0; j < r; j++)
        {
            remain_need[p[i]].push_back(max_req[p[i]][j] - curr_al[p[i]][j]);
        }
    }

    for (int i = 0; i < q; i++)
    {
        int a;
        cin >> a;
        vector<int> req;
        for (int i = 0; i < r; i++)
        {
            int r;
            cin >> r;
            req.push_back(r);
        }
        que[a].push_back(req);
    }
}

void ready_set()
{
    for (it; it != arr_pr.end(); it++)
    {
        if (it->first <= t)
         s1.insert({ it->second, 0});
        else
            break;
    }
}

int main()
{
    take_data();

    t = arr_pr.begin()->first;
    it = arr_pr.begin();
    ready_set();
    set<pair<int, int> >::iterator s_it;
    s_it = s1.begin();
    int flag = 0;
    for (s_it = s1.begin(); s_it != s1.end();)
    {
        if (s_it->second == INT_MAX)
        {
            s_it++;
             if(s_it==s1.end())
               {     
                if(it!=arr_pr.end())
                 {
                 t++; 
                 ready_set();
                 s_it = s1.begin();
                 }
               }
            continue;
        }
        else if (s_it->second == 1)
        {
            int p2 = s_it->first;
            vector<vector<int> >::iterator y = que[p2].begin();
            int k = 0;
            for (int j = 0; j < r; j++)
            {
                if (y[in[p2]][j] <= r_avail[j])
                    continue;
                else
                {
                    k = 1;
                    break;
                }
            }
            if (k)
            {
                cout << "Deny" << endl;
                s_it++;
                if (s_it == s1.end())
                {
                if (it != arr_pr.end())
                {
                    t++;
                    ready_set();
                    s_it = s1.begin();
                }
                }
                continue;
            }
        }
        int p1 = s_it->first;
        vector<vector<int> >::iterator x = que[p1].begin();
        int f = 0;
        for (int j = 0; j < r; j++)
        {
            if (x[in[p1]][j] <= r_avail[j])
                continue;
            else
            {
                f = 1;
                break;
            }
        }
        if (!f) // request can be satisfied
        {
            seq.push(p1);
            int c = 0;
            cout << "Grant   " << endl;
            for (int j = 0; j < r; j++)
            {
                r_avail[j] = r_avail[j] - x[in[p1]][j];
                curr_al[p1][j] = curr_al[p1][j] + x[in[p1]][j];
                remain_need[p1][j] = remain_need[p1][j] - x[in[p1]][j];
                if (remain_need[p1][j] != 0)
                {
                    c = 1;
                }
            }
            in[p1]++;
            if (!c) // all request are granted
            {
                s1.erase({ p1, 1 });
                s1.erase({ p1, 0 });
                s1.insert({ p1, INT_MAX });
                for (int j = 0; j < r; j++)
                    r_avail[j] = r_avail[j] + curr_al[p1][j];
                    
                if(it!=arr_pr.end())  
                t++;
                ready_set();
                s_it = s1.begin();
            }
            else // request partially granted
            {
                s1.erase({ p1, 1 });
                s1.erase({ p1, 0 });
                s1.insert({ p1, 0 });
                /* if(s_it==s1.end())
                {
                if(it!=arr_pr.end())
                 t++;
                }
                else */ 
                if(it!=arr_pr.end())
                t++;
                ready_set();
                s_it = s1.begin();
            }
        }
        else // request cannot be satisfied
        {
            s1.erase({ p1, 1 });
            s1.erase({ p1, 0 });
            s1.insert({ p1, 1 });
            cout << "Deny" << endl;
            s_it++;
            if (s_it == s1.end())
            {
                if (it != arr_pr.end())
                {
                    t++;
                    ready_set();
                    s_it = s1.begin();
                }
                else
                {
                    cout << "Not safe" << endl;
                    flag = 1;
                    break;
                }
            }
        }
        
    }

    while (!seq.empty() && !flag)
    {
        cout << seq.front() << " ";
        seq.pop();
    }

    return 0;
}
