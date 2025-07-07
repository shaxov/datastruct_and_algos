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