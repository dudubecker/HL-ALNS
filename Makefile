.PHONY: clean All

All:
	@echo "----------Building project:[ HL_ALNS - Debug ]----------"
	@cd "HL_ALNS" && "$(MAKE)" -f  "HL_ALNS.mk"
clean:
	@echo "----------Cleaning project:[ HL_ALNS - Debug ]----------"
	@cd "HL_ALNS" && "$(MAKE)" -f  "HL_ALNS.mk" clean
