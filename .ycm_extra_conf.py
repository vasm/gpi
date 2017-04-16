import os
import ycm_core
from clang_helpers import PrepareClangFlags


# These are the compilation flags that will be used in case there's no
# compilation database set.
flags = [
'-D__AVR_ATtiny24__',
# THIS IS IMPORTANT! Without a "-std=<something>" flag, clang won't know which
# language to use when compiling headers. So it will guess. Badly. So C++
# headers will be compiled as C headers. You don't want that so ALWAYS specify
# a "-std=<something>".
# For a C project, you would set this to something like 'c99' instead of
# 'c++11'.
'-std=c99',
# ...and the same thing goes for the magic -x option which specifies the
# language that the files to be compiled are written in. This is mostly
# relevant for c++ headers.
# For a C project, you would set this to 'c' instead of 'c++'.
'-I',
'/usr/local/Cellar/avr-gcc/6.2.0/avr/include/'
'-I',
'/usr/local/Cellar/avr-gcc/6.2.0/avr/include/'
'-I',
'/usr/local/Cellar/avr-gcc/6.2.0/avr/'
'-DARCH=ARCH_ATTINY',
'-DBOARD=BOARD_NONE',
'-DNO_STREAM_CALLBACKS',
'-DINTERRUPT_CONTROL_ENDPOINT',
'-DUSE_FLASH_DESCRIPTORS',
'-DUSE_STATIC_OPTIONS="(USB_DEVICE_OPT_FULLSPEED | USB_OPT_PLLCLKSRC)"',
'-DFIXED_CONTROL_ENDPOINT_SIZE=32',
'-DFIXED_NUM_CONFIGURATIONS=1',
'-DMPU9150',
'-Os',
'-funsigned-char',
'-fpack-struct',
'-fshort-enums',
'-Wall',
'-Wstrict-prototypes',
'-I',
'.',
'-I', 'include',
'-I.',
# This path is for AVR gcc. 
]


def FlagsForFile( filename ):
    return {'flags': flags}

