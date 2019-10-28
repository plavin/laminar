#include <stdio.h>
#include <numa.h>
#include <unistd.h>
//#include <pthread.h>

void print_bits(unsigned long num) {
    for (int bit = 0; bit < sizeof(unsigned long) * 8; bit++) {
        printf("%1lu", num & 0x01);
        num = num >> 1;
    }

}
int main(int argc, char **argv) {
    int num_nodes = numa_num_task_nodes();
    printf("num_nodes: %d\n", num_nodes);

    int num_procs = sysconf(_SC_NPROCESSORS_ONLN);
    printf("num_procs: %d\n", num_procs);

    struct bitmask *mask = numa_allocate_cpumask();
    unsigned long bits = mask->size;
    unsigned long bytes = bits / 8;
    size_t len = bytes / sizeof(unsigned long);
    printf("bits: %lu\n", mask->size);

    for (int i = 0; i < num_nodes; i++) {
       numa_bitmask_clearall(mask);
       numa_node_to_cpus(i, mask);
        for (size_t i = 0; i < len; i++) {
            print_bits(mask->maskp[i]);
            //printf("%lu ", mask->maskp[i]);
        }
        printf("\n");
    }

    numa_free_cpumask(mask);
}
