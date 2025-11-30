#include <gtk/gtk.h>
#include <libsoup/soup.h>

// Function to send HTTP POST request to the server
char* send_post_request(const char *url, const char *payload) {
    SoupSession *session = soup_session_new();
    SoupMessage *msg = soup_message_new("POST", url);
    soup_message_set_request(msg, "application/json", SOUP_MEMORY_COPY, payload, strlen(payload));

    // Send request and wait for response
    soup_session_send_message(session, msg);

    if (msg->status_code == SOUP_STATUS_OK) {
        return g_strdup(msg->response_body->data);
    } else {
        g_print("HTTP Error: %d\n", msg->status_code);
        return NULL;
    }
}

// Callback for login button
void on_login_clicked(GtkWidget *button, gpointer user_data) {
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
        g_free(response);
    }

    g_free(payload);
}

// Function to create login window
void create_login_window() {
    GtkWidget *window, *grid, *username_label, *username_entry;
    GtkWidget *password_label, *password_entry, *login_button;

    gtk_init(NULL, NULL);

    // Create window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "ERP System - Login");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create grid layout
    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    // Username label and entry
    username_label = gtk_label_new("Username:");
    username_entry = gtk_entry_new();

    // Password label and entry
    password_label = gtk_label_new("Password:");
    password_entry = gtk_entry_new();
    gtk_entry_set_visibility(GTK_ENTRY(password_entry), FALSE);

    // Login button
    login_button = gtk_button_new_with_label("Login");
    g_object_set_data(G_OBJECT(login_button), "username_entry", username_entry);
    g_object_set_data(G_OBJECT(login_button), "password_entry", password_entry);
    g_signal_connect(login_button, "clicked", G_CALLBACK(on_login_clicked), NULL);

    // Add widgets to grid
    gtk_grid_attach(GTK_GRID(grid), username_label, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), username_entry, 1, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_label, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), password_entry, 1, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), login_button, 1, 2, 1, 1);

    gtk_widget_show_all(window);
    gtk_main();
}

void create_main_window() {

}

int main(int argc, char *argv[]) {
    create_login_window();
    return 0;
}

