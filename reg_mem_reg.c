#include <stdio.h>

#include <infiniband/verbs.h>

int main(int argc, char *argv[]) {
  int num_devices = 0;
  struct ibv_device **ib_devices = NULL;

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

  ibv_free_device_list(ib_devices);

  return 0;
}
