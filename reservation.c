#include "extentions.h"

int main(){
    reservation();
}
void reservation(){ //i am assuming that the load functions will make an array of reserved_room structures
    reserved_room input;
    int condition;
    printf("Enter customer name: ");
    fgets(input.name, sizeof(input.name), stdin); //fgets puts the \n in the string
    int length = strlen(input.name);
    input.name[length - 1] = '\0'; //remove the extra \n added by fgets to the string from the stream
    //make sure there are no numbers in the name, make sure the name has first and second, size
    printf("Enter national ID: ");
    do{
        if(!getnum(input.ID, sizeof(input.ID))){
            printf("You entered a character, only numbers are allowed!\nplease try again!\n");
            continue;
        }
        condition = verifyNum(input.ID, 14, 14, -1, -1);
        if(!condition){
            printf("incorrect ID format, please try again!\n");
        }
    }while(!condition);
    printf("Enter email: "); //make a check email function
    do{
        fgets(input.email, sizeof(input.email), stdin);
        length = strlen(input.email);
        input.email[length - 1] = '\0';
        condition = verifyMail(input.email);
        if(!condition){
        printf("Invalid email format, please try again!\n");
        }
    }while(!condition);
    printf("Enter phone number: ");
    do{
        if(!getnum(input.phone_num, sizeof(input.phone_num))){
            printf("You entered a character, only numbers are allowed!\nplease try again!\n");
            continue;
        }
        condition = verifyNum(input.phone_num, 11, 14, -1, -1);
        if(!condition){
            printf("incorrent phone number format, please try again!\n");
        }
    }while(!condition);
    date current = get_date();
    printf("Enter year of check-in date: ");
    do{
        if(!getnum(input.check_in.year, sizeof(input.check_in.year))){
            printf("You entered a character, only numbers are allowed!\nplease try again!\n");
            continue;
        }
        condition = verifyNum(input.check_in.year, 4, 4, atoi(current.year), 3000); //assuming you cant reserve a room for hundreds of years
        if(!condition){
            printf("Invalid format for year, please try again!");
        }
    }while(!condition);
    printf("Enter month of check-in date: ");
    do{
        if(!getnum(input.check_in.month, sizeof(input.check_in.month))){
            printf("You entered a character, only numbers are allowed!\nplease try again!\n");
            continue;
        }
        condition = verifyNum(input.check_in.month, 1, 2, 1, 12);
        if (!condition){
            printf("Invalid month format or value, please try again!\n");
        }
    }while(!condition);
    printf("Enter day of check-in date: ");
    do{
        if(!getnum(input.check_in.day, sizeof(input.check_in.day))){
            printf("You entered a character, only numbers are allowed!\nplease try again!\n");
            continue;
        }
        int max;
        switch(atoi(input.check_in.month)){
            case 1:
                max = 31; break;
            case 2:
                max = 28; break;
            case 3:
                max = 31; break;
            case 4:
                max = 30; break;
            case 5:
                max = 31; break;
            case 6:
                max = 30; break;
            case 7:
                max = 31; break;
            case 8:
                max = 31; break;
            case 9:
                max = 30; break;
            case 10:
                max = 31; break;
            case 11:
                max = 30; break;
            case 12:
                max = 31; break;
        }
        condition = verifyNum(input.check_in.day, 1, 2, 1, max);
        if(!condition){
            printf("Invalid day format or value, please try again!\n");
        }
    }while(!condition);
    printf("Enter number of nights: ");
    do{
        char buffer[4];
        if(!getnum(buffer, sizeof(buffer))){
            printf("only numbers are allowed, please try again!\n");
            continue;
        }
        input.nights_num = atoi(buffer);
        condition = !(input.nights_num < 1) || (input.nights_num > 365); //lets say you are not allowed to stay for over a year
        if(!condition){
            printf("Invalid format for number of nights, please try again!\n");
        }
    }while(!condition);
    printf("Enter room category: ");
    do{
        fgets(input.cat, sizeof(input.cat), stdin);
        //remove the \n
        length = strlen(input.cat);
        input.cat[length - 1] = '\0';
        char copy[sizeof(input.cat)]; //get a copy of the category because the next few lines of code modiy it
        strcpy(copy, input.cat);
        //make the input case INsensitive
        for(int i = 0; copy[i] != '\0'; i++){
            copy[i] = tolower(copy[i]);
        }
        condition = (strcmp(copy, "seaview")) || (copy, "lakeview") || strcmp(copy, "gardenview");
        if(!condition){
            printf("the room category can only be SeaView, LakeView or GardenView, please try again!\n");
        }
        input.room_num = FindRoomCat(input.cat);
        if(input.room_num == 0){
            printf("sorry! no rooms with the specified category are available now.");
            condition = 0;
        }
    }while(!condition);
    //generate random ID
    int max_id = 999999; //defining the limits of the id
    int min_id = 100000;
    //rand_r generates a random number given a seed. in order to get random values we need to use a different seed each time, so we set the seed to the output of time(0) so that we get a unique seed everytime
    int generated; //to store generated ID
    reserved_room *rooms = Load(rooms); //get the rooms as an array from the load function
    do{
        unsigned int seed = time(0);
        generated = rand_r(&seed) % (max_id - min_id + 1) + min_id;
        if(CheckReservation(rooms, rooms[0].size, generated) == 0)
            break;
    }while(1);
    printf("The ID is: %d\n", generated);
    input.reservation_ID = generated;
    reserved_room *rooms_copy = rooms; //save a backup for the pointer to the array so that we dont lose the array incase realloc returns NULL
    rooms = realloc(rooms, sizeof(reserved_room) * (++rooms[0].size)); //allocate one more space for the extra room
    while(rooms == NULL){
        printf("not enough memory, please close extra programs!\n");
        rooms = realloc(rooms_copy, sizeof(reserved_room) * rooms[0].size);
    }
    strcpy(input.status, "unconfirmed");
    rooms[rooms[0].size - 1] = input;
    for(int i = 0; i < rooms[0].size; i++){
    printf("%d  ", rooms[i].reservation_ID);
    printf("%d  ", rooms[i].room_num);
    printf("%s  ", rooms[i].status);
    printf("%s  ", rooms[i].name);
    printf("%s  ", rooms[i].ID);
    printf("%d  ", rooms[i].nights_num);
    printf("%s  ", rooms[i].check_in.day);
    printf("%s  ", rooms[i].check_in.month);
    printf("%s  ", rooms[i].check_in.year);
    printf("%s  ", rooms[i].email);
    printf("%s  ", rooms[i].phone_num);
    printf("%s  ", rooms[i].cat);
    printf("\n");
  } //TODO check if the reservation is set to the future
    printf("There are %d rooms in total\n", rooms[0].size);
    free(rooms);
}