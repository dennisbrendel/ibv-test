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

  ibv_free_device_list(ib_devices);

  printf("Found %d IB devices\n", num_devices);

  return 0;
}
