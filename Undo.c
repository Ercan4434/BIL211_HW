#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Kelime dugumu yapisi (Stack)
typedef struct Word {
    char text[50];
    struct Word* next;
} Word;

void pushWord(Word** top, char* text) { // Stack'e kelime ekleme (push)
    Word* newWord = (Word*)malloc(sizeof(Word));
    strcpy(newWord->text, text);
    newWord->next = *top;
    *top = newWord;
    printf("> add %s\n", text);
}

void popWord(Word** top) { // Stack'ten kelime cikarma (pop)
    if (*top == NULL) {
        printf("Geri alinacak bir kelime yok (Stack bos).\n");
        return;
    }
    Word* temp = *top;
    *top = (*top)->next;
    printf("> undo (Geri alinan kelime: %s)\n", temp->text);
    free(temp);
}

// Stack LIFO (Son Giren Ilk Cikar) oldugu icin cumleyi bastan sona 
// dogru sirayla okumak adina recursive (ozyineli) fonksiyon kullaniyoruz.
void printSentence(Word* top) { // Stack'teki kelimeleri bastan sona dogru yazdirma
    if (top == NULL) return;
    printSentence(top->next); 
    printf("%s ", top->text);
}

void showWords(Word* top) { // Stack'teki kelimeleri gosterme
    printf("> show -> ");
    if (top == NULL) { // Stack bos ise "(Bos)" yazdir
        printf("(Bos)"); 
    } else {
        printSentence(top); // Kelimeleri bastan sona dogru yazdir
    }
    printf("\n");
}

int main() {
    Word* stackTop = NULL;
    char secim;
    char kelime[50];
    
    while (1) {
        printf("\n--- Undo (Geri Alma) Simulasyonu ---\n");
        printf("1 - Metin Ekle (add)\n");
        printf("2 - Geri Al (undo)\n");
        printf("3 - Kelimeleri Goster (show)\n");
        printf("e - Cikis\n");
        printf("Seciminiz: ");
        scanf(" %c", &secim);
        
        if (secim == 'e' || secim == 'E') {
            printf("Cikis yapiliyor...\n");
            break;
        }
        
        switch (secim) {
            case '1':
                printf("Eklenecek kelimeyi girin: ");
                // Bosluk iceren metinleri de alabilmek icin %[^\n] kullanildi
                scanf(" %[^\n]", kelime); 
                pushWord(&stackTop, kelime);
                break;
            case '2':
                popWord(&stackTop);
                break;
            case '3':
                showWords(stackTop);
                break;
            default:
                printf("Gecersiz secim! Lutfen menudeki seceneklerden birini giriniz.\n");
        }
    }
    
    return 0;
}