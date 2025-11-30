#include <gtk/gtk.h>
#include <libsoup/soup.h>
#include <webkit2/webkit2.h>

void create_login_window() ;
void create_main_window() ;

void create_main_activate(GtkApplication *app, gpointer user_data);

GtkWidget *window_login;
GtkWidget *window_main;

// Function to send HTTP POST request to the server
char* send_post_request(const char *url, const char *payload) {
    SoupSession *session = soup_session_new();
    SoupMessage *message = soup_message_new("POST", url);
    soup_message_set_request(message, "application/json", SOUP_MEMORY_COPY, payload, strlen(payload));

    // Send request and wait for response
    soup_session_send_message(session, message);

    if (message->status_code == SOUP_STATUS_OK) {
        return g_strdup(message->response_body->data);
    } else {
        g_print("HTTP Error: %d\n", message->status_code);
        return NULL;
    }
}

// Callback for login button
void on_login_clicked(GtkWidget *button, gpointer user_data) {
    GtkEntry *username_entry = GTK_ENTRY(g_object_get_data(G_OBJECT(button), "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(g_object_get_data(G_OBJECT(button), "password_entry"));

    const char *username = gtk_entry_get_text(username_entry);
    const char *password = gtk_entry_get_text(password_entry);

    GtkApplication *app;
    int status;

    int   argc;
    char *argv;

    // Prepare JSON payload
    char *payload = g_strdup_printf("{\"username\": \"%s\", \"password\": \"%s\"}", username, password);

    // Send request to server
    char *response = send_post_request("http://localhost/erp_backend/login.php", payload);

    if (response) {
        g_print("Server Response: %s\n", response);
        gtk_widget_destroy(window_login);

        
        app = gtk_application_new("com.example.webbrowser", G_APPLICATION_FLAGS_NONE);
        g_signal_connect(app, "activate", G_CALLBACK(create_main_activate), NULL);

        status = g_application_run(G_APPLICATION(app), argc, argv);
        g_object_unref(app);

        //create_main_window();
        //g_free(response);
    }

    g_free(payload);
}

// Callback for login button
void on_search_clicked(GtkWidget *button, gpointer user_data) {
    GtkEntry *username_entry = GTK_ENTRY(g_object_get_data(G_OBJECT(button), "username_entry"));
    GtkEntry *password_entry = GTK_ENTRY(g_object_get_data(G_OBJECT(button), "password_entry"));

    const char *username = gtk_entry_get_text(username_entry);
    const char *password = gtk_entry_get_text(password_entry);

    // Prepare JSON payload
    char *payload = g_strdup_printf("{\"username\": \"%s\", \"password\": \"%s\"}", username, password);

    // Send request to server
    char *response = send_post_request("http://localhost/erp_backend/login.php", payload);

    if (response) {
        g_print("Server Response: %s\n", response);
        //gtk_widget_destroy(window_login);

        //create_main_window();
        g_free(response);
    }

    g_free(payload);
}


 void create_main_activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *webview;

    // Create a new GTK window
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Web Browser");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 600);

    // Create a WebKit WebView widget
    webview = webkit_web_view_new();
    webkit_web_view_load_uri(WEBKIT_WEB_VIEW(webview), "https://www.google.com");

    // Add WebView to the window
    gtk_container_add(GTK_CONTAINER(window), webview);

    gtk_widget_show_all(window);
}

// Function to create login window
void create_login_window() {
    GtkWidget *grid, *username_label, *username_entry;
    GtkWidget *password_label, *password_entry, *login_button;

    gtk_init(NULL, NULL);

    // Create window
    window_login = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_login), "ERP System - Login");
    gtk_window_set_default_size(GTK_WINDOW(window_login), 400, 200);
    gtk_window_set_resizable(GTK_WINDOW(window_login), FALSE);

#if GTK_CHECK_VERSION(4, 10, 0) // Check for GTK 3.10 or later
    gtk_window_set_flags(GTK_WINDOW(window_login),GDK_WINDOW_STATE_MAXIMIZED ); //GTK_WINDOW_FLAG_NO_MAXIMIZE
#else  // GTK versions older than 3.10
    // ***IMPORTANT***: The following method is less reliable and can have unintended side effects.
    // It's strongly recommended to upgrade to GTK 3.10 or later if possible.

    // Get the window's GtkWindowType (internal structure)
    GtkWindow *gtk_window = GTK_WINDOW(window_login);
    GtkWindowType *window_type = g_object_get_data(G_OBJECT(gtk_window), "window-type");

    if (window_type) {  // Check if window_type data exists
        // Get the decorated property (if available)
        gboolean decorated = TRUE;
        g_object_get(G_OBJECT(gtk_window), "decorated", &decorated, NULL);

        if (decorated) { // Only attempt this if the window is decorated

            // This is a hacky way to remove the maximize button before GTK 3.10.
            // It might not work perfectly on all window managers.
            GList *toplevels = gtk_window_list_toplevels();
            GList *iter = toplevels;

            while (iter) {
                GtkWidget *toplevel = GTK_WIDGET(iter->data);
                if (toplevel == window_login) {
                    // Remove the maximize button (this is window manager specific, may not always work)
                    // This is only a request, window manager may ignore it.
                    gtk_window_set_decorated(gtk_window, FALSE);  // Remove decorations
                    gtk_window_set_decorated(gtk_window, TRUE);   // Re-add decorations (without maximize)

                    break; // Found our window
                }
                iter = iter->next;
            }
            g_list_free(toplevels);
        }
    }
#endif

    g_signal_connect(window_login, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create grid layout
    grid = gtk_grid_new(); //gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window_login), grid);

    GtkWidget *overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(window_login), overlay);

    // Base widget
    GtkWidget *label = gtk_label_new("Background");
    gtk_container_add(GTK_CONTAINER(overlay), label);

    username_label = gtk_label_new("Username:");
    username_entry = gtk_entry_new();  
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 15, 2, 1);  
    gtk_grid_attach(GTK_GRID(grid), username_entry, 16, 15, 2, 1);    
    // Password label and entry
    password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 25, 2, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 16, 25, 2, 1);

    // Login button
    login_button = gtk_button_new_with_label("Login");
    gtk_grid_attach(GTK_GRID(grid), login_button, 16, 35, 2, 1);
    g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);
    
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), grid);
    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    //GtkWidget *fixed = gtk_fixed_new();
    //gtk_container_add(GTK_CONTAINER(window_login), fixed);

    //gtk_fixed_put(GTK_FIXED(fixed), username_label, 100, 100);
    //gtk_fixed_put(GTK_FIXED(fixed), username_entry, 100, 100);
    //gtk_fixed_put(GTK_FIXED(fixed), password_label, 100, 100);
    //gtk_fixed_put(GTK_FIXED(fixed), password_entry, 100, 100);
    //gtk_fixed_put(GTK_FIXED(fixed), login_button, 100, 100);

    //gtk_table_attach(GTK_TABLE(table), username_label, 10, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 50, 50); // In top-left cell with margins
    //gtk_table_attach(GTK_TABLE(table), username_entry, 10, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 50, 50); // In top-left cell with margins
    //gtk_table_attach(GTK_TABLE(table), password_label, 10, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 50, 50); // In top-left cell with margins
    //gtk_table_attach(GTK_TABLE(table), password_entry, 10, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 50, 50); // In top-left cell with margins
    //gtk_table_attach(GTK_TABLE(table), login_button, 10, 1, 0, 1, GTK_SHRINK, GTK_SHRINK, 50, 50); // In top-left cell with margins
    // Add widgets to grid
    //gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), login_button, 1, 2, 1, 1);
    
    //GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5); // 5 pixels spacing
    //gtk_container_add(GTK_CONTAINER(window_login), vbox);
    //gtk_box_pack_start(GTK_BOX(vbox), username_label, FALSE, FALSE, 0); // Don't expand, no padding
    //gtk_box_pack_start(GTK_BOX(vbox), username_entry, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(vbox), password_label, FALSE, FALSE, 0); // Don't expand, no padding
    //gtk_box_pack_start(GTK_BOX(vbox), password_entry, FALSE, FALSE, 0);
    //gtk_box_pack_start(GTK_BOX(vbox), login_button, FALSE, FALSE, 0); // Don't expand, no padding

    gtk_widget_show_all(window_login);
    gtk_main();
}

void create_main_window() {
    GtkWidget *grid, *username_label, *username_entry;
    GtkWidget *password_label, *password_entry, *login_button;
    GtkWidget *search_label, *search_entry, *search_button;

    GtkWidget *order_label, *order_entry;
    GtkWidget *name_label, *name_entry;
    GtkWidget *code_label, *code_entry;
    GtkWidget *hash_label, *hash_entry;
    GtkWidget *detail_label, *detail_entry;
    GtkWidget *date_label, *date_entry;

    GtkWidget *edit_button,*post_button,*scan_button;

    gtk_init(NULL, NULL);

    // Create window
    window_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window_main), "ERP System - Main");
    gtk_window_set_default_size(GTK_WINDOW(window_main), 1024, 768);
    
    gtk_window_set_resizable(GTK_WINDOW(window_main), FALSE);

#if GTK_CHECK_VERSION(4, 10, 0) // Check for GTK 3.10 or later
    gtk_window_set_flags(GTK_WINDOW(window_main),GDK_WINDOW_STATE_MAXIMIZED ); //GTK_WINDOW_FLAG_NO_MAXIMIZE
#else  // GTK versions older than 3.10
    // ***IMPORTANT***: The following method is less reliable and can have unintended side effects.
    // It's strongly recommended to upgrade to GTK 3.10 or later if possible.

    // Get the window's GtkWindowType (internal structure)
    GtkWindow *gtk_window = GTK_WINDOW(window_main);
    GtkWindowType *window_type = g_object_get_data(G_OBJECT(gtk_window), "window-type");

    if (window_type) {  // Check if window_type data exists
        // Get the decorated property (if available)
        gboolean decorated = TRUE;
        g_object_get(G_OBJECT(gtk_window), "decorated", &decorated, NULL);

        if (decorated) { // Only attempt this if the window is decorated

            // This is a hacky way to remove the maximize button before GTK 3.10.
            // It might not work perfectly on all window managers.
            GList *toplevels = gtk_window_list_toplevels();
            GList *iter = toplevels;

            while (iter) {
                GtkWidget *toplevel = GTK_WIDGET(iter->data);
                if (toplevel == window_main) {
                    // Remove the maximize button (this is window manager specific, may not always work)
                    // This is only a request, window manager may ignore it.
                    gtk_window_set_decorated(gtk_window, FALSE);  // Remove decorations
                    gtk_window_set_decorated(gtk_window, TRUE);   // Re-add decorations (without maximize)

                    break; // Found our window
                }
                iter = iter->next;
            }
            g_list_free(toplevels);
        }
    }
#endif

    g_signal_connect(window_main, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create grid layout
    grid = gtk_grid_new();
    gtk_window_set_default_size(GTK_WINDOW(grid), 1024, 768);
    gtk_container_add(GTK_CONTAINER(window_main), grid);
    
    // Create a notebook widget
    GtkWidget *notebook = gtk_notebook_new();
    gtk_container_add(GTK_CONTAINER(window_main), notebook);

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


    GtkWidget *overlay = gtk_overlay_new();
    gtk_container_add(GTK_CONTAINER(page1), overlay);

    // Base widget
    GtkWidget *label = gtk_label_new("Background");
    gtk_container_add(GTK_CONTAINER(overlay), label);

    // Username label and entry
    search_label = gtk_label_new("Search:");
    search_entry = gtk_entry_new();

    username_label = gtk_label_new("Username:");
    username_entry = gtk_entry_new();

    // Password label and entry
    password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    order_label = gtk_label_new("Order:");
    order_entry = gtk_entry_new();

    name_label = gtk_label_new("Name:");
    name_entry = gtk_entry_new();

    code_label = gtk_label_new("Code:");
    code_entry = gtk_entry_new();

    hash_label = gtk_label_new("Hash:");
    hash_entry = gtk_entry_new();

    detail_label = gtk_label_new("Detail:");
    detail_entry = gtk_entry_new();

    date_label = gtk_label_new("Date:");
    date_entry = gtk_entry_new();
    // Login button
    login_button = gtk_button_new_with_label("Login");
    g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);

    search_button = gtk_button_new_with_label("Search");
    g_object_set_data(G_OBJECT(search_button), "search_entry", search_entry);
    g_signal_connect(search_button, "clicked", G_CALLBACK(on_search_clicked), NULL);

    edit_button = gtk_button_new_with_label("Edit");
    g_object_set_data(G_OBJECT(edit_button), "search_entry", search_entry);
    g_signal_connect(edit_button, "clicked", G_CALLBACK(on_search_clicked), NULL);

    post_button = gtk_button_new_with_label("Post");
    g_object_set_data(G_OBJECT(post_button), "search_entry", search_entry);
    g_signal_connect(post_button, "clicked", G_CALLBACK(on_search_clicked), NULL);

    scan_button = gtk_button_new_with_label("Scan");
    g_object_set_data(G_OBJECT(scan_button), "search_entry", search_entry);
    g_signal_connect(scan_button, "clicked", G_CALLBACK(on_search_clicked), NULL);

    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), search_label, 0, 0, 2, 1);  
    gtk_grid_attach(GTK_GRID(grid), search_entry, 16, 0, 100, 1);
    gtk_grid_attach(GTK_GRID(grid), search_button, 200, 0, 2, 1);

    //gtk_grid_attach(GTK_GRID(grid), username_label, 0, 35, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), username_entry, 16, 35, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), password_label, 0, 45, 1, 1);
    //gtk_grid_attach(GTK_GRID(grid), password_entry, 16, 45, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), order_label, 0, 45, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), order_entry, 16, 45, 100, 1);

    gtk_grid_attach(GTK_GRID(grid), name_label, 0, 65, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), name_entry, 16, 65, 100, 1);
    gtk_grid_attach(GTK_GRID(grid), code_label, 0, 85, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), code_entry, 16, 85, 100, 1);
    gtk_grid_attach(GTK_GRID(grid), hash_label, 0, 105, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), hash_entry, 16, 105, 100, 1);
    gtk_grid_attach(GTK_GRID(grid), detail_label, 0, 125, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), detail_entry, 16, 125, 100, 1);
    gtk_grid_attach(GTK_GRID(grid), date_label, 0, 145, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), date_entry, 16, 145, 100, 1);

    gtk_grid_attach(GTK_GRID(grid), edit_button, 16, 165, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), post_button, 32, 165, 1, 1);    
    gtk_grid_attach(GTK_GRID(grid), scan_button, 52, 165, 1, 1);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), grid);

    //gtk_overlay_add_overlay(GTK_NOTEBOOK(page1), grid);

    gtk_widget_set_halign(grid, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(grid, GTK_ALIGN_CENTER);

    gtk_widget_show_all(window_main);
    gtk_main();
}

int main(int argc, char *argv[]) {
    create_login_window();
    return 0;
}

