/**
 * @file	tests/codeforges_1020B.cpp
 * @date	Jul 2025
 * @author	Maksym Shpakovych
 */

/*
 Copyright (c) 2025 Maksym Shpakovych

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */


#include <stdio.h>

int main(void) {
    int n;
    if (scanf("%d", &n) != 1) 
        return 0;

    static int p[1001], state[1001], ans[1001], stack[1001];

    for (int i = 1; i <= n; ++i) 
        scanf("%d", &p[i]);

    for (int v0 = 1; v0 <= n; ++v0) 
    {
        if (state[v0]) 
            continue; 

        int top = 0, v = v0;

        while (!state[v]) {
            stack[top++] = v;
            state[v] = 1;         
            v = p[v];
        }

        if (state[v] == 1) {      
            for (int i = top - 1; ; --i) {
                int u = stack[i];
                ans[u] = u;        
                if (u == v) break;
            }
        }

        while (top--) {
            int u = stack[top];
            if (!ans[u]) 
                ans[u] = ans[p[u]];  
            state[u] = 2;                     
        }
    }

    for (int i = 1; i <= n; ++i)
        printf("%d%c", ans[i], (i == n ? '\n' : ' '));
    return 0;
}