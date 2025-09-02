#ifndef _GUI_H
#define _GUI_H

#include <stdbool.h>
#include <gtk/gtk.h>

/*
 * Handles the user's use of the 'View File' button.
 * button: The 'View File' button component.
 * user_data: The GUI component.
 * return: void.
 */
static void on_view_file(GtkWidget *button, gpointer user_data);

/*
 * Handles the user's use of the 'Compress File' button.
 * button: The 'Compress File' button component.
 * user_data: The GUI component.
 * return: void.
 */
static void on_compress_file(GtkWidget *button, gpointer user_data);

/*
 * The main GUI driver function which displays it to the user.
 * return: 0 if successful, 1 otherwise.
 */
int launch(int argc, char** argv);

#endif