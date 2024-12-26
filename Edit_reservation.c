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
int FindRoomCat(char request[11]) //////casse sensitive
{
  char buffer[50];
  FILE *Rooms = fopen("Room.txt", "r");
  if (Rooms == NULL)
  {
    printf("Error reading rooms file, make sure the file exists and that there is enough memory!\n");
    exit(1);
  }
  while (fgets(buffer, sizeof(buffer), Rooms))
  {
    char *room_num = strtok(buffer, " ");  // this one stores room number
    char *isAvailable = strtok(NULL, " "); // this one stored "available" or "Reserved"
    if (strcmp(isAvailable, "Available") == 0)
    {
      char *category = strtok(NULL, " ");
      if (strcmp(category, request) == 0)
      {
        int room_num2 = atoi(room_num);
        return room_num2;
      }
    }
    else if (strcmp(isAvailable, "Reserved") != 0)
    {
      printf("The rooms file is corrupt. please check the rooms file.\n");
      exit(1);
    }
  }
  return 0;
}
void fieldLabel(int n)
{
  switch (n)
  {
  case 1:
    printf("Enter the new name: ");
    break;
  case 3:
    printf("Enter the new number of nights: ");
    break;
  case 4:
    printf("Enter the new Check-in Date (DD-MM-YYYY): ");
    break;
  case 5:
    printf("Enter your new email: ");
    break;
  case 6:
    printf("Enter your new mobile number: ");
    break;
  case 7:
    printf("Enter your new room category: ");
    break;
  default:;
  }
}

void editReservation(Reservation *Rooms)
{
  int id, lineNum = -1;
  int size = Rooms[0].size; // All the reservationss
  char buffer[256];         // will holds the buffer
  printf("Please enter a reservation id or room number to edit the reservation\n");
  scanf("%d", &id);
  getchar();                           // get rid of the enter you just entered after the id
  FILE *Temp = fopen("Temp.txt", "w"); // create a temporary file
  if (Temp == NULL)
  {
    printf("Error creating the temporary reservation file");
    exit(1);
  }
  for (int i = 0; i < size; i++) /// checks where is the line to edit
  {
    if (Rooms[i].reservation_ID == id || Rooms[i].room_num == id)
    {
      lineNum = i;
      break;
    }
  }
  while (lineNum == -1 || id < 999 || id > 999999) // id validation stuff
  {
    printf("Please enter a valid reservation ID or room number: ");
    scanf("%d", &id);
    getchar();
    for (int i = 0; i < size; i++)
    {
      if (Rooms[i].reservation_ID == id || Rooms[i].room_num == id)
      {
        lineNum = i;
        break;
      }
    }
  }

  fieldLabel(1);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strlen(buffer) - 1] = 0; // Remove the newline character
  if (buffer[0] != '\0')
    strcpy(Rooms[lineNum].name, buffer);

  fieldLabel(3);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strlen(buffer) - 1] = 0;
  if (buffer[0] != '\0')
    Rooms[lineNum].nights_num = atoi(buffer);

  fieldLabel(4);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strlen(buffer) - 1] = 0;
  if (buffer[0] != '\0')
  {
    sscanf(buffer, "%[^-]-%[^-]-%s", Rooms[lineNum].check_in.day,
           Rooms[lineNum].check_in.month, Rooms[lineNum].check_in.year); //%[take until encontered'-'] ignore literal in the input'-'
  }

  fieldLabel(5);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strlen(buffer) - 1] = 0;
  if (buffer[0] != '\0')
    strcpy(Rooms[lineNum].email, buffer);

  fieldLabel(6);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strlen(buffer) - 1] = 0; // targeting the 'n' in "\n"
  if (buffer[0] != '\0')
    strcpy(Rooms[lineNum].phone_num, buffer);

  fieldLabel(7);
  fgets(buffer, sizeof(buffer), stdin);
  buffer[strlen(buffer) - 1] = 0; // make just a regular word without \n
  if (buffer[0] != '\0')
  {
    int newRoomNum = FindRoomCat(buffer); // ffind a new room with the new inputted category
    if (newRoomNum == 0)                  // didn't find
    {
      printf("No available rooms found with this category.\n");
      printf("Your room category is still the same.\n");
    }
    else
    {
      strcpy(Rooms[lineNum].cat, buffer);   // overwrite the new category
      Rooms[lineNum].room_num = newRoomNum; // room num in reservation.txt has been updated now
    }
  }

  printf("Changes have been saved successfully.\n");

  for (int i = 0; i < size; i++) // prints the whole temp file
  {
    fprintf(Temp, "%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s",
            Rooms[i].reservation_ID, Rooms[i].room_num, Rooms[i].status,
            Rooms[i].name, Rooms[i].ID, Rooms[i].nights_num,
            Rooms[i].check_in.day, Rooms[i].check_in.month, Rooms[i].check_in.year,
            Rooms[i].email, Rooms[i].phone_num);
  }
  fclose(Temp);
}

Reservation *Load()
{
  Reservation *Rooms = NULL; // Start with NULL for safety
  char buffer[256];
  int count = 0; // Tracks the number of reservations loaded

  FILE *reservation = fopen("Reservation.txt", "r");
  if (reservation == NULL)
  {
    printf("Error: Unable to open Reservation.txt file.\n");
    exit(EXIT_FAILURE);
  }

  while (fgets(buffer, sizeof(buffer), reservation))
  {
    // Allocate or reallocate memory for the Rooms array
    Reservation *temp = realloc(Rooms, sizeof(Reservation) * (count + 1));
    if (temp == NULL)
    {
      printf("Error: Memory allocation failed.\n");
      free(Rooms); // Free previously allocated memory before exiting
      fclose(reservation);
      exit(EXIT_FAILURE);
    }
    Rooms = temp;

    // Parse the line and fill the reservation structure
    char *token = strtok(buffer, ",");
    if (token == NULL)
      continue; // Skip malformed lines

    Rooms[count].reservation_ID = atoi(token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    Rooms[count].room_num = atoi(token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].status, token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].name, token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].ID, token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    Rooms[count].nights_num = atoi(token);

    token = strtok(NULL, ",-");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].check_in.day, token);

    token = strtok(NULL, ",-");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].check_in.month, token);

    token = strtok(NULL, ",-");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].check_in.year, token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].email, token);

    token = strtok(NULL, ",");
    if (token == NULL)
      continue;
    strcpy(Rooms[count].phone_num, token);

    // Ensure count is incremented only for valid lines
    count++;
  }
  Rooms[0].size = count;

  fclose(reservation);

  // Check if any reservations were loaded
  if (Rooms == NULL)
  {
    printf("Warning: No reservations were loaded from the file.\n");
  }

  return Rooms;
}

int main()
{
  Reservation *Rooms = Load();
  editReservation(Rooms);
  free(Rooms);
  return 0;
}
