#ifndef _GUI_H
#define _GUI_H

#include <stdbool.h>
#include <gtk/gtk.h>

static void on_view_file(GtkWidget *button, gpointer user_data);

static void on_compress_file(GtkWidget *button, gpointer user_data);

int launch(int argc, char** argv);

#endif