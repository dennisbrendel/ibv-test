#include <stdio.h>
#include <stdlib.h>

#include <infiniband/verbs.h>

struct ibv_device **ib_devices = NULL;
struct ibv_context *ib_context = NULL;
struct ibv_pd *ib_pd = NULL;
void *mem_buf = NULL;

int init_device(int dev_id) {
  int num_devices = 0;

  ib_devices = ibv_get_device_list(&num_devices);
  if (ib_devices == NULL) {
    perror("Failed to get IB devices list");
    return 1;
  }

  if (num_devices == 0) {
    fprintf(stderr, "No IB devices detected!\n");
    return 1;
  }

  for (int i = 0; i < num_devices; i++) {
    printf("Found IB device with ID %d: %s\n",
           i,  ibv_get_device_name(ib_devices[i]));
  }

  if (dev_id > num_devices - 1) {
    fprintf(stderr, "Failed to use device with ID %d, falling back to %d\n",
                    dev_id, num_devices - 1);
    dev_id = num_devices - 1;
  }

  printf("Using IB device with ID %d\n", dev_id);

  return 0;
}

int open_device(int dev_id) {
  ib_context = ibv_open_device(ib_devices[dev_id]);
  if (ib_context == NULL) {
    perror("Failed to open IB device");
    return 1;
  }

  return 0;
}

int alloc_pd() {
  ib_pd = ibv_alloc_pd(ib_context);
  if (ib_pd == NULL) {
    perror("Failed to allocate protection domain");
    return 1;
  }

  return 0;
}

int cleanup() {
  int res = 0;

  if (ib_pd != NULL) {
    res = ibv_dealloc_pd(ib_pd);
    if (res != 0) {
      perror("Failed to deallocate protection domain");
    }
  }

  ibv_free_device_list(ib_devices);
  if (ib_context != NULL) {
    res = ibv_close_device(ib_context);
    if (res != 0) {
      return 1;
    }
  }

  return 0;
}

int main(int argc, char *argv[]) {
  int res = 0;
  int dev_id = 0;
  unsigned long mem_size = 1024 * 1024 * 1024;

  if (argc > 1) {
    dev_id = atoi(argv[1]);
  }
  if (argc > 2) {
    mem_size = atol(argv[2]) * 1024 * 1024 * 1024;
  }

  res = init_device(dev_id);
  if (res != 0) {
    fprintf(stderr, "Failed to initialize\n");
    cleanup();
    exit(res);
  }

  res = open_device(dev_id);
  if (res != 0) {
    fprintf(stderr, "Failed to open devices\n");
    cleanup();
    exit(res);
  }

  res = alloc_pd();
  if (res != 0) {
    fprintf(stderr, "Failed to allocate protection domain\n");
    cleanup();
    exit(res);
  }

  mem_buf = malloc(mem_size);
  if (mem_buf == NULL) {
    fprintf(stderr, "Failed to allocate local memory\n");
    cleanup();
    exit(EXIT_FAILURE);
  }

  return 0;
}
