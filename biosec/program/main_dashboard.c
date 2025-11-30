// compile:
// gcc `pkg-config --cflags gtk+-3.0` -o imglist main.c `pkg-config --libs gtk+-3.0` -lcurl
//
// run:
// ./imglist
//
// พิมพ์: ต้องติดตั้ง libgtk-3-dev และ libcurl-dev

#include <gtk/gtk.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char *text;
    char *imgpath; // local path to image
} Item;

GList *items = NULL;
GtkWidget *list_box;

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
    FILE *f = (FILE*)stream;
    return fwrite(ptr, size, nmemb, f);
}

char* download_image(const char *url) {
    CURL *curl = curl_easy_init();
    if(!curl) return NULL;
    char tmpname[] = "/tmp/imgXXXXXX.jpg";
    int fd = mkstemps(tmpname, 4);
    if(fd == -1) { curl_easy_cleanup(curl); return NULL; }
    FILE *f = fdopen(fd, "wb");
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    CURLcode res = curl_easy_perform(curl);
    fclose(f);
    curl_easy_cleanup(curl);
    if(res != CURLE_OK) return NULL;
    return strdup(tmpname);
}

static void refresh_list() {
    GList *l;
    gtk_container_foreach(GTK_CONTAINER(list_box), (GtkCallback)gtk_widget_destroy, NULL);
    for(l = items; l; l = l->next) {
        Item *it = (Item*)l->data;
        GtkWidget *row = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
        GtkWidget *img = gtk_image_new_from_file(it->imgpath);
        gtk_widget_set_size_request(img, 80, 80);
        GtkWidget *lbl = gtk_label_new(it->text);
        gtk_box_pack_start(GTK_BOX(row), img, FALSE, FALSE, 4);
        gtk_box_pack_start(GTK_BOX(row), lbl, TRUE, TRUE, 4);
        gtk_container_add(GTK_CONTAINER(list_box), row);
        gtk_widget_show_all(row);
    }
}

static void on_add(GtkButton *btn, gpointer user_data) {
    GtkEntry *text_entry = g_object_get_data(G_OBJECT(btn), "text_entry");
    GtkEntry *img_entry = g_object_get_data(G_OBJECT(btn), "img_entry");
    const char *text = gtk_entry_get_text(text_entry);
    const char *img = gtk_entry_get_text(img_entry);
    if(strlen(text) == 0) return;
    char *path = NULL;
    if(strlen(img) == 0) {
        // no image: use placeholder
        path = strdup("placeholder.jpg"); // ensure placeholder exists
    } else if (g_str_has_prefix(img, "http://") || g_str_has_prefix(img, "https://")) {
        path = download_image(img);
        if(!path) {
            GtkWidget *d = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Failed download image");
            gtk_dialog_run(GTK_DIALOG(d)); gtk_widget_destroy(d); return;
        }
    } else {
        // treat as local path
        path = strdup(img);
    }
    Item *it = g_malloc(sizeof(Item));
    it->text = strdup(text);
    it->imgpath = path;
    items = g_list_append(items, it);
    refresh_list();
    gtk_entry_set_text(img_entry, "");
    gtk_entry_set_text(text_entry, "");
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    curl_global_init(CURL_GLOBAL_DEFAULT);

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 700, 500);
    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_add(GTK_CONTAINER(win), vbox);

    // controls
    GtkWidget *h = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget *text_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(text_entry), "Enter text...");
    GtkWidget *img_entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(img_entry), "Image path or URL (http...)");
    GtkWidget *add_btn = gtk_button_new_with_label("Add");
    g_object_set_data(G_OBJECT(add_btn), "text_entry", text_entry);
    g_object_set_data(G_OBJECT(add_btn), "img_entry", img_entry);
    g_signal_connect(add_btn, "clicked", G_CALLBACK(on_add), NULL);

    gtk_box_pack_start(GTK_BOX(h), text_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(h), img_entry, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(h), add_btn, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(vbox), h, FALSE, FALSE, 0);

    // list area
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    list_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_container_add(GTK_CONTAINER(scrolled), list_box);
    gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 0);

    gtk_widget_show_all(win);
    gtk_main();

    curl_global_cleanup();
    return 0;
}

