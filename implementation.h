#pragma once

#include <iostream>
#include <fstream>
#include <set>

extern "C" {
    #include <GraphBLAS.h>
}

using namespace std;


GrB_Index build_permute
        (
                _IO_FILE* f,
                GrB_Vector& s, // both s and e should be permuted
                GrB_Vector& e, //
                const GrB_Index tau, 
                const GrB_Index beta, 
                GrB_Matrix& P // output matrix
        )
{
    GrB_Index size;
    GrB_Vector_size(&size, e);

    // cout << tau << ' ' << beta << ' ' << size << endl;

    GrB_Matrix_clear(P);
    for (GrB_Index i = 0; i < tau; i++) {
        GrB_Matrix_setElement_BOOL(P, true, i, i);
    }
    // cout << "bp1" << endl;

    set<GrB_Index> moved_up;
    GrB_Index moved_down = 0;

    bool buf;
    GrB_Index buf_s;

    GxB_Vector_fprint(e, "e", GxB_COMPLETE, f);
    for (GrB_Index i = tau; i < size; i++) {
        GrB_Vector_extractElement_BOOL(&buf, e, i);
        if (buf) {
            GrB_Vector_extractElement_UINT64(&buf_s, s, i);
            GrB_Matrix_setElement_BOOL(P, true, i, tau + moved_up.size());
            moved_up.insert(buf_s);
            cout << i << " inserted" << endl;
            buf = false;
        }
    }
    // cout << "bp2" << endl;
    for (GrB_Index i = tau; i < size; i++) {
        GrB_Vector_extractElement_UINT64(&buf_s, s, i);
        if (moved_up.count(buf_s) == 0) {
            GrB_Matrix_setElement_BOOL(P, true, i, tau + moved_up.size() + moved_down);
            moved_down++;        
        }
    }

    // cout << "bp3" << endl;
    GxB_Vector_fprint(s, "s", GxB_COMPLETE, f);

    GxB_Matrix_fprint(P, "P", GxB_COMPLETE, f);

    cout << moved_up.size() << endl;

    return moved_up.size();
}

GrB_Info top_sort
        (
                GrB_Vector& s, // v(i) is the BFS level of node i in the graph
                GrB_Matrix& A, // input graph, treated as if boolean in semiring
                set<GrB_Index> visited // starting node of the BFS
        )
        //output is the permuted vector s such that s[i] is the i-th vertex
        // in the pre-order traversal sequence
{
    FILE* f = fopen("process.txt", "w");
    
    // GrB_Info info ;
    
    GrB_Index size;
    GrB_Matrix_ncols(&size, A);

    GrB_Index tau = 0;
    GrB_Index beta = 0;

    GrB_Matrix P;
    GrB_Matrix P_1;
    GrB_Matrix A_1;
    GrB_Vector n;
    GrB_Vector e;

    GrB_Matrix_new(&P, GrB_BOOL, size, size);
    GrB_Matrix_new(&P_1, GrB_BOOL, size, size);
    GrB_Matrix_new(&A_1, GrB_BOOL, size, size);
    GrB_Vector_new(&n, GrB_UINT64, size);
    GrB_Vector_new(&e, GrB_UINT64, size);

    // for (GrB_Index i = 0; i < size; i++) {
    //     GrB_Matrix_setElement_BOOL(P, true, i, i);
    // }

    for (GrB_Index i: visited) {
        GrB_Vector_setElement_BOOL(e, true, i);
    }

    GrB_Index nu = build_permute(f, s, e, tau, beta, P);
    
    // GrB_Matrix_clear(P_1);
    GrB_transpose(P_1, NULL, NULL, P, GrB_DESC_R);
    GxB_Matrix_fprint(P_1, "P_1", GxB_COMPLETE, f);

    GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, P_1, A, GrB_DESC_R);
    GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, A, P, GrB_DESC_R);
    GxB_Matrix_fprint(A, "A", GxB_COMPLETE, f);

    // is it right semiring? ANY, MIN?
    GrB_mxv(s, NULL, NULL, GrB_MAX_SECOND_SEMIRING_UINT64, P_1, s, GrB_DESC_R);
    GxB_Vector_fprint(s, "s", GxB_COMPLETE, f);

    // GrB_mxv(e, NULL, NULL, GrB_MAX_SECOND_SEMIRING_UINT64, P_1, e, GrB_DESC_R);
    // GxB_Vector_fprint(e, "e", GxB_COMPLETE, f);
    GrB_Vector_clear(e);
    for (GrB_Index i = 0; i < size; i++) {
        GrB_Vector_setElement_BOOL(e, false, i);
    }

    beta += nu;

    while (tau < size) {
        
        GrB_Vector_setElement_BOOL(e, true, tau);
        // GrB_Matrix_clear(A_1);
        GrB_transpose(A_1, NULL, NULL, A, GrB_DESC_R);


        // GrB_Vector_clear(n);
        GrB_mxv(n, NULL, NULL, GxB_LAND_LE_BOOL, A_1, e, GrB_DESC_R);

        GxB_Matrix_fprint(A_1, "A_1 x e = n", GxB_COMPLETE, f);

        GxB_Vector_fprint(e, "e", GxB_COMPLETE, f);

        GxB_Vector_fprint(n, "n", GxB_COMPLETE, f);

        tau += 1;
        if (beta < tau) {
            beta += 1;
        }

        // GrB_Vector_setElement_BOOL(e, true, tau);

        nu = build_permute(f, s, n, tau, beta, P);
        GxB_Matrix_fprint(P, "P", GxB_COMPLETE, f);

        // GrB_Matrix_clear(P_1);
        GrB_transpose(P_1, NULL, NULL, P, GrB_DESC_R);        
        
        GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, P_1, A, GrB_DESC_R);
        GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, A, P, GrB_DESC_R);
        GxB_Matrix_fprint(A, "A", GxB_COMPLETE, f);


        GrB_mxv(s, NULL, NULL, GrB_MAX_SECOND_SEMIRING_UINT64, P_1, s, GrB_DESC_R);
        GxB_Vector_fprint(s, "s", GxB_COMPLETE, f);

        beta += nu;
    }

    GrB_Matrix_free(&P);
    GrB_Matrix_free(&P_1);
    GrB_Matrix_free(&A_1);
    GrB_Vector_free(&n);
    GrB_Vector_free(&e);
    fclose(f);

    return GrB_SUCCESS;
}
