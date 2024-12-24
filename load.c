#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct
{
  char day[4]; // day number consisting of 2 character + null + extra byte
  char month[4];
  char year[6]; // 4 digits + null + 1 extra
} date;
typedef struct
{ // structure to make everything easier
  int reservation_ID;
  int room_num;
  char status[12];
  char name[50];
  char ID[16]; // number of ID digits is 14 + 1 null terminator + 1 extra byte useful for verification process
  int nights_num;
  date check_in;
  char email[102];    // standrard length for the maximum email length
  char phone_num[16]; //+201124848945 14 characters + null + extra byte
  char cat[11];       // category
  int price;
  int size;
} Reservation;
Reservation *Load()
{
  Reservation *Rooms = malloc(0);
  char buffer[256];
  char empty[256];
  for(int i = 0; i < 256; i++){
    empty[i] = ' ';
  }
  int j = 0, i = 1;
  FILE *reservation = fopen("tests/Reservation.txt", "r");
  // FILE *rooms = fopen("tests/Room.txt", "r");
  if (reservation == NULL)
  {
    printf("Error reading reservation file");
    exit(1);
  }
  // if (rooms == NULL)
  // {
    // printf("Error reading reservation file");
    // exit(1);
  // }

  while (!feof(reservation))
  {
    if(fgets(buffer, 256, reservation) == NULL){
      break;
    }
    printf("%s", buffer);
    Rooms = realloc(Rooms, sizeof(Reservation) * (i));
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
    strcpy(buffer, empty);
  }
  Rooms[0].size = i - 1;
  j = 0;
  // while (fgets(buffer, 256, rooms))
  // {
  //   if (atoi(strtok(buffer, " ")) == Rooms[j].room_num)
  //   {
  //     strtok(NULL, " ");
  //     strcpy(Rooms[j].cat, strtok(NULL, " "));
  //     Rooms[j].price = atoi(strtok(NULL, " "));
  //     j++;
  //   }
  // }
  fclose(reservation);
  // fclose(rooms);
  return Rooms;
}

int main() {
  Reservation *please = Load();
  for(int i = 0; i < 2; i++){
    printf("%d  ", please[i].reservation_ID);
    printf("%d  ", please[i].room_num);
    printf("%s  ", please[i].status);
    printf("%s  ", please[i].name);
    printf("%s  ", please[i].ID);
    printf("%d  ", please[i].nights_num);
    printf("%s  ", please[i].check_in.day);
    printf("%s  ", please[i].check_in.month);
    printf("%s  ", please[i].check_in.year);
    printf("%s  ", please[i].email);
    printf("%s  ", please[i].phone_num);
    printf("%s  ", please[i].cat);
    printf("%d  ", please[i].price);
    printf("%d  ", please[i].size);
    printf("\n");
  }
}