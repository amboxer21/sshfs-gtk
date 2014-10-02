/* 
   Compile me with:
     gcc -Wall -o text text.c `pkg-config --cflags --libs gtk+-2.0`
*/
#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>

static void destroy( GtkWidget *widget, gpointer data ) {

	gtk_main_quit();
}

typedef struct {
const char *get_host_entry_text;
const char *get_remote_folder_entry_text;
const char *get_local_mount_point_entry_text;
const char *get_ip_entry_text;
} data_struct;
 
static void Connect(GtkWidget *widget, data_struct *data) {

//data->get_ip_entry_text = (const char *)data->get_ip_entry_text;

	if( strcmp(data->get_host_entry_text, "HOST NAME") == 0 ) {

		g_print("\nPlease enter a host name.\n\n");

	}

		else if(strcmp(data->get_ip_entry_text, "IP ADDRESS") == 0 ) {

			g_print("\nPlease enter an ip address.\n\n");

		}

		else if(strcmp(data->get_remote_folder_entry_text, "REMOTE FOLDER") == 0 ) {

			g_print("\nPlease specify a remote location to mount.\n\n");

		}

		else if(strcmp(data->get_local_mount_point_entry_text, "LOCAL MOUNT POINT") == 0 ) {

			g_print("\nPlease specify a local mount point.\n\n");

		}


		else {

			char *p;
           		int size = 250;

           		p = malloc(size);

			snprintf(p, size, "%s@%s:%s", data->get_host_entry_text, data->get_ip_entry_text, data->get_remote_folder_entry_text);

			execl("/usr/bin/sshfs", "sshfs", p, data->get_local_mount_point_entry_text, (const char *)NULL );


		}
}

int main(int argc, char *argv[]) {

gtk_init(&argc, &argv);

data_struct data;

GtkWidget *window;
GtkWidget *table;
GtkWidget *connect_button;
GtkWidget *cancel_button;
GtkWidget *ip_entry;
GtkWidget *host_entry;
GtkWidget *ip_entry_label;
GtkWidget *remote_folder_entry;
GtkWidget *local_mount_point_entry;

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "SSHFS-GTK");
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(window), 100, 100);
	gtk_widget_show(window);

table = gtk_table_new(10, 10, TRUE);
	gtk_container_add(GTK_CONTAINER(window), table);
	gtk_widget_show(table);

connect_button = gtk_button_new_with_label("Connect");
	gtk_table_attach_defaults(GTK_TABLE(table), connect_button, 1, 3, 8, 10);
	gtk_widget_show(connect_button);

cancel_button = gtk_button_new_with_label("Cancel");
	gtk_table_attach_defaults(GTK_TABLE(table), cancel_button, 7, 9, 8, 10);
	gtk_widget_show(cancel_button);

host_entry = gtk_entry_new( );
	gtk_entry_set_text( GTK_ENTRY(host_entry), "HOST NAME" );
	gtk_table_attach_defaults(GTK_TABLE(table), host_entry, 1, 4, 1, 3);
	gtk_widget_show(host_entry);

ip_entry_label = gtk_label_new("@");
	gtk_table_attach_defaults(GTK_TABLE(table), ip_entry_label, 4, 5, 1, 3);
	gtk_widget_show(ip_entry_label);

ip_entry = gtk_entry_new();
	gtk_entry_set_text( GTK_ENTRY(ip_entry), "IP ADDRESS" );
	gtk_table_attach_defaults(GTK_TABLE(table), ip_entry, 5, 9, 1, 3);
	gtk_widget_show(ip_entry);

remote_folder_entry = gtk_entry_new();
	gtk_entry_set_text( GTK_ENTRY(remote_folder_entry), "REMOTE FOLDER" );
	gtk_table_attach_defaults(GTK_TABLE(table), remote_folder_entry, 1, 4, 4, 6);
	gtk_widget_show(remote_folder_entry);

local_mount_point_entry = gtk_entry_new();
	gtk_entry_set_text( GTK_ENTRY(local_mount_point_entry), "LOCAL MOUNT POINT" );
	gtk_table_attach_defaults(GTK_TABLE(table), local_mount_point_entry, 5, 9, 4, 6);
	gtk_widget_show(local_mount_point_entry);

	data.get_ip_entry_text = gtk_entry_get_text(GTK_ENTRY(ip_entry));

	data.get_host_entry_text = gtk_entry_get_text(GTK_ENTRY(host_entry));
	
	data.get_remote_folder_entry_text = gtk_entry_get_text(GTK_ENTRY(remote_folder_entry));

	data.get_local_mount_point_entry_text = gtk_entry_get_text(GTK_ENTRY(local_mount_point_entry));


	g_signal_connect(G_OBJECT(connect_button), "clicked", G_CALLBACK(Connect), &data);

	//g_signal_connect(G_OBJECT(connect_button), "clicked", G_CALLBACK(Connect), NULL );

	g_signal_connect(G_OBJECT(cancel_button), "clicked", G_CALLBACK(destroy), NULL );

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(destroy), NULL);	

	//g_signal_connect(G_OBJECT(buttonDown), "clicked", G_CALLBACK(vol_down), NULL);

	//g_signal_connect(G_OBJECT(buttonMute), "clicked", G_CALLBACK(vol_mute), NULL);

gtk_main();

return 0;
}
