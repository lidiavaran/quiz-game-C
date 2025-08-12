#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define EASY_QUESTIONS   4
#define MEDIUM_QUESTIONS 4
#define HARD_QUESTIONS   2   

#define RESET    "\x1b[0m"
#define RED      "\x1b[31m"
#define GREEN    "\x1b[32m"
#define YELLOW   "\x1b[33m"
#define BLUE     "\x1b[34m"
#define MAGENTA  "\x1b[35m"
#define CYAN     "\x1b[36m"
#define BOLD     "\x1b[1m"

typedef struct {
    char intrebare[256];
    char raspunsCorect[256];
    char raspunsuri[4][256];
} Intrebare;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showWelcome() {
    clearScreen();
    printf(CYAN BOLD
           "=======================================\n"
           "      ❓ Quiz despre limbajul C ❓\n"
           "=======================================\n\n"
           RESET);
}

void showMenu() {
    clearScreen();
    printf(CYAN BOLD "===== MENIU PRINCIPAL =====\n\n" RESET);
    printf(CYAN "🎮" RESET " 1. Începe " BLUE "joc nou" RESET "\n");
    printf(CYAN "🔁" RESET " 2. Reia " GREEN "joc salvat" RESET "\n");
    printf(CYAN "📖" RESET " 3. Instructiuni\n");
    printf(CYAN "📊" RESET " 4. Clasament\n");
    printf(CYAN "❌" RESET " 5. Ieșire\n");
    printf("\nAlege o opțiune: ");
}


int getValidOption() {
    int opt;
    int c;
    while (1) {
        if (scanf("%d", &opt) != 1) {
            while ((c = getchar()) != '\n' && c != EOF);
            printf(RED "❗ Input invalid! Tastează un număr.\n" RESET);
            continue;
        }
        while ((c = getchar()) != '\n' && c != EOF);
        if (opt >= 1 && opt <= 5) return opt;
        printf(RED "❗ Opțiune invalidă! Alege între 1 și 5.\n" RESET);
    }
}

void showInstructions() {
    clearScreen();
    printf(BOLD "===== INSTRUCTIUNI =====\n\n" RESET);
    printf("1️⃣  Răspunde " BLUE "corect" RESET " pentru a câștiga puncte.\n");
    printf("2️⃣  " MAGENTA "3 niveluri" RESET ": ușor (1p), mediu (2p), greu (3p).\n");
    printf("3️⃣  " YELLOW "10" RESET " întrebări per joc.\n");
    printf("4️⃣  La final, introdu " GREEN "numele" RESET " pentru clasament.\n");
    printf("5️⃣  Poți " BLUE "relua" RESET " un joc salvat din meniu.\n");
    printf("6️⃣  Tastează " BOLD "s" RESET " pentru a salva " RED "progresul" RESET ".\n\n");
    printf("Apasă Enter pentru a reveni la meniu...");
    getchar();
}

void loadQuestions(const char *files[], Intrebare q[], int nf, int *nq) {
    *nq = 0;
    char buffer[256];
    for (int f = 0; f < nf; f++) {
        FILE *fp = fopen(files[f], "r");
        if (!fp) {
            printf(RED "❗ Nu pot deschide %s\n" RESET, files[f]);
            exit(1);
        }
        while (*nq < EASY_QUESTIONS + MEDIUM_QUESTIONS + HARD_QUESTIONS) {
            if (!fgets(buffer, sizeof(buffer), fp)) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strcpy(q[*nq].intrebare, buffer);
            for (int j = 0; j < 4; j++) {
                if (!fgets(buffer, sizeof(buffer), fp)) break;
                buffer[strcspn(buffer, "\r\n")] = '\0';
                strcpy(q[*nq].raspunsuri[j], buffer);
            }
            if (!fgets(buffer, sizeof(buffer), fp)) break;
            buffer[strcspn(buffer, "\r\n")] = '\0';
            strcpy(q[*nq].raspunsCorect, buffer);
            (*nq)++;
        }
        fclose(fp);
    }
}

void shuffleQuestions(Intrebare q[], int n) {
    srand((unsigned)time(NULL));
    for (int i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        Intrebare tmp = q[i];
        q[i] = q[j];
        q[j] = tmp;
    }
}

int isValidAnswer(char a) {
    return (a >= 'a' && a <= 'd');
}

int askQuestions(Intrebare q[], int nq, int *score, int *idx) {
    int ansScore = *score;
    char ans;
    for (int i = *idx; i < nq; i++) {
        clearScreen();
        if (i == EASY_QUESTIONS)
            printf(CYAN "=== NIVEL MEDIU (2p) ===\n\n" RESET);
        else if (i == EASY_QUESTIONS + MEDIUM_QUESTIONS)
            printf(CYAN "=== NIVEL GREU (3p) ===\n\n" RESET);

        int pts = (i < EASY_QUESTIONS ? 1 :
                   i < EASY_QUESTIONS + MEDIUM_QUESTIONS ? 2 : 3);

        printf("❓ %s\n", q[i].intrebare);
        printf("a) %s\n", q[i].raspunsuri[0]);
        printf("b) %s\n", q[i].raspunsuri[1]);
        printf("c) %s\n", q[i].raspunsuri[2]);
        printf("d) %s\n", q[i].raspunsuri[3]);

        printf(BOLD "Răspuns (a–d, s=salvează): " RESET);
        scanf(" %c", &ans);
        getchar();

        if (ans == 's') {
            char saveName[50];
            int slot;
            clearScreen();
            printf(CYAN BOLD "=== SALVARE PROGRES ===\n" RESET);
            printf("Nume pentru salvare: ");
            scanf("%49s", saveName);
            getchar();
            printf("Slot (1-3): ");
            scanf("%d", &slot);
            getchar();
            if (slot < 1 || slot > 3) slot = 1;
            char fn[16];
            sprintf(fn, "save%d.txt", slot);
            FILE *f = fopen(fn, "w");
            if (f) {
                fprintf(f, "%s %d\n", saveName, ansScore);
                fclose(f);
            }
            printf(GREEN "✅ Salvare %s" RESET, fn);
            getchar();
            return -2;
        }

        if (!isValidAnswer(ans)) {
            printf(RED "❗ Alege între a, b, c sau d!" RESET);
            getchar();
            i--;
            continue;
        }

        if (ans == q[i].raspunsCorect[0]) {
            ansScore += pts;
            printf(GREEN "✅ Corect! +%d p" RESET, pts);
        } else {
            printf(RED "❌ Greșit!" RESET);
            printf(" Scor final: %d", ansScore);
            getchar();
            *idx = i + 1;
            *score = ansScore;
            return -1;
        }

        printf("\nScor: %d\nApasă Enter...", ansScore);
        getchar();
        *idx = i + 1;
    }
    *score = ansScore;
    return ansScore;
}

int loadGame(const char *fn, char *name, int *score) {
    FILE *f = fopen(fn, "r");
    if (!f) return 0;
    if (fscanf(f, "%49s %d", name, score) != 2) {
        fclose(f);
        return 0;
    }
    fclose(f);
    return 1;
}

void saveScore(const char *name, int score) {
    FILE *f = fopen("clasament.txt", "a");
    if (!f) return;
    fprintf(f, "%s - %d\n", name, score);
    fclose(f);
}

void showClasament() {
    clearScreen();
    printf(CYAN BOLD "===== CLASAMENT =====\n\n" RESET);
    char line[128];
    FILE *f = fopen("clasament.txt", "r");
    if (!f) {
        printf("❗ Niciun scor salvat.\n");
        getchar();
        return;
    }
    while (fgets(line, sizeof(line), f)) {
        char *sep = strstr(line, " - ");
        if (sep) {
            *sep = '\0';
            printf(MAGENTA "%s" RESET " - %s", line, sep + 3);
        } else {
            printf("%s", line);
        }
    }
    fclose(f);
    printf("\nApasă Enter...");
    getchar();
}

int main() {
    const char *files[] = { "easy.txt", "medium.txt", "hard.txt" };
    int running = 1;
    while (running) {
        showWelcome();
        showMenu();
        int choice = getValidOption();
        switch (choice) {
            case 1: {
                Intrebare qs[EASY_QUESTIONS+MEDIUM_QUESTIONS+HARD_QUESTIONS];
                int nq = 0, score = 0, idx = 0;
                loadQuestions(files, qs, 3, &nq);
                shuffleQuestions(qs, nq);
                int res = askQuestions(qs, nq, &score, &idx);
                if (res == -2) break;
                clearScreen();
                if (res < 0)
                    printf(RED "🔴 Joc terminat. Scor %d" RESET, score);
                else
                    printf(GREEN "🎉 Ai terminat! Scor %d 🎉" RESET, score);
                printf("\nNume clasament: ");
                char nm[50];
                scanf("%49s", nm);
                getchar();
                saveScore(nm, score);
                getchar();
                break;
            }
            case 2: {
                clearScreen();
                printf(CYAN BOLD "=== REIA JOC SALVAT ===\n" RESET);
                printf("Slot (1-3): ");
                int slot;
                scanf("%d", &slot);
                getchar();
                if (slot < 1 || slot > 3) slot = 1;
                char fn[16];
                sprintf(fn, "save%d.txt", slot);
                char name[50];
                int score;
                if (!loadGame(fn, name, &score)) {
                    printf(RED "❗ Nu există %s" RESET, fn);
                    getchar();
                    break;
                }
                printf("Reluare %s cu scor %d", name, score);
                getchar();
                Intrebare qs[EASY_QUESTIONS+MEDIUM_QUESTIONS+HARD_QUESTIONS];
                int nq = 0, idx = 0;
                loadQuestions(files, qs, 3, &nq);
                shuffleQuestions(qs, nq);
                int res = askQuestions(qs, nq, &score, &idx);
                if (res == -2) break;
                clearScreen();
                if (res < 0)
                    printf(RED "🔴 Joc terminat. Scor %d" RESET, score);
                else
                    printf(GREEN "🎉 Reluare terminată! Scor %d 🎉" RESET, score);
                printf("\nNume clasament: ");
                char nm2[50];
                scanf("%49s", nm2);
                getchar();
                saveScore(nm2, score);
                getchar();
                break;
            }
            case 3:
                showInstructions();
                break;
            case 4:
                showClasament();
                break;
            case 5:
                running = 0;
                break;
        }
    }
    clearScreen();
    printf(CYAN "Mulțumim că ai jucat! 👋\n" RESET);
    return 0;
}