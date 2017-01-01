// POSIX.1-2008
#define _XOPEN_SOURCE 700

#include <string.h>
#include <stdio.h>
#include <sysexits.h>
#include <stdlib.h>


/* Remove trailing newline characters of `string`,
 * provided `string` itself does not contain newline characters in other places.
 */
void rtrim(char* string)
{
    string[strcspn(string, "\n\r")] = '\0';
}

/* Append `name` to `destination`.
 * If `name` is longer than 120 characters,
 * append a shortened version (`UpTo120Characters...`) instead.
 */
void concat_shortened(char* destination, const char* name)
{
    if (strlen(name) > 120)
    {
        char shortened[121];
        strncpy(shortened, name, 120);
        strcat(destination, shortened);
        strcat(destination, "...");
    }
    else
    {
        strcat(destination, name);
    }
}

void write_error_line(char* line)
{
    const int status = fputs(line, stderr);
    if (status == EOF) // error
    {
        perror("write_error_line(): fputs():");
    }
    else // success
    {
        // done
    }
}

// Print failed function with its parameters to stderr
// and exit with status `EX_SOFTWARE`.
void bug(const char* function, const char* parameters)
{
    char error_message[256] = "Error: ";
    concat_shortened(error_message, function);
    strcat(error_message, "(");
    concat_shortened(error_message, parameters);
    concat_shortened(error_message, ")");

    write_error_line(error_message);
    exit(EX_SOFTWARE);
}


// `command_line` must only output one line.
// Up to 4096 characters of the output line will be read.
void exec(const char* command_line, void (*callback)(const int, const char *))
{
    FILE* output = popen(command_line, "r");

    char line[4096];
    fgets(line, sizeof line, output);
    rtrim(line);
    callback(pclose(output), line);
}


void report(const int status, const char* content, const char mode)
{
    char *mode_name;
    char *program;
    switch (mode)
    {
        case 'r':
            mode_name = "Read";
            program = "hdparm";
            break;
        case 'w':
            mode_name = "Write";
            program = "dd";
            break;
        default:
            write_error_line("report(..., ..., char mode) only accepts `r` or `w` as mode name");
            exit(EX_SOFTWARE);
    }
    if (status == 0)
    {
        printf("Sequential %s: %s\n", mode_name, content);
    }
    else
    {
        char error_message[101];
        snprintf(error_message, 100, "Error: %s failed with status %d", program, status);
        write_error_line(error_message);
        exit(status);
    }
}

void read_report(const int status, const char* content)
{
    report(status, content, 'r');
}

void write_report(const int status, const char* content)
{
    report(status, content, 'w');
}

void benchmark(const int status, const char* content)
{
    if (status == 0) {
        for (int i = 3; i > 0; i--)
        {
            char command_line[100] = "hdparm -t ";
            strcat(command_line, content);
            strcat(command_line, " | grep -E -o '[0-9.]+ [GM]B/s'");
            exec(command_line, read_report);
        }
    }
    else
    {
        write_error_line("Error: failed to detect current device.");
        exit(status);
    }
}

// Program `name` must be shorter than 100 characters(bytes).
void has_executable(const char *name)
{
    char which[6 + 100 + 1] = "which ";
    strcat(which, name);
    strcat(which, " > /dev/null");
    int status = system(which);
    if (status != 0)
    {
        char error_message[200] = "Error: cannot find ";
        strcat(error_message, name);
        write_error_line(error_message);
        exit(EX_UNAVAILABLE);
    }
}

// Has at least 1G free space?
void has_enough_free_space()
{
    char *command_line = "test $(df --block-size G . | tail -1 | grep -E -o '[0-9.]+G[ ]++[0-9]+%' | grep -E -o '^[0-9]+') -gt 1";
    int status = system(command_line);
    if (status != 0)
    {
        write_error_line("Make sure the disk to test has at least 1GB free space.");
        exit(EX_TEMPFAIL);
    }
}


void sequential_read_test()
{
    has_executable("hdparm");

    const char* detect_current_device = "df . | tail -1 | cut -d ' ' -f 1";
    exec(detect_current_device, benchmark);
}

void build_dd_command_line(char* dd, char* temporary_file)
{
    strcat(dd, "dd if=/dev/zero of=");
    strcat(dd, temporary_file);
    strcat(dd, " bs=1M count=1024 conv=fdatasync,notrunc 2>&1 > /dev/null | grep -E -o '[0-9.]+ [GM]B/s'");
}

/* Generates a 32 bytes string. */
void random_alphanum(char* result, int length)
{
    char* alphanum = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < length ; i++) {
        result[i] = alphanum[rand() % 62];
    }
    result[length] = '\0';
}

void sequential_write_test()
{
    has_enough_free_space();

    for (int i = 3; i > 0; i--)
    {
        char temporary_file[32 + 1] = "";
        random_alphanum(temporary_file, 32);
        char dd[200] = "";
        build_dd_command_line(dd, temporary_file);
        exec(dd, write_report);
        int is_deleted = remove(temporary_file);
        if (is_deleted != 0)
        {
            char error_message[101];
            snprintf(error_message, 100, "Warn: failed to delete temporary file %s", temporary_file);
            write_error_line(error_message);
        }
    }
}

int main()
{
    sequential_read_test();
    sequential_write_test();

}
