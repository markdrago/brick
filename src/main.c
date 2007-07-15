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
#include "ui.h"

int main(int argc, char *argv[])
{
  model_init();
  ui_init(argc, argv);
  ui_main_loop();

  return 0;
}
