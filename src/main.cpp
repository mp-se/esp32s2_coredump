/*
MIT License

Copyright (c) 2023 Magnus

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 */
#include <Arduino.h>
#if CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/rom/rtc.h"
#include "esp_core_dump.h"
#endif

void setup() {
  Serial.begin(115200L);
  delay(3000); // wait for the serial port to connect 

  // see: rtc.h for reset reasons
  Serial.printf("Reset reason %d.\n", rtc_get_reset_reason(0));
  Serial.printf("Core dump check %d.\n", esp_core_dump_image_check());
  Serial.printf("Default esp32 log level %d\n", esp_log_default_level);

  esp_core_dump_summary_t *summary = static_cast<esp_core_dump_summary_t *>(malloc(sizeof(esp_core_dump_summary_t)));
  if (summary) {
    esp_log_level_set("esp_core_dump_elf", ESP_LOG_VERBOSE);

    if (esp_core_dump_get_summary(summary) == ESP_OK) {
      /*
      typedef struct {
          uint32_t exc_tcb;                           /*!< TCB pointer to the task causing exception 
          char exc_task[16];                          /*!< Name of the task that caused exception 
          uint32_t exc_pc;                            /*!< Program counter for exception 
          esp_core_dump_bt_info_t exc_bt_info;        /*!< Backtrace information for task causing exception 
          uint32_t core_dump_version;                 /*!< Core dump version 
          uint8_t app_elf_sha256[APP_ELF_SHA256_SZ];  /*!< Crashing application's SHA256 sum as a string 
          esp_core_dump_summary_extra_info_t ex_info; /*!< Architecture specific extra data 
      } esp_core_dump_summary_t;
      */

      // Print the point of the exception
      Serial.printf("TCB pointer 0x%x.\n", summary->exc_tcb);
      Serial.printf("PC 0x%x.\n", summary->exc_pc);
      Serial.printf("Exception cause %d.\n", summary->ex_info.exc_cause);

      /*
      typedef struct {
          uint32_t bt[16];        /*!< Backtrace (array of PC) 
          uint32_t depth;         /*!< Number of backtrace entries 
          bool corrupted;         /*!< Status flag for backtrace is corrupt or not 
      } esp_core_dump_bt_info_t;
      */

      // Print the backtrace of the exception (from where was this method called)
      for (int i = 0; i < summary->exc_bt_info.depth; i++) {
        Serial.printf("PC Backtrace %d, 0x%x.\n", i, summary->exc_bt_info.bt[i]);
      }

      // esp_core_dump_image_erase(); // remove the core dump if it has been handled.
    }
  }
  free(summary);
}

int counter = 0;

void loop() {
  Serial.println("Looping....");
  delay(1000);

  counter++;

  if (counter > 10) {
    Serial.println("Crashing....");
    delay(500);
    
    char* ptr = 0;
    *ptr = 0x10; // Cause crash
  }
}

// EOF
