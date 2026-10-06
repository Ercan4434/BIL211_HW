#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Sarki dugumu yapisi
typedef struct Song {
    char name[50];
    struct Song* next;
    struct Song* prev;
} Song;

void addSongToEnd(Song** head, char* name) {
    Song* newSong = (Song*)malloc(sizeof(Song)); //Node hafizasi ayrildi
    strcpy(newSong->name, name); // Node tanimlamalari
    newSong->next = NULL;
    
    if (*head == NULL) { // Eger liste bos ise yeni sarki head olur
        newSong->prev = NULL;
        *head = newSong;
        printf("'%s' listeye eklendi.\n", name);
        return;
    }
    
    Song* temp = *head; // Listeyi sonuna kadar gezmek icin temp pointeri
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newSong; // Yeni sarkiyi listenin sonuna ekle
    newSong->prev = temp;
    printf("'%s' listeye eklendi.\n", name);
}

void removeSong(Song** head, char* name) {
    if (*head == NULL) {
        printf("Liste bos!\n");
        return;
    }
    
    Song* temp = *head; // Listeyi gezmek icin temp pointeri
    while (temp != NULL && strcmp(temp->name, name) != 0) {
        temp = temp->next;
    }
    
    if (temp == NULL) { // Sarki bulunamadiysa
        printf("Silmek istenen '%s' bulunamadi.\n", name);
        return;
    }
    
    if (temp->prev != NULL) temp->prev->next = temp->next; // Orta veya sondaki eleman siliniyorsa onceki elemanin next pointerini guncelle
    else *head = temp->next; // Bastaki eleman siliniyorsa head'i guncelle
    
    if (temp->next != NULL) temp->next->prev = temp->prev; 
    
    free(temp); // Hafizayi serbest birak
    printf("'%s' silindi.\n", name);
}

void playNext(Song** current) {
    if (*current == NULL) {
         printf("Liste bos veya calan sarki secilmedi.\n");
         return;
    }
    if ((*current)->next == NULL) {
        printf("Son sarkidasiniz, sonraki sarki yok.\n");
    } else {
        *current = (*current)->next; // Sonraki sarkiyi cal
    }
}

void playPrevious(Song** current) {
    if (*current == NULL) {
         printf("Liste bos veya calan sarki secilmedi.\n");
         return;
    }
    if ((*current)->prev == NULL) {
        printf("Ilk sarkidasiniz, onceki sarki yok.\n");
    } else {
        *current = (*current)->prev; // Onceki sarkiyi cal
    }
}

void displayPlaylist(Song* head) {
    if (head == NULL) {
        printf("\n[Liste bos]\n");
        return;
    }
    printf("\n--- Calma Listesi ---\n");
    while (head != NULL) {
        printf("- %s\n", head->name); // Sarki adini yazdir
        head = head->next; // Sonraki sarkiya gec
    }
    printf("---------------------\n");
}

int main() {
    Song* playlist = NULL;
    Song* currentSong = NULL;
    char secim;
    char sarkiAdi[50];
    
    while (1) {
        displayPlaylist(playlist);
        
        if (currentSong != NULL) {
            printf("--> Su an caliyor: %s\n", currentSong->name);
        } else {
            printf("--> Su an calan sarki yok.\n");
        }
        
        printf("\nIslemler:\n");
        printf("1 - Onceki Sarki (prev)\n");
        printf("2 - Sonraki Sarki (next)\n");
        printf("3 - Sona Ekle\n");
        printf("0 - Sil (Su anki sarkiyi siler)\n");
        printf("e - Cikis\n");
        printf("Seciminiz: ");
        scanf(" %c", &secim); 
        
        if (secim == 'e' || secim == 'E') {
            printf("Cikis yapiliyor...\n");
            break;
        }
        
        switch (secim) {
            case '1':
                playPrevious(&currentSong);
                break;
            case '2':
                playNext(&currentSong);
                break;
            case '3':
                printf("Eklenecek sarki adi: ");
                scanf(" %[^\n]", sarkiAdi); 
                addSongToEnd(&playlist, sarkiAdi);
                
                // Eger listeye ilk sarki eklendiyse otomatik olarak onu calmaya basla
                if (currentSong == NULL) {
                    currentSong = playlist;
                }
                break;
            case '0':
                if (currentSong == NULL) {
                    printf("Silinecek sarki yok (liste bos).\n");
                } else {
                    char silinecek[50];
                    strcpy(silinecek, currentSong->name);
                    
                    // Sarkiyi silmeden once, calan sarkiyi sonrakine gecir
                    // Sonraki yoksa oncekine gecir
                    Song* yeniCalan = currentSong->next;
                    if (yeniCalan == NULL) {
                        yeniCalan = currentSong->prev;
                    }
                    
                    removeSong(&playlist, silinecek);
                    currentSong = yeniCalan;
                }
                break;
            default:
                printf("Gecersiz secim! Lutfen menudeki seceneklerden birini giriniz.\n");
        }
    }
    
    return 0;
}