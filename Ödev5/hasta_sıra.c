#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hasta düğümü tanımı (Tüm veri yapıları için ortak kullanılacak)
typedef struct Patient {
    int id;
    char name[50];
    char status[20]; // "bekliyor", "muayenede", "tamamlandi"
    struct Patient* next;
} Patient;

// Kuyruk (Queue) yapısı - FIFO
typedef struct {
    Patient* front;
    Patient* rear;
} Queue;

// Yığın (Stack) yapısı - LIFO
typedef struct {
    Patient* top;
} Stack;

// Bağlı Liste (Linked List) yapısı - Geçmiş Kayıtlar
typedef struct {
    Patient* head;
    Patient* tail; // Sona eklemeyi kolaylaştırmak için
} LinkedList;

// Yeni hasta oluşturma fonksiyonu
Patient* createPatient(int id, const char* name, const char* status) {
    Patient* newPatient = (Patient*)malloc(sizeof(Patient));
    newPatient->id = id;
    strcpy(newPatient->name, name);
    strcpy(newPatient->status, status);
    newPatient->next = NULL;
    return newPatient;
}

// 1. Kuyruk İşlemleri (Enqueue - Dequeue)
void enqueue(Queue* q, Patient* p) {
    p->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = p;
        return;
    }
    q->rear->next = p;
    q->rear = p;
}

Patient* dequeue(Queue* q) {
    if (q->front == NULL) {
        printf("Hata: Kuyruk bos, cikarilacak hasta yok!\n");
        return NULL;
    }
    Patient* temp = q->front;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    temp->next = NULL; // Düğümün bağını kopar
    return temp;
}

// 2. Yığın İşlemleri (Push - Pop)
void push(Stack* s, Patient* p) {
    p->next = s->top;
    s->top = p;
}

Patient* pop(Stack* s) {
    if (s->top == NULL) {
        printf("Hata: Yigin bos, muayene odasinda hasta yok!\n");
        return NULL;
    }
    Patient* temp = s->top;
    s->top = s->top->next;
    temp->next = NULL;
    return temp;
}

// 3. Bağlı Liste İşlemleri (Geçmiş listesine sona ekleme)
void addHistory(LinkedList* ll, Patient* p) {
    p->next = NULL;
    if (ll->head == NULL) {
        ll->head = ll->tail = p;
        return;
    }
    ll->tail->next = p;
    ll->tail = p;
}

// Listeleme Fonksiyonları
void listQueue(Queue* q) {
    printf("--- Bekleyen Hastalar ---\n");
    if (q->front == NULL) {
        printf("Hasta yok.\n");
        return;
    }
    Patient* temp = q->front;
    while (temp != NULL) {
        printf("Hasta ID: %d, Isim: %s, Durum: %s\n", temp->id, temp->name, temp->status);
        temp = temp->next;
    }
}

void listStack(Stack* s) {
    printf("--- Muayene Odasindaki Hastalar ---\n");
    if (s->top == NULL) {
        printf("Hasta yok.\n");
        return;
    }
    Patient* temp = s->top;
    while (temp != NULL) {
        printf("Hasta ID: %d, Isim: %s, Durum: %s\n", temp->id, temp->name, temp->status);
        temp = temp->next;
    }
}

void listHistory(LinkedList* ll) {
    printf("--- Tamamlanan Hastalar ---\n");
    if (ll->head == NULL) {
        printf("Hasta yok.\n");
        return;
    }
    Patient* temp = ll->head;
    while (temp != NULL) {
        printf("Hasta ID: %d, Isim: %s, Durum: %s\n", temp->id, temp->name, temp->status);
        temp = temp->next;
    }
}

int main() {
    // Veri yapılarını başlat
    Queue beklemeKuyrugu = {NULL, NULL};
    Stack muayeneYigini = {NULL};
    LinkedList gecmisKayitlar = {NULL, NULL};

    // 4. Hastaları sisteme kaydetme
    enqueue(&beklemeKuyrugu, createPatient(1, "Ahmet", "bekliyor"));
    enqueue(&beklemeKuyrugu, createPatient(2, "Mehmet", "bekliyor"));
    enqueue(&beklemeKuyrugu, createPatient(3, "Ayse", "bekliyor"));

    printf("=== Baslangic Durumu ===\n");
    listQueue(&beklemeKuyrugu);
    printf("\n");
    listStack(&muayeneYigini);
    printf("\n");
    listHistory(&gecmisKayitlar);
    printf("\n----------------------------------\n");

    // 5. Ahmet'in muayene işlemleri
    Patient* islemGoren = dequeue(&beklemeKuyrugu);
    if (islemGoren != NULL) {
        strcpy(islemGoren->status, "muayenede");
        push(&muayeneYigini, islemGoren);
        
        // Muayenesi tamamlanan hastayı yığından çıkarıp geçmişe ekleme
        islemGoren = pop(&muayeneYigini);
        strcpy(islemGoren->status, "tamamlandi");
        addHistory(&gecmisKayitlar, islemGoren);
    }

    // 6. Mehmet'in muayene işlemleri
    islemGoren = dequeue(&beklemeKuyrugu);
    if (islemGoren != NULL) {
        strcpy(islemGoren->status, "muayenede");
        push(&muayeneYigini, islemGoren);
        
        // Muayenesi tamamlanan hastayı yığından çıkarıp geçmişe ekleme
        islemGoren = pop(&muayeneYigini);
        strcpy(islemGoren->status, "tamamlandi");
        addHistory(&gecmisKayitlar, islemGoren);
    }

    // 7. Ayşe bekleme kuyruğunda kalıyor (Ekstra işlem yapılmıyor)

    // 8. Tüm işlemler sonrası beklenen son çıktıyı yazdırma
    printf("=== Tum Islemler Sonrasi ===\n\n");
    
    listQueue(&beklemeKuyrugu);
    printf("\n");
    
    listStack(&muayeneYigini);
    printf("\n");
    
    listHistory(&gecmisKayitlar);

    return 0;
}