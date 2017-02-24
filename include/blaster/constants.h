/*
	blaster
	
	Copyright (C) 2016  Enrique Medina Gremaldos <quiqueiii@gmail.com>

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BLASTER_CONSTANTS
#define BLASTER_CONSTANTS

/*! tile size */
#define BL_TILE_SIZE	64

/*! command queue size */
#define BL_CMD_SIZE	1024

/*! commands */
#define BL_CMD_NONE	0
#define BL_CMD_QUIT	1
#define BL_CMD_CLEAR	2 /*! clear tile */
#define BL_CMD_DRAW	3	/*! render tile */
#define BL_CMD_UPDATE	4 /*! render all tiles */
#define BL_CMD_TEXTURE	5 /*! bind texture */
#define BL_CMD_VBO	6 /*! process vbo */
#define BL_CMD_CLEAR_COLOR	7 /*! sets clear color */

/*! vbo types */
#define BL_VBO_POINTS	1
#define BL_VBO_LINES	2
#define BL_VBO_TRIANGLES	3

/*! vbo attribute types */
#define BL_VBO_BYTE	0
#define BL_VBO_FLOAT	1
#define BL_VBO_INT32	2

/*! vbo attribute settings */
#define BL_VBO_IGNORE	1
#define BL_VBO_LINEAR	2	/*! linear interpolation instead of perspective corrected */


#endif
