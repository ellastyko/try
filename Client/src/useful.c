#include "../inc/header.h"


bool validation(char *login, char *password) { 

    char banned_symbols[35] = {'!', '@', '#', '$', '^', '&', '*', '(', ')', '-', '+',
                             '=', '[', '{', '}', ']', '`', '`', '|', '/','1', '2', '3',
                             '4', '5', '6', '7', '8', '9', '0'};
    for (int i = 0; i < strlen(login); i++) {
        for (int j = 0; j < strlen(banned_symbols); j++) { 
            if (login[i] == banned_symbols[j]) {
                write(2, "Banned symbol used in login\n", 9);
                return false;
            } 
        }
    }
    // for (int i = 0; i < strlen(password); i++) {
    //     for (int j = 0; j < strlen(banned_symbols); j++) { 
    //         if (password[i] == banned_symbols[j]) {
    //             write(2, "Banned symbol used in password\n", 9);
    //             return false;
    //         } 
    //     }
    // }
    // if (strlen(login) < 6) {
    //     return false;
    // }
    // if (strlen(password) < 6) {
    //     return false;
    // }
    // return true;
}  

void prepare() {

    // Вносим стандартные значения в главную структуру
    cl_info.id = -1; 
    strcpy(cl_info.login, "-1");
    strcpy(cl_info.password, "-1");
    strcpy(cl_info.key, "-1");
    // Вносим стандартные значения в структуру с чатами
    for (int i = 0; i < 100; i++) {

        chat[i].chat_id = -1;
        chat[i].friend_id = -1;
        strcpy(chat[i].login, "-1");
    }
}

char *stringify(struct info *req)
{
    char *string = NULL;

    cJSON *json_msg = cJSON_CreateObject();
    cJSON_AddStringToObject(json_msg, "action", req->action);

    cJSON_AddNumberToObject(json_msg, "id", req->id);
    cJSON_AddStringToObject(json_msg, "login", req->login);
    cJSON_AddStringToObject(json_msg, "password", req->password);
    cJSON_AddStringToObject(json_msg, "key", req->key);

    cJSON_AddNumberToObject(json_msg, "chat_id", req->chat_id);
    cJSON_AddNumberToObject(json_msg, "friend_id", req->friend_id);
    cJSON_AddStringToObject(json_msg, "message", req->message);
    cJSON_AddNumberToObject(json_msg, "message_id", req->message_id);
    cJSON_AddNumberToObject(json_msg, "time", req->time);  
    
    string = cJSON_Print(json_msg);

    cJSON_Delete(json_msg);
    return string;
}


// TODO RESPONSE FROM SERVER
struct info *parse(const char *const msg)
{
    struct info *res = malloc(sizeof(struct info));
    const cJSON *action = NULL;
    const cJSON *status = NULL;

    const cJSON *id = NULL;
    const cJSON *login = NULL;
    const cJSON *password = NULL;
    const cJSON *key = NULL;

    const cJSON *chat_id = NULL;
    const cJSON *friend_id = NULL;
    const cJSON *message = NULL;
    const cJSON *message_id = NULL;
    const cJSON *time = NULL;
    
    cJSON *msg_json = cJSON_Parse(msg);

    action = cJSON_GetObjectItemCaseSensitive(msg_json, "action");
    if (action == NULL || action->valuestring == NULL)
        return NULL;
    strcpy(res->action, action->valuestring);

    status = cJSON_GetObjectItemCaseSensitive(msg_json, "status");
    if (status == NULL || !cJSON_IsNumber(status))
        return NULL;
    res->status = status->valueint;

    if (res->status != 1) { // Message in case of error
        message = cJSON_GetObjectItemCaseSensitive(msg_json, "message");
        if (message == NULL || message->valuestring == NULL)
            return NULL;
        strcpy(res->message, message->valuestring);
    }
    else { // If everything ok
        if (strcmp(res->action, "sign_up") == 0) {          
           // Nothing
        }
        else if (strcmp(res->action, "sign_in") == 0) {
            
            id = cJSON_GetObjectItemCaseSensitive(msg_json, "id");
            if (id == NULL || !cJSON_IsNumber(id))
                return NULL;
            res->id = id->valueint;
            login = cJSON_GetObjectItemCaseSensitive(msg_json, "login");
            if (login == NULL || login->valuestring == NULL)
                return NULL;
            strcpy(res->login, login->valuestring);
            password = cJSON_GetObjectItemCaseSensitive(msg_json, "password");
            if (password == NULL || password->valuestring == NULL)
                return NULL;
            strcpy(res->password, password->valuestring);
            key = cJSON_GetObjectItemCaseSensitive(msg_json, "key");
            if (key == NULL || key->valuestring == NULL)
                return NULL;            
            strcpy(res->key, key->valuestring);
        }
        else if (strcmp(res->action, "send_message") == 0) {
            // Nothing
        }
        else if (strcmp(res->action, "get_login_by_id") == 0) {

            login = cJSON_GetObjectItemCaseSensitive(msg_json, "login");
            if (login == NULL || login->valuestring == NULL)
                return NULL;
            strcpy(res->login, login->valuestring);
        }
        else if (strcmp(res->action, "get_chats_info") == 0) {
            
            chat_id = cJSON_GetObjectItemCaseSensitive(msg_json, "chat_id");
            if (chat_id == NULL || !cJSON_IsNumber(chat_id))
                return NULL;             
            res->chat_id = chat_id->valueint;
            login = cJSON_GetObjectItemCaseSensitive(msg_json, "login");
            if (login == NULL || login->valuestring == NULL)
                return NULL;
            strcpy(res->login, login->valuestring);
            friend_id = cJSON_GetObjectItemCaseSensitive(msg_json, "friend_id");
            if (friend_id == NULL || !cJSON_IsNumber(friend_id))
                return NULL;    
            res->friend_id = friend_id->valueint;
        }
        else if (strcmp(res->action, "add_chat") == 0) {

            chat_id = cJSON_GetObjectItemCaseSensitive(msg_json, "chat_id");
            if (chat_id == NULL || !cJSON_IsNumber(chat_id))
                return NULL;    
            res->chat_id = chat_id->valueint;
            login = cJSON_GetObjectItemCaseSensitive(msg_json, "login");
            if (login == NULL || login->valuestring == NULL)
                return NULL;
            strcpy(res->login, login->valuestring);
            friend_id = cJSON_GetObjectItemCaseSensitive(msg_json, "friend_id");
            if (friend_id == NULL || !cJSON_IsNumber(friend_id))
                return NULL;    
            res->friend_id = friend_id->valueint;
        }
        else if (strcmp(res->action, "delete_user") == 0) {
            // Nothing
        }
        else if (strcmp(res->action, "delete_message") == 0) {
            // Nothing
        }
        else if (strcmp(res->action, "get_message") == 0) {
            
            id = cJSON_GetObjectItemCaseSensitive(msg_json, "id");
            if (id == NULL || !cJSON_IsNumber(id))
                return NULL;
            res->id = id->valueint;
            message = cJSON_GetObjectItemCaseSensitive(msg_json, "message");
            if (message == NULL || message->valuestring == NULL)
                return NULL;
            strcpy(res->message, message->valuestring); 
            message_id = cJSON_GetObjectItemCaseSensitive(msg_json, "message_id");
            if (message_id == NULL || !cJSON_IsNumber(message_id))
                return NULL;
            res->message_id = message_id->valueint;
            time = cJSON_GetObjectItemCaseSensitive(msg_json, "time");
            if (time == NULL || !cJSON_IsNumber(time))
                return NULL;
            res->time = time->valueint;
            chat_id = cJSON_GetObjectItemCaseSensitive(msg_json, "chat_id");
            if (chat_id == NULL || !cJSON_IsNumber(chat_id))
                return NULL;
            res->chat_id = chat_id->valueint;   
        }
        else if (strcmp(res->action, "open_chat") == 0) {

            id = cJSON_GetObjectItemCaseSensitive(msg_json, "id");
            if (id == NULL || !cJSON_IsNumber(id))
                return NULL;
            res->id = id->valueint;
            message = cJSON_GetObjectItemCaseSensitive(msg_json, "message");
            if (message == NULL || message->valuestring == NULL)
                return NULL;
            strcpy(res->message, message->valuestring); 
            message_id = cJSON_GetObjectItemCaseSensitive(msg_json, "message_id");
            if (message_id == NULL || !cJSON_IsNumber(message_id))
                return NULL;
            res->message_id = message_id->valueint;
            time = cJSON_GetObjectItemCaseSensitive(msg_json, "time");
            if (time == NULL || !cJSON_IsNumber(time))
                return NULL;
            res->time = time->valueint;
            chat_id = cJSON_GetObjectItemCaseSensitive(msg_json, "chat_id");
            if (chat_id == NULL || !cJSON_IsNumber(chat_id))
                return NULL;
            res->chat_id = chat_id->valueint;   
        }
        else if (strcmp(res->action, "change_password") == 0) {
            password = cJSON_GetObjectItemCaseSensitive(msg_json, "password");
            if (password == NULL || password->valuestring == NULL)
                return NULL;
            strcpy(res->password, password->valuestring);
        }
    }
    
    cJSON_Delete(msg_json);

    return res;
}


int time_converter(int time) {
    
    time %= 86400;
    int hours = time / 3600;
    int minutes = (time % 3600) / 60;
    printf("%d\n", time);
    printf("Hour %d Min %d\n", hours, minutes);
    return 0;
}


void push_chat(int chat_id, int friend_id, char*login) {

    int i = get_free();
    chat[i].chat_id = chat_id;
    chat[i].friend_id = friend_id;
    strcpy(chat[i].login, login);
}


int get_free() {

    for (int i = 0; i < 100; i++) {

        if (chat[i].chat_id == -1 || chat[i].friend_id == -1 || strcmp(chat[i].login, "-1") == 0) {
            return i;
        }
    }
}

void print_all() {

    for (int i = 0; i < get_free(); i++) {
        
        printf("\n- %d -", chat[i].chat_id);
        printf(" %d -", chat[i].friend_id);
        printf(" %s -", chat[i].login);
    }
}

int search(char *name) {

    for (int i = 1; i < get_free(); i++) {

        if (strcmp(name, chat[i].login) == 0) {

            return i;
        }
    }
    return -1;
}