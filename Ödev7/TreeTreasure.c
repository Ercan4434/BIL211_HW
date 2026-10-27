#include <stdio.h>
#include <stdlib.h>
#include <time.h> 

// İkili Arama Ağacı (BST) için düğüm yapısı
struct Node {
    int data;
    int is_treasure; // 1 ise hazine, 0 ise boş/normal düğüm
    struct Node *left, *right;
};

// Yeni düğüm oluşturma fonksiyonu 
struct Node* createNode(int data, int is_treasure) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->is_treasure = is_treasure;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// BST'ye yeni değer ekleme fonksiyonu
struct Node* insert(struct Node* root, int data, int is_treasure) {
    if (root == NULL) return createNode(data, is_treasure);
    
    if (data < root->data)
        root->left = insert(root->left, data, is_treasure);
    else if (data > root->data)
        root->right = insert(root->right, data, is_treasure);
        
    return root;
}

// Yeni sayı üretilirken ağaçta olup olmadığını kontrol eden sessiz fonksiyon
int contains(struct Node* root, int key) {
    if (root == NULL) return 0;
    if (root->data == key) return 1;
    
    if (key < root->data)
        return contains(root->left, key);
    else
        return contains(root->right, key);
}

// Oyun içindeki ekrana adım yazdıran rekürsif arama fonksiyonu
int search(struct Node* root, int key, int step) {
    if (root == NULL) {
        return 0; // Düğüm ağaçta yok (Boşuna kazdın)
    }

    printf("Adim %d: Ziyaret edilen dugum = %d\n", step, root->data);

    if (root->data == key) {
        // Sayı ağaçta bulundu! Peki hazine mi?
        if (root->is_treasure == 1) {
            return 1; // Hazine bulundu!
        } else {
            return 0; // Sayı var ama içi boş (Boşuna kazdın)
        }
    }

    if (root->data < key) {
        return search(root->right, key, step + 1);
    } else {
        return search(root->left, key, step + 1);
    }
}

// En kötü durumdaki adım sayısını (ağaç yüksekliğini) hesaplama
int findHeight(struct Node* root) {
    if (root == NULL) return 0;
    
    int leftHeight = findHeight(root->left);
    int rightHeight = findHeight(root->right);
    
    if (leftHeight > rightHeight) {
        return leftHeight + 1;
    } else {
        return rightHeight + 1;
    }
}

int main() {
    struct Node* root = NULL;
    
    srand(time(NULL));
    
    int dugum_sayisi;
    printf("Agaca toplam kac adet rastgele dugum eklensin?: ");
    scanf("%d", &dugum_sayisi);

    printf("\nAgac olusturuluyor... (Hazine yerleri gizli!)\n\n");
    
    int eklenen = 0;
    int hazine_sayisi = 0;
    
    while (eklenen < dugum_sayisi) {
        int rastgele_sayi = rand() % 1000 + 1; 
        
        if (!contains(root, rastgele_sayi)) {
            // %50 ihtimalle bu düğüm hazine (1) veya boş (0) olacak
            int hazine_mi = rand() % 2; 
            
            if (hazine_mi == 1) hazine_sayisi++;
            
            root = insert(root, rastgele_sayi, hazine_mi);
            eklenen++;
        }
    }

    int max_adim = findHeight(root);
    printf("--- Hazine Agaci Oyunu ---\n");
    printf("Bilgi: %d dugumun sadece %d tanesi gercek hazine barindiriyor.\n", dugum_sayisi, hazine_sayisi);
    printf("Bilgi: Hazineyi bulmak icin en kotu durumda %d adim gerekir (agac yuksekligi).\n\n", max_adim);

    int aranan;
    
    // Sonsuz döngü: Kullanıcı -1 girene kadar arama yapmaya devam eder
    while (1) {
        printf("------------------------------------------------\n");
        printf("Aramak istediginiz degeri girin (Cikmak icin -1): ");
        scanf("%d", &aranan);
        
        if (aranan == -1) {
            printf("Oyundan cikiliyor...\n");
            break; 
        }
        
        printf("\nArama basliyor...\n");
        int sonuc = search(root, aranan, 1);

        if (sonuc == 1) {
            printf("\nHazine bulundu!\n\n");
        } else {
            printf("\nBosuna kazdin!\n\n");
        }
    }

    system("pause");
    return 0;
}