// LED rates
static const int rate_1 = 100;  // ms
static const int rate_2 = 323;  // ms

// Pins
static const int led_pin_1 = 2;
static const int led_pin_2 = 12;

// Our task: blink an LED at one rate
void toggleLED_1(void *parameter) {
  while(1) {
    digitalWrite(led_pin_1, HIGH);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
    digitalWrite(led_pin_1, LOW);
    vTaskDelay(rate_1 / portTICK_PERIOD_MS);
  }
}

// Our task: blink an LED at another rate
void toggleLED_2(void *parameter) {
  while(1) {
    digitalWrite(led_pin_2, HIGH);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
    digitalWrite(led_pin_2, LOW);
    vTaskDelay(rate_2 / portTICK_PERIOD_MS);
  }
}

void setup() {

  // Configure pin
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);

  // Task to run forever
  xTaskCreate(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_1,  // Function to be called
              "Toggle 1",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              0,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL);     // Run on one core for demo purposes (ESP32 only)

  // Task to run forever
  xTaskCreate(  // Use xTaskCreate() in vanilla FreeRTOS
              toggleLED_2,  // Function to be called
              "Toggle 2",   // Name of task
              1024,         // Stack size (bytes in ESP32, words in FreeRTOS)
              NULL,         // Parameter to pass to function
              0,            // Task priority (0 to configMAX_PRIORITIES - 1)
              NULL);     // Run on one core for demo purposes (ESP32 only)

  // If this was vanilla FreeRTOS, you'd want to call vTaskStartScheduler() in
  // main after setting up your tasks.
}

void loop() {
  // Do nothing
  // setup() and loop() run in their own task with priority 1 in core 1
  // on ESP32
}
