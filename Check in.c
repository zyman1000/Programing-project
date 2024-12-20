#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("Enter your number of nights: ");
    break;
  case 4:
    printf("Enter the Check-in Date(dd-mm-yyyy) ");
    break;
  case 5:
    printf("Enter your email: ");
    break;
  case 6:
    printf("Enter your mobile number: ");
    break;
  default:;
  }
}

int idRoomcheck(int id)
{
  char buffer[256] /*takes room id every iteration*/, strID[5];
  char *tok;
  int lineNum = 1;
  FILE *room;
  room = fopen("Room.txt", "r"); // Reads reservation file
  if (room == NULL)
  {
    printf("Error reading room file");
    exit(1);
  }
  sprintf(strID, "%d", id); // convert id from int to str
  while (fgets(buffer, sizeof(buffer), room) /*Copy lines one by one*/)
  {
    tok = strtok(buffer, " "); // initalize the pointer
    if (tok != NULL /*successful tokonization*/ && strncmp(tok, strID, strlen(strID)) == 0)
    { /*see if the id matched*/
      fclose(room);
      return lineNum;
    }
    lineNum++;
  } // it didn't find the id
  fclose(room);
  return -1;
}
int idcheck(int id)
{
  char buffer[256]; // takes room id every iteration
  int lineNum = 1;
  char strInputID[7];
  char strRoomNum[7];
  char strID[7];
  FILE *reservation;

  reservation = fopen("Reservation.txt", "r"); // Reads reservation file
  if (reservation == NULL)
  {
    printf("Error reading reservation file");
    exit(1);
  }
  sprintf(strInputID, "%d", id); // convert id from int to str
  while (fgets(buffer, sizeof(buffer), reservation))
  {
    sscanf(buffer, "%[^,],%[^,]", strID, strRoomNum);
    if (strcmp(strInputID, strID) == 0 || strcmp(strInputID, strRoomNum) == 0)
    {
      fclose(reservation);
      return lineNum; // lineNum is returned here
    }
    lineNum++; // lineNum is incremented before checking the match
  }

  fclose(reservation);
  return -1;
}

void roomUpdate(int id)
{
  char buffer[256]; // takes each line one by one
  char *tok;
  FILE *Temp, *room;
  room = fopen("Room.txt", "r"); // Reads room file
  if (room == NULL)
  {
    printf("Error reading room file");
    exit(1);
  }
  Temp = fopen("TempRoom.txt", "w"); // create a temporary file
  if (Temp == NULL)
  {
    printf("Error creating the temporary room file");
    exit(1);
  }
  int lineNum = idRoomcheck(id);
  for (int i = 1; fgets(buffer, sizeof(buffer), room); i++)
  {

    if (i != lineNum)
    {
      fputs(buffer, Temp);
    }
    else
    {
      tok = strtok(buffer, " ");
      fprintf(Temp, "%s ", tok);  // ID is now ready
      fprintf(Temp, "Reserved "); // ignored the reserved word and instead printed reserved
      tok = strtok(NULL, " ");    // skipped by one token
      tok = strtok(NULL, " ");    // now moved to category
      fprintf(Temp, "%s", tok);   // printed category
      tok = strtok(NULL, " ");    // now moved to price
      fprintf(Temp, " %s", tok);  // printed price
    }
  }
  fclose(Temp);
  fclose(room);
  ////////Replacing TempRoom.txt with Room.txt////////

  if (remove("Room.txt") != 0)
    perror("Can't remove old Room!\n");
  if (rename("TempRoom.txt", "Room.txt") == 0)
    ;
  else
    perror("failed to change the name for tempRoom\n");
}
void reservationUpdate(int id)
{
  char tempBuffer[256];
  char buffer[256]; // takes each line one by one
  char *tok;        // Your friendly neighborhood tokenizer
  FILE *Temp, *reservation;
  reservation = fopen("Reservation.txt", "r"); // Reads reservation file
  if (reservation == NULL)
  {
    printf("Error reading reservation file");
    exit(1);
  }
  Temp = fopen("Temp.txt", "w"); // create a temporary file
  if (Temp == NULL)
  {
    printf("Error creating the temporary reservation file");
    exit(1);
  }
  int lineNum = idcheck(id);
  while (lineNum == -1 || id < 999 || id > 999999)
  { // Loops Until a valid ID room is entered
    printf("Please enter a valid reservation id or room number: ");
    scanf("%d", &id);
    lineNum = idcheck(id);
  }

  for (int i = 1; fgets(buffer, sizeof(buffer), reservation); i++)
  {
    if (i != lineNum)
    {
      fputs(buffer, Temp);
    }
    else
    {
      strcpy(tempBuffer, buffer);
      tok = strtok(tempBuffer, ","); // initialization
      fputs(tok, Temp);              // prints Res ID
      tok = strtok(NULL, ",");       // skips to ROOM NUM
      fputs(",", Temp);              //****** ","
      fputs(tok, Temp);              // prints ID
      fputs(",", Temp);              //****** , room id ','
      // tok now holding the status
      fputs("confirmed", Temp);
      fputs(",", Temp);
      for (int i = 0; i < 2; i++)
      {
        tok = strtok(NULL, ",");
      }
      for (int i = 0; i < 6; i++)
      {
        fputs(tok, Temp);
        if (i < 5)
          fputs(",", Temp);
        tok = strtok(NULL, ",");
      }
    }
  }
  fclose(reservation);
  fclose(Temp);

  ////////Replacing Temp.txt with Reservation.txt////////

  if (remove("Reservation.txt") != 0)
    perror("Can't remove old reservation!\n");
  if (rename("Temp.txt", "Reservation.txt") == 0)
    ;
  else
    perror("failed to change the name for temp\n");

  printf("Your reservation has been confirmed successfully!");
}
void checkIn(int id)
{
  int isInvalid = 0;
  char *tok;
  char buffer[256], input[256], tempBuffer[256];
  FILE *reservation, *room;
  reservation = fopen("Reservation.txt", "r"); // Reads reservation file
  if (reservation == NULL)
  {
    printf("Error reading reservation file");
    exit(1);
  }
  room = fopen("Room.txt", "r"); // Reads room file
  if (room == NULL)
  {
    printf("Error reading room file");
    exit(1);
  }
  int lineNum = idcheck(id);
  while (lineNum == -1 || id < 999 || id > 999999)
  { // Loops Until a valid ID room is entered
    printf("Please enter a valid reservation id or room number: ");
    scanf("%d", &id);
    lineNum = idcheck(id);
  }
  for (int i = 1; fgets(buffer, sizeof(buffer), reservation); i++)
  {
    if (i != lineNum)
      ;
    else
    {
      strcpy(tempBuffer, buffer);    // tempBuffer holds the whole line
      tok = strtok(tempBuffer, ","); // tok holds Res_ID
      tok = strtok(NULL, ",");       // tok holds, room num
      tok = strtok(NULL, ",");       // tok holds status
      for (int i = 1; i < 7 && !isInvalid; i++)
      {
        tok = strtok(NULL, ","); // holds name
        fieldLabel(i);           // choose what to print
        gets(input);             // takes the data
        if (i == 6)
          tok[strlen(tok) - 1] = '\0'; ////////verrrrrryyyy immmppppooorrrtttaaanntt!
        if (strcmp(tok, input) == 0)
          ; // compare it with original data
        else
        {
          isInvalid++;
        }
      }
      if (!isInvalid)
      {

        fclose(reservation);
        fclose(room);
        reservationUpdate(id);
        roomUpdate(id);
      }
      else
        printf("You have entered a wrong data, Try again!");
    }
  }
}

int main()
{
  int id;
  printf("Please enter a reservation id or room number to edit the reservation\n");
  scanf("%d", &id);
  getchar(); // get rid of the enter you just entered after the id
  checkIn(id);
  // roomUpdate(id);
  return 0;
}