#include <stdio.h>
#include <stdlib.h>

#include <infiniband/verbs.h>

struct ibv_device **ib_devices = NULL;

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

int main(int argc, char *argv[]) {
  int res = 0;
  int dev_id = 0;

  if (argc > 1) {
    dev_id = atoi(argv[1]);
  }

  res = init_device(dev_id);
  if (res != 0) {
    fprintf(stderr, "Failed to initialize\n");
  }

  ibv_free_device_list(ib_devices);

  return 0;
}
