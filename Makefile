include .env

ESCAPED_PASS:="$(subst ",\",$(NETWORK_PASS))"

all: upload

secrets.h:
	@echo "#define BAUDRATE $(BAUDRATE)" > $@
	@echo "#define NETWORK_NAME \"$(NETWORK_NAME)\"" >> $@
	@echo "#define NETWORK_PASS \"$(NETWORK_PASS)\"" >> $@
	@echo "#define API_TO_REPORT_SENSES_URL \"$(API_TO_REPORT_SENSES_URL)\"" >> $@
	@echo "#define DHT1_PIN $(DHT1_PIN)" >> $@
	@echo "#define DHT2_PIN $(DHT2_PIN)" >> $@
	@echo "#define SOILMOISTURE_PIN $(SOILMOISTURE_PIN)" >> $@
	@echo "#define FAN_PIN $(FAN_PIN)" >> $@
	@echo "#define LDR_PIN $(LDR_PIN)" >> $@
	@echo "#define RES_SENSOR__SOILMOISTURE__VERYDRY $(RES_SENSOR__SOILMOISTURE__VERYDRY)" >> $@
	@echo "#define RES_SENSOR__SOILMOISTURE__VERYWET $(RES_SENSOR__SOILMOISTURE__VERYWET)" >> $@
	@echo "#define SECONDS_BEFORE_NEXT_SENSOR_READING $(SECONDS_BEFORE_NEXT_SENSOR_READING)" >> $@

install:
	$(ARDUINO_CLI) core install esp32:esp32
	$(ARDUINO_CLI) lib install "WiFi"
	$(ARDUINO_CLI) lib install "DHT sensor library"
	$(ARDUINO_CLI) lib install "ArduinoJson"

compile: 
	echo "Compiling for ESP32"
	rm -rf $(BUILD_DIR)
	rm -f ./secrets.h
	mkdir $(BUILD_DIR)
	make secrets.h
	$(ARDUINO_CLI) compile -v \
		--fqbn=$(BOARD) \
		--build-path=$(BUILD_DIR) \
		.

upload: compile
	$(ARDUINO_CLI) upload -p $(PORT) --fqbn=$(BOARD) --build-path $(BUILD_DIR) .

monitor:
	$(ARDUINO_CLI) monitor -p $(PORT) --config baudrate=$(BAUDRATE)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: install upload monitor clean
