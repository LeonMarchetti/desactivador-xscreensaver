#include <gtk/gtk.h>
#include <pthread.h>


char* img_iconos[2] = {
    "media-playback-start-symbolic",
    "media-playback-pause-symbolic"
};

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

/**
 * Función que ejecuta el hilo. Ejecuta el comando `xscreensaver-command
 * -deactivate` cada 30 segundos para decirle a xscreensaver que pretenda que
 * hubo actividad y no active el salvapantallas.
 */
void* callback(void* vargp) {
    while (1) {
        system("xscreensaver-command -deactivate >/dev/null 2>&1");
        g_print("Señal de desactivación de salvapantallas\n");
        sleep(30);
    }
    return NULL;
}

/**
 * Función que se ejecuta al soltar el botón izquierdo del mouse sobre el
 * ícono. Alterna el estado del ícono entre ejecutando y detenido. Además
 * crea o cancela el hilo que envía la señal a xscreensaver.
 */
static void onButtonRelease(GtkWidget* widget, gpointer data) {
    switch (icon) {
        case Play:
            g_print("Salvapantallas activado\n");
            icon = Stop;
            pthread_cancel(id_hilo);
            break;
        case Stop:
            g_print("Salvapantallas desactivado\n");
            icon = Play;
            pthread_create(&id_hilo, NULL, callback, NULL);
            break;
    }
    gtk_status_icon_set_from_icon_name(tray_icon, img_iconos[icon]);
}

/**
 * Función que se ejecuta al presionar el botón derecho del mouse.
 */
static gboolean onPopupMenu(GtkWidget* widget, GdkEvent* event, gpointer data) {
    g_print("Terminado\n");
    return FALSE;
}

/**
 * Función que se ejecuta cuando se activa la aplicación. Inicializa el ícono y
 * conecta los eventos de los clicks.
 */
static void activate(GtkApplication* app, gpointer user_data) {
    g_print("Activando ventana\n");
    window = gtk_application_window_new(app);

    // Inicializar ícono en la bandeja:
    tray_icon = gtk_status_icon_new_from_icon_name(img_iconos[icon]);

    // Eventos:
    g_signal_connect(tray_icon, "popup-menu", G_CALLBACK(onPopupMenu), NULL);
    g_signal_connect(tray_icon, "button-release-event", G_CALLBACK(onButtonRelease), NULL);
    g_signal_connect_swapped(tray_icon, "popup-menu", G_CALLBACK (gtk_widget_destroy), window);

    // Mostrar ícono:
    gtk_status_icon_set_visible(tray_icon, TRUE);
    gtk_status_icon_set_tooltip_text(tray_icon, "Detener");
}
