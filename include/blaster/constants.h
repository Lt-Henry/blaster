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

/*! data types */
#define BL_U8    0x10
#define BL_U16    0x21
#define BL_U32    0x42
#define BL_U64    0x83
#define BL_F32    0x44
#define BL_F64    0x85

/*! texture types */
#define BL_TEXTURE_U8    0
#define BL_TEXTURE_U16   1
#define BL_TEXTURE_U32   2
#define BL_TEXTURE_F32    3
#define BL_TEXTURE_F64    4

/*! tile size */
#define BL_TILE_SIZE    64

/*! command queue size */
#define BL_CMD_SIZE	1024

/*! commands */
#define BL_CMD_NONE     0
#define BL_CMD_QUIT     1
#define BL_CMD_CLEAR    2 /*! clear */
#define BL_CMD_DRAW     3 /*! render */
#define BL_CMD_UPDATE   4 /*! update fragments */
#define BL_CMD_SYNC     5

/*! vbo types */
#define BL_VBO_POINTS       1
#define BL_VBO_LINES        2
#define BL_VBO_TRIANGLES    3

/*! vbo attribute types */
#define BL_VBO_BYTE	0
#define BL_VBO_FLOAT	1
#define BL_VBO_INT32	2

/*! vbo attribute settings */
#define BL_VBO_IGNORE	1
#define BL_VBO_LINEAR	2	/*! linear interpolation instead of perspective corrected */


#endif
