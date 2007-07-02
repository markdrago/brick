/*
 * Brick - A 'Twenty-One' Score Tracker
 * Copyright (C) 2007 Mark Drago
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "brick.h"

static void setup_ui(HildonWindow *window) {
  GtkWidget *main_hbox;
  GtkListStore *name_list_store;
  GtkTreeIter iter;
  GtkCellRenderer *renderer;
  /*  GtkTreeModel *model; */
  GtkWidget *view;
  GtkWidget *left_label;

  main_hbox = gtk_hbox_new(FALSE, 0);
  name_list_store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_INT);
  view = gtk_tree_view_new();

  /* create some fake data */
  gtk_list_store_append(name_list_store, &iter);
  gtk_list_store_set(name_list_store, &iter, 0, "Mark", 1, 10, -1);
  gtk_list_store_append(name_list_store, &iter);
  gtk_list_store_set(name_list_store, &iter, 0, "Byron", 1, 8, -1);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "Name",
					      renderer, "text", 0, NULL);
  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "Score",
					      renderer, "text", 1, NULL);

  gtk_tree_view_set_model(GTK_TREE_VIEW(view), GTK_TREE_MODEL(name_list_store));
  g_object_unref(name_list_store);

  left_label = gtk_label_new("Test");

  /* pack widgets in to the hbox */
  gtk_box_pack_start(GTK_BOX(main_hbox), left_label, TRUE, TRUE, 2);
  gtk_box_pack_start(GTK_BOX(main_hbox), view, TRUE, TRUE, 2);

  gtk_container_add(GTK_CONTAINER(window), main_hbox);
}

int main(int argc, char *argv[])
{
  HildonProgram *program;
  HildonWindow *window;

  gtk_init(&argc, &argv);

  program = HILDON_PROGRAM(hildon_program_get_instance());
  g_set_application_name("Brick");

  window = HILDON_WINDOW(hildon_window_new());
  hildon_program_add_window(program, window);

  setup_ui(window);

  /* make close work */
  g_signal_connect(G_OBJECT(window), "delete_event",
		   G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show_all(GTK_WIDGET(window));
  gtk_main();

  return 0;
}
