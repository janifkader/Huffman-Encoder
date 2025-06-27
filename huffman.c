#include "huffman.h"

int main(int argc, char** argv){

	gtk_init(&argc, &argv);

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    gtk_window_set_title(GTK_WINDOW(window), "Decoder");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *view_button = gtk_button_new_with_label("View File");
    g_signal_connect(view_button, "clicked", G_CALLBACK(on_view_file), window);

    GtkWidget *compress_button = gtk_button_new_with_label("Compress File");
    g_signal_connect(compress_button, "clicked", G_CALLBACK(on_compress_file), window);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), vbox);


    gtk_box_pack_start(GTK_BOX(vbox), view_button, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), compress_button, TRUE, TRUE, 5);

    gtk_widget_show_all(window);
    gtk_main();
	
}