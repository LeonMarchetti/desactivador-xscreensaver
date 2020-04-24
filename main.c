#include <gtk/gtk.h>
#include <pthread.h>
#include <stdlib.h>


enum Icon {
    Play = 0,
    Stop
};

GtkWidget* window;
GtkStatusIcon* tray_icon;
enum Icon icon;
pthread_t id_hilo;


void* callback(void* vargp);
static void onButtonRelease(GtkWidget* widget, gpointer data);
static gboolean onPopupMenu(GtkWidget* widget, GdkEvent* event, gpointer data);
static void activate(GtkApplication* app, gpointer user_data);


int main (int argc, char **argv) {
    GtkApplication* app;
    int status;

    icon = Stop;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}

void* callback(void* vargp) {
    while (1) {
        system("xscreensaver-command -deactivate >/dev/null 2>&1");
        g_print("Señal de desactivación de salvapantallas\n");
        sleep(30);
    }
    return NULL;
}

static void onButtonRelease(GtkWidget* widget, gpointer data) {
    switch (icon) {
        case Play:
            g_print("Salvapantallas activado\n");
            icon = Stop;
            gtk_status_icon_set_from_file(tray_icon, "icons/stop.png");
            pthread_cancel(id_hilo);
            break;
        case Stop:
            g_print("Salvapantallas desactivado\n");
            icon = Play;
            gtk_status_icon_set_from_file(tray_icon, "icons/play.png");
            pthread_create(&id_hilo, NULL, callback, NULL);
            break;
    }
}

static gboolean onPopupMenu(GtkWidget* widget, GdkEvent* event, gpointer data) {
    g_print("Terminado\n");
    return FALSE;
}

static void activate(GtkApplication* app, gpointer user_data) {
    g_print("activate\n");
    window = gtk_application_window_new(app);

    // Inicializar ícono en la bandeja:
    tray_icon = gtk_status_icon_new_from_file("icons/stop.png");

    // Eventos:
    g_signal_connect(tray_icon, "popup-menu", G_CALLBACK(onPopupMenu), NULL);
    g_signal_connect(tray_icon, "button-release-event", G_CALLBACK(onButtonRelease), NULL);
    g_signal_connect_swapped(tray_icon, "popup-menu", G_CALLBACK (gtk_widget_destroy), window);

    // Mostrar ícono:
    gtk_status_icon_set_visible(tray_icon, TRUE);
    gtk_status_icon_set_tooltip_text(tray_icon, "Detener");
}
