#include <gtk/gtk.h>

// State of the game.
typedef enum State
{
    STOP,                           // Stop state
    PLAY,                           // Play state
    PAUSE,                          // Pause state

} State;



// Structure of the graphical user interface.
typedef struct UserInterface
{
    GtkWindow* window;              // Main window
    GtkDrawingArea* area;           // Drawing area
    GtkButton* start_button;        // Start button
    GtkButton* stop_button;         // Stop button
    
    GtkCheckButton* Auto;
    GtkCheckButton* Manual;
    
    GtkSpinButton* Rotation; //rotation
    
    GtkCheckButton* Noise;
    GtkCheckButton* BW;
    GtkCheckButton* Grid;
    
        
} UserInterface;

// Structure of the game.
typedef struct Game
{
    State state;                    // State of the game
    UserInterface ui;               // User interface
} Game;

// Event handler for the "draw" signal of the drawing area.
gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    // Gets the 'Game' structure.
    //Game *game = user_data;

    // Sets the background to white.
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_paint(cr);


    // Propagates the signal.
    return FALSE;
}

void redraw_item(GtkDrawingArea *area, GdkRectangle *old, GdkRectangle *new)
{
    gdk_rectangle_union(old, new, old);
    gtk_widget_queue_draw_area(GTK_WIDGET(area),old->x, old->y, old->width, old->height);
}


// Sets the 'Play' state.
void set_play(Game* game)
{
    // - Set the state field to PLAY.
    game->state = PLAY;
    // - Set the label of the start button to "Pause".
    gtk_button_set_label (game->ui.start_button,"Pause");
    // - Disable the stop button.
    gtk_widget_set_sensitive (GTK_WIDGET(game->ui.stop_button),FALSE);
}

// Sets the 'Pause' state.
void set_pause(Game* game)
{
    // - Set the state field to PAUSE.
    game->state = PAUSE;
    // - Set the label of the start button to "Resume".
    gtk_button_set_label (game->ui.start_button,"Resume");
    // - Enable the stop button.
    gtk_widget_set_sensitive (GTK_WIDGET(game->ui.stop_button),TRUE);
 

}

// Sets the 'Stop' state.
void set_stop(Game *game)
{
    // - Set the state field to STOP.
    game->state = STOP;
    // - Set the label of the start button to "Start".
    gtk_button_set_label (game->ui.start_button,"Start");
    // - Disable the stop button.
    gtk_widget_set_sensitive (GTK_WIDGET(game->ui.stop_button),FALSE);
}


gboolean Automatic(GtkWidget *widget, gpointer user_data)
{
	Game *game = user_data;
	
		

	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Manual)) == FALSE)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(game->ui.Auto),TRUE);
		return 0;
	}
		
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Auto)) == TRUE)
	{
		
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(game->ui.Manual),FALSE);
		
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.Rotation),TRUE);
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.Noise),TRUE);
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.BW),TRUE);
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.Grid),TRUE);
		g_print("AUTO \n");
		return 0;
	}

	
	return 0;
	
}

gboolean Manu(GtkWidget *widget, gpointer user_data)
{
	Game *game = user_data;
	

	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Auto)) == FALSE)
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(game->ui.Manual),TRUE);
		return 0;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Manual)) == TRUE)
	{
		
		
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(game->ui.Auto),FALSE);
			
				
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.Rotation),FALSE);
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.Noise),FALSE);
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.BW),FALSE);
		gtk_widget_set_sensitive (GTK_WIDGET(game->ui.Grid),FALSE);
		
		g_print("MANU \n");
		return 0;
	}

	return 0;
}

gboolean value_changed(GtkWidget *widget, gpointer user_data)
{
	Game *game = user_data;
	g_print("%i\n",gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(game->ui.Rotation)));
	return 0;
}

gboolean NoiseReduc(GtkWidget *widget, gpointer user_data)
{
	Game *game = user_data;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Noise)) == TRUE)
	{
		g_print("Noise \n");
		return 0;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Noise)) == FALSE)
	{
		g_print("Not Noise \n");
		return 0;
	}
	
	return 0;
}

gboolean BlackWhite(GtkWidget *widget, gpointer user_data)
{
	Game *game = user_data;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.BW)) == TRUE)
	{
		g_print("BW \n");
		return 0;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.BW)) == FALSE)
	{
		g_print("Not BW \n");
		return 0;
	}
	
	return 0;
}

gboolean GridDetec(GtkWidget *widget, gpointer user_data)
{
	Game *game = user_data;
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Grid)) == TRUE)
	{
		g_print("Grid \n");
		return 0;
	}
	
	if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(game->ui.Noise)) == FALSE)
	{
		g_print("Not Grid \n");
		return 0;
	}
	
	return 0;
}
// Event handler for the "clicked" signal of the start button.
void on_start(GtkButton *button, gpointer user_data)
{
    // Gets the `Game` structure.
    Game *game = user_data;

    // Sets the next state according to the current state.
    switch (game->state)
    {
        case STOP: set_play(game); break;
        case PLAY: set_pause(game); break;
        case PAUSE: set_play(game); break;
    };
}

// Event handler for the "clicked" signal of the stop button.
void on_stop(GtkButton *button, gpointer user_data)
{
    set_stop(user_data);
}


int main (int argc, char *argv[])
{
    // Initializes GTK.
    gtk_init(NULL, NULL);

    // Constructs a GtkBuilder instance.
    GtkBuilder* builder = gtk_builder_new ();

    // Loads the UI description.
    // (Exits if an error occurs.)
    GError* error = NULL;
    if (gtk_builder_add_from_file(builder, "plain.glade", &error) == 0)
    {
        g_printerr("Error loading file: %s\n", error->message);
        g_clear_error(&error);
        return 1;
    }

    // Gets the widgets.
    GtkWindow* window = GTK_WINDOW(gtk_builder_get_object(builder, "org.gtk.duel"));
    GtkDrawingArea* area = GTK_DRAWING_AREA(gtk_builder_get_object(builder, "area"));
    GtkButton* start_button = GTK_BUTTON(gtk_builder_get_object(builder, "start_button"));
    GtkButton* stop_button = GTK_BUTTON(gtk_builder_get_object(builder, "stop_button"));
    
    
    GtkCheckButton* Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));
    GtkCheckButton* Manual = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Manual"));
    
    GtkSpinButton* Rotation = GTK_SPIN_BUTTON(gtk_builder_get_object(builder,"Rotation"));
    
    GtkCheckButton* Noise = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Noise"));
    GtkCheckButton* BW = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "BW"));
    GtkCheckButton* Grid = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Grid"));
 

    // Creates the "Game" structure.
    Game game =
    {
        .state = STOP,
	
        .ui =
            {
                .window = window,
                .area = area,
                .start_button = start_button,
                .stop_button = stop_button,
                .Auto = Auto,
                .Manual = Manual,
                .Rotation = Rotation,
                .Noise = Noise,
                .BW=BW,
                .Grid = Grid,
              
            },
    };

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Auto),TRUE);
	Auto = GTK_CHECK_BUTTON(gtk_builder_get_object(builder, "Auto"));

    // Connects event handlers.
    g_signal_connect(stop_button, "clicked", G_CALLBACK(on_stop), &game);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(area, "draw", G_CALLBACK(on_draw), &game);
    g_signal_connect(start_button, "clicked", G_CALLBACK(on_start), &game);
    
	g_signal_connect(Manual, "clicked", G_CALLBACK(Manu), &game);
	g_signal_connect(Auto, "clicked", G_CALLBACK(Automatic), &game);
	
	g_signal_connect(Rotation,"value-changed",G_CALLBACK(value_changed),&game);
	g_signal_connect(Noise, "clicked", G_CALLBACK(NoiseReduc), &game);
	g_signal_connect(BW, "clicked", G_CALLBACK(BlackWhite), &game);
	g_signal_connect(Grid, "clicked", G_CALLBACK(GridDetec), &game);
	

 
    // Runs the main loop.
    gtk_main();

    // Exits.
    return 0;
}
