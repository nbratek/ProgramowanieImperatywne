#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int KOLORY = 4;
const int KARTY = 13 * 4;
const int NORMALNA = 0;
const int UPROSZCZONA = 1;

int starszenstwo(int kod_karty)
{
    return kod_karty / KOLORY;
}

void rand_permutation(int *tab, int n)
{
    int i, k, tmp;

    for (i = 0; i < n; i++)
        tab[i] = i;

    for (i = 0; i < n - 1; i++)
    {
        k = rand() % (n - i) + i;

        tmp = tab[i];
        tab[i] = tab[k];
        tab[k] = tmp;
    }
}

struct Gracz
{
    int* karty;
    int poczatek;
    int koniec;
    int liczba_kart;
};

void inicjuj_gracza(struct Gracz* gracz)
{
    gracz->karty = malloc(KARTY * sizeof(int));
    gracz->poczatek = 0;
    gracz->koniec = 0;
    gracz->liczba_kart = 0;
}

void dodaj_karte(struct Gracz* gracz, int karta)
{
    gracz->karty[gracz->koniec] = karta;
    gracz->koniec++;
    if (gracz->koniec == KARTY)
        gracz->koniec = 0;
    gracz->liczba_kart++;
}

int zabierz_karte(struct Gracz* gracz)
{
    int karta = gracz->karty[gracz->poczatek];
    gracz->poczatek++;
    if (gracz->poczatek == KARTY)
        gracz->poczatek = 0;
    gracz->liczba_kart--;
    return karta;
}

void usun_gracza(struct Gracz* gracz)
{
    free(gracz->karty);
}

int czy_ma_karty(struct Gracz* gracz)
{
    return gracz->liczba_kart > 0;
}

int main()
{
    // Wczytanie danych wejściowych
    int seed, wersja, maksymalna_liczba_konfliktow;

    scanf("%d%d%d", &seed, &wersja, &maksymalna_liczba_konfliktow);

    // Generowanie talii kart
    srand(seed);

    int* talia = malloc(KARTY * sizeof(int));

    rand_permutation(talia, KARTY);

    // Rozdanie kart
    struct Gracz gracz1, gracz2;

    inicjuj_gracza(&gracz1);
    inicjuj_gracza(&gracz2);

    int i;

    for (i = 0; i < KARTY / 2; i++)
        dodaj_karte(&gracz1, talia[i]);
    for (i = KARTY / 2; i < KARTY; i++)
        dodaj_karte(&gracz2, talia[i]);

    free(talia);

    // Rozgrywka
    for (i = 0; i < maksymalna_liczba_konfliktow; i++)
    {
        // Jeden z graczy nie ma już kart - kończymy rozgrywkę
        if (!czy_ma_karty(&gracz1) || !czy_ma_karty(&gracz2))
            break;

        if (wersja == UPROSZCZONA)
        {
            int karta_1 = zabierz_karte(&gracz1);
            int karta_2 = zabierz_karte(&gracz2);

            if (starszenstwo(karta_1) > starszenstwo(karta_2))
            {
                dodaj_karte(&gracz1, karta_1);
                dodaj_karte(&gracz1, karta_2);
            }
            else if (starszenstwo(karta_1) < starszenstwo(karta_2))
            {
                dodaj_karte(&gracz2, karta_2);
                dodaj_karte(&gracz2, karta_1);
            }
            else
            {
                dodaj_karte(&gracz1, karta_1);
                dodaj_karte(&gracz2, karta_2);
            }
        }
        else
        {
            int* stol_1 = malloc(KARTY * sizeof(int));
            int* stol_2 = malloc(KARTY * sizeof(int));

            int stol_1_liczba_kart = 0;
            int stol_2_liczba_kart = 0;

            int karta_1 = zabierz_karte(&gracz1);
            int karta_2 = zabierz_karte(&gracz2);

            stol_1[stol_1_liczba_kart] = karta_1;
            stol_1_liczba_kart++;
            stol_2[stol_2_liczba_kart] = karta_2;
            stol_2_liczba_kart++;

            while (starszenstwo(karta_1) == starszenstwo(karta_2))
            {
                i++;
                if (i == maksymalna_liczba_konfliktow)
                {
                    int k;

                    printf("0 %d %d", gracz1.liczba_kart + stol_1_liczba_kart, gracz2.liczba_kart + stol_2_liczba_kart);

                    //for (k = 0; k < stol_1_liczba_kart; k++)
                    //    printf("%d ", stol_1[k]);
                    //while (czy_ma_karty(&gracz1))
                    //    printf("%d ", zabierz_karte(&gracz1));
                    //printf("\n");

                    //for (k = 0; k < stol_2_liczba_kart; k++)
                    //    printf("%d ", stol_2[k]);
                    //while (czy_ma_karty(&gracz2))
                    //    printf("%d ", zabierz_karte(&gracz2));
                    //printf("\n");

                    usun_gracza(&gracz1);
                    usun_gracza(&gracz2);
                    free(stol_1);
                    free(stol_2);

                    return 0;
                }

                int j;

                for (j = 0; j < 2; j++)
                {
                    if (!czy_ma_karty(&gracz1) || !czy_ma_karty(&gracz2))
                    {
                        // Zabrakło kart jednemu z graczy - kończymy rozgrywkę
                        int k;

                        printf("1 %d %d", gracz1.liczba_kart + stol_1_liczba_kart, gracz2.liczba_kart + stol_2_liczba_kart);

                        //for (k = 0; k < stol_1_liczba_kart; k++)
                        //    printf("%d ", stol_1[k]);
                        //while (czy_ma_karty(&gracz1))
                        //    printf("%d ", zabierz_karte(&gracz1));
                        //printf("\n");

                        //for (k = 0; k < stol_2_liczba_kart; k++)
                        //    printf("%d ", stol_2[k]);
                        //while (czy_ma_karty(&gracz2))
                        //    printf("%d ", zabierz_karte(&gracz2));
                        //printf("\n");

                        usun_gracza(&gracz1);
                        usun_gracza(&gracz2);
                        free(stol_1);
                        free(stol_2);

                        return 0;
                    }

                    karta_1 = zabierz_karte(&gracz1);
                    karta_2 = zabierz_karte(&gracz2);

                    stol_1[stol_1_liczba_kart] = karta_1;
                    stol_1_liczba_kart++;
                    stol_2[stol_2_liczba_kart] = karta_2;
                    stol_2_liczba_kart++;
                }
            }

            if (starszenstwo(karta_1) > starszenstwo(karta_2))
            {
                int j;

                for (j = 0; j < stol_1_liczba_kart; j++)
                    dodaj_karte(&gracz1, stol_1[j]);
                for (j = 0; j < stol_2_liczba_kart; j++)
                    dodaj_karte(&gracz1, stol_2[j]);
            }
            else
            {
                int j;

                for (j = 0; j < stol_2_liczba_kart; j++)
                    dodaj_karte(&gracz2, stol_2[j]);
                for (j = 0; j < stol_1_liczba_kart; j++)
                    dodaj_karte(&gracz2, stol_1[j]);
            }

            free(stol_1);
            free(stol_2);
        }
    }

    // Wypisanie wyniku
    if (!czy_ma_karty(&gracz1))
    {
        // Wygrana gracza B
        printf("3\n");
        while (czy_ma_karty(&gracz2))
            printf("%d ", zabierz_karte(&gracz2));
        //printf("\n");
    }
    else if (!czy_ma_karty(&gracz2))
    {
        // Wygrana gracza A
        printf("2 ");
        printf("%d", i);
    }
    else
    {
        // Przekroczenie maksymalnej liczby konfliktów
        printf("0 %d %d", gracz1.liczba_kart, gracz2.liczba_kart);
        //while (czy_ma_karty(&gracz1))
            //("%d ", zabierz_karte(&gracz1));
        //printf("\n");
        //while (czy_ma_karty(&gracz2))
            //printf("%d ", zabierz_karte(&gracz2));
        //printf("\n");
    }

    // Czyszczenie pamięci
    usun_gracza(&gracz1);
    usun_gracza(&gracz2);

    return 0;
}