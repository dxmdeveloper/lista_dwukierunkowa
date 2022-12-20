/**************************************************************
 * Title: Projekt na zaliczenie przedmiotu Programowanie 1    *
 * Description: Lista dwustronna z zapisem i menu             *
 * Created at: 2022-12-14                                     *
 * Author: Maciej Bąbolewski (bm53943)                        *
 **************************************************************/

#include <stdlib.h> 
#include <stdio.h> 
#include "dbl_list.h"

typedef unsigned int uint;

// clear_screen macro
#ifdef _WIN32
    #define clear_screen() system("cls")
#elif defined(__unix__) || defined(__APPLE__)
    #define clear_screen() system("clear")
#else
    #define clear_screen() printf("\n")
#endif

// ---- function declarations ----
/// @return chosen option.
char showInitMenu();
/// @return chosen option.
char showMainMenu();
void showListCallbackFunc(uint iteration, const char * nodeContent);
/// @return str on success, null pointer on failure.
char * my_getline(FILE * infile, char * str, size_t num);

enum menuIndexEnum {
    MENU_LIST_INIT,
    MENU_MAIN,
    MENU_NULL //exit
};


// -----------------------------------------------------------
// -----------------   Program Entry Point   -----------------
// -----------------------------------------------------------
int main(int argc, char * argv[]){ 
    int exitCode = EXIT_SUCCESS;
    FILE * file = NULL;
    List list = {0};
    int menuNo = MENU_LIST_INIT;
    char option = '0';
    char inputText[100];

    // Menu 1
    while(menuNo == MENU_LIST_INIT){
        option = showInitMenu();
        if(option == 'e') break; // exit
        if(option == '2'){
            file = fopen("list.txt", "r");
            if(file == NULL){
                printf("Nie można otworzyć pliku list.txt!\n");
                option = '0';
            } else {
                list = list_LoadFromFile(file);
                fclose(file); file = NULL;
            }
        }
        if(option != '1' && option != '2'){
            printf("Nieprawidłowa opcja\n");
        } 
        else 
        {
            clear_screen();
            menuNo = MENU_MAIN;
            // __________________ Main Menu __________________ {
            while(menuNo == MENU_MAIN){
                option = showMainMenu();
                switch(option){
                    // print list content
                    case '1': {
                        printf("\n");
                        list_Iterate(&list, showListCallbackFunc);
                        break;
                    }
                    // push back
                    case '2': {
                        printf("wprowadź tekst [do 99 znaków]: ");
                        my_getline(stdin, inputText, 100);
                        list_PushBack(&list, inputText);
                        break;
                    }
                    // push front
                    case '3': {
                        printf("wprowadź tekst [do 99 znaków]: ");
                        my_getline(stdin, inputText, 100);
                        list_PushFront(&list, inputText);
                        break;
                    }
                    // insert
                    case '4': {
                        uint index = 0;
                        printf("wstaw po indeksie numer: ");
                        my_getline(stdin, inputText, 10);
                        sscanf(inputText, " %u ", &index);
                        printf("wprowadź tekst [do 99 znaków]: ");
                        my_getline(stdin, inputText, 100);
                        list_Insert(&list, index, inputText);
                        break;
                    }
                    // pop back
                    case '5': {
                        list_PopBack(&list);
                        break;
                    }
                    // pop front
                    case '6': {
                        list_PopFront(&list);
                        break;
                    }
                    // remove by index
                    case '7': {
                        uint index = 0;
                        printf("usuń element z indeksem: ");
                        my_getline(stdin, inputText, 10);
                        sscanf(inputText, " %u ", &index);
                        list_Remove(&list, index);
                        break;
                    }
                    // save list
                    case 's' : {
                        if((file = fopen("list.txt", "w") == NULL))
                            printf("Error: Nie można otworzyć pliku list.txt\n");
                        else {
                            list_WriteToFile(&list, file);
                            fclose(file); file = NULL;
                        }
                        break;
                    }
                    // back to init menu
                    case 'n' : {
                        menuNo = MENU_LIST_INIT;
                        break; 
                    }
                    // exit application
                    case 'e': menuNo = MENU_NULL; break;
                }
                if(option != '1') clear_screen();
            }
            // ______________________________________________ }
            list_Flush(&list);
        }
    }
    
    clean_exit_label:
    list_Flush(&list);
    return exitCode;
} 
// -----------------------------------------------------------


char showInitMenu(){
    char opt;
    printf("1. Stwórz nową listę\n");
    printf("2. Załaduj listę z pliku list.txt\n");
    printf("e. Wyjście z programu\n>");
    fgets(opt, 100, stdin);
    my_getline(stdin, &opt, 1);
    return opt;
}
char showMainMenu(){
    char opt;
    printf("\n1. Wyświetl listę\n");
    printf("2. Dodaj element na koniec listy\n");
    printf("3. Dodaj element na początek listy\n");
    printf("4. Wstaw element po żądanym indeksie\n");
    printf("5. Usuń element z końca listy\n");
    printf("6. Usuń element z początku listy\n");
    printf("7. Usuń element o wybranym indeksie\n");
    printf("s. Zapis do pliku list.txt\n\n");
    printf("n. Nowa lista / załaduj z pliku\n");
    printf("e. Wyjście z programu\n>");
    my_getline(stdin, &opt, 1);
    return opt;
}

void showListCallbackFunc(uint iteration, const char * nodeContent){
    printf("[%u] %s\n", iteration, nodeContent);
}

char * my_getline(FILE * infile, char * str, size_t num){
    char c = EOF;
    uint i = 0;

    while((c = fgetc(infile)) != EOF && c != '\n' && i < num - 1){
        if(i < num - 1) str[i++] = c; 
    }

    if(num > 1) str[num-1] = '\0';
    return str;
}