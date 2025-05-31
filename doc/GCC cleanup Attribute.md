cleanup` 属性是 GCC 特有的扩展，允许你指定一个函数，在变量离开作用域时自动调用。

This attribute can only be applied to auto function scope variables; it may not be applied to parameters or variables with static storage duration. **The function must take one parameter, a pointer to a type compatible with the variable**.

> cleanup 函数的型参必须是指向自动变量类型的指针。

**自动释放内存**

```c
#include <stdlib.h>
#include <stdio.h>

void cleanup_func(int **ptr) {
    if (ptr && *ptr) {
        printf("Cleaning up\n");
        free(*ptr);
    }
}

#define AUTO_CLEANUP(func) __attribute__((cleanup(func)))

int main() {
    AUTO_CLEANUP(cleanup_func) int *ptr = malloc(sizeof(int) * 10);
    if (!ptr) {
        perror("malloc");
        return 1;
    }
    // Use ptr for some operations

    // No need to explicitly free(ptr), it will be done automatically
    return 0;
}
```

**自动释放锁**

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void cleanup_mutex(pthread_mutex_t **mutex_ptr) {
    if (mutex_ptr && *mutex_ptr) {
        pthread_mutex_unlock(*mutex_ptr);
        printf("Mutex unlocked\n");
    }
}

#define AUTO_CLEANUP_MUTEX __attribute__((cleanup(cleanup_mutex)))

void some_function() {
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    AUTO_CLEANUP_MUTEX pthread_mutex_t *mutex_ptr = &mutex;
    pthread_mutex_lock(mutex_ptr);

    // Do some work while the mutex is locked
    printf("Mutex is locked\n");

    // No need to explicitly unlock the mutex, it will be done automatically
}

int main() {
    some_function();
    return 0;
}
```

**自动释放文件句柄**

```c
#include <stdio.h>

void cleanup_file(FILE **file) {
    if (file && *file) {
        fclose(*file);
        printf("File closed\n");
    }
}

#define AUTO_CLEANUP_FILE __attribute__((cleanup(cleanup_file)))

void some_function() {
    AUTO_CLEANUP_FILE FILE *file = fopen("example.txt", "w");
    if (!file) {
        perror("Failed to open file");
        return;
    }
    
    // Perform file operations
    fprintf(file, "Hello, World!\n");
    
    // No need to explicitly close the file, it will be done automatically
}

int main() {
    some_function();
    return 0;
}
```

**自动释放 open 打开的文件句柄**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

void cleanup_fd(int *fd) {
    if (fd && *fd != -1) {
        close(*fd);
        printf("File descriptor closed\n");
    }
}

#define AUTO_CLEANUP_FD __attribute__((cleanup(cleanup_fd)))

void some_function() {
    AUTO_CLEANUP_FD int fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("Failed to open file");
        return;
    }
    
    // Perform file operations
    if (write(fd, "Hello, World!\n", 14) == -1) {
        perror("Failed to write to file");
        return;
    }
    
    // No need to explicitly close the file descriptor, it will be done automatically
}

int main() {
    some_function();
    return 0;
}
```

### Reference

[Common Variable Attributes](https://gcc.gnu.org/onlinedocs/gcc/Common-Variable-Attributes.html)