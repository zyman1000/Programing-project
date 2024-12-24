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

int verifyMail(char* input);
void reservation();
int getnum(char* input, int buff_size);
int verifyNum(char* num, int min_len, int max_len, long min_val, long max_val); //verify that a positive number in string format has the specified min and max lengths and min and max values
date get_date();
// reserved_room* Load();
int FindRoomCat(char request[11]); //returns the room number with the selected category, returns 0 if not found
int idRoomcheck(int id);
int CheckReservation(reserved_room* rooms, int num, int RID);
reserved_room* Load();