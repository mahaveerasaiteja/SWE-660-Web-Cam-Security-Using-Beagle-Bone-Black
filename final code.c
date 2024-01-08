#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#define BUTTON 60 // p9-12
#define GPIO_EXPORT_PATH "/sys/class/gpio/export"
#define GPIO_PIN_PATH_TEMPLATE "/sys/class/gpio/gpio%d"
#define GPIO_DIRECTION_PATH_TEMPLATE "/sys/class/gpio/gpio%d/direction"
#define GPIO_VALUE_PATH_TEMPLATE "/sys/class/gpio/gpio%d/value"
#define IMAGE_PATH "/home/debian/image.jpg"
#define EMAIL_COMMAND "/usr/bin/mpack -s 'Image Captured' /home/debian/image.jpg mahaveerasaiteja@gmail.com"

void export_gpio(int gpio_pin) {
    int export_fd = open(GPIO_EXPORT_PATH, O_WRONLY);
    if (export_fd == -1) {
        perror("Error opening export file");
        exit(EXIT_FAILURE);
    }

    dprintf(export_fd, "%d", gpio_pin);

    close(export_fd);
}

void set_gpio_direction(int gpio_pin, const char *direction) {
    char direction_path[100];
    sprintf(direction_path, GPIO_DIRECTION_PATH_TEMPLATE, gpio_pin);

    int direction_fd = open(direction_path, O_WRONLY);
    if (direction_fd == -1) {
        perror("Error opening direction file");
        exit(EXIT_FAILURE);
    }

    dprintf(direction_fd, "%s", direction);

    close(direction_fd);
}

int read_gpio_value(int gpio_pin) {
    char value_path[100];
    sprintf(value_path, GPIO_VALUE_PATH_TEMPLATE, gpio_pin);

    int value_fd = open(value_path, O_RDONLY);
    if (value_fd == -1) {
        perror("Error opening value file");
        exit(EXIT_FAILURE);
    }

    char buffer[2];
    read(value_fd, buffer, sizeof(buffer));

    close(value_fd);

    return atoi(buffer);
}

void* button_thread(void* arg) {
    while (1) {
        while (read_gpio_value(BUTTON) == 1)
            usleep(100000);  // Sleep for 100ms

        printf("Button pressed. Triggering image capture...\n");
        system("fswebcam -r 640x480 --jpeg 85 -D 1 /home/debian/image.jpg");
    }
}

void* capture_thread(void* arg) {
    while (1) {
        // Insert image capture logic here (if needed)
        usleep(100000);  // Sleep for 100ms
    }
}

void* email_thread(void* arg) {
    while (1) {
        // Insert email notification logic here (if needed)
        usleep(100000);  // Sleep for 100ms
    }
}

int main() {
    pthread_t button_tid, capture_tid, email_tid;

    export_gpio(BUTTON);
    set_gpio_direction(BUTTON, "in");

    // Create threads
    pthread_create(&button_tid, NULL, button_thread, NULL);
    pthread_create(&capture_tid, NULL, capture_thread, NULL);
    pthread_create(&email_tid, NULL, email_thread, NULL);

    // Wait for threads to finish (which they won't)
    pthread_join(button_tid, NULL);
    pthread_join(capture_tid, NULL);
    pthread_join(email_tid, NULL);

    return 0;
}
