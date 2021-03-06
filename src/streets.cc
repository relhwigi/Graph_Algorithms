777#include <iostream>
#include <string>
#include <math.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include <list>
#include <vector>
#include <utility>
#include <queue>
#include <iomanip>
#include <climits>
#include <algorithm>


using namespace std;

list<int> row;
vector<list<int> > rows;
int query;
double vertices, edges;
typedef pair<int, int> iPair;

class coordinate{
    private:
    double x;
    double y;
public:
    coordinate(int x, int y){
        this->x = x;
        this->y = y;
    }
    double getX() const {
        return x;
    }
    double getY() const {
        return y;
    }
};

vector<coordinate> coordinates;

void findDegrees() {
    int max_vertex = 0;
    int max_degree = 0;
    float total_degree = 0;
    float average_degree = 0;
    
    for(int i = 1; i < rows.size(); i++) {
        int current_degree = rows[i].size();
        if (current_degree > max_degree) {
            max_vertex = i;
            max_degree = current_degree;
        }
        total_degree += current_degree;
    }
    
    average_degree = float (total_degree / (rows.size()-1));
    if(query == 2){
        cout << "v= " << max_vertex << "; |N(v)|= " << max_degree << "." << endl;
    } else if(query == 3){
        cout << "avg |N(v)|= " << fixed << setprecision(6) << average_degree << "." << endl;
    }
    
}

list<int> getNeighbours(int vertex, int hops){
    list<int> neighbours;
    neighbours.push_back(vertex);
    list<int> temp;
    
    while(hops > 0){
        hops--;
        list<int>::const_iterator it;
        for(it = neighbours.begin(); it != neighbours.end(); it++){
            list<int> r = rows[*it];
            list<int>::const_iterator it_1;
            for(it_1 = r.begin(); it_1 != r.end(); it_1++){
                if(*it_1 != *it && (!(find(temp.begin(), temp.end(), *it_1) != temp.end()))){
                    temp.push_back(*it_1);
                }
            }
        }
        neighbours = temp;
        temp.clear();
    }
    
    neighbours.sort();
    return neighbours;
}

float getDistance (int x1, int y1, int x2, int y2) {
    return sqrt ((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
    
}

void BFS (int sourceVertex, int destinationVertex) {
    list <int> l;
    l.push_back(sourceVertex);
   
    vector <bool> visited (vertices + 1, false);
    vector <int> previous (vertices + 1, 0);
    
    visited[sourceVertex] = true;
    while (!l.empty()) {
        int vertex = l.front();
        l.pop_front();
        list<int>::const_iterator it;
        list <int> n = getNeighbours (vertex, 1);
        for (it = n.begin(); it != n.end(); it++) {
            int next = *it;
            if (!visited[next]) {
                previous[next] = vertex;
                visited[next] = true;
                l.push_back(next);
            }
            if (next == destinationVertex) break;
        }
    }
    
    list <int> path;
    for (int i = destinationVertex; i != 0; i = previous[i]) {
        path.push_back(i);
    }
    
    path.reverse();
 
        /*cout << "Path: ";
            list<int>::const_iterator it;
            for(it = path.begin(); it != path.end(); it++){
                if(*it != destinationVertex){
                    cout << *it << " - ";
                }else{
                    cout << *it << " // optional output";;
                }
            }
        cout << endl;*/
        cout <<  "ed(" << sourceVertex << "," << destinationVertex << ")= " << path.size()-1 << "." << endl;
}

void djikstra(int sourceVertex, int destinationVertex){
    vector<int> distance(vertices+1, INT_MAX);
    vector<int> previous(vertices+1, 0);
    
    priority_queue<iPair, vector <iPair>, greater<iPair> > pq;
    
    pq.push(make_pair(0, sourceVertex));
    
    distance[sourceVertex] = 0;
    
    while(!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        list<int>::iterator it;
        list<int> n = rows[u];
        for(it = n.begin(); it != n.end(); it++){
            int v = *it;
            int cost = getDistance(coordinates[u].getX(), coordinates[u].getY(), coordinates[v].getX(), coordinates[v].getY());
            if(distance[v] > distance[u] + cost){
                distance[v] = distance[u] + cost;
                previous[v] = u;
                pq.push(make_pair(distance[v], v));
            }
        }
    }
    list<int> path;
    for(int i = destinationVertex; i != 0; i = previous[i]){
        path.push_back(i);
    }
    
    path.reverse();
    /*
    cout << "Path: ";
    list<int>::const_iterator it;
    for(it = path.begin(); it != path.end(); it++){
        if(*it != destinationVertex){
         cout << *it << " - ";
        }else{
         cout << *it << " // optional output";
        }
    }
        
    cout << endl;*/
        
        vector<int> result;
        float dist;
        list<int>::const_iterator c;
        for(c = path.begin(); c != path.end(); c++){
            result.push_back(*c);
        }
        
        for(int i = 0; i < result.size()-1; i++){
            coordinate v = coordinates[result[i]];
            coordinate u = coordinates[result[i+1]];
            float currentDistance = getDistance(v.getX(), v.getY(), u.getX(), u.getY());
            dist += currentDistance;
        }
        
        cout <<  "d(" << sourceVertex << "," << destinationVertex << ")=  "  << fixed << setprecision(6) << dist << "." << endl;
    
}

void readMat(vector<list <int> >& rows, double& vertices, double& edges, double& longestRow, double& sumDegrees, string filename){
    
    //string filename_graph = filename + ".osm.graph";
    string filename_graph = filename + ".osm.graph";
    string line;
    ifstream inGraph;
    inGraph.open(filename_graph);
    
    if(!inGraph){
        cout << "Unable to open graph file";
        exit(1);
    }
    
    //string filename_coordinates = filename +".osm.xyz";
    string filename_coordinates = filename + ".osm.xyz";
        ifstream inXYZ;
        inXYZ.open(filename_coordinates);

        if(!inXYZ){
           cout << "Unable to open coordinates file";
           exit(1);
       }
        coordinates.push_back(coordinate(0,0));
            while(getline(inXYZ, line)){
            std::istringstream lstream(line);
            double x, y, z;
            lstream >> x >> y >> z;
            coordinates.push_back(coordinate(x,y));
            }
        
    inXYZ.close();
    
    //getting the number of vertices
    bool found = false; //found number of vertices and edges
 
        while(getline(inGraph, line)){
         if(line.length() == 0 || line[0] == '%' || line[0] == '='){
             continue;
         } else {
             std::istringstream lstream(line);
             lstream >> vertices >> edges;
             break;
         }
      }
    
    //initialise every row of vector
    for(int i = 0; i < vertices+1; i++){
        rows.push_back(row);
    }
    
     int index = 1;
    while (getline(inGraph, line)) {
        int temp = 0;
       if(line.length() == 0 || line[0] == '%' || line[0] == '='){
            continue;
        } else {
            std::istringstream lstream(line);
            int value;
           
            while(lstream >> value){
                    rows[index].push_back(value);
                    sumDegrees++;
                    temp++;
                }
            index++;
            if(temp > longestRow) longestRow = temp;
            }
    }
            
        
    inGraph.close();
    
}

int main(){
    double longestRow = 0; //to keep track of longest row read in i.e most columns in a row
    double sumDegrees = 0;
    string filename;
    cin >> filename;
    readMat(rows, vertices, edges, longestRow, sumDegrees, filename);
    cin >> query;
    if(query == 1){
        cout <<"n= " << vertices << "; " << "m= " << edges << "." << endl; //order and size (n,m)
        exit(0);
    }
    if(query == 2 || query == 3){
        findDegrees();
        exit(0);
    }
    if(query == 4){
        int vertex, hops;
        cin >> vertex;
        list<int> result = getNeighbours(vertex,1);
        cout << "N(" << vertex << ")= ";
        list<int>::const_iterator it;
        for(it = result.begin(); it != result.end(); it++) {
             cout << *it << " " ;
         }
        cout << "." << endl;
        exit(0);
    }
    if(query == 5){
        int vertex, hops;
        cin >> vertex >> hops;
        cout << "N(" << vertex << "," << hops << ")= ";
        list<int> result = getNeighbours(vertex, hops);
        list<int>::const_iterator it;
        for(it = result.begin(); it != result.end(); it++) {
             cout << *it << " " ;
         }
        cout << "." << endl;
        exit(0);
    }
    if(query == 6){
        int sourceVertex, destinationVertex;
        cin >> sourceVertex >> destinationVertex;
        djikstra(sourceVertex, destinationVertex);
        exit(0);
    }
    if(query == 7){
        int sourceVertex, destinationVertex;
        cin >> sourceVertex >> destinationVertex;
        BFS(sourceVertex, destinationVertex);
        exit(0);
    }
}
