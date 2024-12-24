//#include "extentions.h"

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#define RESERVATION_PATH "tests/Reservation.txt" //defining paths for modularity
#define ROOM_PATH "tests/Rooms.txt"

typedef struct{
    char day[4]; //day number consisting of 2 character + null + extra byte
    char month[4];
    char year[6]; //4 digits + null + 1 extra
}date;
typedef struct{ //structure to make everything easier
    int reservation_ID;
    int room_num;
    char status[12]; 
    char name[50];
    char ID[16]; //number of ID digits is 14 + 1 null terminator + 1 extra byte useful for verification process
    int nights_num;
    date check_in;
    char email[102]; //standrard length for the maximum email length 
    char phone_num[16]; //+201124848945 14 characters + null + extra byte
    char cat[12]; //category, category with longest length is 10 characters + null character + 1 byte so that the part that replaces the \n with \0 doesnt break
    int size;
}reserved_room;

reserved_room* Load()
{
    reserved_room *Rooms = malloc(0);
    char buffer[256];
    int j = 0, i = 1;
    FILE *reservation = fopen("tests/Reservation.txt", "r");
    if (reservation == NULL)
    {
        printf("Error reading reservation file, make sure the file exists and that there is enough memory!\n");
        exit(1);
    }
    while (!feof(reservation))
    {
        if(fgets(buffer, sizeof(buffer), reservation) == NULL){
            break;
        }
        Rooms = realloc(Rooms, sizeof(reserved_room) * (i));
        if(Rooms == NULL){
            printf("unable to allocate memory, please free up some memory!\n");
            exit(1);
        }
        Rooms[j].reservation_ID = atoi(strtok(buffer, ","));
        Rooms[j].room_num = atoi(strtok(NULL, ","));
        strcpy(Rooms[j].status, strtok(NULL, ","));
        strcpy(Rooms[j].name, strtok(NULL, ","));
        strcpy(Rooms[j].ID, strtok(NULL, ","));
        Rooms[j].nights_num = atoi(strtok(NULL, ","));
        strcpy(Rooms[j].check_in.day, strtok(NULL, ",-"));
        strcpy(Rooms[j].check_in.month, strtok(NULL, ",-"));
        strcpy(Rooms[j].check_in.year, strtok(NULL, ",-"));
        strcpy(Rooms[j].email, strtok(NULL, ","));
        strcpy(Rooms[j].phone_num, strtok(NULL, ","));
        i += 1;
        j++;
    } 
    Rooms[0].size = i-1;
    fclose(reservation);
    return Rooms;
}

int main(){
    reserved_room* test = Load();
    printf("function didnt break?\n");
    free(test);
}