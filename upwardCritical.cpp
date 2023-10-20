/* 
input format:


#nodes #edges
u1 v1 cap1
u2 v2 cap2
.
.
.
um vm capm
 */

#include <bits/stdc++.h>
using namespace std;

bool bfs(vector<vector<int>> &g, int s, int t,vector<int> &par){
    par[s]=-1;
    queue<int> q;
    vector<int> vis(g.size(),0);
    q.push(s);
    vis[s]=1;

    while(!q.empty()){
        int u=q.front();
        q.pop();
        for(int v=0;v<g.size();v++){
            if(!vis[v] and g[u][v]>0){
                vis[v]=1;
                q.push(v);
                par[v]=u;
                if(v==t) return true;
            }
        }
    }
    return false;
}

void dfs(int u, int s, int t,vector<vector<int>> &g,vector<vector<int>> &gOrig, vector<int> &vis,
            vector<vector<pair<int,int>>> &ans, vector<pair<int,int>> &upCriticalEdgeSet){
    if(u==s){
        ans.push_back(upCriticalEdgeSet);
        return;
    }
    if(vis[u]) return;
    vis[u]=1;
    for(int v=0;v<g.size();v++){
        if(g[u][v]>0){
            if(gOrig[v][u]==g[u][v]){
                upCriticalEdgeSet.push_back({v,u});
            }
            dfs(v,s,t,g,gOrig,vis,ans,upCriticalEdgeSet);
            if(gOrig[v][u]==g[u][v]){
                upCriticalEdgeSet.pop_back();
            }
        }
    }
}

int main(){
    int n,m;
    cin>>n>>m;
    int f;
    int x,y;
    vector<vector<int>> g(n,vector<int>(n,0)),gOrig(n,vector<int>(n,0));
    for(int i=0;i<m;i++){
        cin>>x>>y>>f;
        g[x][y]=f;
        gOrig[x][y]=f;
    }
    int max_flow=0;
    vector<int> par(n);
    int s=0,t=n-1;

    while(bfs(g,s,t,par)){
        int bottleneck = 1e9;
        for(int v=t;v!=s;v=par[v]){
            int u=par[v];
            bottleneck = min(bottleneck,g[u][v]);
        }
        for(int v=t;v!=s;v=par[v]){
            int u=par[v];
            g[u][v]-=bottleneck;
            g[v][u]+=bottleneck;
        }
        max_flow+=bottleneck;
    }
    cout<<endl<<"max flow = "<<max_flow<<endl<<endl;
    vector<int> vis(n,0);
    vector<vector<pair<int,int>>> ans;
    vector<pair<int,int>> upCriticalEdgeSet;
    cout<<"Upward Critical edges:\nCapacity of all the edges in a set must be increased to increase max flow\n\nSets:"<<endl;
    dfs(t,s,t,g,gOrig,vis,ans,upCriticalEdgeSet);
    for(auto x:ans){
        cout<<"{ ";
        reverse(x.begin(),x.end());
        for(auto p:x){
            cout<<p.first<<"-"<<p.second<<"  ";
        }
        cout<<"}"<<endl;
    }
    return 0;
}
