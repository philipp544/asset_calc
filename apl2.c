#include <stdio.h>          // printf, fopen (fkt. für input, output)
#include <stdlib.h>         // malloc, qsort (speicherverwaltung, dateityp konvertierung usw.)
#include <string.h>         // strlen, strcpy (fkt. zur manipulation von zeichenketten)

#define MAX_STOCKS 50
#define MAX_NAME_LENGTH 50
#define FILE_PATH "C:\\Users\\test\\Documents\\uni\\stocks.txt"     // doppelt backslash da vs sonst probleme hat datei zu öffnen

struct StockEntry {                 // allgemeine Struktur festgelegt
    char isin[13];                  // char da Zahlen und Buchstaben vorkommen
    char name[MAX_NAME_LENGTH];     // sonderzeichen
    int quantity;
    double price;                   // fliesskommazahl
    double dividend;
    char date[11];
    char buyer[50];
};
  
void swap(struct StockEntry* a, struct StockEntry* b) {      //quick sort funktion beginn, void da keine rückgabe erwartet wird
    struct StockEntry temp = *a;                             // temporäre variable die wert in a zwischenspeichert 
    *a = *b;                                                 // wert von a wird an die adresse von b übergeben, pointer da effizienter, es werden keine kopien der stockEntry[] elemente gemacht 
    *b = temp;                                               // wert von b wird an temp übergeben -> ermöglicht vertauschen der stockEntry[] elemente 
}

int partition(struct StockEntry stocks[], int low, int high) {      // teilt array in zwei teile 
    char pivot[MAX_NAME_LENGTH];                                    // pivot = name des wertes an letzter stelle
        strcpy_s(pivot, sizeof(pivot), stocks[high].name);          // pivot string wird initialisiert und name kopiert
        int i = low - 1;                                            // i = element kleiner als pivot

    for (int j = low; j <= high - 1; j++) {                         // durchläuft das Array von low bis high - 1 und vergleicht die Namen der Elemente mit dem Pivot-String
        if (strcmp(stocks[j].name, pivot) < 0) {                    // wenn name kleiner ist als pivot string, wird i erhöht, und i und j wird getauscht 
            i++;
            swap(&stocks[i], &stocks[j]);
        }
    }
    swap(&stocks[i + 1], &stocks[high]);                            // pivot wird an richtige position geschoben indem es mit i + 1 getauscht wird
    return (i + 1);                                                 // i + 1 wird zurückgegeben 
}

void quickSort(struct StockEntry stocks[], int low, int high) {
    if (low < high) {                                               // überprüfung ob low<high, wenn ja 
        int pi = partition(stocks, low, high);                      // wird partition aufgerufen um index des pivot-elementes zu erhalten
        quickSort(stocks, low, pi - 1);
        quickSort(stocks, pi + 1, high);                            // quick sort für teillisten links und rechts aufgerufen, bis array sortiert ist 
    }
}                                                                   // quick sort funktion ende

double dividendYield(double price, double dividend) {           // dividendenrendite 
    return (dividend / price) * 100.0;                          // double, da finanzberechnung ausreichende genauigkeit benötigt wird
}

double averageQuantity(struct StockEntry stocks[], int count) {     // durchschnittliche stückzahl gekaufter anteile (als double), count = anzahl der einträge im stocks[] array
    int totalQuantity = 0;                                          // speichert gesamtstückzahl ab 
    int numPurchases = 0;                                           // speichert Anzahl der durchgeführten Käufe ab
    for (int i = 0; i < count; i++) {                               //
        if (stocks[i].quantity > 0) {                               // prüft ob ein kauf erfolgt ist, wenn dies der fall ist, wird stkzahl zu totalQuantity addiert und numPurchases um 1 erhöht
            totalQuantity += stocks[i].quantity;
            numPurchases++;
        }
    }
    return (numPurchases > 0) ? (double)totalQuantity / numPurchases : 0.0; // prüft ob käufe getätigt wurden, falls ja, wird berechnung durchgeführt, sonst 0.0 als rückgabe
}

void topThreeDividends(struct StockEntry stocks[], int count) {             // Aktien mit den drei größten dividenden, void da keine rückgabe erfolgt -> printf, count -> anzahl der einträge im stocks[] array
    struct StockEntry temp;                                                 // bubble-sort, temp wird benutzt als zwischenspeicher der sortierung
    for (int i = 0; i < count - 1; i++) {                                   // zuständig dafür das höchste dividende an letzte stelle wandert                      
        for (int j = 0; j < count - i - 1; j++) {                           // es wird verglichen ob benachbarte dividende > ist, wenn ja tauschen, damit die liste der größe nach sortiert ist 
            if (stocks[j].dividend < stocks[j + 1].dividend) {
                temp = stocks[j];
                stocks[j] = stocks[j + 1];
                stocks[j + 1] = temp;
            }
        }
    }

    printf("\nTop 3 Aktien mit groesster Dividende:\n");                    // ausgabe der größten dividende 
    for (int i = 0; i < count && i < 3; i++) {                              // for schleife um 3 aktien zu präsentieren
        printf("Aktie: %s, Dividende: %.2lf EUR\n", stocks[i].name, stocks[i].dividend);
    }
}

void addNewEntry(struct StockEntry stocks[], int* count) {          // neuen aktieneintrag erstellen
    if (*count >= MAX_STOCKS) {                                     // überprüfung ob MAX_STOCKS limít erreicht wurde
        printf("Maximale Anzahl an Einträgen erreicht.\n");
        return;
    }

    printf("Neuen Aktieneintrag erfassen:\n");

    do {
        printf("ISIN: ");
        scanf_s("%s", &stocks[*count].isin, sizeof(stocks[*count].isin));
        if (strlen(stocks[*count].isin) != 12) {                            // überprüfung ob isin gültige länge hat, strlen da char und nicht int vorliegt
            printf("Ungueltige ISIN. Die ISIN muss genau 12 Zeichen lang sein. Bitte geben Sie die ISIN erneut ein.\n");
        }
    } while (strlen(stocks[*count].isin) != 12);                            // solange bis länge genau 12 hat 

    printf("Name: ");                                                           // aktienname ohne überprüfung
    scanf_s(" %[^\n]s", stocks[*count].name, sizeof(stocks[*count].name));
        
    while (1) {
        printf("Anzahl: ");                                                             // anzahl der anteile wird überprüft, verhindert dass buchstaben eingegeben werden
        if (scanf_s("%d", &stocks[*count].quantity) == 1) break;
        else {
            printf("Ungueltige Anzahl. Bitte geben Sie eine gueltige Anzahl ein.\n");
            while (getchar() != '\n'); 
        }
    }

    printf("Preis: ");
    scanf_s("%lf", &stocks[*count].price, sizeof(stocks[*count].price));

    printf("Dividende: ");
    scanf_s("%lf", &stocks[*count].dividend, sizeof(stocks[*count].dividend));

    while (1) {
        printf("Datum (DD_MM_YYYY): ");                                     // überprüfung ob datum gültige länge und struktur hat 
        scanf_s("%s", stocks[*count].date, sizeof(stocks[*count].date));
        if (strlen(stocks[*count].date) == 10 && stocks[*count].date[2] == '_' && stocks[*count].date[5] == '_') break;
        else {
            printf("Ungültiges Datum. Bitte geben Sie das Datum erneut ein.\n");
        }
    }

    printf("Kaeufer/Verkaeufer: ");
    scanf_s("%s", stocks[*count].buyer, sizeof(stocks[*count].buyer));

    (*count)++;             //um nächsten freien index im stocks[] array zu markieren

    FILE* file;
    fopen_s(&file, FILE_PATH, "a");     // neuer eintrag wird in datei geschrieben  

    if (file != NULL) {             
        fprintf(file, "%s;%s;%d;%.2lf;%.2lf;%s;%s\n", stocks[*count - 1].isin, stocks[*count - 1].name,        // struktur der angegebenen infos wird festgelegt
            stocks[*count - 1].quantity, stocks[*count - 1].price, stocks[*count - 1].dividend,
            stocks[*count - 1].date, stocks[*count - 1].buyer);

        fclose(file);
        printf("Neuer Eintrag wurde erfolgreich hinzugefuegt.\n");
    }
    else {
        printf("Datei konnte nicht geoeffnet werden.\n");
    }
}

void displayMenu() {                                                // Menu für alle optionen 
    printf("\nMenu:\n");
    printf("1. Aktiennamen sortieren\n");
    printf("2. Dividendenrendite berechnen\n");
    printf("3. Durchschnittliche Stueckzahl aller Kaeufe\n");
    printf("4. Top 3 Aktien mit hoechster Dividende\n");
    printf("5. Neue Eintraege erfassen\n");
    printf("6. Beenden\n");
    printf("Waehlen Sie eine Option: ");
}

int main() {                                    // Funktion zum einlesen der textdatei 
    struct StockEntry stocks[MAX_STOCKS];
    int count = 0;                              // auf 0 gesetzt um anzahl der aktieneinträge zu verfolgen
    FILE* file;                                 
    errno_t err;
    err = fopen_s(&file, FILE_PATH, "r");

    if (err == 0) {
        int read = 0;
        do {
            read = fscanf_s(file, "%13[^;];%31[^;];%d;%lf;%lf;%11[^;];%31[^\n]\n",
                stocks[count].isin, sizeof(stocks[count].isin),
                stocks[count].name, sizeof(stocks[count].name),
                &stocks[count].quantity,
                &stocks[count].price,
                &stocks[count].dividend,
                stocks[count].date, sizeof(stocks[count].date),
                stocks[count].buyer, sizeof(stocks[count].buyer));

            if (read == 7) count++;

            if (read != 7 && feof(file)) {
                printf("Dateiformat nicht korrekt\n");
                return 1;
            }

        } while (!feof(file));


        fclose(file);
    }

    for (int i = 0; i < 7; i++)
    {
        printf("%s;%s;%d;%.2lf;%.2lf;%s;%s\n", stocks[i].isin, stocks[i].name, stocks[i].quantity, stocks[i].price, stocks[i].dividend, stocks[i].date, stocks[i].buyer);
    }

    int choice;
    do {                                                                        // switch case funktion, dient der ausgabe der gewünschten funktion 
        displayMenu();
        scanf_s("%d", &choice);

        switch (choice) {
        case 1:
            quickSort(stocks, 0, count - 1);
            printf("Sortierte Aktiennamen:\n");
            for (int i = 0; i < count; i++) {
                printf("%s\n", stocks[i].name);
            }
            break;
        case 2:
            printf("\nDividendenrendite:\n");
            for (int i = 0; i < count; i++) {
                double yield = dividendYield(stocks[i].price, stocks[i].dividend);
                printf("Aktie: %s, Dividendenrendite: %.2lf%%\n", stocks[i].name, yield);
            }
            break;
        case 3:
            printf("\nDurchschnittliche Stueckzahl aller getaetigten Kaeufe: %.2lf\n", averageQuantity(stocks, count));
            break;
        case 4:
            topThreeDividends(stocks, count);
            break;
        case 5:
            addNewEntry(stocks, &count);
            break;
        case 6:
            printf("Programm wird beendet.\n");
            break;
        default:
            printf("Ungueltige Eingabe. Bitte waehlen Sie eine gueltige Option.\n");
            break;
        }
    } while (choice != 6);
    return 0;
}




