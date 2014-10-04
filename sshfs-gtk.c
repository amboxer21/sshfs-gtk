/* 
   Compile me with:
     gcc -Wall -o text text.c `pkg-config --cflags --libs gtk+-2.0`
*/
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <errno.h>

static void destroy( GtkWidget *widget, gpointer data ) {

	gtk_main_quit();
}

typedef struct {
const char *get_host_entry_text;
const wchar_t *get_local_mount_point_entry_text;
const char *get_ip_entry_text;
const wchar_t *get_remote_folder_entry_text;

FILE *fileERR;
GtkTextBuffer *buffer;
} data_struct;
 
static void Connect(GtkWidget *widget, data_struct *data) {

//data->get_ip_entry_text = (const char *)data->get_ip_entry_text;

	if( (strcmp(data->get_host_entry_text, "HOST NAME") == 0 ) || (data->get_host_entry_text == NULL) ) {

		g_print("\nPlease enter a host name.\n\n");
		gtk_text_buffer_set_text(data->buffer, "Please enter a host name.\n", -1);

	}

		else if( (strcmp(data->get_ip_entry_text, "IP ADDRESS") == 0 ) || (data->get_ip_entry_text == NULL) ) {

			g_print("\nPlease enter an ip address.\n\n");
			gtk_text_buffer_set_text(data->buffer, "Please enter an ip address.\n", -1);

		}


		else if( (wcscmp(data->get_remote_folder_entry_text, (const wchar_t *)"REMOTE FOLDER") == 0 ) || (data->get_remote_folder_entry_text == NULL) ) {

			g_print("\nPlease specify a remote location to mount.\n\n");
			gtk_text_buffer_set_text(data->buffer, "\nPlease specify a remote location to mount.\n", -1);

		}

		else if( (wcscmp(data->get_local_mount_point_entry_text, (const wchar_t *)"LOCAL MOUNT POINT") == 0 ) || (data->get_local_mount_point_entry_text == NULL) ) {

			g_print("\nPlease specify a local mount point.\n\n");
			gtk_text_buffer_set_text(data->buffer, "\nPlease specify a local mount point.\n", -1);

		}


		else {

			char *p;
           		int size = 1500, s_error;

			p = malloc(size);

			snprintf(p, size, "%s@%s:%S", data->get_host_entry_text, data->get_ip_entry_text, data->get_remote_folder_entry_text);

			//g_print("%s", p);

			execl("/usr/bin/sshfs", "sshfs", p, data->get_local_mount_point_entry_text, NULL );

			free(p);

		}
}

int main(int argc, char *argv[]) {

gtk_init(&argc, &argv);


GtkWidget *view;
GtkWidget *table;
GtkWidget *window;
GtkWidget *ip_entry;
GtkWidget *host_entry;
GtkWidget *windowDialog;
GtkWidget *cancel_button;
GtkWidget *connect_button;
GtkWidget *ip_entry_label;
GtkWidget *scrolled_window;
GtkWidget *remote_folder_entry;
GtkWidget *local_mount_point_entry;

gchar *utf8;
gsize length;
gchar *array;
GtkTextIter iter;
//GtkTextBuffer *buffer;

GError *err = NULL;
gsize *bytes_read = NULL; 
gsize *bytes_written = NULL;
	
data_struct data;

data.fileERR = fopen("/var/log/sshfs-gtk_err.log", "w+");

g_file_get_contents("/var/log/sshfs-gtk_err.log", &array, &length, NULL);
//g_file_get_contents("/home/anthony/Documents/Bash/connMann/conn_man", &array, &length, NULL);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "SSHFS-GTK");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);
	gtk_widget_show(window);

windowDialog = gtk_window_new(GTK_WINDOW_POPUP);
	gtk_window_set_title(GTK_WINDOW(window), "SSHFS-GTK_ERROR");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);
	//gtk_widget_show(windowDialog);

//Right 2 = columns. Left 2 = rows
table = gtk_table_new(13, 10, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show(table);

connect_button = gtk_button_new_with_label("Connect");
	gtk_table_attach_defaults(GTK_TABLE(table), connect_button, 1, 3, 6, 7);
	gtk_widget_show(connect_button);

cancel_button = gtk_button_new_with_label("Cancel");
	gtk_table_attach_defaults(GTK_TABLE(table), cancel_button, 7, 9, 6, 7);
	gtk_widget_show(cancel_button);

host_entry = gtk_entry_new( );
	gtk_entry_set_text( GTK_ENTRY(host_entry), "HOST NAME" );
	gtk_table_attach_defaults(GTK_TABLE(table), host_entry, 1, 4, 1, 2);
	gtk_widget_show(host_entry);

ip_entry_label = gtk_label_new("@");
	gtk_table_attach_defaults(GTK_TABLE(table), ip_entry_label, 4, 5, 1, 2);
	gtk_widget_show(ip_entry_label);

ip_entry = gtk_entry_new();
	gtk_entry_set_text( GTK_ENTRY(ip_entry), "IP ADDRESS" );
	gtk_table_attach_defaults(GTK_TABLE(table), ip_entry, 5, 9, 1, 2);
	gtk_widget_show(ip_entry);

remote_folder_entry = gtk_entry_new();
	gtk_entry_set_text( GTK_ENTRY(remote_folder_entry), "REMOTE FOLDER" );
	gtk_table_attach_defaults(GTK_TABLE(table), remote_folder_entry, 1, 4, 3, 5);
	gtk_widget_show(remote_folder_entry);

local_mount_point_entry = gtk_entry_new();
	gtk_entry_set_text( GTK_ENTRY(local_mount_point_entry), "LOCAL MOUNT POINT" );
	gtk_table_attach_defaults(GTK_TABLE(table), local_mount_point_entry, 5, 9, 3, 5);
	gtk_widget_show(local_mount_point_entry);

view = gtk_text_view_new( );
data.buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));      
utf8 = g_convert(array, length, "UTF-8", "us-ascii", bytes_read, bytes_written, &err);

	if (err != NULL) {

		printf("G_CONVERT(!NULL) error --> %s \n", strerror(errno));

		g_error_free(err);

	}

	gtk_text_buffer_get_iter_at_offset(data.buffer, &iter, 0);
	gtk_text_buffer_insert(data.buffer, &iter, utf8, -1);
	//gtk_text_buffer_set_text(buffer, utf8, -1);
	gtk_widget_show(view);

scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);	
	gtk_table_attach_defaults(GTK_TABLE(table), scrolled_window, 1, 9, 8, 13);
	gtk_widget_show(scrolled_window);

	// Struct population
	data.get_ip_entry_text = gtk_entry_get_text(GTK_ENTRY(ip_entry));

	data.get_host_entry_text = gtk_entry_get_text(GTK_ENTRY(host_entry));
	
	data.get_remote_folder_entry_text = (const wchar_t *)gtk_entry_get_text(GTK_ENTRY(remote_folder_entry));

	data.get_local_mount_point_entry_text = (const wchar_t *)gtk_entry_get_text(GTK_ENTRY(local_mount_point_entry));

	// Signal handlers
	g_signal_connect(G_OBJECT(connect_button), "clicked", G_CALLBACK(Connect), &data);

	g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(destroy), NULL );

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);	

gtk_main();

return 0;
}
