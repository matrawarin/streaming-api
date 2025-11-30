// compile:
// gcc `pkg-config --cflags gtk+-3.0` -o show_image show_image.c `pkg-config --libs gtk+-3.0`
// run:
// ./show_image /path/to/image.jpg

#include <gtk/gtk.h>

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    if (argc < 2) {
        g_print("Usage: %s /path/to/image.jpg\n", argv[0]);
        return 1;
    }

    const char *img_path = argv[1];

    GtkWidget *win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(win), 800, 600);
    gtk_window_set_title(GTK_WINDOW(win), "Show Image (GTK3)");

    g_signal_connect(win, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create scrollable area so large images can be scrolled
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_container_add(GTK_CONTAINER(win), scrolled);

    // Load image into GtkImage
    GdkPixbuf *pix = gdk_pixbuf_new_from_file(img_path, NULL);
    if (!pix) {
        g_printerr("Failed to load image: %s\n", img_path);
        return 1;
    }
    GtkWidget *img = gtk_image_new_from_pixbuf(pix);

    gtk_container_add(GTK_CONTAINER(scrolled), img);
    gtk_widget_show_all(win);

    gtk_main();
    g_object_unref(pix);
    return 0;
}

