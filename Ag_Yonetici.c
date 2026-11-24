#include <stdio.h>
#include <stdbool.h>

#define N 6

void printMatrix(int adj[N][N]) {
    printf("Komsuluk Matrisi (6x6):\n    ");
    for (int i = 0; i < N; i++) printf("%d ", i + 1);
    printf("\n   -------------\n");
    for (int i = 0; i < N; i++) {
        printf("%d | ", i + 1);
        for (int j = 0; j < N; j++) {
            printf("%d ", adj[i][j]);
        }
        printf("\n");
    }
}

void calculateDegrees(int adj[N][N]) {
    int totalDegree = 0;
    printf("\nDugum Dereceleri:\n");
    for (int i = 0; i < N; i++) {
        int deg = 0;
        for (int j = 0; j < N; j++) {
            deg += adj[i][j];
        }
        totalDegree += deg;
        printf("  Dugum %d derecesi: %d\n", i + 1, deg);
    }
    printf("Grafin Genel (Toplam) Derecesi: %d\n", totalDegree);
}

void checkBipartite(int adj[N][N], int startNode) {
    int dist[N];
    int queue[N];
    int front = 0, rear = 0;

    for (int i = 0; i < N; i++) dist[i] = -1;

    int startIdx = startNode - 1;
    dist[startIdx] = 0; // Baslangic dugumu: uzaklik = 0 (Cift / Mavi)
    queue[rear++] = startIdx;

    while (front < rear) {
        int u = queue[front++];
        for (int v = 0; v < N; v++) {
            if (adj[u][v] == 1 && dist[v] == -1) {
                dist[v] = dist[u] + 1;
                queue[rear++] = v;
            }
        }
    }

    printf("\nUzaklik ve Renk/Parite Tablosu (Baslangic = %d):\n", startNode);
    for (int i = 0; i < N; i++) {
        const char *color = (dist[i] % 2 == 0) ? "Mavi (0 / Cift)" : "Kirmizi (1 / Tek)";
        printf("  Dugum %d -> Uzaklik: %d, Renk: %s\n", i + 1, dist[i], color);
    }

    bool isBipartite = true;
    for (int u = 0; u < N; u++) {
        for (int v = u + 1; v < N; v++) {
            if (adj[u][v] == 1 && (dist[u] % 2 == dist[v] % 2)) {
                printf("  [!] Cakisma: (%d, %d) komsu dugumleri ayni pariteye (%d ve %d) sahip!\n",
                       u + 1, v + 1, dist[u], dist[v]);
                isBipartite = false;
            }
        }
    }

    if (isBipartite) {
        printf("Sonuc: Graf Iki Parcalidir (BIPARTITE).\n");
    } else {
        printf("Sonuc: Graf Iki Parcali DEGILDIR (NOT BIPARTITE).\n");
    }
}

int main(void) {
    int edges[][2] = {{1, 2}, {1, 4}, {2, 3}, {3, 4}, {3, 6}, {4, 5}, {5, 6}};
    int edgeCount = sizeof(edges) / sizeof(edges[0]);
    int adj[N][N] = {0};

    // 1. Komsuluk Matrisi Olusturma
    for (int i = 0; i < edgeCount; i++) {
        int u = edges[i][0] - 1;
        int v = edges[i][1] - 1;
        adj[u][v] = 1;
        adj[v][u] = 1;
    }

    printf("=== 1-3. MEVCUT GRAF ANALIZI ===\n");
    printMatrix(adj);
    calculateDegrees(adj);
    checkBipartite(adj, 1);

    // 4. Dinamik Test: (2, 4) baglantisinin eklenmesi
    printf("\n=== 4. DINAMIK TEST: (2, 4) EKLENDIKTEN SONRA ===\n");
    adj[2 - 1][4 - 1] = 1;
    adj[4 - 1][2 - 1] = 1;
    printMatrix(adj);
    calculateDegrees(adj);
    checkBipartite(adj, 1);

    return 0;
}