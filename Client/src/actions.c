#include "../inc/header.h"

void send_to_server(char *buf) {
 
    ssize_t result;
    if ((result = send(client_socket, buf, strlen(buf), 0)) == -1) {
        perror("Fail send\n");
    }
}

int send_to_server_and_get(char *buf) {
    
    char response[BUFSIZ];
    ssize_t result;
    if ((result = send(client_socket, buf, strlen(buf), 0)) == -1) {
        perror("Fail send\n");
        return 0; //error
    }
    if ( (result = recv(client_socket, response, sizeof(response), 0)) == -1) { 
            perror("Fail recieve\n");
            close(client_socket);
            no_connection();
            return 0; //error
        }
        if ( result == 0 ) {
            perror("\nDisconnect!\n"); // Перестаем читать сервер
            close(client_socket);
            no_connection();
            return 0; //error
        }
        else {
            strcpy(response, decoding(response));
            write(2, response, strlen(response));
            struct info *res = parse(response);

            type_of_response(res);   
            return 1;
        }
}

//  Функция регистрации
void sign_up() {

    struct info req; 
    strcpy(req.action, "sign_up");  
    req.id = 0;
    if (validation() != true) 
        return;
    strcpy(req.login, gtk_entry_get_text( GTK_ENTRY(login) ));
    strcpy(req.password, gtk_entry_get_text( GTK_ENTRY(password) ));

    if (strlen(req.login) < 4) {
        
        gtk_label_set_text(GTK_LABEL(hint), "Login have to be more than 4 symbols");
        gtk_widget_show (hint);
        return;
    }
    else {
        if (strcmp(gtk_label_get_text (GTK_LABEL(hint)), "Login have to be more than 4 symbols") == 0) {
            gtk_label_set_text(GTK_LABEL(hint), "");
        }
        if (strlen(req.password) < 6) {
        
            gtk_label_set_text(GTK_LABEL(hint), "Password have to be more than 6 symbols");
            gtk_widget_show (hint);
            return;
        }
        else {
            if (strcmp(gtk_label_get_text (GTK_LABEL (hint)), "Password have to be more than 6 symbols") == 0) {
                gtk_label_set_text(GTK_LABEL (hint), "");
            }
            if (strcmp(req.password, gtk_entry_get_text(GTK_ENTRY(repeat))) != 0) {
                
                gtk_label_set_text(GTK_LABEL (hint),  "Passwords are different");
                gtk_widget_show (hint);
                return;
            }
            else {
                if (strcmp(gtk_label_get_text (GTK_LABEL (hint)), "Passwords are different") == 0) {
                    gtk_label_set_text(GTK_LABEL (hint), "");
                }
            }
        }
    }
    
    
    strcpy(req.key, "");
     
    req.chat_id = 0;
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1;
       
    char *buf = stringify(&req); // To JSON
    send_to_server(buf);
}

//  Функция входа
void sign_in() {

    struct info req;

    strcpy(req.action, "sign_in");
    req.id = 0;
    strcpy(req.login, gtk_entry_get_text(GTK_ENTRY(login)) );
    strcpy(req.password, gtk_entry_get_text(GTK_ENTRY(password)) );
    if ((strcmp(req.login, "") == 0) && (strcmp(req.password, "") == 0)) {
            
            gtk_label_set_text(GTK_LABEL(hint),  "Input fields are empty");
            gtk_widget_show (hint);
            return;
    }
    else {
        if (strcmp(gtk_label_get_text (GTK_LABEL(hint)), "Input fields are empty") == 0) {
            gtk_widget_hide (hint);
            gtk_label_set_text(GTK_LABEL(hint), "");
        }
        if (strcmp(req.login, "") == 0) {
            
            gtk_label_set_text(GTK_LABEL(hint),  "Input login");
            gtk_widget_show (hint);
            return;
        }
        else {
            if (strcmp(gtk_label_get_text (GTK_LABEL(hint)), "Input login") == 0) {
                gtk_widget_hide (hint);
                gtk_label_set_text(GTK_LABEL(hint), "");
            }
            if (strcmp(req.password, "") == 0) {
            
                gtk_label_set_text(GTK_LABEL(hint),  "Input password");
                gtk_widget_show (hint);
                return;
            }
            else {
                if (strcmp(gtk_label_get_text (GTK_LABEL(hint)), "Input password") == 0) {
                    gtk_widget_hide (hint);
                    gtk_label_set_text(GTK_LABEL(hint), "");
                }
            }
        }
    }
  
    
    strcpy(req.key, "");

    req.chat_id = 0;
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1;
    gtk_entry_set_text (GTK_ENTRY(login), "");
    gtk_entry_set_text (GTK_ENTRY(password), "");
    char *buf = stringify(&req);
    send_to_server(buf);
}

//  Функция отправки сообщения
void send_message() {

    
    struct info req;
    
    char text[500];

    strcpy(text, gtk_entry_get_text( GTK_ENTRY(Message_Box) ));
    if (strcmp(text, "") == 0) {
        return;
    }
    gtk_entry_set_text( GTK_ENTRY(Message_Box), "" );
    create_message("send_message", req.id, req.message, time_converter(req.time));
    
    strcpy(req.action, "send_message");

    req.id = cl_info.id;
    strcpy(req.login, "");
    strcpy(req.password, "");  
    strcpy(req.key, cl_info.key);

    
    int i = friend_id();
    if (i == -1) {
        write(2, "\nUser is not in your contacts\n", 30);
        //add_chat();
        return;
    }
    req.chat_id = FOCUS;
    req.friend_id = i;
    strcpy(req.message, text);
    req.message_id = -1;
    req.time = time(NULL); 
       

    char *buf = stringify(&req);
    send_to_server(buf);
    
}

//  Функция удаления пользователя
void delete_user() {
    struct info req;
    
    strcpy(req.action, "delete_user");

    req.id = cl_info.id;
    strcpy(req.login, "");
    strcpy(req.password, "");  
    strcpy(req.key, "");

    req.chat_id = 0;
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = 0; 
    
    

    char *buf = stringify(&req);
    send_to_server(buf);
}

//  Функция изменения пароля
void change_password() {

    struct info req;

    strcpy(req.action, "change_password");

    req.id = cl_info.id;
    strcpy(req.login, "");
    printf("New password: "); scanf("%s", req.password);
    strcpy(req.key, cl_info.key);

    req.chat_id = 0;
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1; 
    

    char *buf = stringify(&req);
    send_to_server(buf);
}


void search_friend() {

    struct info req;

    strcpy(req.action, "is_user_exists");
    req.id = 0;
    strcpy(req.login, gtk_entry_get_text( GTK_ENTRY(login) ));
    strcpy(req.password, "");
    strcpy(req.key, "");

    req.chat_id = 0;
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1;
    char *buf = stringify(&req);
    send_to_server(buf);
}


void add_chat() {

    struct info req;
    strcpy(req.action, "add_chat");

    req.id = cl_info.id;
    
    strcpy(req.login, gtk_entry_get_text(GTK_ENTRY (Search_Friends)));
    strcpy(req.password, "");  
    strcpy(req.key, cl_info.key);

    req.chat_id = -1; 
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1;  
    

    char *buf = stringify(&req);
    send_to_server(buf);
}


void delete_message() {
    struct info req;

    strcpy(req.action, "delete_message");
    int to_delete;
    printf("Enter message id: "); scanf("%d", &to_delete);
    req.id = cl_info.id;
    strcpy(req.login, "");
    strcpy(req.password, "");  
    strcpy(req.key, cl_info.key);

    req.chat_id = -1; 
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = to_delete;
    req.time = -1; 
    

    char *buf = stringify(&req);
    send_to_server(buf);
}


void get_chats_info() {

    struct info req;
    strcpy(req.action, "get_chats_info");

    req.id = cl_info.id;
    strcpy(req.login, "");
    strcpy(req.password, "");  
    strcpy(req.key, cl_info.key);

    req.chat_id = -1;  
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1; 

    char *buf = stringify(&req);
    send_to_server(buf);
}

// Open chat
void open_chat(GtkButton *button, gpointer *user_data) {

    
    int id = GPOINTER_TO_INT (user_data);
    if (id == FOCUS) {
        return;
    }
    gtk_container_remove(GTK_CONTAINER(cbox), chat_box1);
    gtk_container_remove(GTK_CONTAINER(cbox), chat_box2);

    chat_box1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    chat_box2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_container_add(GTK_CONTAINER(cbox), chat_box1);
    gtk_container_add(GTK_CONTAINER(cbox), chat_box2);

    struct info req;
    strcpy(req.action, "load_messages");

    req.id = cl_info.id;
    strcpy(req.login, "");
    strcpy(req.password, "");  
    strcpy(req.key, cl_info.key);

    
    req.chat_id = id; 
    req.friend_id = -1;  
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1; 
    

    char *buf = stringify(&req);
    send_to_server(buf);
}


void availability_of_login() {

    struct info req;

    strcpy(req.action, "availability_of_login");
    req.id = 0;
    strcpy(req.login, gtk_entry_get_text( GTK_ENTRY(login) ));
    strcpy(req.password, "");
    strcpy(req.key, "");

    req.chat_id = 0;
    req.friend_id = -1;
    strcpy(req.message, "");
    req.message_id = -1;
    req.time = -1;
    char *buf = stringify(&req);
    send_to_server(buf);
}