#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <cstring>

using namespace cv;
using namespace std;

struct flow_graph{
    static const int MAX_V = 100000000;
    static const int MAX_E = 100000000;
    
    int E,s,t,head,tail;
    int cap[2 * MAX_E],to[2 * MAX_E],next[2 * MAX_E],last[MAX_V],dist[MAX_V],q[MAX_V],now[MAX_V];
    
    flow_graph(){
        E = 0;
        memset(last,-1,sizeof last);
    }
    
    void clear(){
        E = 0;
        memset(last,-1,sizeof last);
    }
    
    void add_edge(int u, int v, int uv){
        to[E] = v, cap[E] = uv, next[E] = last[u]; last[u] = E++;
        to[E] = u, cap[E] = 0, next[E] = last[v]; last[v] = E++;
    }
	
    bool bfs(){
        memset(dist,-1,sizeof dist);
        head = tail = 0;
		
        q[tail] = t; ++tail;
        dist[t] = 0;
		
        while(head < tail){
            int v = q[head]; ++head;
            
            for(int e = last[v];e != -1;e = next[e]){
                if(cap[e^1] > 0 && dist[to[e]] == -1){
                    q[tail] = to[e]; ++tail;
                    dist[to[e]] = dist[v]+1;
                }
            }
        }
        
        return dist[s] != -1;
    }
	
    int dfs(int v, int f){
        if(v == t) return f;
		
        for(int &e = now[v];e != -1;e = next[e]){
            if(cap[e] > 0 && dist[to[e]] == dist[v]-1){
                int ret = dfs(to[e],min(f,cap[e]));
				
                if(ret > 0){
                    cap[e] -= ret;
                    cap[e^1] += ret;
                    return ret;
                }
            }
        }
		
        return 0;
    }
	
    int max_flow(int source, int sink, int V){
        s = source; t = sink;
        int f = 0,df;
		
        while(bfs()){
            for(int i = 0;i < V;++i) now[i] = last[i];
			
            while(true){
                df = dfs(s,INT_MAX);
                if(df == 0) break;
                f += df;
            }
        }
		
        return f;
    }
};

#define MAXT 500

int T = 0,Y,X;
Mat frame[MAXT],grays[MAXT];
Mat frameIn[MAXT],graysIn[MAXT];
Mat frameOut[MAXT],graysOut[MAXT];

int id(int t, int y, int x){
	return t * Y * X + y * X + x;
}

void remove_vertical_seam(){

}

int main(){
    VideoCapture capture;
    capture.open("../videos/basketball.avi");

    while(true){
    	capture >> frame[T];
    	if(frame[T].empty())
    		break;
    	
    	//cout << frame[T].rows << " " << frame[T].cols << endl;
    	cvtColor(frame[T],grays[T],CV_BGR2GRAY);

    	imshow("seam-carving",grays[T]);
    	if(waitKey(2) == 'q') break;
    	++T;
    }

    cout << T << endl;

	return 0;
}