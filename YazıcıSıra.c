#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Yazdirma isi dugumu
typedef struct PrintJob {
    char fileName[50];
    struct PrintJob* next;
} PrintJob;

// Kuyruk yapisi
typedef struct Queue {
    PrintJob* front;
    PrintJob* rear;
} Queue;

void enqueuePrintJob(Queue* q, char* fileName) { // Kuyruga yazdirma isi ekleme
    PrintJob* newJob = (PrintJob*)malloc(sizeof(PrintJob));
    strcpy(newJob->fileName, fileName);
    newJob->next = NULL;
    
    // Kuyruk bossa front ve rear yeni elemani gosterir
    if (q->rear == NULL) {
        q->front = q->rear = newJob;
    } else {
        // Kuyruk bos degilse rear'in sonuna ekle
        q->rear->next = newJob;
        q->rear = newJob;
    }
    printf("Belge kuyruga eklendi: %s\n", fileName);
}

void processNextJob(Queue* q) { // Kuyruktaki siradaki yazdirma isini isleme al
    if (q->front == NULL) {
        printf("Kuyruk bos, yazdirilacak belge yok!\n");
        return;
    }
    
    PrintJob* temp = q->front; 
    q->front = q->front->next; 
    
    // Eger kuyruktaki son eleman cikarildiysa rear da NULL olmali
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    printf("Yazdirildi (Kuyruktan cikarildi): %s\n", temp->fileName);
    free(temp);
}

void showQueue(Queue q) {
    if (q.front == NULL) {
        printf("Kuyruk bos.\n");
        return;
    }
    
    PrintJob* temp = q.front;
    int index = 1;
    printf("\n--- Yazdirma Kuyrugu ---\n");
    while (temp != NULL) {
        printf("%d. %s\n", index++, temp->fileName);
        temp = temp->next;
    }
    printf("------------------------\n");
}

int main() {
    Queue printQueue = {NULL, NULL};
    char secim;
    char dosyaAdi[50];
    
    while (1) {
        printf("\n--- Yazici Islem Sirasi (Queue) Simulasyonu ---\n");
        printf("1 - Yeni Dosya Ekle (Yazdirma isini kuyruga al)\n");
        printf("2 - Yazdir (Siradaki isi isleme al)\n");
        printf("3 - Kuyrugu Goster\n");
        printf("e - Cikis\n");
        printf("Seciminiz: ");
        scanf(" %c", &secim);
        
        if (secim == 'e' || secim == 'E') {
            printf("Cikis yapiliyor...\n");
            break;
        }
        
        switch (secim) {
            case '1':
                printf("Kuyruga eklenecek dosya adi: ");
                // Dosya adi bosluk icerebilme ihtimaline karsi %[^\n] kullanildi
                scanf(" %[^\n]", dosyaAdi); 
                enqueuePrintJob(&printQueue, dosyaAdi);
                break;
            case '2':
                processNextJob(&printQueue);
                break;
            case '3':
                showQueue(printQueue);
                break;
            default:
                printf("Gecersiz secim! Lutfen menudeki seceneklerden birini giriniz.\n");
        }
    }
    
    return 0;
}