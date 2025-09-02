#include "gui.h"
#include "huffman.h"

static void on_view_file(GtkWidget *button, gpointer user_data){
    if (!GTK_IS_WINDOW(user_data)) {
        g_warning("Expected GtkWindow, got invalid object.");
        return;
    }
    GtkWindow *parent = GTK_WINDOW(user_data); // Retrieve parent window
    GtkWidget *message;
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new("Open File", // Open file selector window
                                         parent,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Select", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser); // Store name of selected file
        g_print("Selected file: %s\n", filename);
        if (access(filename, R_OK) != 0) { // Cannot view without file permissions
            printf("Permission Denied!\n");
        }
        else{
            while (true){
                g_print("Selected file: %s\n", filename);
                if (strstr(filename, ".huff\0") != NULL){ // Must be a .huff file
                    int frequencies[256] = {0}; // Initialize frequencies
                    getFrequency(frequencies, filename); // Collect frequencies
                    if (access(filename, F_OK) == 0) {
                        decompress(filename, "put.txt", 256); // Decompress into temp put.txt
                    } 
                    else {
                        return;
                    }
                    GtkWidget *text_window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Create new window to display text
                    gtk_window_set_title(GTK_WINDOW(text_window), "Output");
                    gtk_window_set_default_size(GTK_WINDOW(text_window), 500, 400);

                    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL); // Allow for scroll
                    gtk_container_add(GTK_CONTAINER(text_window), scrolled_window);

                    GtkWidget *text_view = gtk_text_view_new(); // Create the text view
                    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE); // Text cannot be edited
                    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
                    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

                    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
                    FILE *fp = fopen("put.txt", "r"); // Open decoded file
                    if (!fp) {
                        gtk_text_buffer_set_text(buffer, "Failed to open decompressed file.", -1);
                    } 
                    else {
                        fseek(fp, 0, SEEK_END);
                        long length = ftell(fp);
                        fseek(fp, 0, SEEK_SET);
                        char *contents = malloc(length + 1);
                        fread(contents, 1, length, fp); // Read the contents of the file
                        contents[length] = '\0';
                        gtk_text_buffer_set_text(buffer, contents, -1); // Display the decoded file
                        free(contents);
                        fclose(fp); // Close the file
                        remove("put.txt"); // Delete the temp file
                    }

                    gtk_widget_show_all(text_window); // Show the text window
                    break;
                }
                else{ // Show error message if file is not a .huff
                    GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
                    message = gtk_message_dialog_new (parent,
                                            flags,
                                            GTK_MESSAGE_ERROR,
                                            GTK_BUTTONS_CLOSE,
                                            "Incorrect file type!");
                    gtk_dialog_run (GTK_DIALOG (message));
                    gtk_widget_destroy (message);
                    break;
                }
            }
        }
        
    }
    gtk_widget_destroy(dialog);
}

static void on_compress_file(GtkWidget *button, gpointer user_data){
    GtkWidget *dialog;
    GtkWidget *message;
    GtkWidget *err;
    GtkWindow *parent = GTK_WINDOW(user_data); // Retrieve parent window

    dialog = gtk_file_chooser_dialog_new("Open File", // Open file selector windoe
                                         parent,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Select", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser); // Retrieve user selected file 
        g_print("Selected file: %s\n", filename);
        if (access(filename, R_OK) != 0) { // Destroy window if we do not have file permissions
            GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
            err = gtk_message_dialog_new (parent,
                                  flags,
                                  GTK_MESSAGE_ERROR,
                                  GTK_BUTTONS_CLOSE,
                                  "Permission Denied!");
            gtk_dialog_run (GTK_DIALOG (err));
            gtk_widget_destroy (err);
        }
        else{
            int frequencies[256] = {0}; // Initialize frequencies
            getFrequency(frequencies, filename); // Fill the array
            int cap = 0;
            for (int i = 0; i < 256; i++){ // Find the capacity
                if (*(frequencies + i) > 0){
                    cap++;
                }
            }
            HeapNode* root = buildTree(frequencies, cap); // Build the huffman tree
            char** codes = (char**)malloc(256*sizeof(char*));
            char* code = (char*)malloc(256*sizeof(char));
            getCode(root, code, codes, 0); // Find the codes
            char fn[128] = "output.huff"; // Initialize name of output file
            int i = 1;
            while (access(fn, F_OK) == 0){
                snprintf(fn, 128, "output(%d).huff", i); // Add a number if output.huff already exists
                i++;
            }
            compressFile(filename, fn, codes, frequencies); // Compress the selected file into output.huff
            GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
            message = gtk_message_dialog_new (parent, // Show success message
                                  flags,
                                  GTK_MESSAGE_OTHER,
                                  GTK_BUTTONS_CLOSE,
                                  "Downloaded as %s", fn);
            gtk_dialog_run (GTK_DIALOG (message));
            gtk_widget_destroy (message);
        }
    }
    gtk_widget_destroy(dialog);
}

int launch(int argc, char** argv){

    gtk_init(&argc, &argv); // Initialize GUI

    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL); // Create main window
    GtkWidget *scrolled_window;
    GtkWidget *text_view;
    GtkTextBuffer *buffer;
    gtk_window_set_title(GTK_WINDOW(window), "Decoder"); // Set title to decoder
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL); // Set close signal

    GtkWidget *view_button = gtk_button_new_with_label("View File"); // Create view button
    g_signal_connect(view_button, "clicked", G_CALLBACK(on_view_file), window); // Set view signal

    GtkWidget *compress_button = gtk_button_new_with_label("Compress File"); // Create view button
    g_signal_connect(compress_button, "clicked", G_CALLBACK(on_compress_file), window); // Set compress signal

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10); // Create box for buttons
    gtk_container_add(GTK_CONTAINER(window), vbox);


    gtk_box_pack_start(GTK_BOX(vbox), view_button, TRUE, TRUE, 5); // Put view button in box
    gtk_box_pack_start(GTK_BOX(vbox), compress_button, TRUE, TRUE, 5); // Put compress button in box

    gtk_widget_show_all(window); // Display the window
    gtk_main();
    
}