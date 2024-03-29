COMPILER = clang++
LANGUAGE = cpp

SOURCES = main logger/logger proxy/proxy proxy/exception proxy/socket utils/rwbuffer proxy/sockmanager proxy/buffsocket http/httpheader http/httpmethod http/httprequest
LIBS = 
SUBDIRS = logger proxy utils http
MKSUBDIRS = $(addprefix $(OBJPATH)/, $(SUBDIRS))

SRCPATH = ./src
BINPATH = ./bin
OBJPATH = ./bin/obj
DBGSUFF = _DBG.o
OPTSUFF = _OPT.o
DBGFLGS = -Wall -g
OPTFLGS = -Ofast
DBGBINP = $(BINPATH)/debug
OPTBINP = $(BINPATH)/optimized

DEPENDS = $(addprefix $(SRCPATH)/, $(addsuffix $(LANGUAGE), $(SOURCES)) $(addsuffix .h, $(SOURCES)))
DBGOBJ = $(addprefix $(OBJPATH)/, $(addsuffix $(DBGSUFF), $(SOURCES)))
OPTOBJ = $(addprefix $(OBJPATH)/, $(addsuffix $(OPTSUFF), $(SOURCES)))

.DEFAULT_GOAL := debug
.PHONY: debug, run, clean, debug-compile, optimized-compile

$(OBJPATH)/%$(DBGSUFF): $(SRCPATH)/%.$(LANGUAGE) $(SRCPATH)/%.h
	$(COMPILER) $(DBGFLGS) $(LIBS) -c -o $@ $<

$(OBJPATH)/%$(OPTSUFF): $(SRCPATH)/%.$(LANGUAGE) $(SRCPATH)/%.h
	$(COMPILER) $(OPTFLGS) $(LIBS) -c -o $@ $<

debug-compile: $(DBGOBJ)
	$(COMPILER) $(DBGFLGS) $(LIBS) -o $(DBGBINP) $^

optimized-compile: $(OPTOBJ)
	$(COMPILER) $(OPTFLGS) $(LIBS) -o $(OPTBINP) $^

debug: debug-compile
	$(DBGBINP)

run: optimized-compile
	$(OPTBINP)

clean:
	rm -r ./bin
	mkdir $(BINPATH) $(OBJPATH) $(MKSUBDIRS)