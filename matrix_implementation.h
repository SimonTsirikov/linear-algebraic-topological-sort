#pragma once

#include <set>

extern "C" {
    #include <GraphBLAS.h>
}

using namespace std;


GrB_Index build_permute
        (
                GrB_Vector& s,  // vector of values
                GrB_Vector& e,  // boolean mask of visited/not visited 
                const GrB_Index tau,    // lower bound
                const GrB_Index beta,   // upper bound
                GrB_Matrix& P   // output permutation matrix
        )
{
    GrB_Index size;
    GrB_Vector_size(&size, e);

    GrB_Matrix_clear(P);
    
    for (GrB_Index i = 0; i < tau; i++) {
        
        GrB_Matrix_setElement_BOOL(P, true, i, i);
    }

    set<GrB_Index> moved_up;
    GrB_Index moved_down = 0;

    bool buf;
    GrB_Index buf_s;

    for (GrB_Index i = tau; i < size; i++) {
        
        GrB_Vector_extractElement_BOOL(&buf, e, i);
        if (buf) {
            
            GrB_Vector_extractElement_UINT64(&buf_s, s, i);
            GrB_Matrix_setElement_BOOL(P, true, i, tau + moved_up.size());
            moved_up.insert(buf_s);
    
            buf = false;
        }
    }
    
    for (GrB_Index i = tau; i < size; i++) {
        
        GrB_Vector_extractElement_UINT64(&buf_s, s, i);
        if (moved_up.count(buf_s) == 0) {
            
            GrB_Matrix_setElement_BOOL(P, true, i, tau + moved_up.size() + moved_down);
            moved_down++;        
        }
    }
    
    return moved_up.size();
}

GrB_Info top_sort
        (
                GrB_Vector& s,  // output vector such that s[i] is the i-th vertex in the topological sort order 
                GrB_Matrix& A,  // input graph, treated as if boolean in semiring
                set<GrB_Index> visited  // starting node of the BFS
        )
{

    GrB_Index size;
    GrB_Matrix_ncols(&size, A);

    GrB_Vector_new(&s,  GrB_UINT64, size);
    for (GrB_Index i = 0; i < size; i++) {
        
        GrB_Vector_setElement_UINT64(s, i, i);
    }

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

    for (GrB_Index i: visited) {
        
        GrB_Vector_setElement_BOOL(e, true, i);
    }

    GrB_Index nu = build_permute(s, e, tau, beta, P);
    
    GrB_transpose(P_1, NULL, NULL, P, GrB_DESC_R);

    GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, P_1, A, GrB_DESC_R);
    GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, A, P, GrB_DESC_R);

    GrB_mxv(s, NULL, NULL, GrB_MAX_SECOND_SEMIRING_UINT64, P_1, s, GrB_DESC_R);

    GrB_Vector_clear(e);
    for (GrB_Index i = 0; i < size; i++) {
        
        GrB_Vector_setElement_BOOL(e, false, i);
    }

    beta += nu;

    while (tau < size) {
        
        GrB_Vector_setElement_BOOL(e, true, tau);

        GrB_transpose(A_1, NULL, NULL, A, GrB_DESC_R);

        GrB_mxv(n, NULL, NULL, GxB_LAND_LE_BOOL, A_1, e, GrB_DESC_R);

        tau += 1;
        if (beta < tau) {
            beta += 1;
        }

        nu = build_permute(s, n, tau, beta, P);
        
        GrB_transpose(P_1, NULL, NULL, P, GrB_DESC_R);        
        
        GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, P_1, A, GrB_DESC_R);
        GrB_mxm(A, NULL, NULL, GxB_ANY_PAIR_BOOL, A, P, GrB_DESC_R);
        
        GrB_mxv(s, NULL, NULL, GrB_MAX_SECOND_SEMIRING_UINT64, P_1, s, GrB_DESC_R);
        
        beta += nu;
    }

    GrB_Matrix_free(&P);
    GrB_Matrix_free(&P_1);
    GrB_Matrix_free(&A_1);
    GrB_Vector_free(&n);
    GrB_Vector_free(&e);

    return GrB_SUCCESS;
}
