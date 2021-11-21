#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <gtk/gtk.h>

#include "display.h"

typedef struct UserInterface
{
    GtkWindow *window;
    GtkButton *load;
    GtkButton *save;
    GtkButton *resolve;
    GtkButton *network;
    GtkSpinButton *Rotation;
    GtkCheckButton *Auto;
    GtkCheckButton *Manual;
    GtkCheckButton *Noise;
    GtkCheckButton *bw;
    GtkCheckButton *Grid;
}UserInterface;

typedef struct Application
{
    gchar* filename;

    SDL_Surface* image_surface;
    SDL_Surface* screen_surface;

    UserInterface ui;
}App;

void openfile(GtkButton *button, gpointer user_data)
{
    g_print("load\n");

    App* app = user_data;

    GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(button));
    GtkFileFilter *filter = gtk_file_filter_new ();
    GtkWidget* dialog = gtk_file_chooser_dialog_new(("Open image"),
        GTK_WINDOW(toplevel),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "Open", GTK_RESPONSE_ACCEPT,
        "Cancel", GTK_RESPONSE_CANCEL,
        NULL);

    gtk_file_filter_add_pixbuf_formats (filter);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (dialog),filter);

    switch (gtk_dialog_run(GTK_DIALOG(dialog)))
    {
    case GTK_RESPONSE_ACCEPT:
    {
        app->filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        break;
    }
    default:
        break;
    }
    gtk_widget_destroy(dialog);
    
    app->image_surface = load_image(app->filename);
    app->screen_surface = display_image(app->image_surface);
    wait_for_keypressed();
}

void on_save(GtkButton *button, gpointer user_data)
{
    App* app = user_data;
    g_print("save\n");
    g_print("%s\n", app->filename);
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
    App *app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Manual)) == FALSE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Auto), TRUE);
        return 0;
    }


    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Auto)) == TRUE)
    {

        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Manual), FALSE);

        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Rotation), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Noise), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), FALSE);

        g_print("AUTO \n");
        return 0;
    }
    return 0;
}

gboolean Manu(GtkWidget* widget, gpointer user_data)
{
    App *app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Auto)) == FALSE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Manual), TRUE);
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Manual)) == TRUE)
    {


        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Auto), FALSE);


        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Rotation), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Noise), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), TRUE);

        g_print("MANU \n");
        return 0;
    }

    return 0;
}

gboolean value_changed(GtkWidget* widget, gpointer user_data)
{
    App *app = user_data;
    g_print("%i\n", gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->ui.Rotation)));
    return 0;
}

gboolean NoiseReduc(GtkWidget* widget, gpointer user_data)
{
    App* app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Noise)) == TRUE)
    {
        g_print("Noise \n");
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Noise)) == FALSE)
    {
        g_print("Not Noise \n");
        return 0;
    }

    return 0;
}

gboolean BlackWhite(GtkWidget* widget, gpointer user_data)
{
    App* app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw)) == TRUE)
    {
        g_print("BW \n");
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw)) == FALSE)
    {
        g_print("Not BW \n");
        return 0;
    }

    return 0;
}

gboolean GridDetec(GtkWidget* widget, gpointer user_data)
{
    App* app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Grid)) == TRUE)
    {
        g_print("Grid \n");
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Noise)) == FALSE)
    {
        g_print("Not Grid \n");
        return 0;
    }

    return 0;
}

int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Initializes SDL
    init_sdl();

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
    GtkCheckButton* Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));
    GtkCheckButton* Manual = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Manual"));
    GtkSpinButton* Rotation = GTK_SPIN_BUTTON(gtk_builder_get_object(builder, "Rotation"));
    GtkCheckButton* Noise = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Noise"));
    GtkCheckButton* bw = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "bw"));
    GtkCheckButton* Grid = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Grid"));

    App app =
    {
        .filename = "",
        .image_surface = NULL,
        .screen_surface = NULL,
        .ui =
        {
            .window = window,
            .load = load,
            .save = save,
            .resolve = resolve,
            .network = network,
            .Rotation = Rotation,
            .Auto = Auto,
            .Manual = Manual,
            .Noise = Noise,
            .bw = bw,
            .Grid = Grid
        }
    };

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Auto), TRUE);

    Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));

    gtk_widget_set_sensitive(GTK_WIDGET(Rotation), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Noise), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(bw), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Grid), FALSE);

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), &app);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save), &app);
    g_signal_connect(resolve, "clicked", G_CALLBACK(on_resolve), &app);
    g_signal_connect(network, "clicked", G_CALLBACK(on_network), &app);
    g_signal_connect(Manual, "clicked", G_CALLBACK(Manu), &app);
    g_signal_connect(Auto, "clicked", G_CALLBACK(Automatic), &app);
    g_signal_connect(Rotation, "value-changed", G_CALLBACK(value_changed), &app);
    g_signal_connect(Noise, "clicked", G_CALLBACK(NoiseReduc), &app);
    g_signal_connect(bw, "clicked", G_CALLBACK(BlackWhite), &app);
    g_signal_connect(Grid, "clicked", G_CALLBACK(GridDetec), &app);

    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}

