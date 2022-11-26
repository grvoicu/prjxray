N ?= 10
BUILD_DIR = build_$(XRAY_PART)
GENERATE_ARGS ?=
SPECIMENS := $(addprefix $(BUILD_DIR)/specimen_,$(shell seq -f '%03.0f' $(N)))
SPECIMENS_OK := $(addsuffix /OK,$(SPECIMENS))
SPECIMENS_SEGDATA := $(addsuffix /segdata_tilegrid.txt,$(SPECIMENS))

database: $(BUILD_DIR)/segbits_tilegrid.tdb

$(BUILD_DIR)/segbits_tilegrid.tdb: $(SPECIMENS_OK)
	${XRAY_SEGMATCH} -o $(BUILD_DIR)/segbits_tilegrid.tdb $$(find $(BUILD_DIR) -name "segdata_tilegrid.txt")

ifeq (${XRAY_ARCH}, Series7)

$(SPECIMENS_OK):
	GENERATE_ARGS=${GENERATE_ARGS} bash ../fuzzaddr/generate.sh $(subst /OK,,$@)
	touch $@

else
FUZDIR=$(PWD)

$(SPECIMENS_OK): $(SPECIMENS_SEGDATA)
	touch $@

$(BUILD_DIR)/specimen_%/segdata_tilegrid.txt: $(BUILD_DIR)/specimen_%/design.bits
	cd $(subst /segdata_tilegrid.txt,,$@); python3 $(FUZDIR)/../fuzzaddr/generate.py --oneval 1 --design params.csv --dframe ${DFRAME} --dword ${DWORD} > segdata_tilegrid.txt; cd ../..

$(BUILD_DIR)/specimen_%/design.bits: $(BUILD_DIR)/specimen_%/top.bit
	${XRAY_BITREAD} -F ${XRAY_ROI_FRAMES} -architecture ${XRAY_ARCH} -o $@ -z -y $<

$(BUILD_DIR)/specimen_%/top.bit: $(BUILD_DIR)/specimen_%/top_par.ncd
	cd $(subst /top.bit,,$@); ${XRAY_ISE_PREFIX}/bitgen.exe -intstyle xflow -d -w -l -b -g Binary:yes -g IEEE1532:yes -g DebugBitstream:yes -g StartupClk:JtagClk top_par.ncd top.bit; cd ../..

$(BUILD_DIR)/specimen_%/top_par.ncd: $(BUILD_DIR)/specimen_%/top_map.ncd
	cd $(subst /top_par.ncd,,$@); ${XRAY_ISE_PREFIX}/par.exe -w -ol std -intstyle xflow top_map.ncd top_par.ncd top.pcf; cd ../..

$(BUILD_DIR)/specimen_%/top_map.ncd: $(BUILD_DIR)/specimen_%/top.ngd
	cd $(subst /top_map.ncd,,$@); ${XRAY_ISE_PREFIX}/map.exe -p ${XRAY_PART} -o top_map.ncd -intstyle xflow top.ngd top.pcf ; cd ../..

$(BUILD_DIR)/specimen_%/top.ngd: $(BUILD_DIR)/specimen_%/top.ngc
	cd $(subst /top.ngd,,$@); ${XRAY_ISE_PREFIX}/ngdbuild.exe -p ${XRAY_PART} -intstyle xflow -nt timestamp -verbose top.ngc top.ngd; cd ../..

$(BUILD_DIR)/specimen_%/top.ngc: $(BUILD_DIR)/specimen_%/top.v $(BUILD_DIR)/specimen_%/top_xst.scr
	cd $(subst /top.ngc,,$@); ${XRAY_ISE_PREFIX}/xst.exe -intstyle xflow -ifn top_xst.scr; cd ../..

$(BUILD_DIR)/specimen_%/top.v: $(FUZDIR)/top.py
	GENERATE_ARGS="${GENERATE_ARGS}" bash ../fuzzaddr/generate.sh $(subst /top.v,,$@)

define XST_SCRIPT_BODY
run
-ifn top.v
-ifmt Verilog
-ofn top
-ofmt ngc
-p $(XRAY_PART)
-verilog2001 YES
endef

.PRECIOUS: $(BUILD_DIR)/specimen_%/top.v \
	$(BUILD_DIR)/specimen_%/top_xst.scr \
	$(BUILD_DIR)/specimen_%/top.ngc \
	$(BUILD_DIR)/specimen_%/top.ngd \
	$(BUILD_DIR)/specimen_%/top_map.ncd \
	$(BUILD_DIR)/specimen_%/top_par.ncd \
	$(BUILD_DIR)/specimen_%/top.bit \
	$(BUILD_DIR)/specimen_%/design.bits \
	$(BUILD_DIR)/specimen_%/segdata_tilegrid.txt

$(BUILD_DIR)/specimen_%/top_xst.scr: export XST_SCRIPT_BODY:=$(XST_SCRIPT_BODY)
$(BUILD_DIR)/specimen_%/top_xst.scr:
	echo "$${XST_SCRIPT_BODY}" > $@

clean_bits:
	rm -rf build_*/specimen_*/design.bits

endif

clean:
	rm -rf build_*

clean_tilegrid:
	rm -f build_*/specimen_*/segdata_tilegrid.txt
	rm -f $(BUILD_DIR)/segbits_tilegrid.tdb

clean_part:
	rm -rf $(BUILD_DIR)

.PHONY: database clean
