 # Copyright (C) 2014 Daichi Teruya (@maruuusa83)
 #
 # This program is free software; you can redistribute it and/or
 # modify it under the terms of the GNU General Public License
 # as published by the Free Software Foundation; either version 2
 # of the License.
 #
 # This program is destributed in the hope that it will be useful,
 # but WITHOUT ANY WARRANTY; without even the implied warranty of
 # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 # GNU General Public License for more details.
 #
 # You should have received a copy of the GNU General Public License
 # along with this program; if not, write to the Free Software
 # Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.

#-------------------------------------------------------------------
# Makefile for subdir of C/C++ Project
# Author : D.Teruya
#-------------------------------------------------------------------
# This makefile will generate object file(.o files) from each .cpp files.
# It'll not link each object files.
#
#
# This makefile generate "depend.inc" and "objs.inc".
#
# depend.inc :
#   It's a list of header files(.h files) that needed by .cpp files.
# objs.inc :
#   It's a list of object files(.o files) that generated by this Makefile.
#-------------------------------------------------------------------

#-------------------------------------------------------------------
# Default Parameters
#-------------------------------------------------------------------

#-------------------------------------------------------------------
# Compile Option
#-------------------------------------------------------------------
CFLAGS = -Wall -O0 -g -pthread -std=c++11

#-------------------------------------------------------------------
# Definition
#-------------------------------------------------------------------
DIRS = $(shell ls -F |grep / |sed -e "s/\///g")
SRC  = $(shell ls *.cpp)
OBJS = $(SRC:.cpp=.o)

.SUFFIXES: .cpp .o


#-------------------------------------------------------------------
# rules
#-------------------------------------------------------------------
# primary target
.PHONY: all
all: depend gen_obj gen_list make_subdir allobj.lst

.PHONY: gen_obj
gen_obj: $(OBJS)
.cpp.o:
	$(CC) $(CFLAGS) -c $<


.PHONY: make_subdir
make_subdir:
	-@ for i in $(DIRS); do make -C ./$$i; done

# delete files target
.PHONY: clean
clean:
	-@ for i in $(DIRS); do make -C ./$$i clean; done
	$(RM) $(PROGRAM) $(OBJS)
	$(RM) depend.inc objs.lst allobj.lst


# header files
.PHONY: depend
depend: $(OBJS:.o=.cpp)
	-@ $(RM) depend.inc
	-@ for i in $^; do cpp -MM $$i | sed "s/\ [_a-zA-Z0-9][_a-zA-Z0-9]*\.cpp//g" >> depend.inc; done

# generated files
.PHONY: gen_list
gen_list:
	-@ find `pwd` -maxdepth 1 -mindepth 1 |grep \\.o > objs.lst

allobj.lst:
	-@ $(RM) allobj.lst
	-@ cat ./objs.lst >> ./allobj.lst
	-@ for i in $(DIRS); do cat ./$$i/allobj.lst >> ./allobj.lst; done
	-@ echo generated allobj.lst

-include depend.inc
