/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <string.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/lock_core.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "dhcpserver.h"
#include "dnsserver.h"

#include "libft/libft.h"

// input hardware

// GPIO
#define PULSE_IN 28

#define RETRIES 10
#define MAX_STORAGE 1024
#define REG_STORAGE 900

#define TCP_PORT 80
#define DEBUG_printf printf
#define POLL_TIME_S 5
#define HTTP_GET "GET"
#define HTTP_RESPONSE_HEADERS \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: text/html; charset=utf-8\r\nConnection: close\r\n\r\n"
#define HTTP_RESPONSE_HEADERS_JS \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: text/javascript; charset=utf-8\r\nConnection: close\r\n\r\n"
#define HTTP_RESPONSE_HEADERS_CSS \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: text/css; charset=utf-8\r\nConnection: close\r\n\r\n"
#define HTTP_RESPONSE_HEADERS_DATA \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: application/json; charset=utf-8\r\nConnection: close\r\n\r\n"
#define LED_TEST_BODY \
        "<html><body><h1>Hello from Pico W.</h1><p>Led is %s</p><p><a href=\"?led=%d\">Turn led %s</a></body></html>"
#define LED_PARAM "led=%d"
#define LED_TEST "/ledtest"
#define INDEX_HTML "/hr_app.html"
#define SCRIPT_JS "/script.js"
#define STYLE_CSS "/style.css"
#define DATA "/data.json"
#include "script.js.h"
#include "style.css.h"
#include "index.html.h"
#define LED_GPIO 0
#define HTTP_RESPONSE_REDIRECT "HTTP/1.1 302 Redirect\r\nLocation: http://%s" INDEX_HTML "\r\n\r\n"
#define HEADER_MAX_LEN 1024
#define RESULT_MAX_LEN 4096
#define KEY_MAX_LEN 128

typedef struct TCP_SERVER_T_ {
    struct tcp_pcb  *server_pcb;
    bool            complete;
    ip_addr_t       gw;
    async_context_t *context;
} TCP_SERVER_T;

typedef struct TCP_CONNECT_STATE_T_ {
    struct tcp_pcb *pcb;
    int            sent_len;
    char           headers[HEADER_MAX_LEN];
    char           result[RESULT_MAX_LEN];
    int            header_len;
    int            result_len;
    ip_addr_t      *gw;
} TCP_CONNECT_STATE_T;

typedef struct s_pulse {
    uint64_t       timestamp;
    struct s_pulse *prev;
    struct s_pulse *next;
} t_pulse;

typedef struct s_storage {
    mutex_t mtx;
    t_pulse *data;
} t_storage;

t_storage local_storage;

t_pulse *pulse_add(t_pulse **l, uint64_t ts) {
    if (!l)
        return (0);
    t_pulse *ret = calloc(1, sizeof(t_pulse));
    if (!ret)
        return (0);
    ret->timestamp = ts;
    ret->prev = *l;
    if (*l)
        (*l)->next = *l;
    *l = ret;
    return (ret);
}

void pulse_clean(t_pulse **l) {
    t_pulse *i;

    if (!l)
        return;
    while (*l) {
        i = (*l)->prev;
        free(*l);
        *l = i;
    }
}

static err_t tcp_close_client_connection(TCP_CONNECT_STATE_T *con_state, struct tcp_pcb *client_pcb, err_t close_err) {
    if (client_pcb) {
        assert(con_state && con_state->pcb == client_pcb);
        tcp_arg(client_pcb, NULL);
        tcp_poll(client_pcb, NULL, 0);
        tcp_sent(client_pcb, NULL);
        tcp_recv(client_pcb, NULL);
        tcp_err(client_pcb, NULL);
        err_t err = tcp_close(client_pcb);
        if (err != ERR_OK) {
            DEBUG_printf("close failed %d, calling abort\n", err);
            tcp_abort(client_pcb);
            close_err = ERR_ABRT;
        }
        if (con_state) {
            free(con_state);
        }
    }
    return (close_err);
}

static void tcp_server_close(TCP_SERVER_T *state) {
    if (state->server_pcb) {
        tcp_arg(state->server_pcb, NULL);
        tcp_close(state->server_pcb);
        state->server_pcb = NULL;
    }
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *) arg;

    // DEBUG_printf("tcp_server_sent %u\n", len);
    con_state->sent_len += len;
    if (con_state->sent_len >= con_state->header_len + con_state->result_len) {
        // DEBUG_printf("all done\n");
        return (tcp_close_client_connection(con_state, pcb, ERR_OK));
    }
    return (ERR_OK);
}

static int test_server_content(const char *request, const char *params, char *result, size_t max_result_len) {
    int len = 0;

    if (strncmp(request, DATA, sizeof(DATA) - 1) == 0) {
        int32_t  i;
        char     *para;
        uint64_t last;
        char     a[KEY_MAX_LEN];
        t_pulse  *start;
        para = ft_strchr(params, '=');
        if (!para)
            last = 0;
        else
            last = strtoull(para + 1, NULL, 10);
        // DEBUG_printf("last: %" PRIu64 "\n", last);
        i = 0;
        bzero(result, max_result_len);
        while (!mutex_enter_timeout_ms(&local_storage.mtx, 1000) && i++ < RETRIES)
            ;
        if (i >= RETRIES) {
            DEBUG_printf("MUTEX BLOCKED : aborting read\n");
            len = snprintf(result, max_result_len, "{'error':'mutex'}");
        } else {
            start = local_storage.data;
            if (!start || start->timestamp <= last) {
                mutex_exit(&local_storage.mtx);
                len = snprintf(result, max_result_len, "[]");
            } else {
                ft_strlcat(result, "[", max_result_len);
                len++;
                while (start && start->timestamp > last && len < max_result_len) {
                    if (start->prev && start->prev->timestamp > last)
                        len += snprintf(a, KEY_MAX_LEN, "%" PRIu64 ",", start->timestamp);
                    else
                        len += snprintf(a, KEY_MAX_LEN, "%" PRIu64, start->timestamp);
                    ft_strlcat(result, a, max_result_len);
                    start = start->prev;
                }
                mutex_exit(&local_storage.mtx);
                ft_strlcat(result, "]", max_result_len);
                len++;
            }
        }
    } else if (strncmp(request, INDEX_HTML, sizeof(INDEX_HTML) - 1) == 0) {
        len = snprintf(result, max_result_len, index_html_h);
    } else if (strncmp(request, STYLE_CSS, sizeof(STYLE_CSS) - 1) == 0) {
        len = snprintf(result, max_result_len, style_css_h);
    } else if (strncmp(request, SCRIPT_JS, sizeof(SCRIPT_JS) - 1) == 0) {
        len = snprintf(result, max_result_len, script_js_h);
    } else if (strncmp(request, LED_TEST, sizeof(LED_TEST) - 1) == 0) {
        // Get the state of the led
        bool value;
        cyw43_gpio_get(&cyw43_state, LED_GPIO, &value);
        int led_state = value;
        // See if the user changed it
        if (params) {
            int led_param = sscanf(params, LED_PARAM, &led_state);
            if (led_param == 1) {
                if (led_state) {
                    // Turn led on
                    cyw43_gpio_set(&cyw43_state, 0, true);
                } else {
                    // Turn led off
                    cyw43_gpio_set(&cyw43_state, 0, false);
                }
            }
        }
        // Generate result
        if (led_state) {
            len = snprintf(result, max_result_len, LED_TEST_BODY, "ON", 0, "OFF");
        } else {
            len = snprintf(result, max_result_len, LED_TEST_BODY, "OFF", 1, "ON");
        }
    }
    return (len);
}

char *extract_header(char *header, char *value, char *ret, uint32_t ret_size) {
    uint32_t size;

    size = strlen(value);
    char *h = strnstr(header, value, strlen(header));
    char *b;
    if (!h)
        return (NULL);
    h += size;
    while (*h && ft_isspace(*h))
        h++;
    b = h;
    while (*b && *b != '\r' && *b != '\n')
        b++;
    char *tmp = ft_substr(h, 0, b - h);
    if (!tmp)
        return (0);
    if (ret) {
        bzero(ret, ret_size);
        ft_strlcpy(ret, tmp, ret_size);
        free(tmp);
        return (ret);
    }
    return (tmp);
}

/*
 * Processing of ingoing requests
 * */
err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *) arg;

    if (!p) {
        DEBUG_printf("connection closed because no p\n");
        return (tcp_close_client_connection(con_state, pcb, ERR_OK));
    }
    assert(con_state && con_state->pcb == pcb);
    if (p->tot_len > 0) {
        /* DEBUG_printf("tcp_server_recv %d err %d\n", p->tot_len, err);
        for (struct pbuf *q = p; q != NULL; q = q->next) {
            DEBUG_printf("in: %.*s\n", q->len, q->payload);
        } */
        // Copy the request into the buffer
        pbuf_copy_partial(p,
                con_state->headers,
                p->tot_len > sizeof(con_state->headers) - 1 ? sizeof(con_state->headers) - 1 : p->tot_len,
                0);
        // Handle GET request
        if (strncmp(HTTP_GET, con_state->headers, sizeof(HTTP_GET) - 1) == 0) {
            char host[KEY_MAX_LEN];
            extract_header(con_state->headers, "Host:", host, KEY_MAX_LEN);
            // DEBUG_printf("HOST : '%s'\n", host);
            char *request = con_state->headers + sizeof(HTTP_GET); // + space
            char *params  = strchr(request, '?');
            if (strncmp(host, ipaddr_ntoa(con_state->gw), strlen(host))) {
                if (strncmp(host, "hr_app.com", strlen(host)) == 0) {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_REDIRECT,
                            ipaddr_ntoa(con_state->gw));
                    DEBUG_printf("Sending redirect %s", con_state->headers);
                    con_state->sent_len = 0;
                    err_t err = tcp_write(pcb, con_state->headers, con_state->header_len, 0);
                    if (err != ERR_OK) {
                        DEBUG_printf("failed to write header data %d\n", err);
                        return (tcp_close_client_connection(con_state, pcb, err));
                    }
                    tcp_recved(pcb, p->tot_len);
                    pbuf_free(p);
                    tcp_close_client_connection(con_state, pcb, ERR_CLSD);
                    return (ERR_OK);
                }
                pbuf_free(p);
                return (ERR_OK);
            }
            if (params) {
                if (*params) {
                    char *space = strchr(request, ' ');
                    *params++ = 0;
                    if (space) {
                        *space = 0;
                    }
                } else {
                    params = NULL;
                }
            }
            // Generate content
            con_state->result_len = test_server_content(request, params, con_state->result, sizeof(con_state->result));
            // DEBUG_printf("Request: %s?%s\n", request, params);
            // DEBUG_printf("Result: %d\n", con_state->result_len);
            // Check we had enough buffer space
            int16_t ret_code = 200;
            if (con_state->result_len > sizeof(con_state->result) - 1) {
                DEBUG_printf("Too much result data %d\n", con_state->result_len);
                ret_code = 507;
                // return (tcp_close_client_connection(con_state, pcb, ERR_CLSD));
            }
            // Generate web page
            if (con_state->result_len > 0) {
                if (strncmp(request, DATA, sizeof(DATA) - 1) == 0) {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS_DATA,
                            ret_code,
                            con_state->result_len);
                } else if (strncmp(request, STYLE_CSS, sizeof(STYLE_CSS) - 1) == 0) {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS_CSS,
                            ret_code,
                            con_state->result_len);
                } else if (strncmp(request, SCRIPT_JS, sizeof(SCRIPT_JS) - 1) == 0) {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS_JS,
                            ret_code,
                            con_state->result_len);
                } else {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS,
                            ret_code,
                            con_state->result_len);
                }
                if (con_state->header_len > sizeof(con_state->headers) - 1) {
                    DEBUG_printf("Too much header data %d\n", con_state->header_len);
                    return (tcp_close_client_connection(con_state, pcb, ERR_CLSD));
                }
            } else {
                // Send redirect
                con_state->header_len = snprintf(con_state->headers,
                        sizeof(con_state->headers),
                        HTTP_RESPONSE_REDIRECT,
                        ipaddr_ntoa(con_state->gw));
                DEBUG_printf("Sending redirect %s", con_state->headers);
            }
            // Send the headers to the client
            con_state->sent_len = 0;
            err_t err = tcp_write(pcb, con_state->headers, con_state->header_len, 0);
            if (err != ERR_OK) {
                DEBUG_printf("failed to write header data %d\n", err);
                return (tcp_close_client_connection(con_state, pcb, err));
            }
            // Send the body to the client
            if (con_state->result_len) {
                err = tcp_write(pcb, con_state->result, con_state->result_len, 0);
                if (err != ERR_OK) {
                    DEBUG_printf("failed to write result data %d\n", err);
                    return (tcp_close_client_connection(con_state, pcb, err));
                }
            }
        }
        tcp_recved(pcb, p->tot_len);
    }
    pbuf_free(p);
    return (ERR_OK);
}

static err_t tcp_server_poll(void *arg, struct tcp_pcb *pcb) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *) arg;

    // DEBUG_printf("tcp_server_poll_fn\n");
    return (tcp_close_client_connection(con_state, pcb, ERR_OK)); // Just disconnect clent?
}

static void tcp_server_err(void *arg, err_t err) {
    TCP_CONNECT_STATE_T *con_state = (TCP_CONNECT_STATE_T *) arg;

    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err_fn %d\n", err);
        tcp_close_client_connection(con_state, con_state->pcb, err);
    }
}

/*
 * Notes:
 * This function is used to process TCP requests
 * */
static err_t tcp_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    TCP_SERVER_T *state = (TCP_SERVER_T *) arg;

    if (err != ERR_OK || client_pcb == NULL) {
        DEBUG_printf("failure in accept\n");
        return (ERR_VAL);
    }
    // DEBUG_printf("tcp client connected\n");

    // Create the state for the connection
    TCP_CONNECT_STATE_T *con_state = calloc(1, sizeof(TCP_CONNECT_STATE_T));
    if (!con_state) {
        DEBUG_printf("failed to allocate connect state\n");
        return (ERR_MEM);
    }
    con_state->pcb = client_pcb;                                  // for checking
    con_state->gw  = &state->gw;

    // setup connection to client
    tcp_arg(client_pcb, con_state);                               // function used to close connection
    tcp_sent(client_pcb, tcp_server_sent);                        // function used when something is sent to client
    tcp_recv(client_pcb, tcp_server_recv);                        // function called when request is recieved from client
    tcp_poll(client_pcb, tcp_server_poll, POLL_TIME_S * 2);
    tcp_err(client_pcb, tcp_server_err);                          // gestions des erreurs du socket

    return (ERR_OK);
}

/*
 * ALL TCP macros are NOT our problem
 * (you don't wanna look at them)
 * */
static bool tcp_server_open(void *arg, const char *ap_name) {
    TCP_SERVER_T *state = (TCP_SERVER_T *) arg;

    DEBUG_printf("starting server on port %d\n", TCP_PORT);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);       // protocol conntrol blocks
    if (!pcb) {
        DEBUG_printf("failed to create pcb\n");
        return (false);
    }
    err_t err = tcp_bind(pcb, IP_ANY_TYPE, TCP_PORT);             // reserve a specific port (aka 80 for http)
    if (err) {
        DEBUG_printf("failed to bind to port %d\n", TCP_PORT);
        return (false);
    }
    state->server_pcb = tcp_listen_with_backlog(pcb, 1);          // get tcp listening to ingoing connections
    if (!state->server_pcb) {
        DEBUG_printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return (false);
    }
    tcp_arg(state->server_pcb, state);
    tcp_accept(state->server_pcb, tcp_server_accept);

    printf("Try connecting to '%s' (press 'd' to disable access point)\n", ap_name);
    return (true);
}

// This "worker" function is called to safely perform work when instructed by key_pressed_func
void key_pressed_worker_func(async_context_t *context, async_when_pending_worker_t *worker) {
    assert(worker->user_data);
    printf("Disabling wifi\n");
    cyw43_arch_disable_ap_mode();
    ((TCP_SERVER_T *) (worker->user_data))->complete = true;
}

static async_when_pending_worker_t key_pressed_worker = {
    .do_work = key_pressed_worker_func
};

void key_pressed_func(void *param) {
    assert(param);
    int key = getchar_timeout_us(0); // get any pending key press but don't wait
    if (key == 'd' || key == 'D') {
        // We are probably in irq context so call wifi in a "worker"
        async_context_set_work_pending(((TCP_SERVER_T *) param)->context, &key_pressed_worker);
    }
}

void pins_init() {
    gpio_init(PULSE_IN);
    gpio_set_dir(PULSE_IN, GPIO_IN);
}

void gpio_callback(uint gpio, uint32_t events) {
    if (events & GPIO_IRQ_EDGE_RISE) {
        // Le code à exécuter lors d'un front montant
        uint64_t pulse_time = to_us_since_boot(get_absolute_time());
        DEBUG_printf("Front montant détecté sur le GPIO %d à %" PRIu64 "\n", gpio, pulse_time);
        int16_t i = 0;
        while (!mutex_enter_timeout_ms(&local_storage.mtx, 1000) && i++ < RETRIES)
            ;
        if (i >= RETRIES) {
            DEBUG_printf("MUTEX BLOCKED : aborting write\n");
            return;
        }
        pulse_add(&local_storage.data, pulse_time);
        mutex_exit(&local_storage.mtx);
    }
}

void hr_measure(TCP_SERVER_T *state) {
    int32_t i;
    t_pulse *a;
    t_pulse *b;

    DEBUG_printf("Ready to measure heart rate !\n");
    gpio_set_irq_enabled_with_callback(PULSE_IN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // irq_set_enabled(IO_IRQ_BANK0, true);
    while (!state->complete) {
        tight_loop_contents();
        sleep_ms(60000);
        DEBUG_printf("ping\n");
        i = 0;
        while (!mutex_enter_timeout_ms(&local_storage.mtx, 1000) && i++ < RETRIES)
            ;
        if (i >= RETRIES) {
            DEBUG_printf("MUTEX BLOCKED : aborting write\n");
            continue;
        }
        i = 0;
        a = local_storage.data;
        while (a && ++i < REG_STORAGE)
            a = a->prev;
        b = a;
        while (b && ++i < MAX_STORAGE)
            b = b->prev;
        if (b) {
            b       = a->prev;
            a->prev = NULL;
        }
        mutex_exit(&local_storage.mtx);
        if (b)
            DEBUG_printf("Cleaning storage...\n");
        pulse_clean(&b);
    }
}

int main() {
    stdio_init_all();                // init stdio with uart and stuff
    pins_init();
    mutex_init(&local_storage.mtx);
    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return (1);
    }
    if (cyw43_arch_init()) {
        DEBUG_printf("failed to initialise\n");
        return (1);
    }
    // Get notified if the user presses a key
    state->context               = cyw43_arch_async_context();
    key_pressed_worker.user_data = state;
    async_context_add_when_pending_worker(cyw43_arch_async_context(), &key_pressed_worker);
    stdio_set_chars_available_callback(key_pressed_func, state);

    const char *ap_name  = "hr_app";
    const char *password = "password";

    cyw43_arch_enable_ap_mode(ap_name, password, CYW43_AUTH_WPA2_AES_PSK);

    ip4_addr_t mask;
    IP4_ADDR(ip_2_ip4(&state->gw), 192, 168, 4, 1);    // set gateway
    IP4_ADDR(ip_2_ip4(&mask), 255, 255, 255, 0);       // set mask

    // Start the dhcp server
    dhcp_server_t dhcp_server;
    dhcp_server_init(&dhcp_server, &state->gw, &mask); // run dhcp server

    // Start the dns server
    dns_server_t dns_server;
    dns_server_init(&dns_server, &state->gw);          // init dhcp server
    if (!tcp_server_open(state, ap_name)) {
        DEBUG_printf("failed to open server\n");
        return (1);
    }
    state->complete = false;
    hr_measure(state);

    pulse_clean(&local_storage.data);
    tcp_server_close(state);
    dns_server_deinit(&dns_server);
    dhcp_server_deinit(&dhcp_server);
    cyw43_arch_deinit();
    return (0);
}
