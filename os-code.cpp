#include <bits/stdc++.h>

using namespace std;

class Jobs {
public:
    map<int, pair<int,int> > j_dict; //job dict
    //map<int, int> comp; //completion time dict 
    map<int, int> wt_idx;
    int count;
    int p;
    vector<int> R;
    Jobs(int comp) {
        count = 0;
        p = comp;
    }
    void insert(int key, pair<int,int> value) {
        //insert function
        j_dict[key] = value;
        R.push_back(value.first);
        count++;
    }

    map<int, vector<pair<int,int> > > group_by() {
        //groups by weight
        map<int, vector<pair<int,int> > > groups;
        map <int, pair<int,int> >::iterator it;
        for (it = j_dict.begin(); it != j_dict.end(); it++) {
            int job_id = it->first;
            int ri = (it->second).first;
            int alpha = (it->second).second;
            if (groups.find(alpha) == groups.end()) {
                vector<pair<int,int> > list;
                list.push_back(make_pair(ri, job_id));
                groups[alpha] = list;
            }
            else {
                groups[alpha].push_back(make_pair(ri, job_id));
            }
        }
        map<int, vector<pair<int,int> > >::iterator it2;
        int i = 0;
        for (it2 = groups.begin(); it2 != groups.end(); it2++) {
            wt_idx[i] = it2->first;
            i++;
            sort((it2->second).begin(),(it2->second).end()); 
        }
        return groups;
    }

    void printer() {
        map <int, pair<int,int> >::iterator it;
        for (it = j_dict.begin(); it != j_dict.end(); it++) {
            cout<<"Job "<<it->first<<" = ";
            cout<<(it->second).first<<" "<<(it->second).second<<endl;
        }
    }
};

Jobs parse_txt(int P) {
    //parses the text file
    fstream infile;
    infile.open("input.txt", ios::in);
    cout<<"Input"<<endl;
    Jobs job(P);
    if (infile.is_open()) {
        string tp;
        while (getline(infile, tp)) {
            if (tp.length() <= 1)
                break;
            int no, rt, c, w;
            stringstream s(tp);
            s>>no;
            s>>rt;
            //s>>c;
            s>>w;
            job.insert(no, make_pair(rt, w));
        }
        infile.close();
    }
    return job;
}

bool sortcol( const vector<int>& v1, const vector<int>& v2 ) { 
    int s = v1.size();
    for (int i = 0; i < s; i++) {
        if (v1[i] != v2[i])
            return (v1[i] < v2[i]);
    } 
} 

map<int, vector<int> > permute_t(int k, map<int, vector<pair<int,int> > > groups){

    vector<int> no_elems_each(k, 0);
    int ind = 0;
    int curr_max_perm = 1;
    map<int, vector<pair<int,int> > >::iterator it;
    for(it = groups.begin();it!=groups.end();it++){
       int no_elem = (it->second).size();
       curr_max_perm *= no_elem;
       no_elems_each[ind] = no_elem;
       ind++;
    }
    
    int start_id = 0;
    map<int , vector<int> > hash;
   
    for(int l=0; l<curr_max_perm; l++){
       vector<int> curr_t(k, 0);
       int perm_no = l;
       for(int m=curr_t.size()-1;m>=0;m--){
         int curr_ord = perm_no%(no_elems_each[m]);
         curr_t[m] = curr_ord;
         perm_no = perm_no/(no_elems_each[m]); 
       }
     
       hash.insert(make_pair(start_id, curr_t));
       // cout<<start_id<<": ";
       // for(int m=0;m<curr_t.size();m++)
       //      cout<<curr_t[m]<<" ";
       //  cout<<endl;
       start_id++;
     }    

   return hash;
}

map<int, vector<int> > Q(Jobs &j, map<int, vector<pair<int,int> > >& groups, vector<int> t, int x, int y, int z) {
    //vector<int> job_ids;
    int t_max = 0;
    for (int i = t.size()-1; i >= 0; i--)
        t_max = (t[i] > 0)? i : t_max;

    map<int, vector<int> > Q_jobs;
    for (int i = 0; i < t.size(); i++) {
         if (t[i] == 0)
            continue;
        int wt_class = j.wt_idx[i];
        vector<pair<int,int> > curr_class = groups[wt_class];
        vector<int> curr_jobs;
        for (int j = 0; j <= t[i]; j++) {
            if (i == t_max && j == t[t_max]) {
                if ((curr_class[j].first >=x) && (curr_class[j].first < z))
                    curr_jobs.push_back(curr_class[j].second);
            }
            else {
                if ((curr_class[j].first >=y) && (curr_class[j].first < z))
                    curr_jobs.push_back(curr_class[j].second);
            }
        }
        cout<<endl;
        cout<<"W = "<<wt_class<<" ";
        for (int l = 0; l < curr_jobs.size(); l++) {
            cout<<curr_jobs[l]<<",";
        }
        cout<<endl;
        Q_jobs[wt_class] = curr_jobs;
    }
    return Q_jobs;
}

vector<int> t1(vector<int> t, int i) {
    t[i]--;
    return t;
}

vector<int> t11(vector<int> t, int ind) {
    for (int i  = 0; i < ind; i++)
        t[i] = 0;
    return t;
}

int r(map<int, vector<pair<int,int> > >& groups, int y, vector<int> t) {

    vector<int> r_i;
    int ind = 0;
    map<int, vector<pair<int,int> > >::iterator itt;
    for(itt=groups.begin();itt!=groups.end();itt++){
        vector<pair<int,int> > v = itt->second;
        for(int j=0;j<=t[ind];j++){
            r_i.push_back(v[j].first);
        }
    }

    int vals = r_i[0];
    for (int i = 0; i < r_i.size(); i++) {
        if (r_i[i] >= y)
            vals = min(vals, r_i[i]);
    }
    return vals;
}

int countQ(map<int, vector<int> > Q_jobs) {
    map<int, vector<int> >::iterator it;
    int count = 0;
    for (it = Q_jobs.begin(); it != Q_jobs.end(); it++) {
        count += (it->second).size();
    }
    return count;
}

bool feasible(Jobs& j, map<int, vector<int> > jobs_q, int y, int z) {
  //looks like the jobs are already sorted by release time
    //need to check again though
    int c = y;
    //linearize
    map<int, vector<int> >::iterator it;
    vector<int> rt;
    for (it = jobs_q.begin(); it != jobs_q.end(); it++) {
        vector<int> curr = it->second;
        for (int i = 0; i < curr.size(); i++) {
            rt.push_back(j.j_dict[curr[i]].first);
        }
    } 
    sort(rt.begin(), rt.end());
    for (int i = 0; i < rt.size(); i++) {
        c = max(rt[i], c) + j.p;
    }
    return (c<=z);
}

void schedule(Jobs& j, map<vector<int>, int>& t_map, map<int, vector<pair<int,int> > >& groups, vector<vector<int> >& vv, int k) {
    ///this function assumes that you have a lexicographic ordering of t's
    vector<int> r_array = j.R;
    int p = j.p;
    int l = j.count;
    sort(r_array.begin(), r_array.end());
    set<int> T;
    vector<int> T_vec;
    int min_T=INT_MAX, max_T=INT_MIN;
    cout<<"T creation start"<<endl;
    for (int i = 0; i < r_array.size(); i++) {
        for (int j = 0 ; j <= l; j++) {
            T.insert((r_array[i] + j*p));
            min_T = min(min_T, (r_array[i] + j*p));
            max_T = max(max_T, (r_array[i] + j*p));
        }
    }
    cout<<"T creation end"<<endl;
    cout<<min_T<<" "<<max_T<<endl;
    int t_s = t_map.size();

    set<int>::iterator iter_set;
    for(iter_set=T.begin();iter_set!=T.end();iter_set++){
        T_vec.push_back(*iter_set);
    }
    
    vector<vector<vector<vector<int> > > > F(t_s, vector<vector<vector<int> > >(max_T+1, vector<vector<int> >(max_T+1, vector<int>(max_T+1, 999))));
    //initialization
    cout<<"INIT start"<<endl;
    
    vector<int> zero(k, 0);
    for (int x_i = 0; x_i < r_array.size(); x_i++) {
        for (int z_i = 1; z_i <= l; z_i++) {
            for (int y_i = 0; y_i < z_i; y_i++) {
                int x = r_array[x_i];
                int y = x + y_i*p;
                int z = x + z_i*p;
                F[t_map[zero]][x][y][z] = 0;
            }
        }
    }
    cout<<"INIT_DONE"<<endl;
    for (int t_i = 1; t_i < vv.size(); t_i++) {
        vector<int> t = vv[t_i];
        cout<<"T I ="<<t_i<<endl;
        for (int x_i = 0; x_i < r_array.size(); x_i++) {
            for (int z_i = 0; z_i < T_vec.size(); z_i++) {
                if(T_vec[z_i]<=r_array[x_i])
                    continue;
                for (int y_i = z_i - 1; y_i >= 0; y_i--) {
                        int x = r_array[x_i];
                        int y = T_vec[y_i]; //x + y_i*p;
                        int z =  T_vec[z_i]; //x + z_i*p;
                        //need to make this efficient
                        cout<<"******"<<x<<","<<y<<","<<z<<endl;
                        int t_max = 0;
                        for (int i = t.size()-1; i>=0; i--)
                            t_max = (t[i] > 0)? i : t_max;
                        //cout<<"Q init"<<endl;
                        map<int, vector<int> > q_jobs = Q(j, groups, t, x, y, z);
                        //cout<<"Q end"<<endl;
                        int q_size = countQ(q_jobs);
                        //cout<<"Q counted"<<endl;
                        if (q_size == 0) {
                            cout<<"NO ele "<<t_map[t]<<","<<t_s<<endl;
                            F[t_map[t]][x][y][z] = 0;
                        }
                            
                        else if (not feasible(j, q_jobs, y, z)) {
                            cout<<"infisible"<<endl;
                            F[t_map[t]][x][y][z] = INT_MAX;
                        }
                        else {
                            bool modify = false;
                            cout<<"modified"<<endl;
                            for (int i = 0; i < t.size(); i++) {
                                if (t[i] == 0)
                                    continue;
                                cout<<"curr_I="<<i<<endl;
                                int wt_class = j.wt_idx[i];
                                // vector<int> q_i = q_jobs[wt_class];
                                // cout<<"Q_size = "<<q_i.size()<<endl;
                                // if (q_i.size() == 0)
                                //     continue;
                                // int ti = q_i[q_i.size()-1];
                                vector<pair<int,int> > curr_job = groups[wt_class];
                                int ti = t[i];
                                int r_ti = j.j_dict[curr_job[ti].second].first;
                                cout<<"HERE "<<ti<<" , "<<r_ti<<" w = "<<wt_class<<endl;
                                if (i == t_max) {
                                    if ((r_ti<x) or (r_ti>=z)) {
                                        cout<<"MODIFY 1"<<endl;
                                        int y_new = r(groups, y, t);
                                        F[t_map[t]][x][y][z] = F[t_map[t1(t, i)]][y_new][y_new][z];
                                        modify = true;
                                    }
                                }
                                else {
                                    if ((r_ti<y) or (r_ti>=z)) {
                                        cout<<"MODIFY 2"<<endl;
                                        //int y_new = r(r_array, y);
                                        F[t_map[t]][x][y][z] = F[t_map[t1(t, i)]][x][y][z];
                                        modify = true;
                                    }
                                }
                                cout<<"end of "<<modify<<" "<<i<<endl;
                            }


                            if (not modify) {
                                cout<<"DEVIL"<<endl;
                                for (int s = y+1; s < z; s++) {
                                    if (T.find(s) == T.end())
                                        continue;
                                    for (int i = 0; i < t.size(); i++) {
                                        if ((t[i] == 0) && (i == t_max))
                                            continue;
                                        int y_new = r(groups, y, t);;
                                        F[t_map[t]][x][y][z] = F[t_map[t1(t, t_max)]][y_new][y_new][s] + F[t_map[t]][x][s][z];
                                        F[t_map[t]][x][y][z] = min(F[t_map[t1(t, i)]][x][s][z] + F[t_map[t11(t, i)]][y_new][s][z], F[t_map[t]][x][y][z]);
                                        int e = y + p*q_size;
                                        F[t_map[t]][x][y][z] = min(F[t_map[t1(t, t_max)]][x][s][e] + e * j.wt_idx[t_max], F[t_map[t]][x][y][z]);

                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    cout<<"ans "<<F[t_map[vv[vv.size()-1]]][min_T][min_T][max_T];  
   //return F[t_map[vv[vv.size()-1]]][min_T][min_T][max_T];
    //return 0;
}


int main() {
    int p;
    cout<<"Enter the processing time"<<endl;
    cin>>p;
    Jobs j = parse_txt(p);
    j.printer();
    map<int, vector<pair<int,int> > > sort_w = j.group_by();
    map<int, vector<pair<int,int> > >::iterator it;
    int num_wt = 0;
    vector<int> sizes_p;
    for (it = sort_w.begin(); it != sort_w.end(); it++) {
        int weight = it->first;
        num_wt++;
        vector<pair<int, int> > curr = it->second;
        sizes_p.push_back(curr.size());
        cout<<"Weight= "<<weight<<endl;
        for (int i = 0; i < curr.size(); i++) {
            cout<<"Job "<<curr[i].second<<" RT "<<curr[i].first<<" C "<<j.p<<endl;
        }
    }

    int k = sort_w.size();
    map<int , vector<int> > hash = permute_t(k, sort_w);  
   
    map<int , vector<int> >::iterator it2;
    vector<vector<int> > vv;
    for (it2 = hash.begin(); it2 != hash.end(); it2++)
        vv.push_back(it2->second);
    cout<<"NUM unique weights = "<<num_wt<<endl;
    
    sort(vv.begin(), vv.end(), sortcol);
    map<vector<int>, int> t_map;
    for (int i = 0; i < vv.size(); i++) {
        cout<<i<<" ";
        vector<int> curr = vv[i];
        for (int j = 0 ; j < curr.size(); j++) {
            cout<<curr[j]<<",";
        }
        t_map[curr] = i;
        cout<<endl;
    }
    schedule(j, t_map, sort_w, vv, k);
    //cout<<"SCHEDULE "<<ans<<endl;
    return 0;
}

