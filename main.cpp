#include <iostream>
#include <fstream>
#include <set>
#include <chrono>

extern "C" {
    #include <GraphBLAS.h>
}

#include "matrix_implementation.h"
#include "sequential_implementation.h"
#include "tests.h"

using namespace std;
using namespace std::chrono; 


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        
        cout << "No arguments passed to program." << endl;
    } else if (0 == strcmp(argv[1], "--test")) {

        if (0 == run_tests()) {
            
            cout << "All tests are passed." << endl;
        }
    } else if (0 == strcmp(argv[1], "--compare")) { // гипер-разреженные матрицы
        
        GrB_init(GrB_BLOCKING);
        for (const string& i: {"500", "1000", "2000", "5000", "10000"}) {

            for (const string& j: {"100", "50", "10", "5", "1", "0.5"}) {

                ifstream input("data/" + i + "_" + j + ".txt");

                GrB_Index size, starts_count, edges_count;
                input >> size >> starts_count >> edges_count;


                Graph g1(size);

                GrB_Index vertex;
                set<GrB_Index> starts;
                for (GrB_Index i = 0; i < starts_count; i++) {
                    
                        input >> vertex;
                        starts.insert(vertex);
                }

                GrB_Matrix g;
                GrB_Matrix_new(&g, GrB_BOOL, size, size);

                GrB_Index from, to; 
                for (GrB_Index i = 0; i < edges_count; i++) {
                    
                    input >> from >> to;
                    
                    GrB_Matrix_setElement_BOOL(g, true, from, to);
                    g1.add_edge(from, to);
                }

                input.close();

                GrB_Vector s;
            
                auto t_1 = chrono::high_resolution_clock::now();
                top_sort(s, g, starts);
                auto t_2 = chrono::high_resolution_clock::now();

            
                GrB_Vector_free(&s);
                GrB_finalize();

                auto t_3 = high_resolution_clock::now();
                g1.topological_sort();
                auto t_4 = high_resolution_clock::now();

                auto d_1 = duration_cast<microseconds>(t_2 - t_1);
                auto d_2 = duration_cast<microseconds>(t_4 - t_3); 
                cout << i << ' ' << j << ' ' << d_1.count() << ' ' << d_2.count() << endl;
            }
        }

    } else {
        
        GrB_init(GrB_BLOCKING);
        
        ifstream input(argv[1]);

        GrB_Index size, starts_count, edges_count;
        input >> size >> starts_count >> edges_count;

        GrB_Index vertex;
        set<GrB_Index> starts;
        for (GrB_Index i = 0; i < starts_count; i++) {
            
            input >> vertex;
            starts.insert(vertex);
        }

        GrB_Matrix g;
        GrB_Matrix_new(&g, GrB_BOOL, size, size);

        GrB_Index from, to; 
        for (GrB_Index i = 0; i < edges_count; i++) {
            
            input >> from >> to;
            GrB_Matrix_setElement_BOOL(g, true, from, to);
        }

        input.close();

        GrB_Vector s;
        
        top_sort(s, g, starts);
        
        cout << "Ordered indexes of vertexes:" << endl;
        GrB_Index buf;
        for (GrB_Index i = 0; i < size; i++) {
            
            GrB_Vector_extractElement_UINT64(&buf, s, i);
            cout << buf << ' ';
        }
        cout << endl;

        GrB_Vector_free(&s);
        GrB_finalize();
    }
    
    return 0;
}
