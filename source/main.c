#include <3ds.h>
#include <citro2d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gui.c"

#define MAX_NOTES 10
#define MAX_MATIERES 10

typedef struct {
    char nom[32];
    int notes[MAX_NOTES];
    int nbNotes;
} Matiere;

Matiere matieres[MAX_MATIERES];
int nbMatieres = 0;
int selection = 0;

void chargerNotes() {
    FILE *f = fopen("romfs:/notes.txt", "r");
    if (!f) return;

    char ligne[128];
    while (fgets(ligne, sizeof(ligne), f) && nbMatieres < MAX_MATIERES) {
        ligne[strcspn(ligne, "\r\n")] = 0;
        char *nom = strtok(ligne, ":");
        char *notesStr = strtok(NULL, ":");
        if (!nom || !notesStr) continue;

        strcpy(matieres[nbMatieres].nom, nom);
        int i = 0;
        char *token = strtok(notesStr, ",");
        while (token && i < MAX_NOTES) {
            matieres[nbMatieres].notes[i++] = atoi(token);
            token = strtok(NULL, ",");
        }
        matieres[nbMatieres].nbNotes = i;
        nbMatieres++;
    }
    fclose(f);
}

float moyenne(Matiere *m) {
    if (m->nbNotes == 0) return 0.0f;
    int somme = 0;
    for (int i=0;i<m->nbNotes;i++) somme+=m->notes[i];
    return (float)somme/m->nbNotes;
}

int main() {
    gfxInitDefault();
    C2D_Init();
    romfsInit();

    chargerNotes();

    Button nextBtn = makeButton(20, 180, "Suivant", C2D_Color32(0, 100, 255, 255));
    Button prevBtn = makeButton(20, 230, "Precedent", C2D_Color32(255, 100, 0, 255));

    while (aptMainLoop()) {
        hidScanInput();
        u32 kDown = hidKeysDown();
        touchPosition touch;
        hidTouchRead(&touch);

        if (kDown & KEY_START) break;

        if ((kDown & KEY_UP) && selection>0) selection--;
        if ((kDown & KEY_DOWN) && selection<nbMatieres-1) selection++;

        if(buttonTouched(&nextBtn,&touch)) selection=(selection+1)%nbMatieres;
        if(buttonTouched(&prevBtn,&touch)) selection=(selection-1+nbMatieres)%nbMatieres;

        consoleClear();
        printf("SchoolBook 3DS\n\n");
        if(nbMatieres>0){
            Matiere *m=&matieres[selection];
            printf("Matiere: %s\n\n",m->nom);
            printf("Notes:\n");
            for(int i=0;i<m->nbNotes;i++) printf("- %d\n",m->notes[i]);
            printf("\nMoyenne: %.2f\n",moyenne(m));
        }
        printf("\nUP/DOWN: changer | START: quitter\nTouch: changer matiere\n");

        drawButton(&nextBtn);
        drawButton(&prevBtn);

        gspWaitForVBlank();
    }

    romfsExit();
    C2D_Fini();
    gfxExit();
    return 0;
}
