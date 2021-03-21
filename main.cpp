#include <iostream>
#include <fstream>
#include <set>

extern "C" {
    #include <GraphBLAS.h>
}

#include "implementation.h"
#include "tests.h"

using namespace std;


int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        
        cout << "No arguments passed to program." << endl;
    } else if (0 == strcmp(argv[1], "--test")) {

        if (0 == run_tests()) {
            
            cout << "All tests are passed." << endl;
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
