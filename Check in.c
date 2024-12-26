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
void lineNumReservation(char *idStr, int *lineNum, Reservation *Rooms)
{
  printf("Please enter a reservation id or room number to check in\n");
  fgets(idStr, 7, stdin);
  if (strlen(idStr) >= 7)
    while (getchar() != '\n')
    {
    }
  int id = atoi(idStr);
  for (int i = 0; i < Rooms[0].size; i++) /// checks where is the line to cancel
  {
    if (Rooms[i].reservation_ID == id || Rooms[i].room_num == id)
    {
      *lineNum = i;
      break;
    }
  }
  while (*lineNum == -1) // id validation stuff
  {
    printf("Please enter a valid reservation ID or room number: ");
    fgets(idStr, 7, stdin);
    if (strlen(idStr) >= 7)
      while (getchar() != '\n')
      {
      }
    id = atoi(idStr);
    for (int i = 0; i < Rooms[0].size; i++)
    {
      if (Rooms[i].reservation_ID == id || Rooms[i].room_num == id)
      {
        *lineNum = i;
        break;
      }
    }
  }
}
void fieldLabel(int n)
{
  switch (n)
  {
  case 1:
    printf("Enter your name: ");
    break;
  case 2:
    printf("Enter your national ID: ");
    break;
  case 3:
    printf("Enter your email: ");
    break;
  case 4:
    printf("Enter your mobile number: ");
    break;
  default:;
  }
}

void checkIn(Reservation *Rooms)
{
  int lineNum = -1, flag = 0;
  char buffer[256], id[7];
  lineNumReservation(id, &lineNum, Rooms); // Validate the ID and get the reservation lineNum
  FILE *Temp = fopen("Temp.txt", "w");
  if (Temp == NULL)
  {
    printf("Error creating the temporary reservation file\n");
    exit(1);
  }

  FILE *roomFile = fopen("room.txt", "r");
  FILE *roomTemp = fopen("TempRoom.txt", "w");
  if (roomFile == NULL || roomTemp == NULL)
  {
    printf("Error accessing room files\n");
    exit(1);
  }

  // Process Reservations
  for (int i = 0; i <= Rooms[0].size - 1; i++)
  {
    if (i != lineNum)
    {
      fprintf(Temp, "%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n",
              Rooms[i].reservation_ID, Rooms[i].room_num, Rooms[i].status,
              Rooms[i].name, Rooms[i].ID, Rooms[i].nights_num,
              Rooms[i].check_in.day, Rooms[i].check_in.month, Rooms[i].check_in.year,
              Rooms[i].email, Rooms[i].phone_num);
      continue;
    }
    else
    {
      if (strcmp(Rooms[lineNum].status, "confirmed") == 0)
      {
        printf("The room is already confirmed!");
        flag++;
        break;
      }
      // Request customer data for verification
      fieldLabel(1);
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer) - 1] = 0; // Remove newline character
      if (strcmp(buffer, Rooms[i].name) != 0)
      {
        printf("You have entered wrong data!\n");
        flag++;
        break;
      }
      fieldLabel(2);
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer) - 1] = 0;
      if (strcmp(buffer, Rooms[i].ID) != 0)
      {
        printf("You have entered wrong data!\n");
        flag++;
        break;
      }
      fieldLabel(3);
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer) - 1] = 0;
      if (strcmp(buffer, Rooms[i].email) != 0)
      {
        printf("You have entered wrong data!\n");
        flag++;
        break;
      }
      fieldLabel(4);
      fgets(buffer, sizeof(buffer), stdin);
      buffer[strlen(buffer) - 1] = 0;
      if (strcmp(buffer, Rooms[i].phone_num) != 0)
      {
        printf("You have entered wrong data!\n");
        flag++;
        break;
      }
    }

    if (!flag)
    {
      // Update Reservation Status
      char confirmed[10];
      strcpy(confirmed, "confirmed");
      fprintf(Temp, "%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n",
              Rooms[i].reservation_ID, Rooms[i].room_num, confirmed,
              Rooms[i].name, Rooms[i].ID, Rooms[i].nights_num,
              Rooms[i].check_in.day, Rooms[i].check_in.month, Rooms[i].check_in.year,
              Rooms[i].email, Rooms[i].phone_num);

      // Update Room Status in TempRoom.txt
      int roomNum = Rooms[i].room_num;
      char roomBuffer[256];
      while (fgets(roomBuffer, sizeof(roomBuffer), roomFile))
      {
        int currentRoomNum;
        char status[10], view[20];
        int price;
        sscanf(roomBuffer, "%d %s %s %d", &currentRoomNum, status, view, &price);
        if (currentRoomNum == roomNum)
        {
          fprintf(roomTemp, "%d Reserved %s %d\n", currentRoomNum, view, price);
        }
        else
        {
          fprintf(roomTemp, "%s", roomBuffer);
        }
      }
      rewind(roomFile); // Reset the pointer to the beginning of the file
    }
  }

  // Copy any remaining reservations if an error occurred
  if (flag)
  {
    for (int i = lineNum; i <= Rooms[0].size - 1; i++)
    {
      fprintf(Temp, "%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s\n",
              Rooms[i].reservation_ID, Rooms[i].room_num, Rooms[i].status,
              Rooms[i].name, Rooms[i].ID, Rooms[i].nights_num,
              Rooms[i].check_in.day, Rooms[i].check_in.month, Rooms[i].check_in.year,
              Rooms[i].email, Rooms[i].phone_num);
    }
  }
  fclose(Temp);
  fclose(roomFile);
  fclose(roomTemp);
  if (flag)
    remove("TempRoom.txt");
}

Reservation *Load()
{
  char buffer[256];
  Reservation *Rooms = malloc(0);
  int j = 0, i = 1;
  FILE *reservation = fopen("Reservation.txt", "r");
  FILE *rooms = fopen("Room.txt", "r");
  if (reservation == NULL || rooms == NULL)
  {
    printf("Error reading reservation file");
    exit(1);
  }
  while (fgets(buffer, 256, reservation))
  {
    Rooms = realloc(Rooms, sizeof(Reservation) * i);
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
    strcpy(Rooms[j].phone_num, strtok(NULL, ",\n"));
    i++;
    j++;
  }
  Rooms[0].size = i - 1;
  j = 0;
  while (fgets(buffer, 256, rooms))
  {
    if (atoi(strtok(buffer, " ")) == Rooms[j].room_num)
    {
      strtok(NULL, " ");
      strcpy(Rooms[j].cat, strtok(NULL, " "));
      Rooms[j].price = atoi(strtok(NULL, " "));
      j++;
    }
  }

  fclose(rooms);
  fclose(reservation);
  return Rooms;
}

int main()
{
  Reservation *Rooms = Load();
  checkIn(Rooms);
  free(Rooms);
  return 0;
}
