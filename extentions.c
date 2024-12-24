#include "extentions.h"

int getnum(char* input, int buff_size){
    fgets(input, buff_size, stdin);
    int i; //we initialize the i here because we will need it after the loop
    for(i = 0; (input[i] != '\n'); i++){
        if(!isdigit(input[i])){
            return 0;
        }
    }
    input[i] = '\0'; //we replace the \n with \0 because fgets takes the \n from the stream which we dont want
    return 1;
}

int verifyNum(char* num, int min_len, int max_len, long min_val, long max_val){
    long num2 = 0;
    int condition = (strlen(num) < min_len || strlen(num) > max_len);
    if(min_val != -1){
        num2 = atol(num); //like atoi but to long instead of int
        condition = condition || (num2 > max_val) || (num2 < min_val);
    }
    return !condition;
}
date get_date(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    //printf("now: %d-%02d-%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    date result;
    sprintf(result.day, "%d", tm.tm_mday); //prints formatted to a string instead of stdout
    sprintf(result.month, "%d", tm.tm_mon);
    sprintf(result.year, "%d", tm.tm_year + 1900);
    return result;
}

int verifyMail(char* input){ //basic email verification
    char* copy = input; //we get a copy of the email string to apply verficartions on it
    if(strlen(input) > 100 || strlen(input) < 6){ //max and min size for email
        return 0;
    }
    if(strstr(input, "..") != NULL || strstr(input, "@@") != NULL || strchr(input, ' ') != NULL)
        return 0; //make sure there are no consequetive dots or @'s and also make sure there are no spaces
    while(1){ //until we confirm that there is no dot after the @ or if the dot is right after or before the @ or if the email starts with @ or . in the first place
        char *temp1 = strchr(input, '@');
        char *temp2 = strchr(copy, '.'); //we use copy here in order to not ruin the original string
        if(temp1 == NULL || temp2 == NULL || temp2 == input || temp1 == input || fabs(temp1 - temp2) == 1) //make sure the dot is not right before or after the @
            return 0;
        if(temp2 > temp1){ //make sure there is dot is after the @
            break;
        }
        else{
            copy = temp2 + 1; //we skip till right after the dot
        }
    }
    copy = strchr(input, '@') + 1; //make sure there is only one @ symbol
    if(strchr(copy, '@') == NULL)
        return 1;
    else
        return 0;
}

int FindRoomCat(char request[11]){
    char buffer[50];
    FILE* Rooms = fopen(ROOM_PATH, "r");
    if (Rooms == NULL){
        printf("Error reading rooms file, make sure the file exists and that there is enough memory!\n");
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), Rooms)){
        char *room_num = strtok(buffer, " "); //this one stores room number
        char *isAvailable = strtok(NULL, " "); //this one stored "available" or "Reserved"
        if(strcmp(isAvailable, "Available") == 0){
            char *category = strtok(NULL, " ");
            if(strcmp(category, request) == 0){
                int room_num2 = atoi(room_num);
                return room_num2;
            }
        }
        else if(strcmp(isAvailable, "Reserved") != 0){
            printf("The rooms file is corrupt. please check the rooms file.\n");
            exit(1);
        }
    }
    return 0;
}

int CheckReservation(reserved_room* rooms, int num, int RID){ //RID = Reservation ID
    for(int i = 0; i < num; i++){
        if(rooms[i].reservation_ID == RID){ //if the RID sent is found, return that it is found immediatly
            return 1;
        }
    }
    return 0; //0 if not found
}


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

int idRoomcheck(int id)
{
    char buffer[256] /*takes room id every iteration*/, strID[5];
    char *tok;
    int lineNum = 1;
    FILE *room;
    room = fopen("Room.txt", "r"); // Reads rooms file
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