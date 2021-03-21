#pragma once

#include <cassert>
#include "matrix_implementation.h"


void simple_straight() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 2, 2);

/*
    0
    |
    1
*/

    GrB_Matrix_setElement_BOOL(g, true, 0, 1);
    
    GrB_Vector s;
    top_sort(s, g, {0});

    GrB_Index buf;
    
    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 0);

    GrB_Vector_extractElement_UINT64(&buf, s, 1);
    assert(buf == 1);
    
    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void simple_reverse() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 2, 2);

/*
    1
    |
    0
*/

    GrB_Matrix_setElement_BOOL(g, true, 1, 0);
    
    GrB_Vector s;
    top_sort(s, g, {1});

    GrB_Index buf;
    
    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 1);

    GrB_Vector_extractElement_UINT64(&buf, s, 1);
    assert(buf == 0);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void simple_chain_straight() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 3, 3);

/*
    0
    |
    1
    |
    2
*/

    GrB_Matrix_setElement_BOOL(g, true, 0, 1);
    GrB_Matrix_setElement_BOOL(g, true, 1, 2);
    
    GrB_Vector s;
    top_sort(s, g, {0});

    GrB_Index buf;
    
    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 0);

    GrB_Vector_extractElement_UINT64(&buf, s, 1);
    assert(buf == 1);

    GrB_Vector_extractElement_UINT64(&buf, s, 2);
    assert(buf == 2);
    
    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void simple_chain_reversed() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 3, 3);

/*
    2
    |
    1
    |
    0
*/

    GrB_Matrix_setElement_BOOL(g, true, 2, 1);
    GrB_Matrix_setElement_BOOL(g, true, 1, 0);
    
    GrB_Vector s;
    top_sort(s, g, {2});

    GrB_Index buf;
    
    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 2);

    GrB_Vector_extractElement_UINT64(&buf, s, 1);
    assert(buf == 1);

    GrB_Vector_extractElement_UINT64(&buf, s, 2);
    assert(buf == 0);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void simple_chain_mixed() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 3, 3);

/*
    0
    |
    2
    |
    1
*/

    GrB_Matrix_setElement_BOOL(g, true, 0, 2);
    GrB_Matrix_setElement_BOOL(g, true, 2, 1);
    
    GrB_Vector s;
    top_sort(s, g, {0});

    GrB_Index buf;
    
    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 0);

    GrB_Vector_extractElement_UINT64(&buf, s, 1);
    assert(buf == 2);

    GrB_Vector_extractElement_UINT64(&buf, s, 2);
    assert(buf == 1);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void fork_start() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 3, 3);


/*
    1
   / \
  0   2
*/

    GrB_Matrix_setElement_BOOL(g, true, 1, 0);
    GrB_Matrix_setElement_BOOL(g, true, 1, 2);
    
    GrB_Vector s;
    top_sort(s, g, {1});

    GrB_Index buf;

    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 1);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void fork_end() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 3, 3);


/*
  0   2
   \ /
    1
*/

    GrB_Matrix_setElement_BOOL(g, true, 0, 1);
    GrB_Matrix_setElement_BOOL(g, true, 2, 1);
    
    GrB_Vector s;
    top_sort(s, g, {0, 2});

    GrB_Index buf;

    GrB_Vector_extractElement_UINT64(&buf, s, 2);
    assert(buf == 1);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void fork_start_and_end() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 4, 4);

/*
    1
   / \
  2   3
   \ /
    0
*/

    GrB_Matrix_setElement_BOOL(g, true, 3, 0); 
    GrB_Matrix_setElement_BOOL(g, true, 2, 0); 
    GrB_Matrix_setElement_BOOL(g, true, 1, 3); 
    GrB_Matrix_setElement_BOOL(g, true, 1, 2); 
    
    GrB_Vector s;
    top_sort(s, g, {1});

    GrB_Index buf;

    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 1);

    GrB_Vector_extractElement_UINT64(&buf, s, 3);
    assert(buf == 0);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

void fork_triangle() {

    GrB_Matrix g;
    GrB_Matrix_new(&g, GrB_BOOL, 3, 3);

/*
    0
   /|
  2 |
   \|
    1
*/

    GrB_Matrix_setElement_BOOL(g, true, 0, 2); 
    GrB_Matrix_setElement_BOOL(g, true, 2, 1); 
    GrB_Matrix_setElement_BOOL(g, true, 0, 1); 
    
    GrB_Vector s;
    top_sort(s, g, {0});

    GrB_Index buf;

    GrB_Vector_extractElement_UINT64(&buf, s, 0);
    assert(buf == 0);

    GrB_Vector_extractElement_UINT64(&buf, s, 1);
    assert(buf == 2);

    GrB_Vector_extractElement_UINT64(&buf, s, 2);
    assert(buf == 1);

    GrB_Matrix_free(&g);
    GrB_Vector_free(&s);
}

int run_tests() {

    GrB_init(GrB_BLOCKING);

    simple_straight();
    simple_reverse();
    simple_chain_straight();
    simple_chain_reversed();
    simple_chain_mixed();
    fork_start();
    fork_end();
    fork_start_and_end();

    GrB_finalize();

    return 0;
}
