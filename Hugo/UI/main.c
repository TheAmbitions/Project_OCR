#include <gtk/gtk.h>

typedef struct UserInterface
{
    GtkWindow *window;
    GtkButton *load;
    GtkButton *save;
    GtkButton *resolve;
    GtkButton *network;
    GtkSpinButton *rotation;
    GtkCheckButton *autom;
    GtkCheckButton *manual;
    GtkCheckButton *noise;
    GtkCheckButton *bw;
    GtkCheckButton *grid;
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

gboolean Automatic(GtkWidget* widget, gpointer user_data)
{
    Ui *ui = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->manual)) == FALSE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ui->autom), TRUE);
        return 0;
    }


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->autom)) == TRUE)
    {

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ui->manual), FALSE);

        gtk_widget_set_sensitive(GTK_WIDGET(ui->rotation), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(ui->noise), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(ui->bw), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(ui->grid), FALSE);

        g_print("AUTO \n");
        return 0;
    }
    return 0;
}

gboolean Manu(GtkWidget* widget, gpointer user_data)
{
    Ui* ui = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->autom)) == FALSE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ui->manual), TRUE);
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ui->manual)) == TRUE)
    {


        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ui->autom), FALSE);


        gtk_widget_set_sensitive(GTK_WIDGET(ui->rotation), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(ui->noise), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(ui->bw), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(ui->grid), TRUE);

        g_print("MANU \n");
        return 0;
    }

    return 0;
}


gboolean value_changed(GtkWidget* widget, gpointer user_data)
{
    Ui* ui = user_data;
    g_print("%i\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(ui->srotation)));
    return 0;
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
    GtkCheckButton* autom = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "auto"));
    GtkCheckButton* manual = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "manual"));
    GtkSpinButton* rotation = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "rotation"));
    GtkCheckButton* noise = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "niose"));
    GtkCheckButton* bw = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "bw"));
    GtkCheckButton* grid = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "grid"));

    Ui ui =
    {
        .window = window,
        .load = load,
        .save = save,
        .resolve = resolve,
        .network = network,
        .rotation = rotation,
        .autom = autom,
        .manual = manual,
        .noise = noise,
        .bw = bw,
        .grid = grid
    };

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(on_load), &ui);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save), &ui);
    g_signal_connect(resolve, "clicked", G_CALLBACK(on_resolve), &ui);
    g_signal_connect(network, "clicked", G_CALLBACK(on_network), &ui);
    g_signal_connect(Manual, "clicked", G_CALLBACK(Manu), &ui);
    g_signal_connect(Auto, "clicked", G_CALLBACK(Automatic), &ui);
    g_signal_connect(Rotation, "value-changed", G_CALLBACK(value_changed), &ui);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}

