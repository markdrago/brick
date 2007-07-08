#include "ui_gtk.h"

void (*ui_init) (int argc, char *argv[]) = ui_gtk_init;
void (*ui_main_loop) (void) = ui_gtk_main_loop;
