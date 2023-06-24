#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_DIM 3
struct kd_node_t
{
    double x[MAX_DIM];
    double w, t; // rectangle
    struct kd_node_t* left, * right;
};

inline double dist(struct kd_node_t* a, struct kd_node_t* b, int dim)
{
    double t, d = 0;
    while (dim--) {
        t = a->x[dim] - b->x[dim];
        d += t * t;
    }
    return d;
}

inline void swap(struct kd_node_t* x, struct kd_node_t* y) {
    double tmp[MAX_DIM], w, t;
    memcpy(tmp, x->x, sizeof(tmp));
    w = x->w;
    t = x->t;

    memcpy(x->x, y->x, sizeof(tmp));
    x->w = y->w;
    x->t = y->t;

    memcpy(y->x, tmp, sizeof(tmp));
    y->w = w;
    y->t = t;
}

/* see quickselect method */
struct kd_node_t* find_median(struct kd_node_t* start, struct kd_node_t* end, int idx)
{
    if (end <= start) return NULL;
    if (end == start + 1)
        return start;

    struct kd_node_t* p, * store, * md = start + (end - start) / 2;
    double pivot;
    while (1) {
        pivot = md->x[idx];

        swap(md, end - 1);
        for (store = p = start; p < end; p++) {
            if (p->x[idx] < pivot) {
                if (p != store)
                    swap(p, store);
                store++;
            }
        }
        swap(store, end - 1);

        /* median has duplicate values */
        if (store->x[idx] == md->x[idx])
            return md;

        if (store > md) end = store;
        else        start = store;
    }
}

struct kd_node_t* make_tree(struct kd_node_t* t, int len, int i, int dim)
{
    struct kd_node_t* n;

    if (!len) return 0;

    if ((n = find_median(t, t + len, i))) {
        i = (i + 1) % dim;
        n->left = make_tree(t, n - t, i, dim);
        n->right = make_tree(n + 1, t + len - (n + 1), i, dim);
    }
    return n;
}

/* global variable, so sue me */
int visited;

void nearest(struct kd_node_t* root, struct kd_node_t* nd, int i, int dim, struct kd_node_t** best, double* best_dist)
{
    double d, dx, dx2;

    if (!root) return;
    d = dist(root, nd, dim);
    dx = root->x[i] - nd->x[i];
    dx2 = dx * dx;

    visited++;

    if (!*best || d < *best_dist) {
        *best_dist = d;
        *best = root;
    }

    /* if chance of exact match is high */
    if (!*best_dist) return;

    if (++i >= dim) i = 0;

    nearest(dx > 0 ? root->left : root->right, nd, i, dim, best, best_dist);
    if (dx2 >= *best_dist) return;
    nearest(dx > 0 ? root->right : root->left, nd, i, dim, best, best_dist);
}

void range_search(struct kd_node_t* root, double x, double y, double w, double t, int* pnum_vis, int* pnum_found)
{
    if (!root)
        return;

    if (!pnum_vis || !pnum_found)
        return;

    (*pnum_vis)++;

    int dx = x + w - root->x[0];
    int dy = y + t - root->x[1];

    if (dx >= 0 && dy >= 0) // at least starting point smaller than or same with maximum range(x + w, y + t)
    {
        double p[4], q[4];
        p[0] = root->x[0];
        p[1] = root->x[1];
        p[2] = p[0] + root->w;
        p[3] = p[1] + root->t;

        q[0] = x;
        q[1] = y;
        q[2] = q[0] + w;
        q[3] = q[1] + t;

        int i;
        for (i = 0; i < 4; i++) // search for one point of rectangle is contained in range
        {
            int idx = i / 2 ? 2 : 0;
            int idy = i % 2 ? 3 : 1;

            if (!(q[0] <= p[idx] && p[idx] <= q[2] && q[1] <= p[idy] && p[idy] <= q[3])) // check if the node rect's any point is contained in range search
                break;
        }

        if (i == 4)
        {
            (*pnum_found)++;
            printf("found range: %d %d %d %d\n", (int)root->x[0], (int)root->x[1], (int)root->w, (int)root->t);
        }
    }

    if ((*pnum_vis) % 2) // x
    {
        if (dx >= 0) // x contained
        {
            // we should go left because the rectangle go right, it can staisfy range condition.
            range_search(root->left, x, y, w, t, pnum_vis, pnum_found);

            // the right node's x is bigger than this->x or same. we can skip this node if its x value is bigger than thisnode.
            // .. or this starting point is inside of range.
            if (dy >= 0 || (root->right && root->right->x[0] <= root->x[0]))
                range_search(root->right, x, y, w, t, pnum_vis, pnum_found);
        }
        else // x is out of range
        {
            // the left node's x is smaller than this->x. we should search for this.
            range_search(root->left, x, y, w, t, pnum_vis, pnum_found);

            // the right node's x is bigger than this->x. we can skip this node if its y value is bigger than thisnode.
            if (root->right && root->right->x[1] <= root->x[1])
                range_search(root->right, x, y, w, t, pnum_vis, pnum_found);
        }
    }
    else // y
    {
        if (dy >= 0) // y contained
        {
            // we should go left because the rectangle go right, it can staisfy range condition.
            range_search(root->left, x, y, w, t, pnum_vis, pnum_found);

            // the right node's y is bigger than this->y or same. we can skip this node if its y value is bigger than thisnode.
            // .. or this starting point is inside of range.
            if (dx >= 0 || (root->right && root->right->x[1] <= root->x[1]))
                range_search(root->right, x, y, w, t, pnum_vis, pnum_found);
        }
        else // y is out of range
        {
            // the left node's y is smaller than this->y. left node may satisfy the condition.
            range_search(root->left, x, y, w, t, pnum_vis, pnum_found);

            // the right node's y is bigger than this->y. we can skip this node if its x value is bigger than thisnode.
            if (root->right && root->right->x[0] <= root->x[0])
                range_search(root->right, x, y, w, t, pnum_vis, pnum_found);
        }
    }
}

int main(void)
{
    struct kd_node_t wp[] = {
        {{1, 1}, 5, 7},
        {{8, 1}, 2, 7},
        {{3, 2}, 4, 6},
        {{0, 0}, 10, 10},
        {{1, 2}, 2, 2},
        {{8, 7}, 1, 2}
    };

    struct kd_node_t* root;

    root = make_tree(wp, sizeof(wp) / sizeof(wp[1]), 0, 2);

    int num = 0;
    int found = 0;

    printf("Search for (0, 0, 10, 10)\n");
    range_search(root, 0, 0, 10, 10, &num, &found);
    printf("Total visit: %d\nTotal found: %d\n\n", num, found);

    num = 0;
    found = 0;
    printf("Search for (0, 0, 6, 4)\n");
    range_search(root, 0, 0, 6, 4, &num, &found);
    printf("Total visit: %d\nTotal found: %d\n\n", num, found);

    num = 0;
    found = 0;
    printf("Search for (2, 1, 9, 9)\n");
    range_search(root, 2, 1, 9, 9, &num, &found);
    printf("Total visit: %d\nTotal found: %d\n\n", num, found);

    return 0;
}
