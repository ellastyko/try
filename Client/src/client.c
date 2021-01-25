#include "../inc/header.h"

int main(int argc, char *argv[])
{
    cl_info.id = 0; 
    strcpy(cl_info.login, "default");
    strcpy(cl_info.password, "default");
    strcpy(cl_info.key, "default");
    Socket(); // Socket initialization

    //checking_local_storage(); //TODO Проверка локального хранилища на сохранненый пароль с именем
    pthread_t reads;
    pthread_create(&reads, NULL, reading, NULL);
    int action;
    while(1) {
        printf("\nWhat do you want:\n 1 - Sign up\n 2 - Sign in\n 3 - Add chat\n 4 - Change Password\n 5 - Delete account\n");
        scanf("%d",&action);
        switch(action) {
            case 1: sign_up(); break;
            case 2: sign_in(); break; 
            case 3:  add_chat(); break;        
            default: break;
            

        }
        while(cl_info.id != 0 && strcmp(cl_info.key, "default") != 0) {
            printf("\nWhat do you want:\n 1 - Add chat\n 2 - Open chat\n 3 - Send message\n 4 - Delete message\n 5 - Change Password\n 6 - Delete account\n");
            scanf("%d",&action);
            switch(action) {
                case 1: add_chat(); break;
                case 2: open_chat(); break; // load messages
                case 3: send_message(); break;
                case 4: delete_message(); break;
                case 5: change_password(); break;
                case 6: delete_user(); break;
                default: break;
            }
        }
    }
    return 0;
}
