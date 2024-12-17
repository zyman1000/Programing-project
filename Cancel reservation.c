#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
  char buffer[256] /*takes room id every iteration*/;
  char *tok;
  int lineNum = 1;
  FILE *reservation;
  reservation = fopen("Reservation.txt", "r"); // Reads reservation file
  if (reservation == NULL)
  {
    printf("Error reading reservation file");
    exit(1);
  }
  if (id < 10000)
  {
    char strID[5];
    sprintf(strID, "%d", id); // convert id from int to str
    while (fgets(buffer, sizeof(buffer), reservation) /*Copy lines one by one*/)
    {
      tok = strtok(buffer, ","); // initialize the pointer
      tok = strtok(NULL, ",");   // skips the reservaion id
      if (tok != NULL && strncmp(tok, strID, strlen(strID)) == 0)
      { /*see if the id matched*/
        fclose(reservation);
        return lineNum;
      }
      lineNum++;
    } // it didn't find the id
    fclose(reservation);
    return -1;
  }
  else
  {
    char strID[7];
    sprintf(strID, "%d", id); // convert id from int to str
    while (fgets(buffer, sizeof(buffer), reservation) /*Copy lines one by one*/)
    {
      tok = strtok(buffer, ","); // from the start to the end of reservation id
      if (tok != NULL && strncmp(tok, strID, strlen(strID)) == 0)
      { /*see if the id matched*/
        fclose(reservation);
        return lineNum;
      }
      lineNum++;
    } // it didn't find the id
    fclose(reservation);
    return -1;
  }
}

void editRoom(int id)
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
      fprintf(Temp, "%s ", tok);   // ID is now ready
      fprintf(Temp, "Available "); // ignored the reserved word and instead printed available
      tok = strtok(NULL, " ");     // skipped by one token
      tok = strtok(NULL, " ");     // now moved to the rest of the sentence
      fprintf(Temp, "%s\n", tok);  // printed the rest of line
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

void cancelReservation(int id)
{
  char buffer[256]; // takes each line one by one
  char *tok;        // Your friendly neighborhood tokenizer
  int flag = 0;     // Will be used to note if the reservation is confirmed or not
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
      char tempBuffer[256];
      strcpy(tempBuffer, buffer);    // so we don't screw up buffer if reservation wasconfirmed
      tok = strtok(tempBuffer, ","); // initialization
      for (int i = 1; i < 3; i++)    // skips to "confirmed"
      {
        tok = strtok(NULL, ","); // skipped by one token
      }
      if (tok != 0 && strcmp(tok, "confirmed") == 0)
      {
        flag++; // indicate that the reservation was not cancelled
        printf("A confirmed reservation cannot be cancelled.\n");
        fputs(buffer, Temp);
      }
    }
  }
  if (flag == 0)
  { // if the reservation was cancelled it will update Room.txt
    editRoom(id);
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
  if (flag == 0)
  { // will only print if the reserevation was cancelled sucessfully
    printf("Cancelled successuflly!");
  }
}

int main()
{
  int id;
  printf("Please enter a reservation id or room number to cancel the reservation\n");
  scanf("%d", &id);
  cancelReservation(id);
  return 0;
}
