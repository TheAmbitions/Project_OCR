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
#include "otsu.h"
#include "resolve.h"
#include "display_sudoku.h"
#include "network.h"
#include "hough.h"

typedef struct UserInterface
{
    GtkWindow *window;
    GtkButton *load;
    GtkButton *show;
    GtkButton *save;
    GtkButton *resolve;
    GtkButton *network;
    GtkEntry *Rotation;
    GtkButton *Enter;
    GtkCheckButton *Auto;
    GtkCheckButton *Manual;
    GtkCheckButton *IA;
    GtkCheckButton *bw;
    GtkCheckButton *Grid;
    GtkButton *Generate;
    GtkButton *web;
}UserInterface;

typedef struct Create_sudoku
{
    char *file;
    int posx;
    int posy;
    int grid[81];
    int gridx;
    int gridy;
    SDL_Surface* surface;
    GtkWindow *win;
    GtkImage* img;
    GtkButton *new;
    GtkButton *back;
    GtkButton *add;
    GtkButton *_0;
    GtkButton *_1;
    GtkButton *_2;
    GtkButton *_3;
    GtkButton *_4;
    GtkButton *_5;
    GtkButton *_6;
    GtkButton *_7;
    GtkButton *_8;
    GtkButton *_9;
}cre_sud;

typedef struct Image
{
    GtkImage *img;
    SDL_Surface *rot_img;
    SDL_Surface *otsu_img;
    SDL_Surface *hough_img;
    SDL_Surface *cases_img;
}Image;

typedef struct Application
{
    gchar* filename;
    int is_rot;
    int is_resolve;
    int is_otsu;
    int is_hough;
    int is_training;
    int is_generate;
    SDL_Surface* image_surface;
    SDL_Surface* dis_img;

    Network network;
    GtkWindow *pro_w;
    GtkWindow* training_w;
    Image image;
    UserInterface ui;
    cre_sud sud;
}App;


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
    button = button;
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
	app->image_surface = load_image(app->filename);
        app->dis_img = load_image(app->filename);
        //g_print("Weight = %i\n", app->image_surface->w);
        //g_print("Height = %i\n", app->image_surface->h);
        app->dis_img = resize(app->dis_img);
        SDL_SaveBMP(app->dis_img, "../Image/tmp_img/display.bmp");
        //g_print("Weight = %i\n", app->dis_img->w);
        //g_print("Height = %i\n", app->dis_img->h);
        gtk_image_set_from_file(app->image.img, "../Image/tmp_img/display.bmp");
        app->is_resolve = 0;
	app->is_otsu = 0;
	app->is_rot = 0;
	app->is_generate = 0;
	app->is_training = 0;
	app->is_hough = 0;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.bw), FALSE);
        break;
    }
    default:
        break;
    }
    gtk_widget_destroy(dialog);
}

void close_window(GtkWidget *widget, gpointer w)
{
    widget = widget;
    gtk_widget_destroy(GTK_WIDGET(w));
}

void on_show(GtkButton *button, gpointer user_data)
{
    button = button;
    App *app = user_data;
    if (app->image_surface == NULL)
    {
	GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before show.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
    }
    else 
    {
        GtkBuilder* builder = gtk_builder_new();
        GError* error = NULL;
        if (gtk_builder_add_from_file(builder, "../data/show_window.glade", &error) == 0)
        {
            g_printerr("Error loading file: %s\n", error->message);
            g_clear_error(&error);
        }
        else
        {
	    SDL_SaveBMP(app->image_surface, "../Image/tmp_img/vrai_image.bmp");
	    GtkWindow* w = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
            GtkImage* img = GTK_IMAGE(gtk_builder_get_object(builder, "img"));

            gtk_widget_show_all(GTK_WIDGET(w));
	    gtk_image_set_from_file(img, "../Image/tmp_img/vrai_image.bmp");
            g_signal_connect_swapped(G_OBJECT(w), "destroy", G_CALLBACK(close_window), NULL);
        }
    }
}

void on_save(GtkButton *button, gpointer user_data)
{
    button = button;
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
	gtk_widget_destroy(dialog);
    }
    else if (app->is_resolve == 0)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nSudoku is not resolved.\n\nPlease, resolve the sudoku before save.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
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
            //g_print("%s\n", filename);
            SDL_Surface* s = load_image("../Image/tmp_img/result.bmp");
            SDL_SaveBMP(s, filename);
            break;
        }
        default:
            break;
        }
        gtk_widget_destroy(dialog);
    }
}

void display_result()
{
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/show_window.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    else
    {
        GtkWindow* w = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
        GtkImage* img = GTK_IMAGE(gtk_builder_get_object(builder, "img"));

        gtk_widget_show_all(GTK_WIDGET(w));
        gtk_image_set_from_file(img, "../Image/tmp_img/result.bmp");
        g_signal_connect_swapped(G_OBJECT(w), "destroy", G_CALLBACK(close_window), NULL);
    }
}

gboolean handle_progress(GtkProgressBar* bar)
{
    gtk_progress_bar_pulse(bar);
    gtk_progress_bar_set_pulse_step(bar, 3.0);
    return TRUE;
}

gboolean des_w(App *app)
{
    if (app->is_resolve == 1)
    {
        gtk_widget_destroy(GTK_WIDGET(app->pro_w));
    }
    return TRUE;
}

void display_pro(App* app)
{
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/progress.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    else
    {
        GtkWindow* pro_w = GTK_WINDOW(gtk_builder_get_object(builder, "pro_w"));
        GtkProgressBar* pro_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "pro_bar"));
        GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label"));

        gtk_label_set_label(label, "Resolving...\n");
	app->pro_w = pro_w;
        g_timeout_add(1000, (GSourceFunc)des_w, app);
        g_timeout_add(1000, (GSourceFunc)handle_progress, pro_bar);
        gtk_widget_show_all(GTK_WIDGET(pro_w));
        g_signal_connect_swapped(G_OBJECT(pro_w), "destroy", G_CALLBACK(close_window), NULL);
    }
}

void resolve_generate(App *app)
{
    display_pro(app);
    if (resolve() == 0)
    {
	GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nCould not resolve!\n\nPlease, verify that the sudoku is resolvable,\nor there was a probleme in the network.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
        gtk_widget_destroy(dialog);
    }
    else
    {
        /*GtkBuilder* builder = gtk_builder_new();
        GError* error = NULL;
        if (gtk_builder_add_from_file(builder, "../data/progress.glade", &error) == 0)
        {
            g_printerr("Error loading file: %s\n", error->message);
            g_clear_error(&error);
        }
        else
        {
	    GtkWindow* pro_w = GTK_WINDOW(gtk_builder_get_object(builder, "pro_w"));
            GtkProgressBar* pro_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "pro_bar"));
	    GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label"));
	
	    gtk_label_set_label(label, "Resolving...\n");
            app->pro_w = pro_w;

            g_timeout_add(1000, (GSourceFunc)handle_progress, pro_bar);
            g_timeout_add(1000, (GSourceFunc)des_w, app);
            gtk_widget_show_all(GTK_WIDGET(pro_w));
            g_signal_connect_swapped(G_OBJECT(pro_w), "destroy", G_CALLBACK(close_window), NULL);
        }*/

	apply_display();
        display_result();
    }
}

void resolve_auto(App *app)
{
    display_pro(app);
    apply_hough(&(app->network), app->filename, 1);
    if (resolve() == 0)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nCould not resolve!\n\nPlease, verify that the sudoku is resolvable,\nor there was a probleme in the network.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
        gtk_widget_destroy(dialog);
    }
    else
    {
	apply_display();
        display_result();
    }
}

void resolve_manual(App* app)
{
    display_pro(app);
    if (resolve() == 0)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nCould not resolve!\n\nPlease, verify that the sudoku is resolvable,\nor there was a probleme in the network.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
        gtk_widget_destroy(dialog);
    }
    else
    {
        apply_display();
        display_result();
    }
}

void on_resolve(GtkButton *button, gpointer user_data)
{
    button = button;
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
	gtk_widget_destroy(dialog);
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Manual))
		&& (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.IA))
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
	gtk_widget_destroy(dialog);
    }
    else
    {
	FILE* file = NULL;
	file = fopen("../data/is_training.txt", "r");
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
		gtk_widget_destroy(dialog);
		free(str);
		return;
	    }
	    free(str);
	}

	if (app->is_generate == 1)
	    resolve_generate(app);
	else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Auto)) == TRUE)
	    resolve_auto(app);
	else
	    resolve_manual(app);
	app->is_resolve = 1;
    }
}

gboolean des_training(App* app)
{
    if (app->is_training == 1)
    {
        gtk_widget_destroy(GTK_WIDGET(app->training_w));
    }
    return 0;
}

void on_network(GtkButton *button, gpointer user_data)
{
    App* app = user_data;
    button = button;
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/progress.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    else
    {
	GtkWindow* pro_w = GTK_WINDOW(gtk_builder_get_object(builder, "pro_w"));
        GtkProgressBar* pro_bar = GTK_PROGRESS_BAR(gtk_builder_get_object(builder, "pro_bar"));
	GtkLabel *label = GTK_LABEL(gtk_builder_get_object(builder, "label"));
	
	gtk_label_set_label(label, "Network training...\n");

        g_timeout_add(1000, (GSourceFunc)handle_progress, pro_bar);
	g_timeout_add(1000, (GSourceFunc)des_training, app);

	app->training_w = pro_w;

        gtk_widget_show_all(GTK_WIDGET(pro_w));
        g_signal_connect_swapped(G_OBJECT(pro_w), "destroy", G_CALLBACK(close_window), NULL);

	init_network(&(app->network), 0);
   	apply_training(&(app->network), "../data/save.txt");
	app->is_training = 1;
        FILE* file = NULL;
        file = fopen("../data/is_training.txt", "w");
        if (file != NULL)
        {
            fprintf(file, "1");
        }

        fclose(file);
    }
}

gboolean Automatic(GtkWidget* widget, gpointer user_data)
{
    widget = widget;
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
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Enter), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.IA), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), FALSE);
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.IA), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.bw), FALSE);
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Grid), FALSE);

        return 0;
    }
    return 0;
}

gboolean Manu(GtkWidget* widget, gpointer user_data)
{
    widget = widget;
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
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Enter), TRUE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), TRUE);
        //gtk_widget_set_sensitive(GTK_WIDGET(app->ui.bw), TRUE);
        //gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), TRUE);

        return 0;
    }

    return 0;
}

gboolean value_changed(GtkWidget* widget, gpointer user_data)
{
    widget = widget;
    App *app = user_data;
    if (app->image_surface == NULL)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before rotate.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
	return 0;
    }

    int res=0;
    int acc=1;
    const gchar* s= gtk_entry_get_text(app->ui.Rotation);
    size_t len = strlen(s);
    size_t i=0;
    if (len==3)
    {
         i = 1;
    }
    else
    {
         i=0;
    }
    for (;len>i;len--)
    {
        if (s[len - 1] >='0' && s[len - 1] <= '9')
        {
            res += acc*(s[len - 1] - '0');
            acc *=10;
        }
        else
        {
            return 0;
        }
    }

    if (s[0]=='-')
    {
        res=-res;
    }

    if ( (s[0]!= '-') && (s[0] <'0' || s[0] > '9'))
    {
        return 0;
    }

    printf("%i\n",res);

    //apply_rotation(&(app->dis_img),
    //    gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(app->ui.Rotation)));
    app->image_surface = rotozoomSurface(app->image_surface, res, 1, 1);
    SDL_SaveBMP(app->image_surface, "../Image/tmp_img/rotation.bmp");
    app->image.rot_img = load_image("../Image/tmp_img/rotation.bmp");
    app->image.rot_img = resize(app->image.rot_img);
    SDL_SaveBMP(app->image.rot_img, "../Image/tmp_img/rotation.bmp");
    gtk_image_set_from_file(app->image.img, "../Image/tmp_img/rotation.bmp");
    app->is_otsu = 0;
    return 0;
}

gboolean IA_recognition(GtkWidget* widget, gpointer user_data)
{
    widget = widget;
    App* app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.IA)) == TRUE)
    {
        app->image.cases_img = load_image("../Image/tmp_img/grille.bmp");
        app->image.cases_img = resize(app->image.cases_img);
        SDL_SaveBMP(app->image.cases_img, "../Image/tmp_img/cases.bmp");
        gtk_image_set_from_file(app->image.img, "../Image/tmp_img/cases.bmp");
        return 0;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.IA)) == FALSE)
    {
	g_print("ok2\n");
        return 0;
    }

    return 0;
}

gboolean BlackWhite(GtkWidget* widget, gpointer user_data)
{
    widget = widget;
    App* app = user_data;
    if (app->image_surface == NULL)
    {
        GtkWidget* dialog;
        GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
        dialog = gtk_message_dialog_new_with_markup(app->ui.window,
            flags,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_CLOSE,
            "Error!\n\nNo image.\n\nPlease, load an image before grayscale.");

        gtk_dialog_run(GTK_DIALOG(dialog));
        g_signal_connect_swapped(dialog, "response",
            G_CALLBACK(gtk_widget_destroy),
            dialog);
	gtk_widget_destroy(dialog);
	return 0;
    }

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw)) == TRUE)
    {
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), TRUE);
	if (app->is_otsu == 0)
	{
	    g_print("ok\n");
	    app->image_surface = apply_otsu(app->image_surface);
            app->image.otsu_img = load_image("../Image/tmp_img/otsu.bmp");
            app->image.otsu_img = resize(app->image.otsu_img);
            SDL_SaveBMP(app->image.otsu_img, "../Image/tmp_img/otsu.bmp");
            gtk_image_set_from_file(app->image.img, "../Image/tmp_img/otsu.bmp");
	    app->is_otsu = 1;
	}
	else
            gtk_image_set_from_file(app->image.img, "../Image/tmp_img/otsu.bmp");
        return 0;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.bw)) == FALSE)
    {
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.Grid), FALSE);
	gtk_widget_set_sensitive(GTK_WIDGET(app->ui.Grid), FALSE);
	if (app->is_rot == 1)
	    gtk_image_set_from_file(app->image.img, "../Image/tmp_img/rotation.bmp");
	else
	    gtk_image_set_from_file(app->image.img, "../Image/tmp_img/display.bmp");
        return 0;
    }

    return 0;
}

gboolean GridDetec(GtkWidget* widget, gpointer user_data)
{
    widget = widget;
    App* app = user_data;

    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Grid)) == TRUE)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.IA), TRUE);
	if (app->is_hough == 0)
	{
	    SDL_SaveBMP(app->image_surface, "../Image/tmp_img/hough.bmp");
	    apply_hough(&(app->network), "../Image/tmp_img/hough.bmp", 0);
	    app->image.hough_img = load_image("../Image/tmp_img/test.bmp");
	    app->image.hough_img = resize(app->image.hough_img);
            SDL_SaveBMP(app->image.hough_img, "../Image/tmp_img/test.bmp");
            gtk_image_set_from_file(app->image.img, "../Image/tmp_img/test.bmp");
	    app->is_hough = 1;
	}
	else
	    gtk_image_set_from_file(app->image.img, "../Image/tmp_img/hough2.bmp");
        return 0;
    }
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(app->ui.Grid)) == FALSE)
    {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(app->ui.IA), FALSE);
        gtk_widget_set_sensitive(GTK_WIDGET(app->ui.IA), FALSE);
        return 0;
    }

    return 0;
}

void draw_square(App* app, char* path)
{
    SDL_Surface* sur = load_image(path);
    int i, j;
    Uint32 p;
    for (i = 0; i < sur->h; i++)
        for (j = 0; j < sur->w; j++)
        {
            p = get_pixel(sur, j, i);
            put_pixel(app->sud.surface, 
                        app->sud.posx + j, 
                        app->sud.posy + i, p);
        }
    SDL_SaveBMP(app->sud.surface, "../Image/tmp_img/create_grid.bmp");
    gtk_image_set_from_file(app->sud.img, "../Image/tmp_img/create_grid.bmp");
    SDL_FreeSurface(sur);
}

void update_pos(App* app)
{
    if (app->sud.posy > 658)
    {
        g_print("stop\n");
    }
    else
    {
        app->sud.posx += 73;
	app->sud.gridx++;
        if (app->sud.posx > 658)
        {
    	     app->sud.posx = 4;
	     app->sud.gridx = 0;
	     app->sud.posy += 73;
	     app->sud.gridy++;
        }
	if (app->sud.posy < 658)
	    draw_square(app, "../Image/UI_img/carre.png");
    }
}

void on_back(GtkButton* button, gpointer user_data)
{
    button = button;
    App *app = user_data;
    draw_square(app, "../Image/UI_img/blanc.png");
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 0;
    app->sud.gridx--;
    app->sud.posx -= 73;
    if (app->sud.posx < 0)
    {
        app->sud.posx = 585;
        app->sud.gridx = 8;
        app->sud.posy -= 73;
        app->sud.gridy--;
        if (app->sud.posy < 0)
        {
            app->sud.posx = 4;
            app->sud.posy = 4;
            app->sud.gridx = 0;
            app->sud.gridy = 0;
        }
    }
    draw_square(app, "../Image/UI_img/carre.png");
}

void draw_fig(App* app)
{
    if (app->sud.posy >= 658)
	return;
    SDL_Surface* sur = load_image(app->sud.file);
    int i, j;
    Uint32 p;
    for (i = 0; i < sur->h; i++)
	for (j = 0; j < sur->w; j++)
	{
	    p = get_pixel(sur, j, i);
	    put_pixel(app->sud.surface, 
			app->sud.posx + j, 
			app->sud.posy + i, p);
	}
    SDL_SaveBMP(app->sud.surface, "../Image/tmp_img/create_grid.bmp");
    gtk_image_set_from_file(app->sud.img, "../Image/tmp_img/create_grid.bmp");
    update_pos(app);
    SDL_FreeSurface(sur);
}

void add_1(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/1.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 1;
    draw_fig(app);
}

void add_2(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/2.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 2;
    draw_fig(app);
}

void add_3(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/3.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 3;
    draw_fig(app);
}

void add_4(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/4.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 4;
    draw_fig(app);
}

void add_5(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/5.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 5;
    draw_fig(app);
}

void add_6(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/6.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 6;
    draw_fig(app);
}

void add_7(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/7.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 7;
    draw_fig(app);
}

void add_8(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/8.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 8;
    draw_fig(app);
}

void add_9(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    app->sud.file = "../Image/UI_img/9.png";
    *(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 9;
    draw_fig(app);
}

void add_0(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    draw_square(app, "../Image/UI_img/blanc.png");
    //*(app->sud.grid + app->sud.gridy * 9 + app->sud.gridx) = 0;
    update_pos(app);
}

void quit(App* app, gpointer w)
{
    app = app;
    gtk_widget_destroy(GTK_WIDGET(w));
}

void add_to_resolve(GtkButton *button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    if (app->sud.posy < 658)
        draw_square(app, "../Image/UI_img/blanc.png");
    app->image_surface = app->sud.surface;
    SDL_SaveBMP(app->sud.surface, "../Image/tmp_img/create_sud.bmp");
    gtk_image_set_from_file(app->image.img, "../Image/tmp_img/create_sud.bmp");
    app->is_generate = 1;

    FILE* file = NULL;
    file = fopen("../data/sudoku.txt", "w");

    if (file != NULL)
    {
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
	    {
		if (app->sud.grid[i * 9 + j] == 0)
		    fprintf(file, ".");
	        else	
                    fprintf(file, "%d", app->sud.grid[i * 9 + j]);
	    }
            fprintf(file, "\n");
        }
            fclose(file);
    }
    else
        errx(1, "Error : cannot create the file");

    gtk_widget_destroy(GTK_WIDGET(app->sud.win));
}

void generate_sud(GtkButton* button, gpointer user_data)
{
    button = button;
    App* app = user_data;
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/create_sudoku.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
    }
    else
    {
	GtkWindow* w = GTK_WINDOW(gtk_builder_get_object(builder, "create_sudoku"));
	GtkImage* img = GTK_IMAGE(gtk_builder_get_object(builder, "image"));
	GtkButton* add = GTK_BUTTON(gtk_builder_get_object(builder, "add"));
	GtkButton* _1 = GTK_BUTTON(gtk_builder_get_object(builder, "1"));
        GtkButton* _2 = GTK_BUTTON(gtk_builder_get_object(builder, "2"));
	GtkButton* _3 = GTK_BUTTON(gtk_builder_get_object(builder, "3"));
	GtkButton* _4 = GTK_BUTTON(gtk_builder_get_object(builder, "4"));
	GtkButton* _5 = GTK_BUTTON(gtk_builder_get_object(builder, "5"));
	GtkButton* _6 = GTK_BUTTON(gtk_builder_get_object(builder, "6"));
	GtkButton* _7 = GTK_BUTTON(gtk_builder_get_object(builder, "7"));
	GtkButton* _8 = GTK_BUTTON(gtk_builder_get_object(builder, "8"));
	GtkButton* _9 = GTK_BUTTON(gtk_builder_get_object(builder, "9"));
        GtkButton* _0 = GTK_BUTTON(gtk_builder_get_object(builder, "0"));
    GtkButton* back = GTK_BUTTON(gtk_builder_get_object(builder, "back"));

	app->sud.win = w;
	app->sud.img = img;
    app->sud.back = back;
	app->sud.posx = 4;
	app->sud.posy = 4;
	app->sud.gridx = 0;
	app->sud.gridy = 0;
	app->sud.add = add;
	app->sud.surface = load_image("../Image/UI_img/grid.png");
	app->sud._1 = _1;
	app->sud._2 = _2;
	app->sud._3 = _3;
	app->sud._4 = _4;
	app->sud._5 = _5;
	app->sud._6 = _6;
	app->sud._7 = _7;
	app->sud._8 = _8;
	app->sud._9 = _9;
    app->sud._0 = _0;

	for (int i = 0; i < 9; i++)
	    for (int j = 0; j < 9; j++)
		 app->sud.grid[i * 9 + j] = 0;

	draw_square(app, "../Image/UI_img/carre.png");

        gtk_widget_show_all(GTK_WIDGET(w));
        g_signal_connect_swapped(G_OBJECT(w), "destroy", G_CALLBACK(quit), app);
	g_signal_connect(G_OBJECT(add), "clicked", G_CALLBACK(add_to_resolve), app);
	g_signal_connect(G_OBJECT(_1), "clicked", G_CALLBACK(add_1), app);
	g_signal_connect(G_OBJECT(_2), "clicked", G_CALLBACK(add_2), app);
	g_signal_connect(G_OBJECT(_3), "clicked", G_CALLBACK(add_3), app);
	g_signal_connect(G_OBJECT(_4), "clicked", G_CALLBACK(add_4), app);
	g_signal_connect(G_OBJECT(_5), "clicked", G_CALLBACK(add_5), app);
	g_signal_connect(G_OBJECT(_6), "clicked", G_CALLBACK(add_6), app);
	g_signal_connect(G_OBJECT(_7), "clicked", G_CALLBACK(add_7), app);
	g_signal_connect(G_OBJECT(_8), "clicked", G_CALLBACK(add_8), app);
	g_signal_connect(G_OBJECT(_9), "clicked", G_CALLBACK(add_9), app);
	g_signal_connect(G_OBJECT(_0), "clicked", G_CALLBACK(add_0), app);
    g_signal_connect(G_OBJECT(back), "clicked", G_CALLBACK(on_back), app);
    }
}

void open_website(GtkButton* button, gpointer user_data)
{
    user_data = user_data;
    button = button;
    int cr = system("firefox https://theambitionsocr.webflow.io/");
    if (cr != 0)
        g_print("rate\n");
}

int main ()
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Initializes SDL
    init_sdl();

    // Loads the UI description and builds the UI.
    // (Exits if an error occurs.)
    GtkBuilder* builder = gtk_builder_new();
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "../data/main_window.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "The Ambitions"));
    GtkButton* load = GTK_BUTTON(gtk_builder_get_object(builder, "load"));
    GtkButton* show = GTK_BUTTON(gtk_builder_get_object(builder, "show"));
    GtkButton* save = GTK_BUTTON(gtk_builder_get_object(builder, "save"));
    GtkButton* resolve = GTK_BUTTON(gtk_builder_get_object(builder, "resolve"));
    GtkButton* network = GTK_BUTTON(gtk_builder_get_object(builder, "network"));
    GtkCheckButton* Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));
    GtkCheckButton* Manual = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Manual"));
    GtkCheckButton* IA = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "IA"));
    GtkCheckButton* bw = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "bw"));
    GtkCheckButton* Grid = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Grid"));
    GtkImage *img = GTK_IMAGE(gtk_builder_get_object(builder, "img_sud"));
    GtkButton* generate = GTK_BUTTON(gtk_builder_get_object(builder, "generate"));
    GtkButton* web = GTK_BUTTON(gtk_builder_get_object(builder, "web"));
    GtkEntry* Rotation = GTK_ENTRY(gtk_builder_get_object(builder,"Rotation"));
    GtkButton* Enter = GTK_BUTTON(gtk_builder_get_object(builder, "Enter"));

    App app =
    {
        .filename = "",
	.is_rot = 0,
	.is_resolve = 0,
	.is_otsu = 0,
	.is_hough = 0,
	.is_generate = 0,
	.is_training = 0,
        .pro_w = NULL,
	.training_w = NULL,
        .image_surface = NULL,
        .dis_img = NULL,
	.network =
	{
	    .inputsize = SIZE,
	    .hiddensize = HID,
	    .outputsize = OUT,
	    .input = {},
	    .values = {},
	    .output = {},
	    .b1 = {},
	    .b2 = {},
	    .w1 = {},
	    .w2 = {}, 
	},
        .image = 
	{ 
	    .img = img,
	    .rot_img = NULL,
	    .otsu_img = NULL,
	    .hough_img = NULL,
	    .cases_img = NULL,
	},
        .ui =
        {
            .window = window,
            .load = load,
	    .show = show,
            .save = save,
            .resolve = resolve,
            .network = network,
            .Rotation = Rotation,
            .Enter = Enter,
            .Auto = Auto,
            .Manual = Manual,
            .IA = IA,
            .bw = bw,
            .Grid = Grid,
	    .Generate = generate,
	    .web = web,
        },
	.sud =
	{
	    .file = "",
	    .posx = 4,
	    .posy = 4,
	    .grid = {},
	    .gridx = 0,
	    .gridy = 0,
	    .surface = NULL,
	    .img = NULL,
	    .win = NULL,
	    .new = NULL,
	    .add = NULL,
	    .back = NULL,
	    ._0 = NULL,
	    ._1 = NULL,
	    ._2 = NULL,
	    ._3 = NULL,
	    ._4 = NULL,
	    ._5 = NULL,
	    ._6 = NULL,
	    ._7 = NULL,
	    ._8 = NULL,
	    ._9 = NULL,
	}
    };

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Auto), TRUE);

    Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));

    gtk_widget_set_sensitive(GTK_WIDGET(Rotation), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(IA), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(bw), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Grid), FALSE);
    gtk_widget_set_sensitive(GTK_WIDGET(Enter), FALSE);

    // Connects signal handlers.
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(load, "clicked", G_CALLBACK(openfile), &app);
    g_signal_connect(show, "clicked", G_CALLBACK(on_show), &app);
    g_signal_connect(save, "clicked", G_CALLBACK(on_save), &app);
    g_signal_connect(resolve, "clicked", G_CALLBACK(on_resolve), &app);
    g_signal_connect(network, "clicked", G_CALLBACK(on_network), &app);
    g_signal_connect(Manual, "clicked", G_CALLBACK(Manu), &app);
    g_signal_connect(Auto, "clicked", G_CALLBACK(Automatic), &app);
    g_signal_connect(IA, "clicked", G_CALLBACK(IA_recognition), &app);
    g_signal_connect(bw, "clicked", G_CALLBACK(BlackWhite), &app);
    g_signal_connect(Grid, "clicked", G_CALLBACK(GridDetec), &app);
    g_signal_connect(generate, "clicked", G_CALLBACK(generate_sud), &app);
    g_signal_connect(web, "clicked", G_CALLBACK(open_website), &app);
    g_signal_connect(Enter,"clicked",G_CALLBACK(value_changed), &app);

    // Runs the main loop.
    gtk_main();

    SDL_FreeSurface(app.image_surface);
    SDL_FreeSurface(app.dis_img);
    SDL_FreeSurface(app.image.otsu_img);
    SDL_FreeSurface(app.image.rot_img);
    SDL_FreeSurface(app.image.hough_img);
    SDL_FreeSurface(app.image.cases_img);

    return 0;
}

