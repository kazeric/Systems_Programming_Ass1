#include <gtk/gtk.h>
#include <stdio.h>

#define HISTORY_SIZE 60
#define UPDATE_INTERVAL_MS 1000

// Data structure to hold CPU usage history
typedef struct {
    double values[HISTORY_SIZE];
    int index;
} CPUUsageData;

static CPUUsageData cpu_data = { {0}, 0 };
static GtkWidget *drawing_area;

gboolean update_cpu_usage(gpointer data) {
    FILE *fp;
    char buffer[128];
    double cpu_usage;

    // Read CPU usage from /proc/stat
    fp = fopen("/proc/stat", "r");
    if (fp) {
        fgets(buffer, sizeof(buffer), fp);
        sscanf(buffer, "cpu %*u %*u %*u %*u %*u %*u %*u %*u %*u %*u %lf", &cpu_usage);
        fclose(fp);

        // Add the new value to the history and update the index
        cpu_data.values[cpu_data.index] = cpu_usage;
        cpu_data.index = (cpu_data.index + 1) % HISTORY_SIZE;

        // Redraw the graph
        gtk_widget_queue_draw(drawing_area);

        // Print the real-time CPU usage
        printf("Real-time CPU Usage: %.2f%%\n", cpu_usage);
    }

    return G_SOURCE_CONTINUE;
}

static gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    // Draw a simple CPU usage graph
    int i;
    int width = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
    cairo_set_line_width(cr, 2);

    double step = (double)width / HISTORY_SIZE;
    double x = 0;

    for (i = 0; i < HISTORY_SIZE; i++) {
        double y = height - (height * cpu_data.values[(cpu_data.index + i) % HISTORY_SIZE]);
        if (i == 0) {
            cairo_move_to(cr, x, y);
        } else {
            cairo_line_to(cr, x, y);
        }
        x += step;
    }

    cairo_stroke(cr);

    return FALSE;
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *vbox;

    // Initialize GTK
    gtk_init(&argc, &argv);

    // Create the main window
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CPU Usage Graph");
    gtk_window_set_default_size(GTK_WINDOW(window), 800, 400);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Create a drawing area for the graph
    drawing_area = gtk_drawing_area_new();
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), NULL);

    // Create a vertical box to contain the drawing area
    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);
    gtk_box_pack_start(GTK_BOX(vbox), drawing_area, TRUE, TRUE, 0);

    gtk_widget_show_all(window);

    // Start the CPU usage update timer
    g_timeout_add(UPDATE_INTERVAL_MS, update_cpu_usage, NULL);

    // Run the GTK main loop
    gtk_main();

    return 0;
}
