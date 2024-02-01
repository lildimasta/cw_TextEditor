#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SIZE 50
#define MORE 50

char* read_sentence(){
    char sym;
    int index = 0, size1 = SIZE;
    char* sentence = malloc(size1 * sizeof(char));
    while (1){
        sym = (char)getchar();
        if(sym == '\n') {
            free(sentence);
            return NULL;
        }
        sentence[index++] = sym;
        if( index == size1){
            size1 += MORE;
            sentence = realloc(sentence, size1);
        }
        if(sym =='.'){
            break;
        }
    }
    sentence[index] = '\0';
    return sentence;
}

char** read_text(int* size){
    char* sentence;
    int text_size = SIZE, index = 0;
    char** text = malloc(text_size * sizeof(char*));
    while(1){
        sentence = read_sentence();
        if(sentence != NULL){
            text[index++] = sentence;
        }
        if(sentence == NULL){
            break;
        }
        if (index == text_size){
            text_size += MORE;
            text = realloc(text, text_size);
        }
    }
    *size = index;
    return text;
}

int delete_povt_predl(char* sent1, char* sent2){
    if(strlen(sent1) == strlen(sent2)){
        for (int i = 0; i < strlen(sent1); i++) {
            if (tolower(sent1[i]) != tolower(sent2[i])) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

void del_povt(char** text, int* size) {
    for(int i = 0; i< *size; i++) {
        for (int j = i + 1; j < *size; j++) {
            if(delete_povt_predl((char *) text[i], (char *) text[j]) == 0){
                free(text[j]);
                memmove(text + j, text + j + 1, (*size - j) * sizeof(char *));
                *size -= 1;
                j--;
            }
        }
    }
}

void del_digits(char** text, int size) {
    for(int i = 0; i< size; i++) {
        for(int j = 0; j<strlen(text[i]); j++){
            if(isdigit(text[i][j])){
                memmove(&text[i][j], &text[i][j + 1], strlen(text[i]) - (j + 1) + 1);
            }
        }
    }
}

int is_palindrome(char *sen){
    int i = 0;
    int j = strlen(sen)-2;
    while(i < j){
        while(sen[i] == ' ' || sen[i] == ','){
            i++;
        }
        while(sen[j] == ' ' || sen[j] == ','){
            j--;
        }
        if(sen[i++] != sen[j--]){
            return 0;
        }
    }
    return 1;
}

void palindrome(char** text, int size){
    for(int i = 0; i< size; i++){
        if(is_palindrome( text[i]) == 1){
            printf("%s - Palindrome\n", text[i]);
        }
        else printf("%s - Nothing interesting\n", text[i]);
    }
}

int delete_simv(char *sen1 ){
    if(tolower(sen1[0]) != tolower(sen1[strlen(sen1)-2])){
        return 1;
    }
    return 0;
}

void del_simv(char** text, int* size){
    for(int i = 0; i< *size; i++) {
        if (delete_simv((char *) text[i]) == 0) {
            memmove(text + i, text + i + 1, (*size - i) * sizeof(char *));
            *size -= 1;
            i--;
        }
    }
}

int cmp(const void *a, const void *b){
    char* sep = " ,";
    char* str1 =(char*)malloc(strlen(*(char**)a)+1);
    char* str2 =(char*)malloc(strlen(*(char**)b)+1);
    strcpy(str1,*(char**)a);
    strcpy(str2,*(char**)b);
    int len1 = 3, len2 =3;
    char* word1 = strtok(str1, sep);
    for(int i = 0; i < 2; i++){
        if(word1 != NULL){
            word1 = strtok(NULL, sep);
        }
    }
    char* word2 = strtok(str2, sep);
    for(int i = 0; i < 2; i++){
        if(word2 != NULL){
            word2 = strtok(NULL, sep);
        }
    }
    if(word1 != NULL){
        len1 = strlen(word1);
    }
    if(word2 != NULL){
        len2 = strlen(word2);
    }
    free(str1);
    free(str2);
    return len1-len2;
}

void sort_predl(char** text,int size){
    qsort(text, size, sizeof(char*), cmp);
}

void print_txt(char** text, int* size){
    for(int i = 0; i< *size; i++){
        printf("%s", text[i]);
    }
}

int main(){
    int size;
    char** text;
    printf("Введите текст.\n");
    text = read_text(&size);
    del_povt(text, &size);
    printf("Повторяющиеся предложения удалены\n");
    print_txt(text, &size);
    printf("\n Выберите операцию с текстом:\n"
           "1)В каждом предложении удалить встречающиеся в нем цифры.\n"
           "2)Для каждого предложения вывести “Palindrome”, если оно является палиндромом, иначе “Nothing interesting”.\n"
           "3)Удалить все предложения у которых совпадают первый и последний символ без учета регистра.\n"
           "4)Отсортировать предложения по увеличению длины третьего слова, если третьего слова нет, то для такого предложения длина третьего слова равняется 3.\n"
           "5)Выход из программы\n");
    char a;
    while( a !='5'){
        a = getchar();
        getchar();
        switch(a){
            case '1':
                del_digits(text, size);
                print_txt(text, &size);
                break;
            case '2':
                palindrome(text,size);
                break;
            case '3':
                del_simv(text,&size);
                print_txt(text, &size);
                break;
            case '4':
                sort_predl(text,size);
                print_txt(text,&size);
                break;
            case '5':
                printf("\nПрограмма завершена\n");
                return 0;
            default:
                printf("Выбрана несуществующая операция\n");
                break;
        }
        printf("\nВыбрите операцию с текстом:");
    }
    for(int i = 0; i<size; i++ ) {
        free(text[i]);
    }
    free(text);
    return 0;
}