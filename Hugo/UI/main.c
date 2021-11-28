#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <string.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <gtk/gtk.h>

#include "display.h"
#include "rotations.h"

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

typedef struct Image
{
    GtkImage *img;
}Image;

typedef struct Application
{
    gchar* filename;

    SDL_Surface* image_surface;
    SDL_Surface* dis_img;
    
    Image image;
    UserInterface ui;
}App;

typedef struct ProgressBar
{
    int is_training;
    GtkDialog *pro_w;
    GtkProgressBar* pro_bar;
}Bar;

SDL_Surface* resize(SDL_Surface *img)
{
    /*double zoomx = (double)nw / (double)w;
    double zoomy = (double)nh / (double)h;

    img = zoomSurface(img, zoomx, zoomy, 0);*/
    
    while (img->w > 740 || img->h > 700)
    {
        img = rotozoomSurface(img, 0, 0.9, 0);
    }
    return img;
}

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
    app->dis_img = load_image(app->filename);
    //g_print("Weight = %i\n", app->image_surface->w);
    //g_print("Height = %i\n", app->image_surface->h);
    app->dis_img = resize(app->dis_img);
    SDL_SaveBMP(app->dis_img, "display.bmp");
    //g_print("Weight = %i\n", app->dis_img->w);
    //g_print("Height = %i\n", app->dis_img->h);
    gtk_image_set_from_file(app->image.img, "display.bmp");
}

void on_save(GtkButton *button, gpointer user_data)
{
    App* app = user_data;
    if (app->image_surface == NULL)
    {
	    GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before save.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
    }
    else if (app->is_resolve == 0)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, resolve the sudoku before save.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
    }
    else
    {
        GtkWidget* dialog;
        GtkWidget* toplevel = gtk_widget_get_toplevel(GTK_WIDGET(button));
        dialog = gtk_file_chooser_dialog_new("Save Text ",
            GTK_WINDOW(toplevel),
            GTK_FILE_CHOOSER_ACTION_SAVE,
            "Cancel", GTK_RESPONSE_CANCEL,
            "Save", GTK_RESPONSE_ACCEPT,
            NULL);
        switch (gtk_dialog_run(GTK_DIALOG(dialog)))
        {
        case GTK_RESPONSE_ACCEPT:
        {
            gchar* filename;
            filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
            g_print("%s\n", filename)
            //Image_Save_BMP("result.bmp", filename);
            break;
        }
        default:
            break;
        }
        gtk_widget_destroy(dialog);
    }
}

void on_resolve(GtkButton *button, gpointer user_data)
{
    App* app = user_data;
    if (app->image_surface == NULL)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before save.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Manual))
		&& (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Noise))
		|| !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw))
		|| !gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Grid))))
    {
	GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, do the image processing before resolve.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
    }
    else
    {
	FILE* file = NULL;
	file = fopen("is_training.txt", "r");
	if (file != NULL)
	{
	    char *str = malloc(20 * sizeof(char));
	    str = fgets(str, 20, file);
	    fclose(file);
	    if (atoi(str) == 0)
	    {
		GtkWidget* dialog;
                GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
                dialog = gtk_message_dialog_new_with_markup(app->ui.window,
                    flags,
                    GTK_MESSAGE_WARNING,
                    GTK_BUTTONS_CLOSE,
                    "WARNING!\n\nThe network is not train.\n\nPlease, train the network before resolve.");

                gtk_dialog_run(GTK_DIALOG(dialog));
                g_signal_connect_swapped(dialog, "response",
                    G_CALLBACK(gtk_widget_destroy),
                    dialog);
		free(str);
		return;
	    }
	    free(str);
	}
	g_print("On peut résoudre\n");
    }
}

gboolean handle_progress(GtkProgressBar* bar)
{
    gtk_progress_bar_pulse(bar);
    gtk_progress_bar_set_pulse_step(bar, 1.0);
    return TRUE;
}

void on_network(GtkButton *button, gpointer user_data)
{
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "progress.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    else
    {
	GtkDialog* pro_w = GTK_DIALOG(gtk_builder_get_object(builder, "pro_w"));
        GtkProgressBar* pro_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "pro_bar"));

        gtk_dialog_run(pro_w);
        g_timeout_add(1000, (GSourceFunc)handle_progress, pro_bar);
    }
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
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Noise), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.bw), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Grid), FALSE);

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
        //gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), TRUE);
        //gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), TRUE);

        return 0;
    }

    return 0;
}

gboolean value_changed(GtkWidget* widget, gpointer user_data)
{
    App *app = user_data;
    if (app->image_surface == NULL)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before resolve.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	return 0;
    }

    //apply_rotation(&(app->dis_img),
    //    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->ui.Rotation)));
    app->dis_img = rotozoomSurface(app->dis_img, 
	gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->ui.Rotation)),
	1, 1);
    SDL_SaveBMP(app->dis_img, "display.bmp");
    gtk_image_set_from_file(app->image.img, "display.bmp");
    return 0;
}

gboolean NoiseReduc(GtkWidget* widget, gpointer user_data)
{
    App* app = user_data;
    if (app->image_surface == NULL)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before reduce noise.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Noise), FALSE);
	return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Noise)) == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), TRUE);
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Noise)) == FALSE)
    {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.bw), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), FALSE);
        return 0;
    }

    return 0;
}

gboolean BlackWhite(GtkWidget* widget, gpointer user_data)
{
    App* app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw)) == TRUE)
    {
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), TRUE);
        return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw)) == FALSE)
    {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Grid), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), FALSE);
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
    GtkImage *img = GTK_IMAGE(gtk_builder_get_object(builder, "img_sud"));

    App app =
    {
        .filename = "",
        .image_surface = NULL,
        .dis_img = NULL,
        .image = { .img = img },
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
            .Grid = Grid,
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

    SDL_FreeSurface(app.image_surface);
    SDL_FreeSurface(app.dis_img);

    // Exits.
    return 0;
}

