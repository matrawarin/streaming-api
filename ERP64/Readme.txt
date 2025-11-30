    // Create a notebook widget
    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // Create first tab
    GtkWidget *label1 = gtk_label_new("This is Tab 1");
    GtkWidget *page1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(page1), label1, TRUE, TRUE, 0);
    GtkWidget *tab_label1 = gtk_label_new("Tab 1");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page1, tab_label1);

    // Create second tab
    GtkWidget *label2 = gtk_label_new("This is Tab 2");
    GtkWidget *page2 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(page2), label2, TRUE, TRUE, 0);
    GtkWidget *tab_label2 = gtk_label_new("Tab 2");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page2, tab_label2);

    // Create third tab
    GtkWidget *label3 = gtk_label_new("This is Tab 3");
    GtkWidget *page3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(page3), label3, TRUE, TRUE, 0);
    GtkWidget *tab_label3 = gtk_label_new("Tab 3");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page3, tab_label3);

#include <gtk/gtk.h>

// Callback to close the application
static void on_destroy(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    // Create main window
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK3 Tabs with Overlay");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(on_destroy), NULL);

    // Create a notebook widget (Tabs)
    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window), notebook);

    // ========== First Tab ==========
    GtkWidget *page1 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *label1 = gtk_label_new("This is Tab 1");
    GtkWidget *button1 = gtk_button_new_with_label("Click Me");
    gtk_box_pack_start(GTK_BOX(page1), label1, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(page1), button1, TRUE, TRUE, 0);
    GtkWidget *tab_label1 = gtk_label_new("Tab 1");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page1, tab_label1);

    // ========== Second Tab (With Overlay) ==========
    GtkWidget *page2 = gtk_overlay_new();

    // Background Widget (Main Content)
    GtkWidget *background_label = gtk_label_new("This is an overlay example.");
    gtk_widget_set_halign(background_label, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(background_label, GTK_ALIGN_CENTER);
    gtk_container_add(GTK_CONTAINER(page2), background_label);

    // Overlayed Button
    GtkWidget *overlay_button = gtk_button_new_with_label("Overlay Button");
    gtk_widget_set_halign(overlay_button, GTK_ALIGN_END);
    gtk_widget_set_valign(overlay_button, GTK_ALIGN_START);
    gtk_overlay_add_overlay(GTK_OVERLAY(page2), overlay_button);

    // Add to Notebook
    GtkWidget *tab_label2 = gtk_label_new("Tab 2 (Overlay)");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page2, tab_label2);

    // ========== Third Tab ==========
    GtkWidget *page3 = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    GtkWidget *label3 = gtk_label_new("This is Tab 3");
    gtk_box_pack_start(GTK_BOX(page3), label3, TRUE, TRUE, 0);
    GtkWidget *tab_label3 = gtk_label_new("Tab 3");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), page3, tab_label3);

    // Show everything
    gtk_widget_show_all(window);

    gtk_main();
    return 0;
}


