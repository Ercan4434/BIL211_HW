#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#define N 6
#define INF (INT_MAX / 2)

const char NODES[N] = {'A', 'B', 'C', 'D', 'E', 'F'};

int getNodeIndex(char c) {
    c = (char)toupper((unsigned char)c);
    for (int i = 0; i < N; i++) {
        if (NODES[i] == c) return i;
    }
    return -1;
}

void addEdge(int adj[N][N], char uChar, char vChar, int weight) {
    int u = getNodeIndex(uChar);
    int v = getNodeIndex(vChar);
    if (u != -1 && v != -1) {
        adj[u][v] = weight;
        adj[v][u] = weight;
    }
}

void printStepTable(int step, char selectedNode, bool known[N], int dist[N], int parent[N]) {
    printf("\n--- Adim %d (Secilen Dugum: %c) ---\n", step, selectedNode);
    printf("%-8s %-15s %-15s %-10s\n", "Dugum", "Isaret (Known)", "Maliyet (Dist)", "Yol (Path)");
    printf("----------------------------------------------------\n");
    for (int i = 0; i < N; i++) {
        char distStr[16];
        if (dist[i] >= INF) {
            snprintf(distStr, sizeof(distStr), "INF");
        } else {
            snprintf(distStr, sizeof(distStr), "%d", dist[i]);
        }
        char pathChar = (parent[i] == -1) ? '-' : NODES[parent[i]];
        printf("%-8c %-15s %-15s %-10c\n",
               NODES[i],
               known[i] ? "True (1)" : "False (0)",
               distStr,
               pathChar);
    }
}

void printShortestPath(int parent[N], int dist[N], int root, int target) {
    if (dist[target] >= INF) {
        printf("%c -> %c : Ulasilamiyor\n", NODES[root], NODES[target]);
        return;
    }

    int stack[N];
    int top = 0;
    int curr = target;

    while (curr != -1) {
        stack[top++] = curr;
        curr = parent[curr];
    }

    printf("%c -> %c (Toplam Maliyet = %d): ", NODES[root], NODES[target], dist[target]);
    for (int i = top - 1; i >= 0; i--) {
        printf("%c", NODES[stack[i]]);
        if (i > 0) printf(" -> ");
    }
    printf("\n");
}

int runDijkstra(int adj[N][N], int root, bool verbose, int outDist[N], int outParent[N]) {
    bool known[N] = {false};
    int dist[N];
    int parent[N];

    for (int i = 0; i < N; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[root] = 0;

    for (int step = 1; step <= N; step++) {
        int u = -1;
        int minDist = INF;

        for (int i = 0; i < N; i++) {
            if (!known[i] && dist[i] < minDist) {
                minDist = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        known[u] = true;

        for (int v = 0; v < N; v++) {
            if (adj[u][v] > 0 && !known[v]) {
                if (dist[u] + adj[u][v] < dist[v]) {
                    dist[v] = dist[u] + adj[u][v];
                    parent[v] = u;
                }
            }
        }

        if (verbose) {
            printStepTable(step, NODES[u], known, dist, parent);
        }
    }

    int totalTreeCost = 0;
    for (int i = 0; i < N; i++) {
        if (outDist) outDist[i] = dist[i];
        if (outParent) outParent[i] = parent[i];
        totalTreeCost += dist[i];
    }
    return totalTreeCost;
}

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);

    int adj[N][N] = {0};

    addEdge(adj, 'A', 'B', 7);
    addEdge(adj, 'A', 'C', 5);
    addEdge(adj, 'A', 'F', 1);
    addEdge(adj, 'B', 'C', 1);
    addEdge(adj, 'B', 'D', 1);
    addEdge(adj, 'B', 'E', 3);
    addEdge(adj, 'C', 'D', 2);
    addEdge(adj, 'C', 'E', 1);
    addEdge(adj, 'D', 'F', 7);
    addEdge(adj, 'E', 'F', 2);

    // GOREV 1: Kullanicidan kok dugum alma ve adim adim tablo olusturma
    char rootInput;
    printf("Kok dugumu giriniz (A-F): ");
    if (scanf(" %c", &rootInput) != 1) return 1;

    int rootIdx = getNodeIndex(rootInput);
    if (rootIdx == -1) {
        printf("Gecersiz dugum! Varsayilan olarak 'A' secildi.\n");
        rootIdx = 0;
    }

    int dist[N], parent[N];
    printf("\n=== GOREV 1: DIJKSTRA ADIM TABLOSU (KOK = %c) ===\n", NODES[rootIdx]);
    runDijkstra(adj, rootIdx, true, dist, parent);

    // GOREV 2: Rota ciktisi (Tersten okuma yontemi)
    printf("\n=== GOREV 2: ROTA CIKTISI (KOK = %c) ===\n", NODES[rootIdx]);
    if (NODES[rootIdx] == 'A') {
        printShortestPath(parent, dist, rootIdx, getNodeIndex('B'));
        printShortestPath(parent, dist, rootIdx, getNodeIndex('D'));
    } else {
        for (int i = 0; i < N; i++) {
            if (i != rootIdx) printShortestPath(parent, dist, rootIdx, i);
        }
    }

    // GOREV 3: Tum dugumler icin merkezi dugum tespiti
    printf("\n=== GOREV 3: MERKEZI DUGUM ANALIZI (A - F) ===\n");
    int minTotalCost = INF;
    int totalCosts[N];

    for (int r = 0; r < N; r++) {
        int dRow[N];
        totalCosts[r] = runDijkstra(adj, r, false, dRow, NULL);
        printf("Kok Dugum %c -> En Kisa Yollar: [", NODES[r]);
        for (int j = 0; j < N; j++) {
            printf("%c:%d%s", NODES[j], dRow[j], (j < N - 1) ? ", " : "");
        }
        printf("] | Toplam Maliyet = %d\n", totalCosts[r]);

        if (totalCosts[r] < minTotalCost) {
            minTotalCost = totalCosts[r];
        }
    }

    printf("\nEn Dusuk Toplam Maliyet: %d -> En Merkezi Dugum(ler): ", minTotalCost);
    bool first = true;
    for (int r = 0; r < N; r++) {
        if (totalCosts[r] == minTotalCost) {
            if (!first) printf(", ");
            printf("%c", NODES[r]);
            first = false;
        }
    }
    printf("\n");

    return 0;
}