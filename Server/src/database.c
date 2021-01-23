#include "../inc/header.h"


// Из числа в строку itoa()
// Из строки в число atoi()

int db_init(char *db_name)
{
    int ret;
    if (SQLITE_OK != (ret = sqlite3_initialize())) {
        printf("Failed to initialize library: %d\n", ret);
        return -1;
        sqlite3_close(db);
    }
    // open connection to a DB
    if (SQLITE_OK != (ret = sqlite3_open_v2(db_name, &db, SQLITE_OPEN_READWRITE, NULL))) {
        printf("Failed to open conn: %d\n", ret);
        return -1;
    }
    return 1;
}

void create_db(char* statement, sqlite3* db) {
   char* error = 0; 
   int rc = sqlite3_exec(db, statement, NULL, 0, &error);
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", error);
      sqlite3_free(error);
   } 
} 


bool db_add_user(char *login, char *password, int key)
{
    sqlite3_stmt *res = NULL;
    int rc;
    char *query_f = sqlite3_mprintf("INSERT INTO users VALUES(NULL,'%s','%s','%i')",
                                    login,
                                    password,
                                    key);

    rc = sqlite3_prepare_v2(db, query_f, -1, &res, 0);
    rc = sqlite3_step(res);
    if (rc == SQLITE_DONE) {
        printf("Login and password inserted\n");
    }
    else {
        printf("Not inserter!\n");
        return 0;
    }
    return 1;
}

bool check_login(char *login) {
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT LOGIN FROM users", -1, &stmt, NULL);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        const char*login2 = sqlite3_column_text(stmt, 0);
        if (strcmp(login, login2) == 0) {
            write(2, "LOGIN ALREADY EXISTS!\n", 23);
            sqlite3_finalize(stmt);
            return false;
        }
	}	
    sqlite3_finalize(stmt);
    return true;
}

bool verification(char *login, char *password) {
    sqlite3_stmt *stmt;
    int state = 0; // Need to equel 2
    sqlite3_prepare_v2(db, "SELECT LOGIN FROM users", -1, &stmt, NULL);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        const char*login2 = sqlite3_column_text(stmt, 0);
        if (strcmp(login, login2) == 0) {   
            state += 1;    // +1 if login correct
        }
	}
    stmt = NULL;
    sqlite3_prepare_v2(db, "SELECT PASSWORD FROM users", -1, &stmt, NULL);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
        const char*password2 = sqlite3_column_text(stmt, 0);
        if (strcmp(password, password2) == 0) {
            state += 1; //+1 if password correct
        }
	}
    if (state == 2) {
        write(2, "WELCOME IN YOUR ACC\n", 23);
        sqlite3_finalize(stmt);
        return true;
    }
    else {
        sqlite3_finalize(stmt);
        return false;
    }  
}


void get_id_and_key(int client_socket, char *login) {

    sqlite3_stmt *stmt; int r;
    //  !!!!!!!!!!!!!FUNCTION  THAT SEND ID AND KEY TO USER BY LOGIN
}

/*void db_print_all() { //db_user_t user
    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT * FROM users", -1, &stmt, NULL);
    while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		int num_cols = sqlite3_column_count(stmt);
		
		for (i = 0; i < num_cols; i++)
		{
			switch (sqlite3_column_type(stmt, i))
			{
			case (SQLITE3_TEXT):
				printf("%s, ", sqlite3_column_text(stmt, i));
				break;
			case (SQLITE_INTEGER):
				printf("%d, ", sqlite3_column_int(stmt, i));
				break;
			case (SQLITE_FLOAT):
				printf("%g, ", sqlite3_column_double(stmt, i));
				break;
                
			default:
				break;
			}
		}
		printf("\n");

	}

	sqlite3_finalize(stmt);

	sqlite3_close(db);


}*/

/*int db_add_chat(db_chats_t chat)
{
    sqlite3_stmt *res = NULL;
    int rc;

    char *query_f = sqlite3_mprintf("INSERT INTO chats VALUES(NULL,'%s','%s')",
                                    chat.name,
                                    chat.type);

    rc = sqlite3_prepare_v2(db, query_f, -1, &res, 0);

    rc = sqlite3_step(res);

    if (rc == SQLITE_DONE)
    {
        printf("Inserter!\n");
    }
    else
    {
        printf("Not inserter!\n");
        return 0;
    }

    rc = sqlite3_finalize(res);
    return 1;
}

int db_add_message(db_messages_t message)
{
    sqlite3_stmt *res = NULL;
    int rc;

    char *query_f = sqlite3_mprintf("INSERT INTO messages VALUES(NULL, %d,%d,'%s',%d)",
                                    message.chat_id,
                                    message.user_id,
                                    message.message,
                                    message.time);

    rc = sqlite3_prepare_v2(db, query_f, -1, &res, 0);

    rc = sqlite3_step(res);

    if (rc == SQLITE_DONE)
    {
        printf("Inserter!\n");
    }
    else
    {
        printf("Not inserter!\n");
        return 0;
    }

    rc = sqlite3_finalize(res);

    return 1;
}

int db_add_user_chat(db_users_chats_t user_chat)
{
    return 1;
}*/

