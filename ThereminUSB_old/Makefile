HID_PATH = hidapi-0.7.0
HID_INC_PATH = $(HID_PATH)/hidapi
HID_OBJ_PATH = $(HID_PATH)/windows

HID_OBJ_NAME = hid.o
HID_OBJ = $(HID_OBJ_PATH)/$(HID_OBJ_NAME)
HID_MAKEFILE = Makefile.mingw

#QT_DIR = C:/Qt/Qt5.4.1/5.4/mingw491_32
#QT_INCLUDE = -I$(QT_DIR)/include
#QT_LIB_DIR = $(QT_DIR)/lib
#QT_LIBS = $(basename $(notdir $(wildcard $(QT_LIB_DIR)/*.a)))
#QT_LIBS = Qt5Cored Qt5Guid Qt5Quickd Qt5Widgetsd
#QT_LINKS = $(addprefix -l, $(QT_LIBS))

EXECUTABLE = ThereminUSB.exe

LIBS= -lsetupapi

.PHONY : all

all : $(EXECUTABLE)

clean :
	rm -f $(EXECUTABLE)
	rm -f $(HID_OBJ)

$(HID_OBJ) : 
	make -f $(HID_MAKEFILE) -C $(HID_OBJ_PATH) $(HID_OBJ_NAME)
	
$(EXECUTABLE) : $(HID_OBJ) *.cpp
	g++ $(HID_OBJ) *.cpp -I$(HID_INC_PATH) $(LIBS) -o $@