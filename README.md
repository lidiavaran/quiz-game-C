# Quiz Game (C)

Un joc simplu de tip quiz în consolă despre limbajul C, cu întrebări pe trei niveluri de dificultate: ușor, mediu și greu.

---

## Descriere

Acest joc testează cunoștințele despre limbajul C printr-un set de întrebări. Fiecare răspuns corect aduce puncte în funcție de dificultate:

- **Ușor**: 1 punct  
- **Mediu**: 2 puncte  
- **Greu**: 3 puncte  

Poți începe un joc nou, relua un joc salvat sau vizualiza clasamentul.

---

## Compilare și rulare

Pentru a compila jocul, folosește comanda:

```bash
gcc Quiz_game.c -o quiz
```

Pentru a rula jocul:

./quiz

## Fișiere importante

Quiz_game.c — codul sursă al jocului
easy.txt — întrebări ușoare
medium.txt — întrebări medii
hard.txt — întrebări grele
save1.txt, save2.txt, save3.txt — fișiere pentru salvarea progresului (generate automat)
clasament.txt — fișier pentru scorurile jucătorilor

## Instrucțiuni

1. Alege opțiunea din meniu pentru a începe jocul, a relua un joc salvat, a vedea instrucțiunile sau clasamentul.
2. Răspunde întrebărilor tastând litera corespunzătoare răspunsului (a, b, c sau d).
3. Pentru a salva progresul în timpul jocului, tastează s.
4. La final, introdu numele pentru a fi înregistrat în clasament.
5. Poți salva progresul în unul dintre cele 3 sloturi disponibile.

## Autor
Văran Lidia - https://github.com/lidiavaran
