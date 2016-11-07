# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := libuv
DEFS_Debug := \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-D_GNU_SOURCE' \
	'-DHAVE_CONFIG_H' \
	'-DDEBUG' \
	'-D_DEBUG' \
	'-DEV_VERIFY=2'

# Flags passed to all source files.
CFLAGS_Debug := \
	-Wall \
	-fvisibility=hidden \
	-Wall \
	-fvisibility=hidden \
	-Wall \
	-fvisibility=hidden \
	-Wall \
	-fvisibility=hidden \
	-g \
	--std=gnu89 \
	--std=c++11 \
	-pedantic \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wstrict-aliasing \
	-g \
	-O0 \
	-fwrapv \
	-g \
	-O0 \
	-fwrapv \
	-g \
	-O0 \
	-fwrapv \
	-g \
	-O0 \
	-fwrapv

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-fno-exceptions \
	-fno-rtti \
	-fno-exceptions \
	-fno-rtti \
	-fno-exceptions \
	-fno-rtti \
	-fno-exceptions

INCS_Debug := \
	-I$(srcdir)/include \
	-I$(srcdir)/src \
	-I$(srcdir)/gamenet

DEFS_Release := \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-D_GNU_SOURCE' \
	'-DHAVE_CONFIG_H' \
	'-DNDEBUG'

# Flags passed to all source files.
CFLAGS_Release := \
	-Wall \
	-fvisibility=hidden \
	-Wall \
	-fvisibility=hidden \
	-Wall \
	-fvisibility=hidden \
	-Wall \
	-fvisibility=hidden \
	-g \
	--std=gnu89 \
	--std=c++11 \
	-pedantic \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wstrict-aliasing \
	-O3 \
	-fstrict-aliasing \
	-fomit-frame-pointer \
	-fdata-sections \
	-ffunction-sections \
	-O3 \
	-fstrict-aliasing \
	-fomit-frame-pointer \
	-fdata-sections \
	-ffunction-sections \
	-O3 \
	-fstrict-aliasing \
	-fomit-frame-pointer \
	-fdata-sections \
	-ffunction-sections \
	-O3 \
	-fstrict-aliasing \
	-fomit-frame-pointer \
	-fdata-sections \
	-ffunction-sections

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-fno-exceptions \
	-fno-rtti \
	-fno-exceptions \
	-fno-rtti \
	-fno-exceptions \
	-fno-rtti \
	-fno-exceptions

INCS_Release := \
	-I$(srcdir)/include \
	-I$(srcdir)/src \
	-I$(srcdir)/gamenet

OBJS := \
	$(obj).target/$(TARGET)/src/fs-poll.o \
	$(obj).target/$(TARGET)/src/inet.o \
	$(obj).target/$(TARGET)/src/uv-common.o \
	$(obj).target/$(TARGET)/src/version.o \
	$(obj).target/$(TARGET)/gamenet/stdafx.o \
	$(obj).target/$(TARGET)/gamenet/kcircularbuffer.o \
	$(obj).target/$(TARGET)/gamenet/knetclient.o \
	$(obj).target/$(TARGET)/gamenet/knetserver.o \
	$(obj).target/$(TARGET)/gamenet/ksocketmgr.o \
	$(obj).target/$(TARGET)/gamenet/kuvsocket.o \
	$(obj).target/$(TARGET)/src/unix/async.o \
	$(obj).target/$(TARGET)/src/unix/core.o \
	$(obj).target/$(TARGET)/src/unix/dl.o \
	$(obj).target/$(TARGET)/src/unix/fs.o \
	$(obj).target/$(TARGET)/src/unix/getaddrinfo.o \
	$(obj).target/$(TARGET)/src/unix/loop.o \
	$(obj).target/$(TARGET)/src/unix/loop-watcher.o \
	$(obj).target/$(TARGET)/src/unix/pipe.o \
	$(obj).target/$(TARGET)/src/unix/poll.o \
	$(obj).target/$(TARGET)/src/unix/process.o \
	$(obj).target/$(TARGET)/src/unix/signal.o \
	$(obj).target/$(TARGET)/src/unix/stream.o \
	$(obj).target/$(TARGET)/src/unix/tcp.o \
	$(obj).target/$(TARGET)/src/unix/thread.o \
	$(obj).target/$(TARGET)/src/unix/threadpool.o \
	$(obj).target/$(TARGET)/src/unix/timer.o \
	$(obj).target/$(TARGET)/src/unix/tty.o \
	$(obj).target/$(TARGET)/src/unix/udp.o \
	$(obj).target/$(TARGET)/src/unix/proctitle.o \
	$(obj).target/$(TARGET)/src/unix/linux-core.o \
	$(obj).target/$(TARGET)/src/unix/linux-inotify.o \
	$(obj).target/$(TARGET)/src/unix/linux-syscalls.o \
	$(obj).target/$(TARGET)/src/unix/pthread-fixes.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug :=

LDFLAGS_Release :=

LIBS :=

$(obj).target/libuv.a: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/libuv.a: LIBS := $(LIBS)
$(obj).target/libuv.a: TOOLSET := $(TOOLSET)
$(obj).target/libuv.a: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,alink)

all_deps += $(obj).target/libuv.a
# Add target alias
.PHONY: libuv
libuv: $(obj).target/libuv.a

# Add target alias to "all" target.
.PHONY: all
all: libuv

# Add target alias
.PHONY: libuv
libuv: $(builddir)/libuv.a

# Copy this to the static library output path.
$(builddir)/libuv.a: TOOLSET := $(TOOLSET)
$(builddir)/libuv.a: $(obj).target/libuv.a FORCE_DO_CMD
	$(call do_cmd,copy)

all_deps += $(builddir)/libuv.a
# Short alias for building this static library.
.PHONY: libuv.a
libuv.a: $(obj).target/libuv.a $(builddir)/libuv.a

# Add static library to "all" target.
.PHONY: all
all: $(builddir)/libuv.a

