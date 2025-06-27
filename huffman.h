#include "heap.h"
#include <gtk/gtk.h>

void getFrequency(int* frequencies, const char *filename){
    FILE *file = fopen(filename, "r");
    if (!file){
        perror("File error");
        return;
    }
    char c;
    while ((c = fgetc(file)) != EOF){
        if (c >= 0 && c < 256) {
            frequencies[c]++;
        }
    }
    fclose(file);
}

HeapNode* buildTree(int* freqs, int cap){
    MinHeap* heap = newHeap(cap);
    for (int i = 0; i < 256; i++){
        if (*(freqs + i) > 0){
            insert(heap, createNode(*(freqs + i), i));
        }
    }
    while (heap->size > 1) {
        HeapNode *left = extractMin(heap);
        HeapNode *right = extractMin(heap);
        HeapNode *newNode = createNode('\0', left->priority + right->priority);
        newNode->left = left;
        newNode->right = right;
        insert(heap, newNode);
    }
    return extractMin(heap);;
}

void getCode(HeapNode* root, char* code, char** codes, int len){
    if (root == NULL){
        return;
    }
    if (root->left == NULL && root->right == NULL){
        code[len] = '\0';
        codes[(unsigned char)root->character] = strdup(code);
        return;
    }
    code[len] = '0';
    getCode(root->left, code, codes, len+1);
    code[len] = '1';
    getCode(root->right, code, codes, len+1);
}

void writeBit(FILE* out, unsigned char bit, unsigned char* byteBuffer, int* bitCount) {
    *byteBuffer = (*byteBuffer << 1) | (bit & 1);
    (*bitCount)++;

    if (*bitCount == 8) {
        fwrite(byteBuffer, 1, 1, out);
        *bitCount = 0;
        *byteBuffer = 0;
    }
}

void flushBits(FILE* out, unsigned char* byteBuffer, int* bitCount) {
    if (*bitCount > 0) {
        *byteBuffer <<= (8 - *bitCount);
        fwrite(byteBuffer, 1, 1, out);
    }
}

void compressFile(char* input, char* output, char** codes, int* freqs){
    FILE* inputFile = fopen(input, "r");
    FILE* outputFile = fopen(output, "wb");

    fwrite(freqs, sizeof(int), 256, outputFile);

    char byteBuffer = 0;
    int bitCount = 0;

    int c = 0;
    while ((c = fgetc(inputFile)) != EOF) {
        char* code = codes[(unsigned char)c];
        for (int i = 0; code[i]; i++) {
            writeBit(outputFile, code[i] - '0', &byteBuffer, &bitCount);
        }
    }

    flushBits(outputFile, &byteBuffer, &bitCount);
    fclose(inputFile);
    fclose(outputFile);
}

void decompress(const char* inputFilename, const char* outputFilename, int cap) {
    FILE* in = fopen(inputFilename, "rb");
    if (!in) {
        perror("Input file error");
        exit(1);
    }

    FILE* out = fopen(outputFilename, "w");
    if (!out) {
        perror("Output file error");
        exit(1);
    }

    int freqs[256] = {0};
    fread(freqs, sizeof(int), 256, in);

    HeapNode* root = buildTree(freqs, cap);
    HeapNode* curr = root;

    int byte;
    while ((byte = fgetc(in)) != EOF) {
        for (int i = 7; i >= 0; i--) {
            int bit = (byte >> i) & 1;
            curr = (bit == 0) ? curr->left : curr->right;

            if (curr->left == NULL && curr->right == NULL) {
                fputc(curr->character, out);
                curr = root;
            }
        }
    }

    fclose(in);
    fclose(out);
}

static void on_view_file(GtkWidget *button, gpointer user_data){
    if (!GTK_IS_WINDOW(user_data)) {
        g_warning("Expected GtkWindow, got invalid object.");
        return;
    }
    GtkWindow *parent = GTK_WINDOW(user_data);
    GtkWidget *message;
    GtkWidget *dialog;

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         parent,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Select", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        g_print("Selected file: %s\n", filename);
        if (access(filename, R_OK) != 0) {
            printf("Permission Denied!\n");
        }
        else{
            while (true){
                g_print("Selected file: %s\n", filename);
                if (strstr(filename, ".huff\0") != NULL){
                    int frequencies[256] = {0};
                    getFrequency(frequencies, filename);
                    if (access(filename, F_OK) == 0) {
                        decompress(filename, "put.txt", 256);
                    } 
                    else {
                        return;
                    }
                    GtkWidget *text_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
                    gtk_window_set_title(GTK_WINDOW(text_window), "Output");
                    gtk_window_set_default_size(GTK_WINDOW(text_window), 500, 400);

                    GtkWidget *scrolled_window = gtk_scrolled_window_new(NULL, NULL);
                    gtk_container_add(GTK_CONTAINER(text_window), scrolled_window);

                    GtkWidget *text_view = gtk_text_view_new();
                    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_view), FALSE);
                    gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(text_view), GTK_WRAP_WORD_CHAR);
                    gtk_container_add(GTK_CONTAINER(scrolled_window), text_view);

                    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
                    FILE *fp = fopen("put.txt", "r");
                    if (!fp) {
                        gtk_text_buffer_set_text(buffer, "Failed to open decompressed file.", -1);
                    } 
                    else {
                        fseek(fp, 0, SEEK_END);
                        long length = ftell(fp);
                        fseek(fp, 0, SEEK_SET);
                        char *contents = malloc(length + 1);
                        fread(contents, 1, length, fp);
                        contents[length] = '\0';
                        gtk_text_buffer_set_text(buffer, contents, -1);
                        free(contents);
                        fclose(fp);
                        remove("put.txt");
                    }

                    gtk_widget_show_all(text_window);
                    break;
                }
                else{
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
    GtkWindow *parent = GTK_WINDOW(user_data);

    dialog = gtk_file_chooser_dialog_new("Open File",
                                         parent,
                                         GTK_FILE_CHOOSER_ACTION_OPEN,
                                         "_Cancel", GTK_RESPONSE_CANCEL,
                                         "_Select", GTK_RESPONSE_ACCEPT,
                                         NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        GtkFileChooser *chooser = GTK_FILE_CHOOSER(dialog);
        filename = gtk_file_chooser_get_filename(chooser);
        g_print("Selected file: %s\n", filename);
        if (access(filename, R_OK) != 0) {
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
            int frequencies[256] = {0};
            getFrequency(frequencies, filename);
            int cap = 0;
            for (int i = 0; i < 256; i++){
                if (*(frequencies + i) > 0){
                    cap++;
                }
            }
            HeapNode* root = buildTree(frequencies, cap);
            char** codes = (char**)malloc(256*sizeof(char*));
            char* code = (char*)malloc(256*sizeof(char));
            getCode(root, code, codes, 0);
            char fn[128] = "output.huff";
            int i = 1;
            while (access(fn, F_OK) == 0){
                snprintf(fn, 128, "output(%d).huff", i);
                i++;
            }
            compressFile(filename, fn, codes, frequencies);
            GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
            message = gtk_message_dialog_new (parent,
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

