#include <gtk/gtk.h>

typedef struct UserInterface
{
    GtkWindow *window;
    GtkButton *load;
    GtkButton *save;
    GtkButton *resolve;
    GtkButton *network;
}Ui;

void on_load(GtkButton *button, gpointer user_data)
{
    g_print("load\n");
}

void on_save(GtkButton *button, gpointer user_data)
{
    g_print("save\n");
}

void on_resolve(GtkButton *button, gpointer user_data)
{
    g_print("resolve\n");
}

void on_network(GtkButton *button, gpointer user_data)
{
    g_print("training\n");
}


int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "main_window.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "The Ambitions"));
    GtkButton* load = GTK_BUTTON(gtk_builder_get_object(builder, "load"));
    GtkButton* save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    GtkButton* resolve = GTK_BUTTON(gtk_builder_get_object(builder, "resolve"));
    GtkButton* network = GTK_BUTTON(gtk_builder_get_object(builder, "network"));

    Ui ui =
    {
        .window = window,
        .load = load,
        .save = save,
        .resolve = resolve,
        .network = network,
    };

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(on_load), &ui);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save), &ui);
    g_signal_connect(resolve, "clicked", G_CALLBACK(on_resolve), &ui);
    g_signal_connect(network, "clicked", G_CALLBACK(on_network), &ui);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}

