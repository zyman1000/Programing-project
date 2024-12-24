//This function takes the room number, search Room.txt file, and return its line number
//if it didn't find anything it returns -1


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
