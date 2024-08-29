//
// Created by 13472 on 2022/3/30.
//

#include "SHADER.h"

void mSort(int l, int r) {
    int a[100000000], c[100000000];
    if (l == r)
        return;
    int mid = (l + r) >> 1, k = l, i = l, j = mid + 1;
    mSort(l, mid);
    mSort(mid + 1, r);
    while (i <= mid && j <= r) {
        if (a[i] <= a[j])
            c[k++] = a[i++];
        else
            c[k++] = a[j++];
    }
    while (i <= mid)
        c[k++] = a[j++];
    while (j <= r)
        c[k++] = a[j++];
    for (int s = l; s <= r; ++s) {
        a[s] = c[s];
    }
}