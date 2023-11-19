/*GTK Hello World*/
/*Build this program using the command in readme, if it work it's setup correctly*/

#include <gtk/gtk.h>

static void print_hello(GtkWidget* widget,gpointer data){
    g_print("Hello World Test\n");
}

static void activate(GtkApplication* app,gpointer user_data){
    GtkWidget* window;
    GtkWidget* button;
    GtkWidget* button2;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Memory Game");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);

    button = gtk_button_new_with_label("Button 1");
    button2 = gtk_button_new_with_label("Button 2");
    g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_window_set_child(GTK_WINDOW(window), button);

    gtk_window_present(GTK_WINDOW(window));
}

int main(int argc,char** argv){
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}