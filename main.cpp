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
    } else if (argv[1] == "test") {
        // TODO tests
        // GrB_Matrix_setElement_BOOL(g, true, 3, 2);
        // GrB_Matrix_setElement_BOOL(g, true, 1, 2);
        // GrB_Matrix_setElement_BOOL(g, true, 0, 2);  
        // GrB_Matrix_setElement_BOOL(g, true, 3, 2);
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

        FILE* f = fopen("out.txt", "w");
        GxB_Matrix_fprint(g, "g", GxB_COMPLETE, f);

        GrB_Vector s;
        GrB_Vector_new(&s,  GrB_UINT64, size);
        for (GrB_Index i = 0; i < size; i++) {
            GrB_Vector_setElement_UINT64(s, i, i);
        }
        
        GxB_Vector_fprint(s, "s", GxB_COMPLETE, f);
        top_sort(s, g, starts);
        GxB_Vector_fprint(s, "s", GxB_COMPLETE, f);
        
        fclose(f);
        GrB_finalize();
    }
    
    return 0;
}
