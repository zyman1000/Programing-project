void cancelReservation(Reservation *Rooms)
{
  int id, lineNum = -1, flag = 0;
  FILE *Temp;
  lineNumberReservation(&id, &lineNum, Rooms); // validate the ID and get the resrevation lineNum
  Temp = fopen("Temp.txt", "w");               // create a temporary file
  if (Temp == NULL)
  {
    printf("Error creating the temporary reservation file");
    exit(1);
  }
  for (int i = 0; i < Rooms[0].size; i++) // prints the whole temp file
  {
    if (i != lineNum || strcmp(Rooms[lineNum].status, "unconfirmed") != 0)
    {
      fprintf(Temp, "%d,%d,%s,%s,%s,%d,%s-%s-%s,%s,%s",
              Rooms[i].reservation_ID, Rooms[i].room_num, Rooms[i].status,
              Rooms[i].name, Rooms[i].ID, Rooms[i].nights_num,
              Rooms[i].check_in.day, Rooms[i].check_in.month, Rooms[i].check_in.year,
              Rooms[i].email, Rooms[i].phone_num);
    }
    else
      flag++;
  }
  if (!flag)
    printf("A confirmed reservation cannot be cancelled!");
  fclose(Temp);
}
