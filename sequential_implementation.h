#include<iostream> 
#include<stack>
#include<list>  


using namespace std; 
  
class Graph {
    public:
        stack<uint64_t> ordered;

        Graph(uint64_t n) { 
            no_of_vertices = n; 
            l1 = new list<uint64_t>[n]; 
        }

        void add_edge(uint64_t x, uint64_t y) {
            l1[x].push_back(y);
        }

        void topological(uint64_t vertex_no, uint64_t visited[], stack<uint64_t>& s) { 
            visited[vertex_no] = 1; 
        
            list<uint64_t>::iterator i; 
            for (i = l1[vertex_no].begin(); i != l1[vertex_no].end(); i++) { 
                if (visited[*i] == 0) { 
                    topological(*i, visited, s);
                }
            }
            s.push(vertex_no); 
        } 
        
        stack<uint64_t> topological_sort() { 
            stack<uint64_t> s = ordered;
            uint64_t i, visited[no_of_vertices];
            
            for (i = 0; i < no_of_vertices; i++) {
                visited[i]=0; 
            } 

            for (i = 0; i < no_of_vertices; i++) {
                if (visited[i] == 0) {
                    topological(i, visited, s); 
                } 
            } 
        
            return s;
        } 
    
    private:
        uint64_t no_of_vertices;
        list<uint64_t>* l1;  
}; 
  
  
// int main() 
// { 
//     Graph p(4);  // 4: no of vertices
//     p.add_edge(0, 1); 
//     p.add_edge(2, 1); 
//     // p.add_edge(1, 2); 
//     p.add_edge(1, 3); 
//     p.add_edge(2, 3); 
//     /* This Graph is shown in above figure */ 
  
//     cout <<"\nTopological order of the vertices of the Graph: "; 
//     auto s = p.topological_sort(); 
//     while (!s.empty()) {
//         cout << s.top() << ' ';
//         s.pop();
//     }
//     cout << endl;

//     return 0; 
// }
