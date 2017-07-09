CC := arm-vita-eabi-gcc
CXX := arm-vita-eabi-g++
STRIP := arm-vita-eabi-strip
LD := arm-vita-eabi-ld

PROJECT_TITLE := VitaGXM sample
PROJECT_TITLEID := VGXM00001

PROJECT := vita_gxm
CFLAGS := -O3
CXXFLAGS := $(CFLAGS) -std=c++11

LIBS := -lSceLibKernel_stub -lSceDisplay_stub -lSceGxm_stub -lSceSysmodule_stub -lSceCtrl_stub -lSceCommonDialog_stub -lm -lc -lstdc++

SHADERS := shaders/clear_f.o shaders/clear_v.o shaders/vertexShader.o shaders/fragmentShader.o
OBJS := RenderEngine/utils.o RenderEngine/DisplayManager.o models/RawModel.o models/TexturedModel.o RenderEngine/Loader.o  \
		RenderEngine/Renderer.o shaders/ShaderProgram.o shaders/StaticShader.o textures/ModelTexture.o toolbox/Maths.o toolbox/math_utils.o \
		entities/Entity.o main.o entities/Camera.o RenderEngine/ObjLoader.o $(SHADERS)

all: package

package: $(PROJECT).vpk

$(PROJECT).vpk: eboot.bin param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin $(PROJECT).vpk
	
eboot.bin: $(PROJECT).velf
	vita-make-fself $(PROJECT).velf eboot.bin

param.sfo:
	vita-mksfoex -s TITLE_ID="$(PROJECT_TITLEID)" "$(PROJECT_TITLE)" param.sfo

$(PROJECT).velf: $(PROJECT).elf
	$(STRIP) -g $<
	vita-elf-create $< $@

$(PROJECT).elf: $(OBJS)
	$(CXX) -Wl,-q $(CXXFLAGS)  -o $@ $^ $(LIBS)

%.o : %.cpp
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.o : %.c
	$(CXX) -c $(CXXFLAGS) -o $@ $<

%.o : %.gxp
	$(LD) -r -b binary -o $@ $^

clean:
	rm -f $(PROJECT).velf $(PROJECT).elf $(PROJECT).vpk param.sfo eboot.bin $(OBJS)
