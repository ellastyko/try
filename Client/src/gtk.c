#include "../inc/header.h"

void destroy() { gtk_main_quit(); }

void SIGN_BOXES() {

    SignLog = GTK_CONTAINER(gtk_builder_get_object(builder, "SignLog"));
    SignBox = GTK_CONTAINER(gtk_builder_get_object(builder, "SignBox"));

    login = GTK_WIDGET(gtk_builder_get_object(builder, "Login"));
    password = GTK_WIDGET(gtk_builder_get_object(builder, "Password"));
    repeat = GTK_WIDGET(gtk_builder_get_object(builder, "Repeat"));
    hint = GTK_WIDGET(gtk_builder_get_object(builder, "Hint"));

    To_Sign = GTK_WIDGET(gtk_builder_get_object(builder, "To_sign"));
    Log_in = GTK_WIDGET(gtk_builder_get_object(builder, "Log_in"));

    To_Log = GTK_WIDGET(gtk_builder_get_object(builder, "To_log"));
    Sign_up = GTK_WIDGET(gtk_builder_get_object(builder, "Sign_up"));
}

void MAIN_BOXES() {
    
    Main = GTK_CONTAINER(gtk_builder_get_object(builder, "Main"));

    friends = GTK_CONTAINER(gtk_builder_get_object(builder, "friends"));
    Open_settings = GTK_WIDGET(gtk_builder_get_object(builder, "Open_settings"));
    Search_Friends = GTK_WIDGET(gtk_builder_get_object(builder, "Search_friends"));

    settings = GTK_CONTAINER(gtk_builder_get_object(builder, "settings"));
    Open_Friends = GTK_WIDGET(gtk_builder_get_object(builder, "Open_friends"));
    Change_password = GTK_WIDGET(gtk_builder_get_object(builder, "Change_password"));
    Log_out = GTK_WIDGET(gtk_builder_get_object(builder, "Log_out"));
    Theme = GTK_WIDGET(gtk_builder_get_object(builder, "Theme"));

    your_chat = GTK_CONTAINER(gtk_builder_get_object(builder, "your_chat"));
    Message_Box = GTK_WIDGET(gtk_builder_get_object(builder, "Message_Box"));
    Select_file_button = GTK_WIDGET(gtk_builder_get_object(builder, "Select_file_button"));
    Send_button = GTK_WIDGET(gtk_builder_get_object(builder, "Send_button"));
}

void NO_CONNECTION_BOX() {

    Connection_lost = GTK_CONTAINER(gtk_builder_get_object(builder, "connection-lost"));
    Reconnect_button = GTK_WIDGET(gtk_builder_get_object(builder, "Reconnect_button"));
}

void no_connection() {
    
  gtk_widget_show ( GTK_WIDGET(Connection_lost) );
  gtk_widget_hide ( GTK_WIDGET(Main) );
  gtk_widget_hide ( GTK_WIDGET(SignLog) );
}


void to_sign_up() {

    gtk_widget_hide (To_Sign);
    gtk_widget_hide (Log_in); 
    gtk_widget_hide (hint);

    gtk_widget_show (repeat);
    gtk_widget_show (To_Log);
    gtk_widget_show (Sign_up);
    
    STATE = 1;
}

void to_log_in() {

    gtk_widget_hide (repeat);
    gtk_widget_hide (To_Log);
    gtk_widget_hide (Sign_up);
    gtk_widget_hide (hint);

    gtk_widget_show (To_Sign);
    gtk_widget_show (Log_in);
    STATE = 0;
}

void open_settings() {

    gtk_widget_hide ( GTK_WIDGET(friends) );
    gtk_widget_show ( GTK_WIDGET(settings) );     
}

void open_friends() {

    gtk_widget_hide ( GTK_WIDGET(settings) );
    gtk_widget_show ( GTK_WIDGET(friends) );  
}

void log_out() {

    gtk_widget_show ( GTK_WIDGET(SignLog) );
    gtk_widget_hide ( GTK_WIDGET(Main) );
    cl_info.id = -1;
    strcpy(cl_info.login, "");
    strcpy(cl_info.password, "");
    strcpy(cl_info.key, "");
    STATE = 0;
}

void theme () {
    if (gtk_switch_get_state ( GTK_SWITCH (Theme) ) == true) {
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(style), GTK_STYLE_PROVIDER_PRIORITY_USER);
    }
    else {
        gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(style_dark), GTK_STYLE_PROVIDER_PRIORITY_USER);
    } 
}



void open_main() {

    gtk_widget_hide ( GTK_WIDGET(Connection_lost) );
    gtk_widget_hide ( GTK_WIDGET(SignLog) );
    gtk_widget_show ( GTK_WIDGET(friends) );  
    gtk_widget_show ( GTK_WIDGET(Main) );   
}