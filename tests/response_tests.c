#include "minunit.h"
#include "response.h"
#include "connection.h"
#include "register.h"
#include <fcntl.h>

FILE *LOG_FILE = NULL;


char *test_Response_send_status() 
{
    int zero_fd = open("/dev/zero", O_WRONLY);
    IOBuf *buf = IOBuf_create(1024, zero_fd, IOBUF_FILE);
    Connection conn = {.iob = buf};
    Register_connect(zero_fd, &conn);

    int rc = Response_send_status(&conn, &HTTP_405);
    mu_assert(rc != -1, "Failed to send 405 status.");

    Register_disconnect(zero_fd);
    return NULL;
}

char *test_Response_send_socket_policy()
{
    int zero_fd = open("/dev/zero", O_WRONLY);
    IOBuf *buf = IOBuf_create(1024, zero_fd, IOBUF_FILE);
    Connection conn = {.iob = buf};
    Register_connect(zero_fd, &conn);

    int rc = Response_send_socket_policy(&conn);
    mu_assert(rc != -1, "Failed to send the flash socket policy.");

    Register_disconnect(zero_fd);
    return NULL;
}


char * all_tests() {
    mu_suite_start();
    Register_init();

    mu_run_test(test_Response_send_status);
    mu_run_test(test_Response_send_socket_policy);

    return NULL;
}

RUN_TESTS(all_tests);

