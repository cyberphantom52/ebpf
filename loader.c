#include<stdio.h>
#include<unistd.h>
#include<bpf/libbpf.h>
#include "sensor.h"


static int event_handler(void *ctx, void *data, size_t len) {
    struct exec_event *event = data;
    printf("command: %s, pid: %d, ppid: %d, status: %s\n", event->filename, event->pid, event->ppid, event->exit_code == 0 ? "success" : "fail");
    return 0;
}

int main ()
{
    const char *file = "sensor.bpf.o";
    const char *map = "exec_rb";
    struct bpf_object *obj;
    struct bpf_program *prog;
    int err;

    obj = bpf_object__open_file(file, NULL);
    if (libbpf_get_error(obj))
    {
        printf("Error opening file: %s\n", file);
        return 1;
    }

    err = bpf_object__load(obj);
    if (err)
    {
        printf("Error loading file: %s\n", file);
        return 1;
    }

    int fd = bpf_object__find_map_fd_by_name(obj, map);
    struct ring_buffer *ringbuf = ring_buffer__new(fd, event_handler, NULL, NULL);
    if (!ringbuf)
    {
        printf("Error creating ring buffer\n");
        return 1;
    }
    prog = bpf_object__find_program_by_name(obj, "syscall_execve_probe");
    if (!prog)
    {
        printf("Error finding program \n");
        return 1;
    }

    bpf_program__attach(prog);

    while (1)
    {
        ring_buffer__consume(ringbuf);
        sleep(1);
    }

    return 0;
}