/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdint.h>
#include <string.h>

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"

#include "lwip/pbuf.h"
#include "lwip/tcp.h"

#include "dhcpserver.h"
#include "dnsserver.h"

#include "libft/libft.h"
#include "sha1/sha1.h"
#include "base64/b64.h"

// input hardware

#define PULSE_IN 28
#define HR_PORT 9000
#define HR_PULSE_HEADERS \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: application/json; charset=utf-8\r\nConnection: keep-alive\r\n\r\n"
#define HR_HEADER_HANDSHAKE \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: application/json; charset=utf-8\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n"
#define HR_HEADER_400 \
        "HTTP/1.1 400 Bad request\r\nContent-Length: %d\r\nContent-Type: application/json; charset=utf-8\r\nConnection: close\r\n\r\n"
#define HR_END_HEADERS \
        "HTTP/1.1 %d OK\r\nContent-Length: %d\r\nContent-Type: application/json; charset=utf-8\r\nConnection: close\r\n\r\n"
#define HR_DATA \
        "{type:\"%s\",timestamp:%" PRIu64 "}"
#define HR_CON_OK \
        "CONNECTED"

#define WS_MAGIC_STRING "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

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
#define LED_TEST_BODY \
        "<html><body><h1>Hello from Pico W.</h1><p>Led is %s</p><p><a href=\"?led=%d\">Turn led %s</a></body></html>"
#define LED_PARAM "led=%d"
#define LED_TEST "/ledtest"
#define INDEX_HTML "/hr_app.html"
#define SCRIPT_JS "/script.js"
#define STYLE_CSS "/style.css"
#include "script.js.h"
#include "style.css.h"
#include "index.html.h"
#define LED_GPIO 0
#define HTTP_RESPONSE_REDIRECT "HTTP/1.1 302 Redirect\r\nLocation: http://%s" INDEX_HTML "\r\n\r\n"
#define HEADER_MAX_LEN 1024
#define RESULT_MAX_LEN 2048
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

typedef struct s_hr_con_list {
    struct HR_CONNECT_STATE_T_ *con;
    char                       client_key[KEY_MAX_LEN];
    struct s_hr_con_list       *next;
} t_hr_con_list;

typedef struct HR_SERVER_T_ {        // to add special list of connected clients
    struct tcp_pcb  *server_pcb;
    bool            complete;
    ip_addr_t       gw;
    async_context_t *context;
    t_hr_con_list   *con_list;
} HR_SERVER_T;

typedef struct HR_CONNECT_STATE_T_ { // to add special keep alive stuff
    struct tcp_pcb *pcb;
    int            sent_len;
    char           headers[HEADER_MAX_LEN];
    char           result[RESULT_MAX_LEN];
    int            header_len;
    int            result_len;
    ip_addr_t      *gw;
    HR_SERVER_T    *server;
    char           done;
} HR_CONNECT_STATE_T;

t_hr_con_list *add_con(t_hr_con_list **list, HR_CONNECT_STATE_T *con) {
    t_hr_con_list *i;
    t_hr_con_list *to_add;

    if (!list)
        return (DEBUG_printf("No list provided to add, abort.\n"), NULL);
    i      = *list;
    to_add = calloc(1, sizeof(t_hr_con_list));
    if (!to_add)
        return (DEBUG_printf("Failed to allocate memory.\n"), NULL);
    to_add->con = con;
    if (!i) {
        *list = to_add;
        return (*list);
    }
    while (i->next)
        i = i->next;
    i->next = to_add;
    return (to_add);
}

void remove_con(t_hr_con_list **list, HR_CONNECT_STATE_T *con) {
    t_hr_con_list *prev;
    t_hr_con_list *i;

    if (!list)
        return;
    prev = NULL;
    i    = *list;
    while (i) {
        if (i->con == con) {
            if (prev)
                prev->next = i->next;
            else {
                *list = i->next;
                free(i);
                i = (*list)->next;
                continue;
            }
            free(i);
        } else
            prev = i;
        i = prev->next;
    }
}

void clean_con(t_hr_con_list **list) {
    t_hr_con_list *i;

    if (!list)
        return;
    i = *list;
    while (i) {
        *list = i->next;
        if (i->con)
            free(i->con);
        free(i);
        i = *list;
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

    DEBUG_printf("tcp_server_sent %u\n", len);
    con_state->sent_len += len;
    if (con_state->sent_len >= con_state->header_len + con_state->result_len) {
        DEBUG_printf("all done\n");
        return (tcp_close_client_connection(con_state, pcb, ERR_OK));
    }
    return (ERR_OK);
}

static int test_server_content(const char *request, const char *params, char *result, size_t max_result_len) {
    int len = 0;

    if (strncmp(request, INDEX_HTML, sizeof(INDEX_HTML) - 1) == 0) {
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
            if (con_state->result_len > sizeof(con_state->result) - 1) {
                DEBUG_printf("Too much result data %d\n", con_state->result_len);
                return (tcp_close_client_connection(con_state, pcb, ERR_CLSD));
            }
            // Generate web page
            if (con_state->result_len > 0) {
                if (strncmp(request, STYLE_CSS, sizeof(STYLE_CSS) - 1) == 0) {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS_CSS,
                            200,
                            con_state->result_len);
                } else if (strncmp(request, SCRIPT_JS, sizeof(SCRIPT_JS) - 1) == 0) {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS_JS,
                            200,
                            con_state->result_len);
                } else {
                    con_state->header_len = snprintf(con_state->headers,
                            sizeof(con_state->headers),
                            HTTP_RESPONSE_HEADERS,
                            200,
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

/*
 * Below this is not TCP but HR
 * */
static err_t hr_close_client_connection(HR_CONNECT_STATE_T *con_state, struct tcp_pcb *client_pcb, err_t close_err) {
    if (!con_state->done) {
        bzero(con_state->headers, sizeof(con_state->headers));
        con_state->header_len = snprintf(con_state->headers, sizeof(con_state->headers), HR_END_HEADERS, 200, 0);
        tcp_write(client_pcb, con_state->headers, con_state->header_len, 0);
        con_state->done = 1;
    }
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
            remove_con(&con_state->server->con_list, con_state);
        }
    }
    return (close_err);
}

static void hr_server_close(HR_SERVER_T *state) {
    clean_con(&state->con_list);
    if (state->server_pcb) {
        tcp_arg(state->server_pcb, NULL);
        tcp_close(state->server_pcb);
        state->server_pcb = NULL;
    }
}

static err_t hr_server_sent(void *arg, struct tcp_pcb *pcb, u16_t len) {
    HR_CONNECT_STATE_T *con_state = (HR_CONNECT_STATE_T *) arg;

    DEBUG_printf("hr_server_sent %u\n", len);
    con_state->sent_len += len;
    if (con_state->sent_len >= con_state->header_len + con_state->result_len) {
        if (con_state->done) { // change to constate value
            DEBUG_printf("all done, closing hr connection\n");
            return (hr_close_client_connection(con_state, pcb, ERR_OK));
        } else {
            DEBUG_printf("Message sent successfully.\n");
        }
    }
    return (ERR_OK);
}

int generate_handshake(char *client_key, char *server_key) {
    char tmp[KEY_MAX_LEN];

    return (0);
    bzero(tmp, KEY_MAX_LEN);
    bzero(server_key, KEY_MAX_LEN);
    ft_strlcpy(tmp, client_key, KEY_MAX_LEN);
    ft_strlcat(tmp, WS_MAGIC_STRING, KEY_MAX_LEN);
    SHA1(server_key, tmp, strlen(tmp));
    char *a = b64_encode(server_key, strlen(server_key));
    if (!a)
        return (-1);
    DEBUG_printf("server key : %s\n", a);
    ft_strlcpy(server_key, a, KEY_MAX_LEN);
    free(a);
    return (0);
}

/*
 * Processing of ingoing requests
 * this par is using the ws protocol
 * no ssl because of cryptographic issues (it's a PICO)
 * */
err_t hr_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    HR_CONNECT_STATE_T *con_state = (HR_CONNECT_STATE_T *) arg;

    if (!p) {
        DEBUG_printf("hr connection closed\n");
        return (hr_close_client_connection(con_state, pcb, ERR_OK));
    }
    assert(con_state && con_state->pcb == pcb);
    if (p->tot_len > 0) {
        DEBUG_printf("hr_server_recv %d err %d\n", p->tot_len, err);
        for (struct pbuf *q = p; q != NULL; q = q->next) {
            DEBUG_printf("in: %.*s\n", q->len, q->payload);
        }
        // Copy the request into the buffer
        pbuf_copy_partial(p,
                con_state->headers,
                p->tot_len > sizeof(con_state->headers) - 1 ? sizeof(con_state->headers) - 1 : p->tot_len,
                0);
        // Handle GET request
        t_hr_con_list *cl;
        if (strncmp(HTTP_GET, con_state->headers, sizeof(HTTP_GET) - 1) == 0) {
            // parse request and params
            /* char *request = con_state->headers + sizeof(HTTP_GET); // + space
            char *params  = strchr(request, '?');
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
            DEBUG_printf("Request: %s?%s\n", request, params); */
            // Generate content
            // con_state->result_len = snprintf(con_state->result, sizeof(con_state->result), HR_CON_OK,
            //         to_us_since_boot(get_absolute_time()));
            DEBUG_printf("It's a GET\n");
            char client_key[KEY_MAX_LEN];
            extract_header(con_state->headers, "Sec-WebSocket-Key:", client_key, KEY_MAX_LEN);
            if (!client_key) {
                DEBUG_printf("No key : bad request");
                // no key -> bad request
                return (hr_close_client_connection(con_state, pcb, ERR_ABRT));
            }
            DEBUG_printf("Client key : %s\n", client_key);
            cl = con_state->server->con_list;
            uint32_t size = strlen(client_key);
            while (cl) {
                if (strncmp(cl->client_key, client_key, size) == 0) {
                    DEBUG_printf("Socket already authenticated.\n");
                    break;
                }
            }
            if (!cl) { // handshake not done yet
                char server_key[KEY_MAX_LEN];
                generate_handshake(cl->client_key, server_key);
                con_state->header_len = snprintf(con_state->headers,
                        sizeof(con_state->headers),
                        HR_HEADER_HANDSHAKE,
                        101,
                        con_state->result_len,
                        server_key);
                err_t err = tcp_write(pcb, con_state->headers, con_state->header_len, 0);
                if (err != ERR_OK) {
                    DEBUG_printf("Handshake failed,\n");
                    return (hr_close_client_connection(con_state, pcb, ERR_CLSD));
                }
                add_con(&con_state->server->con_list, con_state);
            }
            // Check we had enough buffer space
            // if (con_state->result_len > sizeof(con_state->result) - 1) {
            //     DEBUG_printf("Too much result data %d\n", con_state->result_len);
            //     return (hr_close_client_connection(con_state, pcb, ERR_CLSD));
            // }
            // // Send the headers to the client
            // con_state->sent_len = 0;
            // err_t err = tcp_write(pcb, con_state->headers, con_state->header_len, 0);
            // if (err != ERR_OK) {
            //     DEBUG_printf("failed to write header data %d\n", err);
            //     return (hr_close_client_connection(con_state, pcb, err));
            // }
            // // Send the body to the client
            // if (con_state->result_len) {
            //     err = tcp_write(pcb, con_state->result, con_state->result_len, 0);
            //     if (err != ERR_OK) {
            //         DEBUG_printf("failed to write result data %d\n", err);
            //         return (hr_close_client_connection(con_state, pcb, err));
            //     }
            // }
        }
        tcp_recved(pcb, p->tot_len);
    }
    pbuf_free(p);
    return (ERR_OK);
}

static err_t hr_server_push(HR_CONNECT_STATE_T *con, char *data) {
    if (!con)
        return (ERR_ABRT);
    con->sent_len   = 0;
    con->header_len = 0;
    con->result_len = 0;
    bzero(con->headers, sizeof(con->headers));
    bzero(con->result,  sizeof(con->result));
    con->result_len = snprintf(con->result, sizeof(con->result), "%s", data);
    con->header_len = snprintf(con->headers, sizeof(con->headers), HR_PULSE_HEADERS, 200, con->result_len);
    DEBUG_printf("Sending data : '%s'\n", data);
    err_t err = tcp_write(con->pcb, con->headers, con->header_len, 0);
    if (err != ERR_OK) {
        DEBUG_printf("failed to write header data %d\n", err);
        return (hr_close_client_connection(con, con->pcb, err));
    }
    // Send the body to the client
    if (con->result_len) {
        err = tcp_write(con->pcb, con->result, con->result_len, 0);
        if (err != ERR_OK) {
            DEBUG_printf("failed to write result data %d\n", err);
            return (hr_close_client_connection(con, con->pcb, err));
        }
    }
    DEBUG_printf("Data sent.");
}

static err_t hr_server_poll(void *arg, struct tcp_pcb *pcb) {
    HR_CONNECT_STATE_T *con_state = (HR_CONNECT_STATE_T *) arg;

    DEBUG_printf("hr_server_poll_fn <== INVESTIGATE THIS CALL\n");
    return (hr_close_client_connection(con_state, pcb, ERR_OK)); // Just disconnect client?
}

static void hr_server_err(void *arg, err_t err) {
    HR_CONNECT_STATE_T *con_state = (HR_CONNECT_STATE_T *) arg;

    if (err != ERR_ABRT) {
        DEBUG_printf("tcp_client_err_fn %d\n", err);
        hr_close_client_connection(con_state, con_state->pcb, err);
    }
}

/*
 * Notes:
 * This function is used to process TCP requests
 * */
static err_t hr_server_accept(void *arg, struct tcp_pcb *client_pcb, err_t err) {
    HR_SERVER_T *state = (HR_SERVER_T *) arg;

    if (err != ERR_OK || client_pcb == NULL) {
        DEBUG_printf("failure in hr accept\n");
        return (ERR_VAL);
    }
    DEBUG_printf("client connected on hr diffusion list\n");

    // Create the state for the connection
    HR_CONNECT_STATE_T *con_state = calloc(1, sizeof(HR_CONNECT_STATE_T));
    if (!con_state) {
        DEBUG_printf("failed to allocate hr connect state\n");
        return (ERR_MEM);
    }
    con_state->server = state;
    con_state->pcb    = client_pcb;                              // for checking
    con_state->gw     = &state->gw;

    // setup connection to client
    tcp_arg(client_pcb, con_state);                              // function used to close connection
    tcp_sent(client_pcb, hr_server_sent);                        // function used when something is sent to client
    tcp_recv(client_pcb, hr_server_recv);                        // function called when request is recieved from client
    tcp_poll(client_pcb, hr_server_poll, POLL_TIME_S * 2);
    tcp_err(client_pcb, hr_server_err);                          // gestions des erreurs du socket

    return (ERR_OK);
}

/*
 * ALL TCP macros are NOT our problem
 * (you don't wanna look at them)
 * */
static bool hr_server_open(void *arg, const char *ap_name) {
    HR_SERVER_T *state = (HR_SERVER_T *) arg;

    DEBUG_printf("starting hr server on port %d\n", HR_PORT);

    struct tcp_pcb *pcb = tcp_new_ip_type(IPADDR_TYPE_ANY);      // protocol conntrol blocks
    if (!pcb) {
        DEBUG_printf("failed to create pcb\n");
        return (false);
    }
    err_t err = tcp_bind(pcb, IP_ANY_TYPE, HR_PORT);             // reserve a specific port
    if (err) {
        DEBUG_printf("failed to bind to port %d\n", HR_PORT);
        return (false);
    }
    state->server_pcb = tcp_listen_with_backlog(pcb, 1);         // get tcp listening to ingoing connections
    if (!state->server_pcb) {
        DEBUG_printf("failed to listen\n");
        if (pcb) {
            tcp_close(pcb);
        }
        return (false);
    }
    tcp_arg(state->server_pcb, state);
    tcp_accept(state->server_pcb, hr_server_accept);

    printf("hr server is online %s.\n", ap_name);
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

void *g_hr_server = NULL;

void gpio_callback(uint gpio, uint32_t events) {
    HR_SERVER_T   *server = NULL;
    t_hr_con_list *i      = NULL;

    if (events & GPIO_IRQ_EDGE_RISE) {
        // Le code à exécuter lors d'un front montant
        uint64_t    pulse_time = to_us_since_boot(get_absolute_time());
        HR_SERVER_T *server    = (HR_SERVER_T *) g_hr_server;
        DEBUG_printf("Front montant détecté sur le GPIO %d à %" PRIu64 "\n", gpio, pulse_time);
        if (!server)
            return;
        char result[RESULT_MAX_LEN];
        bzero(result, sizeof(result));
        snprintf(result, sizeof(result), HR_DATA, "PULSE", pulse_time);
        i = server->con_list;
        while (i) {
            DEBUG_printf("%d ", hr_server_push(i->con, result));
            i = i->next;
        }
        DEBUG_printf("\nFront montant diffusé.\n");
    }
}

void hr_measure(HR_SERVER_T *state) {
    DEBUG_printf("Ready to measure heart rate !\n");
    g_hr_server = (void *) state;
    gpio_set_irq_enabled_with_callback(PULSE_IN, GPIO_IRQ_EDGE_RISE, true, &gpio_callback);
    // irq_set_enabled(IO_IRQ_BANK0, true);
    while (!state->complete) {
        tight_loop_contents();
        DEBUG_printf("ping\n");
        sleep_ms(5000);
    }
}

int main() {
    stdio_init_all();                // init stdio with uart and stuff
    pins_init();

    TCP_SERVER_T *state = calloc(1, sizeof(TCP_SERVER_T));
    if (!state) {
        DEBUG_printf("failed to allocate state\n");
        return (1);
    }
    HR_SERVER_T *hr_state = calloc(1, sizeof(HR_SERVER_T));
    if (!hr_state) {
        DEBUG_printf("failed to allocate hr_state\n");
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
    if (!hr_server_open(state, ap_name)) {
        DEBUG_printf("failed to open hr_server\n");
        return (1);
    }
    state->complete    = false;
    hr_state->complete = false;
    hr_measure(hr_state);

    tcp_server_close(state);
    hr_server_close(hr_state);
    dns_server_deinit(&dns_server);
    dhcp_server_deinit(&dhcp_server);
    cyw43_arch_deinit();
    return (0);
}
